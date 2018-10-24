/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/wallet/stealth_address.hpp>

#include <algorithm>
#include <cstdint>
#include <iostream>

#include <boost/program_options.hpp>

#include <bitcoin/bitcoin/math/stealth.hpp>
#include <bitcoin/infrastructure/formats/base_58.hpp>
#include <bitcoin/infrastructure/math/checksum.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/assert.hpp>
#include <bitcoin/infrastructure/utility/binary.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>

namespace libbitcoin {
namespace wallet {

using namespace bc::chain;

static constexpr uint8_t version_size = sizeof(uint8_t);
static constexpr uint8_t options_size = sizeof(uint8_t);
static constexpr uint8_t number_keys_size = sizeof(uint8_t);
static constexpr uint8_t number_sigs_size = sizeof(uint8_t);
static constexpr uint8_t filter_length_size = sizeof(uint8_t);
static constexpr uint8_t max_spend_key_count = max_uint8;

// wiki.unsystem.net/index.php/DarkWallet/Stealth#Address_format
// [version:1=0x2a][options:1][scan_pubkey:33][N:1][spend_pubkey_1:33]..
// [spend_pubkey_N:33][number_signatures:1][prefix_number_bits:1]
// [filter:prefix_number_bits / 8, round up][checksum:4]
// Estimate assumes N = 0 and prefix_length = 0:
constexpr size_t min_address_size = version_size + options_size +
                                    ec_compressed_size + number_keys_size + number_sigs_size +
                                    filter_length_size + checksum_size;

// Document the assumption that the prefix is defined with an 8 bit block size.
static_assert(binary::bits_per_block == byte_bits,
              "The stealth prefix must use an 8 bit block size.");

const uint8_t stealth_address::mainnet_p2kh = 0x2a;
const uint8_t stealth_address::reuse_key_flag = 1 << 0;
const size_t stealth_address::min_filter_bits = 1 * byte_bits;
const size_t stealth_address::max_filter_bits = sizeof(uint32_t) * byte_bits;

stealth_address::stealth_address()
    : valid_(false), version_(0), scan_key_(null_compressed_point), spend_keys_(), signatures_(0), filter_() {
}

stealth_address::stealth_address(const stealth_address& x)
    : valid_(x.valid_), version_(x.version_), scan_key_(x.scan_key_), spend_keys_(x.spend_keys_), signatures_(x.signatures_), filter_(x.filter_) {
}

stealth_address::stealth_address(std::string const& encoded)
    : stealth_address(from_string(encoded)) {
}

stealth_address::stealth_address(data_chunk const& decoded)
    : stealth_address(from_stealth(decoded)) {
}

stealth_address::stealth_address(const binary& filter,
                                 const ec_compressed& scan_key,
                                 const point_list& spend_keys,
                                 uint8_t signatures,
                                 uint8_t version)
    : stealth_address(from_stealth(filter, scan_key, spend_keys, signatures, version)) {
}

stealth_address::stealth_address(uint8_t version, const binary& filter, const ec_compressed& scan_key, const point_list& spend_keys, uint8_t signatures)
    : valid_(true), filter_(filter), scan_key_(scan_key), spend_keys_(spend_keys), signatures_(signatures), version_(version) {
}

// Factories.
// ----------------------------------------------------------------------------

stealth_address stealth_address::from_string(std::string const& encoded) {
    data_chunk decoded;
    return decode_base58(decoded, encoded) ? stealth_address(decoded) : stealth_address();
}

// This is the stealth address parser.
stealth_address stealth_address::from_stealth(data_chunk const& decoded) {
    // Size is guarded until we get to N.
    auto required_size = min_address_size;
    if (decoded.size() < required_size || !verify_checksum(decoded)) {
        return {};
    }

    // [version:1 = 0x2a]
    auto iterator = decoded.begin();
    auto const version = *iterator;

    // [options:1]
    ++iterator;
    auto const options = *iterator;
    if (options > reuse_key_flag) {
        return {};
    }

    // [scan_pubkey:33]
    ++iterator;
    auto scan_key_begin = iterator;
    iterator += ec_compressed_size;
    ec_compressed scan_key;
    std::copy_n(scan_key_begin, ec_compressed_size, scan_key.begin());

    // [N:1]
    auto number_spend_pubkeys = *iterator;
    ++iterator;

    // Adjust and retest required size. for pubkey list.
    required_size += number_spend_pubkeys * ec_compressed_size;
    if (decoded.size() < required_size) {
        return {};
    }

    // We don't explicitly save 'reuse', instead we add to spend_keys_.
    point_list spend_keys;
    if (options == reuse_key_flag) {
        spend_keys.push_back(scan_key);
}

    // [spend_pubkey_1:33]..[spend_pubkey_N:33]
    ec_compressed point;
    for (auto key = 0; key < number_spend_pubkeys; ++key) {
        auto spend_key_begin = iterator;
        iterator += ec_compressed_size;
        std::copy_n(spend_key_begin, ec_compressed_size, point.begin());
        spend_keys.push_back(point);
    }

    // [number_signatures:1]
    auto const signatures = *iterator;
    ++iterator;

    // [prefix_number_bits:1]
    auto const filter_bits = *iterator;
    if (filter_bits > max_filter_bits) {
        return {};
    }

    // [prefix:prefix_number_bits / 8, round up]
    ++iterator;
    auto const filter_bytes = (filter_bits + (byte_bits - 1)) / byte_bits;

    // Adjust and retest required size.
    required_size += filter_bytes;
    if (decoded.size() != required_size) {
        return {};
    }

    // Deserialize the filter bytes/blocks.
    data_chunk const raw_filter(iterator, iterator + filter_bytes);
    const binary filter(filter_bits, raw_filter);
    return {filter, scan_key, spend_keys, signatures, version};
}

// This corrects signature and spend_keys.
stealth_address stealth_address::from_stealth(const binary& filter,
                                              const ec_compressed& scan_key,
                                              const point_list& spend_keys,
                                              uint8_t signatures,
                                              uint8_t version) {
    // Ensure there is at least one spend key.
    auto spenders = spend_keys;
    if (spenders.empty()) {
        spenders.push_back(scan_key);
}

    // Guard against too many keys.
    auto const spend_keys_size = spenders.size();
    if (spend_keys_size > max_spend_key_count) {
        return {};
    };

    // Guard against prefix too long.
    auto prefix_number_bits = filter.size();
    if (prefix_number_bits > max_filter_bits) {
        return {};
    }

    // Coerce signatures to a valid range.
    auto const maximum = signatures == 0 || signatures > spend_keys_size;
    auto const coerced = maximum ? static_cast<uint8_t>(spend_keys_size) : signatures;

    // Parameter order is used to change the constructor signature.
    return {version, filter, scan_key, spenders, coerced};
}

// Cast operators.
// ----------------------------------------------------------------------------

stealth_address::operator const bool() const {
    return valid_;
}

stealth_address::operator data_chunk const() const {
    return to_chunk();
}

// Serializer.
// ----------------------------------------------------------------------------

std::string stealth_address::encoded() const {
    return encode_base58(to_chunk());
}

uint8_t stealth_address::version() const {
    return version_;
}

// Accessors.
// ----------------------------------------------------------------------------

const binary& stealth_address::filter() const {
    return filter_;
}

const ec_compressed& stealth_address::scan_key() const {
    return scan_key_;
}

uint8_t stealth_address::signatures() const {
    return signatures_;
}

const point_list& stealth_address::spend_keys() const {
    return spend_keys_;
}

// Methods.
// ----------------------------------------------------------------------------

data_chunk stealth_address::to_chunk() const {
    data_chunk address;
    address.push_back(version());
    address.push_back(options());
    extend_data(address, scan_key_);

    // Spend_pubkeys must have been guarded against a max size of 255.
    auto number_spend_pubkeys = static_cast<uint8_t>(spend_keys_.size());

    // Adjust for key reuse.
    if (reuse_key()) {
        --number_spend_pubkeys;
}

    address.push_back(number_spend_pubkeys);

    // Serialize the spend keys, excluding any that match the scan key.
    for (auto const& key : spend_keys_) {
        if (key != scan_key_) {
            extend_data(address, key);
        }
    }

    address.push_back(signatures_);

    // The prefix must be guarded against a size greater than 32
    // so that the bitfield can convert into uint32_t and sized by uint8_t.
    auto const prefix_number_bits = static_cast<uint8_t>(filter_.size());

    // Serialize the prefix bytes/blocks.
    address.push_back(prefix_number_bits);
    extend_data(address, filter_.blocks());

    append_checksum(address);
    return address;
}

// Helpers.
// ----------------------------------------------------------------------------

bool stealth_address::reuse_key() const {
    // If the spend_keys_ contains the scan_key_ then the key is reused.
    return std::find(spend_keys_.begin(), spend_keys_.end(), scan_key_) !=
           spend_keys_.end();
}

uint8_t stealth_address::options() const {
    // There is currently only one option.
    return reuse_key() ? reuse_key_flag : 0x00;
}

// Operators.
// ----------------------------------------------------------------------------

stealth_address& stealth_address::operator=(const stealth_address& x) {
    valid_ = x.valid_;
    version_ = x.version_;
    scan_key_ = x.scan_key_;
    spend_keys_ = x.spend_keys_;
    signatures_ = x.signatures_;
    filter_ = x.filter_;
    return *this;
}

bool stealth_address::operator<(const stealth_address& x) const {
    return encoded() < x.encoded();
}

bool stealth_address::operator==(const stealth_address& x) const {
    return valid_ == x.valid_ && version_ == x.version_ &&
           scan_key_ == x.scan_key_ && spend_keys_ == x.spend_keys_ &&
           signatures_ == x.signatures_ && filter_ == x.filter_;
}

bool stealth_address::operator!=(const stealth_address& x) const {
    return !(*this == x);
}

std::istream& operator>>(std::istream& in, stealth_address& to) {
    std::string value;
    in >> value;
    to = stealth_address(value);

    if ( ! to) {
        using namespace boost::program_options;
        BOOST_THROW_EXCEPTION(invalid_option_value(value));
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const stealth_address& of) {
    out << of.encoded();
    return out;
}

}  // namespace wallet
}  // namespace libbitcoin

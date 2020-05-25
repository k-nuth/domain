// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/wallet/payment_address.hpp>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

#include <boost/program_options.hpp>

#include <kth/domain/multi_crypto_support.hpp>
#include <kth/domain/wallet/ec_private.hpp>
#include <kth/domain/wallet/ec_public.hpp>
#include <kth/infrastructure/formats/base_58.hpp>
#include <kth/infrastructure/math/checksum.hpp>
#include <kth/infrastructure/math/elliptic_curve.hpp>
#include <kth/infrastructure/math/hash.hpp>

#ifdef KTH_CURRENCY_BCH
#include <kth/infrastructure/wallet/cashaddr.hpp>
#endif  //KTH_CURRENCY_BCH

using namespace kth::infrastructure::machine;
using namespace kth::infrastructure::wallet;

namespace kth::domain::wallet {

#ifdef KTH_CURRENCY_LTC
uint8_t const payment_address::mainnet_p2kh = 0x30;
#else
uint8_t const payment_address::mainnet_p2kh = 0x00;
#endif

uint8_t const payment_address::mainnet_p2sh = 0x05;

uint8_t const payment_address::testnet_p2kh = 0x6f;
uint8_t const payment_address::testnet_p2sh = 0xc4;

#ifdef KTH_CURRENCY_BCH
std::string const payment_address::cashaddr_prefix_mainnet = "bitcoincash";
std::string const payment_address::cashaddr_prefix_testnet = "bchtest";
#endif

payment_address::payment_address()
    : hash_(null_short_hash) 
{}

payment_address::payment_address(payment_address&& x) noexcept
    : valid_(x.valid_), version_(x.version_), hash_(x.hash_) 
{}

// payment_address::payment_address(payment_address const& x)
//     : valid_(x.valid_), version_(x.version_), hash_(x.hash_) {
// }

payment_address::payment_address(payment const& decoded)
    : payment_address(from_payment(decoded)) {
}

payment_address::payment_address(std::string const& address)
    : payment_address(from_string(address)) {
}

payment_address::payment_address(ec_private const& secret)
    : payment_address(from_private(secret)) {
}

payment_address::payment_address(ec_public const& point, uint8_t version)
    : payment_address(from_public(point, version)) {
}

payment_address::payment_address(chain::script const& script, uint8_t version)
    : payment_address(from_script(script, version)) {
}

payment_address::payment_address(short_hash const& hash, uint8_t version)
    : valid_(true), version_(version), hash_(hash) {
}

// payment_address& payment_address::operator=(payment_address const& x) {
//     valid_ = x.valid_;
//     version_ = x.version_;
//     hash_ = x.hash_;
//     return *this;
// }

// Validators.
// ----------------------------------------------------------------------------

bool payment_address::is_address(data_slice decoded) {
    return (decoded.size() == payment_size) && verify_checksum(decoded);
}

// Factories.
// ----------------------------------------------------------------------------
#ifdef KTH_CURRENCY_BCH

template <unsigned int frombits, unsigned int tobits, bool pad, typename O, typename I>
bool convert_bits(O& out, I it, I end) {
    size_t acc = 0;
    size_t bits = 0;
    constexpr size_t maxv = (1U << tobits) - 1;
    constexpr size_t max_acc = (1U << (frombits + tobits - 1)) - 1;
    while (it != end) {
        acc = ((acc << frombits) | *it) & max_acc;
        bits += frombits;
        while (bits >= tobits) {
            bits -= tobits;
            out.push_back((acc >> bits) & maxv);
        }
        ++it;
    }

    // We have remaining bits to encode but do not pad.
    if ( ! pad && bits) {
        return false;
    }

    // We have remaining bits to encode so we do pad.
    if (pad && bits) {
        out.push_back((acc << (tobits - bits)) & maxv);
    }

    return true;
}

enum cash_addr_type : uint8_t { PUBKEY_TYPE = 0,
                              SCRIPT_TYPE = 1 };

// CashAddrContent DecodeCashAddrContent(std::string const& address) {
payment_address payment_address::from_string_cashaddr(std::string const& address) {
    // In order to avoid using the wrong network address, the from_string method
    // only accepts the cashaddr_prefix set on the multi_crypto_support file

    // TODO(kth): validate the network on RPC/Interface calls and make payment_address independent of the network

    std::string prefix;
    data_chunk payload;
    std::tie(prefix, payload) = cashaddr::decode(address, cashaddr_prefix());

    if (prefix != cashaddr_prefix()) {
        return {};
    }

    if (payload.empty()) {
        return {};
    }

    // Check that the padding is zero.
    size_t extrabits = payload.size() * 5 % 8;
    if (extrabits >= 5) {
        // We have more padding than allowed.
        return {};
    }

    uint8_t last = payload.back();
    uint8_t mask = (1U << extrabits) - 1;
    if ((last & mask) != 0) {
        // We have non zero bits as padding.
        return {};
    }

    data_chunk data;
    data.reserve(payload.size() * 5 / 8);
    convert_bits<5, 8, false>(data, std::begin(payload), std::end(payload));

    // Decode type and size from the version.
    uint8_t version = data[0];
    if ((version & 0x80) != 0) {
        // First bit is reserved.
        return {};
    }

    auto type = cash_addr_type((version >> 3U) & 0x1f);
    uint32_t hash_size = 20 + 4 * (version & 0x03);
    if ((version & 0x04) != 0) {
        hash_size *= 2;
    }

    // Check that we decoded the exact number of bytes we expected.
    if (data.size() != hash_size + 1) {
        return {};
    }

    //uint8_t version2;
    //switch (type) {
    //    case PUBKEY_TYPE:
    //        version2 = 0x00;
    //        break;
    //    case SCRIPT_TYPE:
    //        version2 = 0x05;
    //        break;
    //}

    short_hash hash;
    std::copy(std::begin(data) + 1, std::end(data), std::begin(hash));

    if (prefix == payment_address::cashaddr_prefix_mainnet) {
        return {hash, type == PUBKEY_TYPE ? payment_address::mainnet_p2kh : payment_address::mainnet_p2sh};
    }
    return {hash, type == PUBKEY_TYPE ? payment_address::testnet_p2kh : payment_address::testnet_p2sh};

    // // Pop the version.
    // data.erase(data.begin());
    // return {type, std::move(data)};
}

#endif  //KTH_CURRENCY_BCH

payment_address payment_address::from_string(std::string const& address) {
    payment decoded;
    if ( ! decode_base58(decoded, address) || !is_address(decoded)) {
#ifdef KTH_CURRENCY_BCH
        return from_string_cashaddr(address);
#else
        return {};
#endif  //KTH_CURRENCY_BCH
    }

    return {decoded};
}

payment_address payment_address::from_payment(payment const& decoded) {
    if ( ! is_address(decoded)) {
        return {};
    }

    auto const hash = slice<1, short_hash_size + 1>(decoded);
    return {hash, decoded.front()};
}

payment_address payment_address::from_private(ec_private const& secret) {
    if ( ! secret) {
        return {};
    }

    return {secret.to_public(), secret.payment_version()};
}

payment_address payment_address::from_public(ec_public const& point, uint8_t version) {
    if ( ! point) {
        return {};
    }

    data_chunk data;
    if ( ! point.to_data(data)) {
        return {};
    }

    return {bitcoin_short_hash(data), version};
}

payment_address payment_address::from_script(chain::script const& script, uint8_t version) {
    // Working around VC++ CTP compiler break here.
    auto const data = script.to_data(false);
    return {bitcoin_short_hash(data), version};
}

// Cast operators.
// ----------------------------------------------------------------------------

payment_address::operator bool() const {
    return valid_;
}

payment_address::operator short_hash const&() const {
    return hash_;
}

// Serializer.
// ----------------------------------------------------------------------------

std::string payment_address::encoded() const {
    return encode_base58(wrap(version_, hash_));
}

#ifdef KTH_CURRENCY_BCH

// Convert the data part to a 5 bit representation.
template <typename T>
data_chunk pack_addr_data_(T const& id, uint8_t type) {
    uint8_t version_byte(type << 3U);
    size_t size = id.size();
    uint8_t encoded_size = 0;

    switch (size * 8) {
        case 160:
            encoded_size = 0;
            break;
        case 192:
            encoded_size = 1;
            break;
        case 224:
            encoded_size = 2;
            break;
        case 256:
            encoded_size = 3;
            break;
        case 320:
            encoded_size = 4;
            break;
        case 384:
            encoded_size = 5;
            break;
        case 448:
            encoded_size = 6;
            break;
        case 512:
            encoded_size = 7;
            break;
        default:
            throw std::runtime_error("Error packing cashaddr: invalid address length");
    }

    version_byte |= encoded_size;
    data_chunk data = {version_byte};
    data.insert(data.end(), std::begin(id), std::end(id));

    data_chunk converted;
    // Reserve the number of bytes required for a 5-bit packed version of a
    // hash, with version byte.  Add half a byte(4) so integer math provides
    // the next multiple-of-5 that would fit all the data.
    converted.reserve(((size + 1) * 8 + 4) / 5);
    convert_bits<8, 5, true>(converted, std::begin(data), std::end(data));

    return converted;
}

std::string encode_cashaddr_(payment_address const& wallet) {
    if (wallet.version() == payment_address::mainnet_p2kh || wallet.version() == payment_address::mainnet_p2sh) {
        // Mainnet
        return cashaddr::encode(payment_address::cashaddr_prefix_mainnet, pack_addr_data_(wallet.hash(), wallet.version() == payment_address::mainnet_p2kh ? PUBKEY_TYPE : SCRIPT_TYPE));
    }
    if (wallet.version() == payment_address::testnet_p2kh || wallet.version() == payment_address::testnet_p2sh) {
        // Testnet
        return cashaddr::encode(payment_address::cashaddr_prefix_testnet, pack_addr_data_(wallet.hash(), wallet.version() == payment_address::testnet_p2kh ? PUBKEY_TYPE : SCRIPT_TYPE));
    }
    return "";
}

std::string payment_address::encoded_cashaddr() const {
    return encode_cashaddr_(*this);
}

#endif  //KTH_CURRENCY_BCH

// Accessors.
// ----------------------------------------------------------------------------

uint8_t payment_address::version() const {
    return version_;
}

short_hash const& payment_address::hash() const {
    return hash_;
}

// Methods.
// ----------------------------------------------------------------------------

payment payment_address::to_payment() const {
    return wrap(version_, hash_);
}

// Operators.
// ----------------------------------------------------------------------------
bool payment_address::operator<(payment_address const& x) const {
    return encoded() < x.encoded();
}

bool payment_address::operator==(payment_address const& x) const {
    return valid_ == x.valid_ && version_ == x.version_ &&
           hash_ == x.hash_;
}

bool payment_address::operator!=(payment_address const& x) const {
    return !(*this == x);
}

std::istream& operator>>(std::istream& in, payment_address& to) {
    std::string value;
    in >> value;
    to = payment_address(value);

    if ( ! to) {
        using namespace boost::program_options;
        BOOST_THROW_EXCEPTION(invalid_option_value(value));
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, payment_address const& of) {
    out << of.encoded();
    return out;
}

// Static functions.
// ----------------------------------------------------------------------------

// Context free input extraction is provably ambiguous (see extract_input).
payment_address::list payment_address::extract(chain::script const& script, uint8_t p2kh_version, uint8_t p2sh_version) {
    auto const input = extract_input(script, p2kh_version, p2sh_version);
    return input.empty() ? extract_output(script, p2kh_version, p2sh_version) : input;
}

// Context free input extraction is provably ambiguous. See inline comments.
payment_address::list payment_address::extract_input(chain::script const& script, uint8_t p2kh_version, uint8_t p2sh_version) {
    // A sign_key_hash result always implies sign_script_hash as well.
    auto const pattern = script.input_pattern();

    switch (pattern) {
        // Given lack of context (prevout) sign_key_hash is always ambiguous
        // with sign_script_hash, so return both potentially-correct addresses.
        // A server can differentiate by extracting from the previous output.
        case script_pattern::sign_key_hash: {
            return {
                {ec_public{script[1].data()}, p2kh_version},
                {bitcoin_short_hash(script.back().data()), p2sh_version}};
        }
        case script_pattern::sign_script_hash: {
            return {{bitcoin_short_hash(script.back().data()), p2sh_version}};
        }

        // There is no address in sign_public_key script (signature only)
        // and the public key cannot be extracted from the signature.
        // Given lack of context (prevout) sign_public_key is always ambiguous
        // with sign_script_hash (though actual conflict seems very unlikely).
        // A server can obtain by extracting from the previous output.
        case script_pattern::sign_public_key:

        // There are no addresses in sign_multisig script, signatures only.
        // Nonstandard (non-zero) first op sign_multisig may conflict with
        // sign_key_hash and/or sign_script_hash (or will be non_standard).
        // A server can obtain the public keys extracting from the previous
        // output, but bare multisig does not associate a payment address.
        case script_pattern::sign_multisig:
        case script_pattern::non_standard:
        default: {
            return {};
        }
    }
}

// A server should use this against the prevout instead of using extract_input.
payment_address::list payment_address::extract_output(
    chain::script const& script,
    uint8_t p2kh_version,
    uint8_t p2sh_version) {
    auto const pattern = script.output_pattern();

    switch (pattern) {
        case script_pattern::pay_key_hash: {
            return {
                {to_array<short_hash_size>(script[2].data()), p2kh_version}};
        }
        case script_pattern::pay_script_hash: {
            return {
                {to_array<short_hash_size>(script[1].data()), p2sh_version}};
        }
        case script_pattern::pay_public_key: {
            return {
                // pay_public_key is not p2kh but we conflate for tracking.
                {ec_public{script[0].data()}, p2kh_version}};
        }

        // Bare multisig and null data do not associate a payment address.
        case script_pattern::pay_multisig:
        case script_pattern::null_data:
        case script_pattern::non_standard:
        default: {
            return {};
        }
    }
}

}  // namespace kth
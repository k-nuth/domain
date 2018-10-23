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
#include <bitcoin/bitcoin/chain/header.hpp>

#include <chrono>
#include <cstddef>
#include <utility>

#include <bitcoin/bitcoin/chain/chain_state.hpp>
#include <bitcoin/bitcoin/chain/compact.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/infrastructure/error.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace chain {

// Use system clock because we require accurate time of day.
using wall_clock = std::chrono::system_clock;

// Constructors.
//-----------------------------------------------------------------------------

header::header()
    // : header(0, null_hash, null_hash, 0, 0, 0) {
    : version_(0)
    , previous_block_hash_(null_hash)
    , merkle_(null_hash)
    , timestamp_(0)
    , bits_(0)
    , nonce_(0)
    , validation{} 
{}

// header::header(uint32_t version, hash_digest&& previous_block_hash, hash_digest&& merkle, uint32_t timestamp, uint32_t bits, uint32_t nonce)
//     : version_(version),
//       previous_block_hash_(previous_block_hash),
//       merkle_(merkle),
//       timestamp_(timestamp),
//       bits_(bits),
//       nonce_(nonce),
//       validation{} 
// {}

header::header(uint32_t version, const hash_digest& previous_block_hash, const hash_digest& merkle, uint32_t timestamp, uint32_t bits, uint32_t nonce)
    : version_(version)
    , previous_block_hash_(previous_block_hash)
    , merkle_(merkle)
    , timestamp_(timestamp)
    , bits_(bits)
    , nonce_(nonce)
    , validation{} 
{}

header::header(header const& x)
    // : header(x.version_, x.previous_block_hash_, x.merkle_, x.timestamp_, x.bits_, x.nonce_) 

    : version_(x.version_)
    , previous_block_hash_(x.previous_block_hash_)
    , merkle_(x.merkle_)
    , timestamp_(x.timestamp_)
    , bits_(x.bits_)
    , nonce_(x.nonce_)
    , validation{} 

{
    // TODO(libbitcoin): implement safe private accessor for conditional cache transfer.
    validation = x.validation;
}

header::header(header const& x, const hash_digest& hash)
    // : header(x.version_, x.previous_block_hash_, x.merkle_, x.timestamp_, x.bits_, x.nonce_) 
    : version_(x.version_)
    , previous_block_hash_(x.previous_block_hash_)
    , merkle_(x.merkle_)
    , timestamp_(x.timestamp_)
    , bits_(x.bits_)
    , nonce_(x.nonce_)
    , validation{} 
{
    hash_ = std::make_shared<hash_digest>(hash);
    validation = x.validation;
}



// Operators.
//-----------------------------------------------------------------------------

header& header::operator=(header const& x) {
    // TODO(libbitcoin): implement safe private accessor for conditional cache transfer.
    version_ = x.version_;
    previous_block_hash_ = x.previous_block_hash_;
    merkle_ = x.merkle_;
    timestamp_ = x.timestamp_;
    bits_ = x.bits_;
    nonce_ = x.nonce_;
    validation = x.validation;
    return *this;
}

bool header::operator==(header const& x) const {
    return (version_ == x.version_) && (previous_block_hash_ == x.previous_block_hash_) && (merkle_ == x.merkle_) && (timestamp_ == x.timestamp_) && (bits_ == x.bits_) && (nonce_ == x.nonce_);
}

bool header::operator!=(header const& x) const {
    return !(*this == x);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
header header::factory_from_data(data_chunk const& data, bool wire) {
    header instance;
    instance.from_data(data, wire);
    return instance;
}

// // static
// header header::factory_from_data(data_source& stream, bool wire)
// {
//     header instance;
//     instance.from_data(stream, wire);
//     return instance;
// }

// static
header header::factory_from_data(data_source& stream, bool wire) {
    header instance;
    instance.from_data(stream, wire);
    return instance;
}

// static
//header header::factory_from_data(reader& source, bool wire)
//{
//    header instance;
//    instance.from_data(source, wire);
//    return instance;
//}

bool header::from_data(data_chunk const& data, bool wire) {
    data_source istream(data);
    return from_data(istream, wire);
}

bool header::from_data(data_source& stream, bool wire) {
    istream_reader stream_r(stream);
    return from_data(stream_r, wire);
}

//TODO(fernando): check what happend when replacing std::istream to data_source
// bool header::from_data(data_source& stream, bool wire)
// {
//     istream_reader stream_r(stream);
//     return from_data(stream_r, wire);
// }

//bool header::from_data(reader& source, bool wire)
//{
//    ////reset();
//
//    version_ = source.read_4_bytes_little_endian();
//    previous_block_hash_ = source.read_hash();
//    merkle_ = source.read_hash();
//    timestamp_ = source.read_4_bytes_little_endian();
//    bits_ = source.read_4_bytes_little_endian();
//    nonce_ = source.read_4_bytes_little_endian();
//
//    if (!wire)
//        validation.median_time_past = source.read_4_bytes_little_endian();
//
//    if (!source)
//        reset();
//
//    return source;
//}

// protected
void header::reset() {
    version_ = 0;
    previous_block_hash_.fill(0);
    merkle_.fill(0);
    timestamp_ = 0;
    bits_ = 0;
    nonce_ = 0;
    invalidate_cache();
}

bool header::is_valid() const {
    return (version_ != 0) ||
           (previous_block_hash_ != null_hash) ||
           (merkle_ != null_hash) ||
           (timestamp_ != 0) ||
           (bits_ != 0) ||
           (nonce_ != 0);
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk header::to_data(bool wire) const {
    data_chunk data;
    auto const size = serialized_size(wire);
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream, wire);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

// void header::to_data(data_sink& stream, bool wire) const
// {
//     ostream_writer sink_w(stream);
//     to_data(sink, wire);
// }

void header::to_data(data_sink& stream, bool wire) const {
    ostream_writer sink_w(stream);
    to_data(sink_w, wire);
}

//void header::to_data(writer& sink, bool wire) const
//{
//    sink.write_4_bytes_little_endian(version_);
//    sink.write_hash(previous_block_hash_);
//    sink.write_hash(merkle_);
//    sink.write_4_bytes_little_endian(timestamp_);
//    sink.write_4_bytes_little_endian(bits_);
//    sink.write_4_bytes_little_endian(nonce_);
//
//    if (!wire)
//        sink.write_4_bytes_little_endian(validation.median_time_past);
//}

// Size.
//-----------------------------------------------------------------------------

// static
size_t header::satoshi_fixed_size() {
    return sizeof(version_) + hash_size + hash_size + sizeof(timestamp_) + sizeof(bits_) + sizeof(nonce_);
}

size_t header::serialized_size(bool wire) const {
    return satoshi_fixed_size() + (wire ? 0 : sizeof(uint32_t));
}

// Accessors.
//-----------------------------------------------------------------------------

uint32_t header::version() const {
    return version_;
}

void header::set_version(uint32_t value) {
    version_ = value;
    invalidate_cache();
}

hash_digest& header::previous_block_hash() {
    return previous_block_hash_;
}

const hash_digest& header::previous_block_hash() const {
    return previous_block_hash_;
}

void header::set_previous_block_hash(const hash_digest& value) {
    previous_block_hash_ = value;
    invalidate_cache();
}

void header::set_previous_block_hash(hash_digest&& value) {
    previous_block_hash_ = value;
    invalidate_cache();
}

hash_digest& header::merkle() {
    return merkle_;
}

const hash_digest& header::merkle() const {
    return merkle_;
}

void header::set_merkle(const hash_digest& value) {
    merkle_ = value;
    invalidate_cache();
}

void header::set_merkle(hash_digest&& value) {
    merkle_ = value;
    invalidate_cache();
}

uint32_t header::timestamp() const {
    return timestamp_;
}

void header::set_timestamp(uint32_t value) {
    timestamp_ = value;
    invalidate_cache();
}

uint32_t header::bits() const {
    return bits_;
}

void header::set_bits(uint32_t value) {
    bits_ = value;
    invalidate_cache();
}

uint32_t header::nonce() const {
    return nonce_;
}

void header::set_nonce(uint32_t value) {
    nonce_ = value;
    invalidate_cache();
}

// Cache.
//-----------------------------------------------------------------------------

// protected
void header::invalidate_cache() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (hash_) {
        mutex_.unlock_upgrade_and_lock();
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        hash_.reset();
        //---------------------------------------------------------------------
        mutex_.unlock_and_lock_upgrade();
    }

    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////
}

hash_digest header::hash() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (!hash_) {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        mutex_.unlock_upgrade_and_lock();
        hash_ = std::make_shared<hash_digest>(bitcoin_hash(to_data()));
        mutex_.unlock_and_lock_upgrade();
        //---------------------------------------------------------------------
    }

    auto const hash = *hash_;
    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return hash;
}

#ifdef BITPRIM_CURRENCY_LTC
hash_digest header::litecoin_proof_of_work_hash() const {
    return litecoin_hash(to_data());
}
#endif  //BITPRIM_CURRENCY_LTC

uint256_t header::proof(uint32_t bits) {
    auto const header_bits = compact(bits);

    if (header_bits.is_overflowed())
        return 0;

    uint256_t target(header_bits);

    //*************************************************************************
    // CONSENSUS: satoshi will throw division by zero in the case where the
    // target is (2^256)-1 as the overflow will result in a zero divisor.
    // While actually achieving this work is improbable, this method operates
    // on user data method and therefore must be guarded.
    //*************************************************************************
    auto const divisor = target + 1;

    // We need to compute 2**256 / (target + 1), but we can't represent 2**256
    // as it's too large for uint256. However as 2**256 is at least as large as
    // target + 1, it is equal to ((2**256 - target - 1) / (target + 1)) + 1, or
    // (~target / (target + 1)) + 1.
    return (divisor == 0) ? 0 : (~target / divisor) + 1;
}

uint256_t header::proof() const {
    return proof(bits());
}

// Validation helpers.
//-----------------------------------------------------------------------------

/// BUGBUG: bitcoin 32bit unix time: en.wikipedia.org/wiki/Year_2038_problem
bool header::is_valid_timestamp() const {
    using namespace std::chrono;
    static auto const two_hours = seconds(timestamp_future_seconds);
    auto const time = wall_clock::from_time_t(timestamp_);
    auto const future = wall_clock::now() + two_hours;
    return time <= future;
}

// [CheckProofOfWork]
bool header::is_valid_proof_of_work(bool retarget) const {
    auto const bits = compact(bits_);
    static const uint256_t pow_limit(compact{work_limit(retarget)});

    if (bits.is_overflowed())
        return false;

    uint256_t target(bits);

    // Ensure claimed work is within limits.
    if (target < 1 || target > pow_limit)
        return false;

        // Ensure actual work is at least claimed amount (smaller is more work).
#ifdef BITPRIM_CURRENCY_LTC
    return to_uint256(litecoin_proof_of_work_hash()) <= target;
#else
    return to_uint256(hash()) <= target;
#endif
}

// Validation.
//-----------------------------------------------------------------------------

code header::check(bool retarget) const {
    if (!is_valid_proof_of_work(retarget))
        return error::invalid_proof_of_work;

    else if (!is_valid_timestamp())
        return error::futuristic_timestamp;

    else
        return error::success;
}

code header::accept(const chain_state& state) const {
    if (bits_ != state.work_required())
        return error::incorrect_proof_of_work;

    else if (state.is_checkpoint_conflict(hash()))
        return error::checkpoints_failed;

    else if (state.is_under_checkpoint())
        return error::success;

    else if (version_ < state.minimum_version())
        return error::old_version_block;

    else if (timestamp_ <= state.median_time_past())
        return error::timestamp_too_early;

    else
        return error::success;
}

}  // namespace chain
}  // namespace libbitcoin

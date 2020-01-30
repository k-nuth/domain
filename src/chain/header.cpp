// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/header.hpp>

#include <cstddef>
#include <utility>

#include <kth/domain/chain/chain_state.hpp>
#include <kth/domain/chain/compact.hpp>
#include <kth/domain/constants.hpp>
#include <kth/infrastructure/error.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth {
namespace chain {

// Constructors.
//-----------------------------------------------------------------------------

header::header(header const& x, hash_digest const& hash)
    : header_basis(x)
    , validation(x.validation)
{
    hash_ = std::make_shared<hash_digest>(hash);
}

header::header(header const& x)
    : header_basis(x)
    , validation(x.validation)
{}

header& header::operator=(header const& x) {
    header_basis::operator=(x);
    validation = x.validation;
    return *this;
}

// Operators.
//-----------------------------------------------------------------------------

// bool header::operator==(header const& x) const {
//     return (version_ == x.version_) 
//         && (previous_block_hash_ == x.previous_block_hash_) 
//         && (merkle_ == x.merkle_) 
//         && (timestamp_ == x.timestamp_) 
//         && (bits_ == x.bits_) 
//         && (nonce_ == x.nonce_);
// }

// bool header::operator!=(header const& x) const {
//     return !(*this == x);
// }

// Deserialization.
//-----------------------------------------------------------------------------

// static
header header::factory_from_data(data_chunk const& data, bool wire) {
    header instance;
    instance.from_data(data, wire);
    return instance;
}

// static
header header::factory_from_data(std::istream& stream, bool wire) {
    header instance;
    instance.from_data(stream, wire);
    return instance;
}

bool header::from_data(data_chunk const& data, bool wire) {
    data_source istream(data);
    return from_data(istream, wire);
}

bool header::from_data(std::istream& stream, bool wire) {
    istream_reader stream_r(stream);
    return from_data(stream_r, wire);
}

// protected
void header::reset() {
    header_basis::reset();
    invalidate_cache();
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

void header::to_data(data_sink& stream, bool wire) const {
    ostream_writer sink_w(stream);
    to_data(sink_w, wire);
}

// Size.
//-----------------------------------------------------------------------------

size_t header::serialized_size(bool wire) const {
    return satoshi_fixed_size() + (wire ? 0 : sizeof(uint32_t));
}

// Accessors.
//-----------------------------------------------------------------------------

void header::set_version(uint32_t value) {
    header_basis::set_version(value);
    invalidate_cache();
}

void header::set_previous_block_hash(hash_digest const& value) {
    header_basis::set_previous_block_hash(value);
    invalidate_cache();
}

void header::set_merkle(hash_digest const& value) {
    header_basis::set_merkle(value);
    invalidate_cache();
}

void header::set_timestamp(uint32_t value) {
    header_basis::set_timestamp(value);
    invalidate_cache();
}

void header::set_bits(uint32_t value) {
    header_basis::set_bits(value);
    invalidate_cache();
}

void header::set_nonce(uint32_t value) {
    header_basis::set_nonce(value);
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

    if ( ! hash_) {
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

#ifdef KTH_CURRENCY_LTC
hash_digest header::litecoin_proof_of_work_hash() const {
    return litecoin_hash(to_data());
}
#endif  //KTH_CURRENCY_LTC

inline
hash_digest header::hash_pow() const {
#ifdef KTH_CURRENCY_LTC
    return litecoin_proof_of_work_hash();
#else
    return hash();
#endif

}

// Validation helpers.
//-----------------------------------------------------------------------------

// [CheckProofOfWork]
bool header::is_valid_proof_of_work(bool retarget) const {
    return header_basis::is_valid_proof_of_work(hash_pow(), retarget);
}

// Validation.
//-----------------------------------------------------------------------------

code header::check(bool retarget) const {
    return header_basis::check(hash_pow(), retarget);
}

code header::accept(chain_state const& state) const {
    return header_basis::accept(state, hash_pow());
}

}  // namespace chain
}  // namespace kth

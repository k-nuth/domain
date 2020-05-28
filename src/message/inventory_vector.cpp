// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/inventory_vector.hpp>

#include <cstdint>
#include <string>

#include <kth/domain/message/inventory.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::domain::message {

uint32_t inventory_vector::to_number(type_id type) {
    return static_cast<uint32_t>(type);
}

inventory_vector::type_id inventory_vector::to_type(uint32_t value) {
    return static_cast<type_id>(value);
}

std::string inventory_vector::to_string(type_id type) {
    switch (type) {
        case type_id::transaction:
            return "transaction";
        case type_id::block:
            return "block";
        case type_id::filtered_block:
            return "filtered_block";
        case type_id::compact_block:
            return "compact_block";
        case type_id::witness_transaction:
            return "witness_transaction";
        case type_id::witness_block:
            return "witness_block";
        case type_id::reserved:
            return "reserved";
        case type_id::error:
        default:
            return "error";
    }
}

inventory_vector::inventory_vector()
    : hash_(null_hash) 
{}

inventory_vector::inventory_vector(type_id type, hash_digest const& hash)
    : type_(type), hash_(hash) 
{}

bool inventory_vector::operator==(inventory_vector const& x) const {
    return (hash_ == x.hash_) && (type_ == x.type_);
}

bool inventory_vector::operator!=(inventory_vector const& x) const {
    return !(*this == x);
}

bool inventory_vector::is_valid() const {
    return (type_ != type_id::error) || (hash_ != null_hash);
}

void inventory_vector::reset() {
    type_ = type_id::error;
    hash_.fill(0);
}

#if defined(KTH_SEGWIT_ENABLED)
void inventory_vector::to_witness() {
    if (type_ == type_id::block || type_ == type_id::transaction) {
        type_ = to_type(to_number(type_) | to_number(type_id::witness));
    }
}
#endif

bool inventory_vector::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool inventory_vector::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk inventory_vector::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void inventory_vector::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t inventory_vector::serialized_size(uint32_t version) const {
    return inventory_vector::satoshi_fixed_size(version);
}

size_t inventory_vector::satoshi_fixed_size(uint32_t /*version*/) {
    //TODO(fernando): what is the sizeof() of a std::array?
    // return sizeof(hash_) + sizeof(uint32_t);
    // return hash_.size() + sizeof(uint32_t);
    return std::tuple_size<hash_digest>::value + sizeof(uint32_t);
}

bool inventory_vector::is_block_type() const {
    return type_ == type_id::witness_block || type_ == type_id::block || type_ == type_id::compact_block || type_ == type_id::filtered_block;
}

bool inventory_vector::is_transaction_type() const {
    return type_ == type_id::witness_transaction || type_ == type_id::transaction;
}

inventory_vector::type_id inventory_vector::type() const {
    return type_;
}

void inventory_vector::set_type(type_id value) {
    type_ = value;
}

hash_digest& inventory_vector::hash() {
    return hash_;
}

hash_digest const& inventory_vector::hash() const {
    return hash_;
}

void inventory_vector::set_hash(hash_digest const& value) {
    hash_ = value;
}

}  // namespace kth

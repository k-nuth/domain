// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/keoken/message/send_tokens.hpp>

#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::keoken::message {

using kth::data_chunk;
using kth::data_sink;
using kth::data_source;
using kth::istream_reader;
using kth::ostream_writer;
// using kth::writer;

// Constructors.
//-------------------------------------------------------------------------

send_tokens::send_tokens(asset_id_t asset_id, amount_t amount)
    : asset_id_(asset_id), amount_(amount) {}

// Operators.
//-----------------------------------------------------------------------------

// friend
bool operator==(send_tokens const& a, send_tokens const& b) {
    return a.asset_id_ == b.asset_id_ && a.amount_ == b.amount_;
}

// friend
bool operator!=(send_tokens const& a, send_tokens const& b) {
    return !(a == b);
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk send_tokens::to_data() const {
    data_chunk data;
    auto const size = serialized_size();
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void send_tokens::to_data(data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(sink_w);
}

// //Note: from_data and to_data are not longer simetrical.
// void send_tokens::to_data(writer& sink) const {
//     base::to_data(sink, version, type);
//     sink.write_4_bytes_big_endian(asset_id_);
//     sink.write_8_bytes_big_endian(amount_);
// }

// Properties (size, accessors, cache).
//-----------------------------------------------------------------------------

size_t send_tokens::serialized_size() const {
    return base::serialized_size() +
           sizeof(asset_id_) +
           sizeof(amount_);
}

asset_id_t send_tokens::asset_id() const {
    return asset_id_;
}

void send_tokens::set_asset_id(asset_id_t x) {
    asset_id_ = x;
}

amount_t send_tokens::amount() const {
    return amount_;
}

void send_tokens::set_amount(amount_t x) {
    amount_ = x;
}

} // namespace kth::keoken::message

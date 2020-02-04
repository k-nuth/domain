// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/keoken/message/send_tokens.hpp>

#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth {
namespace keoken {
namespace message {

using bc::data_chunk;
using bc::data_sink;
using bc::data_source;
using bc::istream_reader;
using bc::ostream_writer;
// using bc::writer;

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

// Deserialization.
//-----------------------------------------------------------------------------

// static
send_tokens send_tokens::factory_from_data(data_chunk const& data) {
    send_tokens instance;  //NOLINT
    instance.from_data(data);
    return instance;
}

// static
send_tokens send_tokens::factory_from_data(std::istream& stream) {
    send_tokens instance;  //NOLINT
    instance.from_data(stream);
    return instance;
}

// // static
// send_tokens send_tokens::factory_from_data(bc::reader& source) {
//     send_tokens instance;  //NOLINT
//     instance.from_data(source);
//     return instance;
// }

bool send_tokens::from_data(data_chunk const& data) {
    data_source istream(data);
    return from_data(istream);
}

bool send_tokens::from_data(std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(stream_r);
}

// //Note: from_data and to_data are not longer simetrical.
// bool send_tokens::from_data(bc::reader& source) {
//     asset_id_ = source.read_4_bytes_big_endian();
//     amount_ = source.read_8_bytes_big_endian();

//     // if ( ! source)
//     //     reset();

//     return source;
// }

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

}  // namespace message
}  // namespace keoken
}  // namespace kth

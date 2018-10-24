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
#include <bitcoin/bitcoin/message/get_blocks.hpp>

#include <bitcoin/bitcoin/message/messages.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const get_blocks::command = "getblocks";
uint32_t const get_blocks::version_minimum = version::level::minimum;
uint32_t const get_blocks::version_maximum = version::level::maximum;

get_blocks get_blocks::factory_from_data(uint32_t version, data_chunk const& data) {
    get_blocks instance;
    instance.from_data(version, data);
    return instance;
}

get_blocks get_blocks::factory_from_data(uint32_t version, data_source& stream) {
    get_blocks instance;
    instance.from_data(version, stream);
    return instance;
}

//get_blocks get_blocks::factory_from_data(uint32_t version, reader& source)
//{
//    get_blocks instance;
//    instance.from_data(version, source);
//    return instance;
//}

get_blocks::get_blocks()
    : start_hashes_(), stop_hash_(null_hash) {
}

get_blocks::get_blocks(hash_list const& start, hash_digest const& stop)
    : start_hashes_(start), stop_hash_(stop) {
}

get_blocks::get_blocks(hash_list&& start, hash_digest const& stop)
    : start_hashes_(std::move(start)), stop_hash_(std::move(stop)) {
}

get_blocks::get_blocks(const get_blocks& x)
    : get_blocks(x.start_hashes_, x.stop_hash_) {
}

get_blocks::get_blocks(get_blocks&& x)
    : get_blocks(std::move(x.start_hashes_), std::move(x.stop_hash_)) {
}

bool get_blocks::is_valid() const {
    return !start_hashes_.empty() || (stop_hash_ != null_hash);
}

void get_blocks::reset() {
    start_hashes_.clear();
    start_hashes_.shrink_to_fit();
    stop_hash_.fill(0);
}

bool get_blocks::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool get_blocks::from_data(uint32_t version, data_source& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

//bool get_blocks::from_data(uint32_t version, reader& source)
//{
//    reset();
//
//    // Discard protocol version because it is stupid.
//    source.read_4_bytes_little_endian();
//    auto const count = source.read_size_little_endian();
//
//    // Guard against potential for arbitary memory allocation.
//    if (count > max_get_blocks)
//        source.invalidate();
//    else
//        start_hashes_.reserve(count);
//
//    for (size_t hash = 0; hash < count && source; ++hash)
//        start_hashes_.push_back(source.read_hash());
//
//    stop_hash_ = source.read_hash();
//
//    if ( ! source)
//        reset();
//
//    return source;
//}

data_chunk get_blocks::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void get_blocks::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

//void get_blocks::to_data(uint32_t version, writer& sink) const
//{
//    sink.write_4_bytes_little_endian(version);
//    sink.write_variable_little_endian(start_hashes_.size());
//
//    for (auto const& start_hash: start_hashes_)
//        sink.write_hash(start_hash);
//
//    sink.write_hash(stop_hash_);
//}

size_t get_blocks::serialized_size(uint32_t version) const {
    return size_t(36) + message::variable_uint_size(start_hashes_.size()) +
           hash_size * start_hashes_.size();
}

hash_list& get_blocks::start_hashes() {
    return start_hashes_;
}

hash_list const& get_blocks::start_hashes() const {
    return start_hashes_;
}

void get_blocks::set_start_hashes(hash_list const& value) {
    start_hashes_ = value;
}

void get_blocks::set_start_hashes(hash_list&& value) {
    start_hashes_ = std::move(value);
}

hash_digest& get_blocks::stop_hash() {
    return stop_hash_;
}

hash_digest const& get_blocks::stop_hash() const {
    return stop_hash_;
}

void get_blocks::set_stop_hash(hash_digest const& value) {
    stop_hash_ = value;
}

get_blocks& get_blocks::operator=(get_blocks&& x) {
    start_hashes_ = std::move(x.start_hashes_);
    stop_hash_ = std::move(x.stop_hash_);
    return *this;
}

bool get_blocks::operator==(const get_blocks& x) const {
    auto result = (start_hashes_.size() == x.start_hashes_.size()) &&
                  (stop_hash_ == x.stop_hash_);

    for (size_t i = 0; i < start_hashes_.size() && result; i++) {
        result = (start_hashes_[i] == x.start_hashes_[i]);
    }

    return result;
}

bool get_blocks::operator!=(const get_blocks& x) const {
    return !(*this == x);
}

}  // namespace message
}  // namespace libbitcoin

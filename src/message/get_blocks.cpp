// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/get_blocks.hpp>

#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/limits.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth {
namespace message {

std::string const get_blocks::command = "getblocks";
uint32_t const get_blocks::version_minimum = version::level::minimum;
uint32_t const get_blocks::version_maximum = version::level::maximum;

get_blocks get_blocks::factory_from_data(uint32_t version, data_chunk const& data) {
    get_blocks instance;
    instance.from_data(version, data);
    return instance;
}

get_blocks get_blocks::factory_from_data(uint32_t version, std::istream& stream) {
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
    : stop_hash_(null_hash) {
}

get_blocks::get_blocks(hash_list const& start, hash_digest const& stop)
    : start_hashes_(start), stop_hash_(stop) {
}

get_blocks::get_blocks(hash_list&& start, hash_digest const& stop)
    : start_hashes_(std::move(start)), stop_hash_(stop) {
}

// get_blocks::get_blocks(get_blocks const& x)
//     : get_blocks(x.start_hashes_, x.stop_hash_) {
// }

// get_blocks::get_blocks(get_blocks&& x) noexcept
//     : get_blocks(std::move(x.start_hashes_), std::move(x.stop_hash_)) 
// {}

// get_blocks& get_blocks::operator=(get_blocks&& x) noexcept {
//     start_hashes_ = std::move(x.start_hashes_);
//     stop_hash_ = std::move(x.stop_hash_);
//     return *this;
// }

bool get_blocks::operator==(get_blocks const& x) const {
    auto result = (start_hashes_.size() == x.start_hashes_.size()) &&
                  (stop_hash_ == x.stop_hash_);

    for (size_t i = 0; i < start_hashes_.size() && result; i++) {
        result = (start_hashes_[i] == x.start_hashes_[i]);
    }

    return result;
}

bool get_blocks::operator!=(get_blocks const& x) const {
    return !(*this == x);
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

bool get_blocks::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

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

size_t get_blocks::serialized_size(uint32_t /*version*/) const {
    return size_t(36) + message::variable_uint_size(start_hashes_.size()) + hash_size * start_hashes_.size();
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

}  // namespace message
}  // namespace kth

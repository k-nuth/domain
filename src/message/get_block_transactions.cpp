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
#include <bitcoin/bitcoin/message/get_block_transactions.hpp>

#include <initializer_list>

#include <bitcoin/bitcoin/message/messages.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/bitcoin/multi_crypto_support.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const get_block_transactions::command = "getblocktxn";
uint32_t const get_block_transactions::version_minimum = version::level::bip152;
uint32_t const get_block_transactions::version_maximum = version::level::bip152;

get_block_transactions get_block_transactions::factory_from_data(uint32_t version, data_chunk const& data) {
    get_block_transactions instance;
    instance.from_data(version, data);
    return instance;
}

get_block_transactions get_block_transactions::factory_from_data(uint32_t version, std::istream& stream) {
    get_block_transactions instance;
    instance.from_data(version, stream);
    return instance;
}

get_block_transactions::get_block_transactions()
    : block_hash_(null_hash) 
{}

get_block_transactions::get_block_transactions(hash_digest const& block_hash, const std::vector<uint64_t>& indexes)
    : block_hash_(block_hash)
    , indexes_(indexes) 
{}

get_block_transactions::get_block_transactions(hash_digest const& block_hash, std::vector<uint64_t>&& indexes)
    : block_hash_(block_hash)
    , indexes_(std::move(indexes)) 
{}

// get_block_transactions::get_block_transactions(get_block_transactions const& x)
//     // : get_block_transactions(x.block_hash_, x.indexes_) 
//     : block_hash_(x.block_hash_)
//     , indexes_(x.indexes_) 
// {}

// get_block_transactions::get_block_transactions(get_block_transactions&& x) noexcept
//     // : get_block_transactions(x.block_hash_, std::move(x.indexes_)) 
//     : block_hash_(x.block_hash_)
//     , indexes_(std::move(x.indexes_)) 
// {}

// get_block_transactions& get_block_transactions::operator=(get_block_transactions&& x) noexcept {
//     block_hash_ = x.block_hash_;
//     indexes_ = x.indexes_;
//     return *this;
// }

bool get_block_transactions::operator==(get_block_transactions const& x) const {
    return (block_hash_ == x.block_hash_) && (indexes_ == x.indexes_);
}

bool get_block_transactions::operator!=(get_block_transactions const& x) const {
    return !(*this == x);
}


bool get_block_transactions::is_valid() const {
    return (block_hash_ != null_hash);
}

void get_block_transactions::reset() {
    block_hash_ = null_hash;
    indexes_.clear();
    indexes_.shrink_to_fit();
}

bool get_block_transactions::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool get_block_transactions::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk get_block_transactions::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void get_block_transactions::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t get_block_transactions::serialized_size(uint32_t /*version*/) const {
    auto size = hash_size + message::variable_uint_size(indexes_.size());

    for (auto const& element : indexes_) {
        size += message::variable_uint_size(element);
    }

    return size;
}

hash_digest& get_block_transactions::block_hash() {
    return block_hash_;
}

hash_digest const& get_block_transactions::block_hash() const {
    return block_hash_;
}

void get_block_transactions::set_block_hash(hash_digest const& value) {
    block_hash_ = value;
}

std::vector<uint64_t>& get_block_transactions::indexes() {
    return indexes_;
}

const std::vector<uint64_t>& get_block_transactions::indexes() const {
    return indexes_;
}

void get_block_transactions::set_indexes(const std::vector<uint64_t>& values) {
    indexes_ = values;
}

void get_block_transactions::set_indexes(std::vector<uint64_t>&& values) {
    indexes_ = values;
}

}  // namespace message
}  // namespace libbitcoin

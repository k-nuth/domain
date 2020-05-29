// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/get_block_transactions.hpp>

#include <initializer_list>

#include <kth/domain/message/version.hpp>
#include <kth/domain/multi_crypto_support.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/limits.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::domain::message {

std::string const get_block_transactions::command = "getblocktxn";
uint32_t const get_block_transactions::version_minimum = version::level::bip152;
uint32_t const get_block_transactions::version_maximum = version::level::bip152;

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

data_chunk get_block_transactions::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void get_block_transactions::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t get_block_transactions::serialized_size(uint32_t /*version*/) const {
    auto size = hash_size + infrastructure::message::variable_uint_size(indexes_.size());

    for (auto const& element : indexes_) {
        size += infrastructure::message::variable_uint_size(element);
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

} // namespace kth

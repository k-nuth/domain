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
#include <bitcoin/bitcoin/message/block_transactions.hpp>

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

std::string const block_transactions::command = "blocktxn";
uint32_t const block_transactions::version_minimum = version::level::bip152;
uint32_t const block_transactions::version_maximum = version::level::bip152;

block_transactions block_transactions::factory_from_data(uint32_t version, data_chunk const& data) {
    block_transactions instance;
    instance.from_data(version, data);
    return instance;
}

block_transactions block_transactions::factory_from_data(uint32_t version, data_source& stream) {
    block_transactions instance;
    instance.from_data(version, stream);
    return instance;
}

//block_transactions block_transactions::factory_from_data(uint32_t version, reader& source)
//{
//    block_transactions instance;
//    instance.from_data(version, source);
//    return instance;
//}

block_transactions::block_transactions()
    : block_hash_(null_hash) 
{}

block_transactions::block_transactions(hash_digest const& block_hash, chain::transaction::list const& transactions)
    : block_hash_(block_hash)
    , transactions_(transactions) 
{}

block_transactions::block_transactions(hash_digest const& block_hash, chain::transaction::list&& transactions)
    : block_hash_(block_hash)
    , transactions_(std::move(transactions)) 
{}

// block_transactions::block_transactions(block_transactions const& x)
//     // : block_transactions(x.block_hash_, x.transactions_) 
//     : block_hash_(x.block_hash_)
//     , transactions_(x.transactions_) 
// {}

block_transactions::block_transactions(block_transactions&& x) noexcept
    // : block_transactions(std::move(x.block_hash_), std::move(x.transactions_)) 
    : block_hash_(x.block_hash_)
    , transactions_(std::move(x.transactions_)) 
{}

bool block_transactions::is_valid() const {
    return (block_hash_ != null_hash);
}

void block_transactions::reset() {
    block_hash_ = null_hash;
    transactions_.clear();
    transactions_.shrink_to_fit();
}

bool block_transactions::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool block_transactions::from_data(uint32_t version, data_source& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk block_transactions::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void block_transactions::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t block_transactions::serialized_size(uint32_t /*version*/) const {
    auto size = hash_size + message::variable_uint_size(transactions_.size());

    for (auto const& element : transactions_) {
        size += element.serialized_size(/*wire*/ true, witness_default()
#ifdef BITPRIM_CACHED_RPC_DATA
                                       , /*unconfirmed*/ false
#endif
                                       );
    }

    return size;
}

hash_digest& block_transactions::block_hash() {
    return block_hash_;
}

hash_digest const& block_transactions::block_hash() const {
    return block_hash_;
}

void block_transactions::set_block_hash(hash_digest const& value) {
    block_hash_ = value;
}

chain::transaction::list& block_transactions::transactions() {
    return transactions_;
}

chain::transaction::list const& block_transactions::transactions() const {
    return transactions_;
}

void block_transactions::set_transactions(chain::transaction::list const& x) {
    transactions_ = x;
}

void block_transactions::set_transactions(chain::transaction::list&& x) {
    transactions_ = std::move(x);
}

block_transactions& block_transactions::operator=(block_transactions&& x) {
    block_hash_ = std::move(x.block_hash_);
    transactions_ = std::move(x.transactions_);
    return *this;
}

bool block_transactions::operator==(block_transactions const& x) const {
    return (block_hash_ == x.block_hash_) && (transactions_ == x.transactions_);
}

bool block_transactions::operator!=(block_transactions const& x) const {
    return !(*this == x);
}

}  // namespace message
}  // namespace libbitcoin

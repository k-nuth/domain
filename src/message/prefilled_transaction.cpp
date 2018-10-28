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
#include <bitcoin/bitcoin/message/prefilled_transaction.hpp>

#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/infrastructure/message/message_tools.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

#ifdef BITPRIM_CURRENCY_BCH
constexpr size_t max_index = max_uint32;
#else
constexpr size_t max_index = max_uint16;
#endif

prefilled_transaction prefilled_transaction::factory_from_data(uint32_t version, data_chunk const& data) {
    prefilled_transaction instance;
    instance.from_data(version, data);
    return instance;
}

prefilled_transaction prefilled_transaction::factory_from_data(uint32_t version, std::istream& stream) {
    prefilled_transaction instance;
    instance.from_data(version, stream);
    return instance;
}

prefilled_transaction::prefilled_transaction()
    : index_(max_index) 
{}

prefilled_transaction::prefilled_transaction(uint64_t index,
                                             chain::transaction const& tx)
    : index_(index), transaction_(tx) {
}

prefilled_transaction::prefilled_transaction(uint64_t index,
                                             chain::transaction&& tx)
    : index_(index), transaction_(std::move(tx)) {
}

// prefilled_transaction::prefilled_transaction(prefilled_transaction const& x)
//     : prefilled_transaction(x.index_, x.transaction_) {
// }

// prefilled_transaction::prefilled_transaction(prefilled_transaction&& x) noexcept
//     : prefilled_transaction(x.index_, std::move(x.transaction_)) 
// {}

// prefilled_transaction& prefilled_transaction::operator=(prefilled_transaction&& x) noexcept {
//     index_ = x.index_;
//     transaction_ = std::move(x.transaction_);
//     return *this;
// }

// prefilled_transaction& prefilled_transaction::operator=(prefilled_transaction const& x) {
//     index_ = x.index_;
//     transaction_ = x.transaction_;
//     return *this;
// }

bool prefilled_transaction::operator==(prefilled_transaction const& x) const {
    return (index_ == x.index_) && (transaction_ == x.transaction_);
}

bool prefilled_transaction::operator!=(prefilled_transaction const& x) const {
    return !(*this == x);
}
bool prefilled_transaction::is_valid() const {
    return (index_ < max_index) && transaction_.is_valid();
}

void prefilled_transaction::reset() {
    index_ = max_index;
    transaction_ = chain::transaction{};
}

bool prefilled_transaction::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool prefilled_transaction::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk prefilled_transaction::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void prefilled_transaction::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t prefilled_transaction::serialized_size(uint32_t /*version*/) const {
    // TODO(bitprim): serialize size should use witness for ! BCH
    return message::variable_uint_size(index_) +
           transaction_.serialized_size(/*wire*/ true, witness_default()
#ifdef BITPRIM_CACHED_RPC_DATA           
                                      , /*unconfirmed*/ false
#endif
                                       );
}

uint64_t prefilled_transaction::index() const {
    return index_;
}

void prefilled_transaction::set_index(uint64_t value) {
    index_ = value;
}

chain::transaction& prefilled_transaction::transaction() {
    return transaction_;
}

chain::transaction const& prefilled_transaction::transaction() const {
    return transaction_;
}

void prefilled_transaction::set_transaction(chain::transaction const& tx) {
    transaction_ = tx;
}

void prefilled_transaction::set_transaction(chain::transaction&& tx) {
    transaction_ = std::move(tx);
}

}  // namespace message
}  // namespace libbitcoin

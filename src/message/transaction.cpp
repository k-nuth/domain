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
#include <bitcoin/bitcoin/message/transaction.hpp>

#include <istream>
#include <utility>

#include <bitcoin/bitcoin/chain/input.hpp>
#include <bitcoin/bitcoin/chain/output.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>

namespace libbitcoin {
namespace message {

std::string const transaction::command = "tx";
uint32_t const transaction::version_minimum = version::level::minimum;
uint32_t const transaction::version_maximum = version::level::maximum;

transaction transaction::factory_from_data(uint32_t version, data_chunk const& data) {
    transaction instance;
    instance.from_data(version, data);
    return instance;
}

transaction transaction::factory_from_data(uint32_t version, data_source& stream) {
    transaction instance;
    instance.from_data(version, stream);
    return instance;
}

transaction::transaction(chain::transaction&& x)
    : chain::transaction(std::move(x)) {
}

transaction::transaction(chain::transaction const& x)
    : chain::transaction(x) {
}

transaction::transaction(uint32_t version, uint32_t locktime, chain::input::list&& inputs, chain::output::list&& outputs)
    : chain::transaction(version, locktime, std::move(inputs), std::move(outputs)) {
}

transaction::transaction(uint32_t version, uint32_t locktime, const chain::input::list& inputs, const chain::output::list& outputs)
    : chain::transaction(version, locktime, inputs, outputs) {
}

// transaction::transaction(transaction&& x) noexcept
//     : chain::transaction(std::move(x)) 
// {}

// transaction::transaction(transaction const& x)
//     : chain::transaction(x) {
// }

// transaction& transaction::operator=(transaction&& x) noexcept {
//     chain::transaction::operator=(std::move(x));
//     return *this;
// }

transaction& transaction::operator=(chain::transaction&& x) {
    reset();
    chain::transaction::operator=(std::move(x));
    return *this;
}

bool transaction::operator==(chain::transaction const& x) const {
    return chain::transaction::operator==(x);
}

bool transaction::operator!=(chain::transaction const& x) const {
    return chain::transaction::operator!=(x);
}

bool transaction::operator==(transaction const& x) const {
    return chain::transaction::operator==(x);
}

bool transaction::operator!=(transaction const& x) const {
    return !(*this == x);
}

// Witness is always deserialized if present.
// NOTE: Witness on bch is dissabled on the chain::block class

bool transaction::from_data(uint32_t /*version*/, data_chunk const& data) {
    return chain::transaction::from_data(data, true, true);
}

bool transaction::from_data(uint32_t /*version*/, data_source& stream) {
    return chain::transaction::from_data(stream, true, true);
}

// Witness is always serialized if present.
// NOTE: Witness on bch is dissabled on the chain::block class

data_chunk transaction::to_data(uint32_t /*version*/, bool witness) const {
    return chain::transaction::to_data(true, witness);
}

void transaction::to_data(uint32_t /*version*/, data_sink& stream, bool witness) const {
    chain::transaction::to_data(stream, true, witness);
}


// Witness size is always counted if present.
// NOTE: Witness on bch is dissabled on the chain::block class

size_t transaction::serialized_size(uint32_t /*unused*/) const {
    return chain::transaction::serialized_size(true, true);
}

}  // namespace message
}  // namespace libbitcoin

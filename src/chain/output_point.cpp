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
#include <bitcoin/bitcoin/chain/output_point.hpp>

#include <cstddef>
#include <cstdint>
#include <utility>

#include <bitcoin/bitcoin/chain/point.hpp>
#include <bitcoin/bitcoin/constants.hpp>

namespace libbitcoin {
namespace chain {

// Constructors.
//-----------------------------------------------------------------------------

output_point::output_point()
    : validation{} 
{}

output_point::output_point(hash_digest const& hash, uint32_t index)
    : point(hash, index)
    , validation{} 
{}

output_point::output_point(point const& x)
    : point(x)
    , validation{} 
{}

// output_point::output_point(output_point&& x) noexcept
//     : point(x)
//     , validation(std::move(x.validation))
// {}

// Operators.
//-----------------------------------------------------------------------------

output_point& output_point::operator=(point const& x) {
    reset();
    point::operator=(x);
    return *this;
}

// output_point& output_point::operator=(output_point&& x) noexcept {
//     point::operator=(x);
//     validation = std::move(x.validation);
//     return *this;
// }

bool output_point::operator==(point const& x) const {
    return point::operator==(x);
}

bool output_point::operator!=(point const& x) const {
    return point::operator!=(x);
}

bool output_point::operator==(output_point const& x) const {
    return point::operator==(x);
}

bool output_point::operator!=(output_point const& x) const {
    return !(*this == x);
}

// Deserialization.
//-----------------------------------------------------------------------------

output_point output_point::factory_from_data(data_chunk const& data, bool wire) {
    output_point instance;
    instance.from_data(data, wire);
    return instance;
}

output_point output_point::factory_from_data(data_source& stream, bool wire) {
    output_point instance;
    instance.from_data(stream, wire);
    return instance;
}

// output_point output_point::factory_from_data(reader& source, bool wire)
// {
//     output_point instance;
//     instance.from_data(source, wire);
//     return instance;
// }

// Validation.
//-----------------------------------------------------------------------------

// For tx pool validation height is that of the candidate block.
bool output_point::is_mature(size_t height) const {
    // Coinbase (null) inputs and those with non-coinbase prevouts are mature.
    if ( ! validation.coinbase || is_null()) {
        return true;
    }

    // The (non-coinbase) input refers to a coinbase output, so validate depth.
    return floor_subtract(height, validation.height) >= coinbase_maturity;
}

}  // namespace chain
}  // namespace libbitcoin

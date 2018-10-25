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
#ifndef LIBBITCOIN_CHAIN_POINT_VALUE_HPP
#define LIBBITCOIN_CHAIN_POINT_VALUE_HPP

#include <cstdint>
#include <vector>

#include <bitcoin/bitcoin/chain/point.hpp>
#include <bitcoin/bitcoin/define.hpp>

namespace libbitcoin {
namespace chain {

/// A valued point, does not implement specialized serialization methods.
class BC_API point_value : public point {
public:
    using list = std::vector<point_value>;

    // Constructors.
    //-------------------------------------------------------------------------

    point_value();
    point_value(point_value const& x) = default;
    point_value(point const& p, uint64_t value);

    /// This class is move assignable and copy assignable.
    point_value& operator=(point_value const& x) = default;

    // Operators.
    //-------------------------------------------------------------------------

    friend
    bool operator==(point_value const& x, point_value const& y);

    friend
    bool operator!=(point_value const& x, point_value const& y);

    // Swap implementation required to properly handle base class.
    friend 
    void swap(point_value& x, point_value& y);

    // Properties (accessors).
    //-------------------------------------------------------------------------

    uint64_t value() const;
    void set_value(uint64_t value);

private:
    uint64_t value_;
};

}  // namespace chain
}  // namespace libbitcoin

#endif

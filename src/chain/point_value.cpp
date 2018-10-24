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
#include <cstdint>
#include <utility>
#include <vector>

#include <bitcoin/bitcoin/chain/point.hpp>
#include <bitcoin/bitcoin/chain/point_value.hpp>

namespace libbitcoin {
namespace chain {

// Constructors.
//-------------------------------------------------------------------------

point_value::point_value()
    : value_(0) {
}

point_value::point_value(point const& p, uint64_t value)
    : point(p), value_(value) {
}

// Operators.
//-------------------------------------------------------------------------

// // Copy and swap idiom, see: stackoverflow.com/a/3279550/1172329
// point_value& point_value::operator=(point_value x) {
//     swap(*this, x);
//     return *this;
// }

// friend
bool operator==(point_value const& x, point_value const& y) {
    return static_cast<point const&>(x) == static_cast<point const&>(y) && x.value_ == y.value_;
}

// friend
bool operator!=(point_value const& x, point_value const& y) {
    return !(x == y);
}

// friend
void swap(point_value& x, point_value& y) {
    using std::swap;

    swap(static_cast<point&>(x), static_cast<point&>(y));
    swap(x.value_, y.value_);
}

// Properties (accessors).
//-------------------------------------------------------------------------

uint64_t point_value::value() const {
    return value_;
}

void point_value::set_value(uint64_t value) {
    value_ = value;
}

}  // namespace chain
}  // namespace libbitcoin

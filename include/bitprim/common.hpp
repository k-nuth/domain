/**
 * Copyright (c) 2016-2018 Bitprim Inc.
 *
 * This file is part of Bitprim.
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
#ifndef BITPRIM_COMMON_HPP_
#define BITPRIM_COMMON_HPP_

namespace libbitcoin {

constexpr inline
bool witness_default() {
#ifdef BITPRIM_CURRENCY_BCH
    return false;
#else
    return true;
#endif
}

constexpr inline
#ifdef BITPRIM_CURRENCY_BCH
bool witness_val(bool /*x*/) {
    return false;
#else
bool witness_val(bool x) {
    return x;
#endif
}

} // namespace libbitcoin

#endif //BITPRIM_COMMON_HPP_

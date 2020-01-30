/**
 * Copyright (c) 2016-2018 Knuth Project.
 *
 * This file is part of Knuth Project.
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
#ifndef KTH_COMMON_HPP_
#define KTH_COMMON_HPP_

namespace kth {

constexpr inline bool witness_default() {
#ifdef KTH_CURRENCY_BCH
    return false;
#else
    return true;
#endif
}

constexpr inline
#ifdef KTH_CURRENCY_BCH
bool witness_val(bool /*x*/) {
    return false;
#else
bool witness_val(bool x) {
    return x;
#endif
}

#ifdef KTH_CURRENCY_BCH
#define KTH_DECL_WITN_ARG bool /*witness*/ = false
#define KTH_DEF_WITN_ARG bool /*witness = false*/
#else
#define KTH_DECL_WITN_ARG bool witness = false
#define KTH_DEF_WITN_ARG bool witness /*= false*/
#endif



}  // namespace kth

#endif  //KTH_COMMON_HPP_

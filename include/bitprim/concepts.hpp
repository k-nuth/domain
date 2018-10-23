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
#ifndef BITPRIM_CONCEPTS_HPP_
#define BITPRIM_CONCEPTS_HPP_

#define Reader typename
#define Writer typename

namespace bitprim {

template <typename R, bool result = std::is_same<decltype(((R*)nullptr)->read_size_little_endian()), size_t>::value>    //NOLINT
constexpr bool is_reader_helper(int /*unused*/) { 
    return result;
}

template <typename R>
constexpr bool is_reader_helper(...) { return false; }  //NOLINT

template <typename R>
constexpr bool is_reader() {
    return is_reader_helper<R>(0);
}

} // namespace bitprim

#define BITPRIM_IS_READER(R) typename std::enable_if<bitprim::is_reader<R>(), int>::type = 0

#endif //BITPRIM_CONCEPTS_HPP_

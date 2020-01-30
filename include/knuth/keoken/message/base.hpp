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
#ifndef KTH_KEOKEN_MESSAGE_BASE_HPP_
#define KTH_KEOKEN_MESSAGE_BASE_HPP_

#include <kth/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

// Platform check.
static_assert(std::is_same<std::uint8_t, char>::value || std::is_same<std::uint8_t, unsigned char>::value,
              "Knuth requires std::uint8_t to be implemented as char or unsigned char.");

namespace knuth {
namespace keoken {
namespace message {

namespace base {

size_t serialized_size();

template <Writer W>
void to_data(W& sink, uint16_t version, uint16_t type) {
    sink.write_2_bytes_big_endian(version);
    sink.write_2_bytes_big_endian(type);
}

}  // namespace base

}  // namespace message
}  // namespace keoken
}  // namespace knuth

#endif  //KTH_KEOKEN_MESSAGE_BASE_HPP_

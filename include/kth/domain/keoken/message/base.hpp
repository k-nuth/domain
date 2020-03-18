// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_MESSAGE_BASE_HPP_
#define KTH_KEOKEN_MESSAGE_BASE_HPP_

#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/common.hpp>
#include <kth/domain/concepts.hpp>

// Platform check.
static_assert(std::is_same<std::uint8_t, char>::value || std::is_same<std::uint8_t, unsigned char>::value,
              "Knuth requires std::uint8_t to be implemented as char or unsigned char.");

namespace kth {
namespace keoken {
namespace message {

namespace base {

size_t serialized_size();

template <typename W>
void to_data(W& sink, uint16_t version, uint16_t type) {
    sink.write_2_bytes_big_endian(version);
    sink.write_2_bytes_big_endian(type);
}

}  // namespace base

}  // namespace message
}  // namespace keoken
}  // namespace kth

#endif  //KTH_KEOKEN_MESSAGE_BASE_HPP_

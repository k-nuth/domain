// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/keoken/message/base.hpp>

#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::keoken::message {
namespace base {

size_t serialized_size() {
    return sizeof(uint16_t) + sizeof(uint16_t);
}

// void to_data(kth::writer& sink, uint16_t version, uint16_t type) {
//     sink.write_2_bytes_big_endian(version);
//     sink.write_2_bytes_big_endian(type);
// }

} // namespace base
} // namespace kth::keoken::message

// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <knuth/keoken/message/base.hpp>

#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace knuth {
namespace keoken {
namespace message {
namespace base {

size_t serialized_size() {
    return sizeof(uint16_t) + sizeof(uint16_t);
}

// void to_data(bc::writer& sink, uint16_t version, uint16_t type) {
//     sink.write_2_bytes_big_endian(version);
//     sink.write_2_bytes_big_endian(type);
// }

}  // namespace base
}  // namespace message
}  // namespace keoken
}  // namespace knuth

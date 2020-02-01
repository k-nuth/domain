// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_CONSTANTS_HPP_
#define KTH_KEOKEN_CONSTANTS_HPP_

#include <bitcoin/bitcoin/machine/opcode.hpp>
#include <bitcoin/bitcoin/machine/operation.hpp>

#include <knuth/keoken/utility.hpp>

namespace knuth {
namespace keoken {

uint8_t const protocol_name[] = {0x00, 0x00, 0x4b, 0x50};  //"  KP"
constexpr size_t kp_min_size = to_underlying(bc::machine::opcode::push_size_4);
constexpr size_t kp_max_size = to_underlying(bc::machine::opcode::push_four_size);
constexpr size_t min_asset_name_size = 1;

}  // namespace keoken
}  // namespace knuth

#endif  //KTH_KEOKEN_CONSTANTS_HPP_

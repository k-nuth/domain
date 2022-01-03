// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_CONSTANTS_HPP_
#define KTH_KEOKEN_CONSTANTS_HPP_

#include <kth/domain/machine/opcode.hpp>
#include <kth/domain/machine/operation.hpp>

#include <kth/domain/keoken/utility.hpp>

namespace kth::keoken {

uint8_t const protocol_name[] = {0x00, 0x00, 0x4b, 0x50};  //"  KP"
constexpr size_t kp_min_size = to_underlying(kth::domain::machine::opcode::push_size_4);
constexpr size_t kp_max_size = to_underlying(kth::domain::machine::opcode::push_four_size);
constexpr size_t min_asset_name_size = 1;

} // namespace keoken
} // namespace kth

#endif  //KTH_KEOKEN_CONSTANTS_HPP_

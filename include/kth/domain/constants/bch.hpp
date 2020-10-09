// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CONSTANTS_BCH_HPP_
#define KTH_DOMAIN_CONSTANTS_BCH_HPP_

#include <cstddef>
#include <cstdint>

#include <kth/infrastructure/config/checkpoint.hpp>

#include <kth/domain/config/network.hpp>
#include <kth/domain/constants/bch_btc.hpp>
#include <kth/domain/constants/common.hpp>

namespace kth {

// Blocks used to calculate the next difficulty
constexpr size_t bch_daa_cw_144_retarget_algorithm = 147;
constexpr size_t chain_state_timestamp_count = bch_daa_cw_144_retarget_algorithm;
constexpr size_t bch_daa_eda_blocks = 6 + 11;
constexpr size_t max_block_size_old = 8000000;   //eight million bytes
constexpr size_t max_block_size_new = 32000000;  //thirty two million bytes
constexpr size_t max_block_size_testnet4 = 1200000;   //1.2 million bytes
constexpr size_t min_transaction_size = 100;   //one hundred bytes
constexpr size_t max_block_sigops_old = max_block_size_old / max_sigops_factor;
constexpr size_t max_block_sigops_new = max_block_size_new / max_sigops_factor;
constexpr size_t max_block_sigops_testnet4 = max_block_size_testnet4 / max_sigops_factor;
constexpr size_t max_tx_sigchecks = 3000;
constexpr size_t block_maxbytes_maxsigchecks_ratio = 141;

// Testnet4 frozen activation heights (frozen_activations).
constexpr size_t testnet4_bip65_freeze = 3; //TODO(fernando): testnet4
constexpr size_t testnet4_bip66_freeze = 4; //TODO(fernando): testnet4
constexpr size_t testnet4_bip34_freeze = 2; //TODO(fernando): testnet4

static
const infrastructure::config::checkpoint testnet4_bip34_active_checkpoint {
    "00000000b0c65b1e03baace7d5c093db0d6aac224df01484985ffd5e86a1a20c", 2};

// enum class pythagoras_t : uint64_t {};
// enum class euclid_t : uint64_t {};
// enum class pisano_t : uint64_t {};
// enum class mersenne_t : uint64_t {};
// enum class fermat_t : uint64_t {};
enum class euler_t : uint64_t {};
enum class gauss_t : uint64_t {};
enum class unnamed_t : uint64_t {}; //TODO(fernando): rename it

// constexpr size_t     bch_activation_height = 478559 //478558;        // 2017-Aug-01 HF
// constexpr uint32_t   bch_daa_cw144_activation_time = 1510600000;     // 2017-Nov-13 HF
// constexpr uint32_t   bch_pythagoras_activation_time = 1526400000;    // 2018-May-15 HF
// constexpr euclid_t   bch_euclid_activation_time = 1542300000;        // 2018-Nov-15 HF
// constexpr pisano_t   bch_pisano_activation_time = 1557921600;        // 2019-May-15 HF
// constexpr mersenne_t bch_mersenne_activation_time = 1573819200;      // 2019-Nov-15 HF
// constexpr fermat_t   bch_fermat_activation_time{1589544000};         // 2020-May-15 HF
constexpr euler_t     bch_euler_activation_time{1605441600};            // 2020-Nov-15 HF
constexpr gauss_t     bch_gauss_activation_time{1621080000};            // 2021-May-15 HF

//2017-August-01 hard fork

constexpr size_t mainnet_uahf_activation_height = 478558;
constexpr size_t testnet_uahf_activation_height = 1155876;
constexpr size_t testnet4_uahf_activation_height = 6;

//2017-November-13 hard fork
constexpr size_t mainnet_daa_cw144_activation_height = 504031;
constexpr size_t testnet_daa_cw144_activation_height = 1188697;
constexpr size_t testnet4_daa_cw144_activation_height = 3000;

//2018-May hard fork
constexpr size_t mainnet_pythagoras_activation_height = 530359;
constexpr size_t testnet_pythagoras_activation_height = 1233078;
constexpr size_t testnet4_pythagoras_activation_height = 0;  //TODO(fernando): testnet4

//2018-November hard fork
constexpr size_t mainnet_euclid_activation_height = 556767;
constexpr size_t testnet_euclid_activation_height = 1267996;
constexpr size_t testnet4_euclid_activation_height = 4000;

//2019-May hard fork
constexpr size_t mainnet_pisano_activation_height = 582680;
constexpr size_t testnet_pisano_activation_height = 1303885;
constexpr size_t testnet4_pisano_activation_height = 0; //TODO(fernando): testnet4

//2019-Nov hard fork
constexpr size_t mainnet_mersenne_activation_height = 609136;
constexpr size_t testnet_mersenne_activation_height = 1341712;
constexpr size_t testnet4_mersenne_activation_height = 5000;

//2020-May hard fork
constexpr size_t mainnet_fermat_activation_height = 635259;
constexpr size_t testnet_fermat_activation_height = 1378461;
constexpr size_t testnet4_fermat_activation_height = 6000;

//2020-Nov hard fork, ASERT Anchor block lock up
//Will be removed once Euler(2020-Nov) update is activated
constexpr size_t mainnet_asert_anchor_lock_up_height = 652500;  // 000000000000000001655f282a3684de3e422290dca55a7ff84753871073c37e
constexpr size_t testnet_asert_anchor_lock_up_height = 1408990; // 0000000000069a8d053a2f34739137cd86722bde2516f03759d9349a0c04fd2e
constexpr size_t testnet4_asert_anchor_lock_up_height = 0;      // Genesis: 000000001dd410c49a788668ce26751718cc797474d3152a5fc073dd44fd9f7b

//2020-Nov hard fork
// Complete after the hard fork

} // namespace kth

#endif // KTH_DOMAIN_CONSTANTS_BCH_HPP_

// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CONSTANTS_BCH_HPP_
#define KTH_DOMAIN_CONSTANTS_BCH_HPP_

#include <cstddef>
#include <cstdint>

#include <kth/infrastructure/config/checkpoint.hpp>

#include <kth/domain/config/settings.hpp>
#include <kth/domain/constants/bch_btc.hpp>
#include <kth/domain/constants/common.hpp>

namespace kth {

// Blocks used to calculate the next difficulty
constexpr size_t bch_daa_cw_144_retarget_algorithm = 147;
constexpr size_t chain_state_timestamp_count = bch_daa_cw_144_retarget_algorithm;
constexpr size_t bch_offset_tip = bch_daa_cw_144_retarget_algorithm - 11;  // 147 - 11 = 136
constexpr size_t bch_offset_tip_minus_6 = bch_offset_tip - 6;
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
const infrastructure::config::checkpoint testnet4_bip34_active_checkpoint{
    "0000000000000000000000000000000000000000000000000000000000000000", 0}; //TODO(fernando): testnet4


// enum class monolith_t : uint64_t {};
// enum class magnetic_anomaly_t : uint64_t {};
// enum class great_wall_t : uint64_t {};
// enum class graviton_t : uint64_t {};
// enum class phonon_t : uint64_t {};
enum class axion_t : uint64_t {};
enum class tachyon_t : uint64_t {};
enum class unnamed_t : uint64_t {}; //TODO(fernando): rename it

// constexpr size_t             bch_activation_height = 478559 //478558;            //2017-Aug-01 HF
// constexpr uint32_t           bch_daa_cw144_activation_time = 1510600000;         //2017-Nov-13 HF
// constexpr uint32_t           bch_monolith_activation_time = 1526400000;          //2018-May-15 HF
// constexpr magnetic_anomaly_t bch_magnetic_anomaly_activation_time = 1542300000;  //2018-Nov-15 HF
// constexpr great_wall_t       bch_great_wall_activation_time = 1557921600;        //2019-May-15 HF
// constexpr graviton_t         bch_graviton_activation_time = 1573819200;          //2019-Nov-15 HF
// constexpr phonon_t           bch_phonon_activation_time{1589544000};             //2020-May-15 HF
constexpr axion_t            bch_axion_activation_time{1605441600};                 //2020-Nov-15 HF
constexpr tachyon_t          bch_tachyon_activation_time{1621080000};               //2021-May-15 HF

//2017-August-01 hard fork
static
const infrastructure::config::checkpoint mainnet_uahf_active_checkpoint{
    "0000000000000000011865af4122fe3b144e2cbeea86142e8ff2fb4107352d43", 478558};
static
const infrastructure::config::checkpoint testnet_uahf_active_checkpoint{
    "00000000000e38fef93ed9582a7df43815d5c2ba9fd37ef70c9a0ea4a285b8f5", 1155876};
static
const infrastructure::config::checkpoint testnet4_uahf_active_checkpoint{
    "0000000000000000000000000000000000000000000000000000000000000000", 0}; //TODO(fernando): testnet4

//2017-November-13 hard fork
static
const infrastructure::config::checkpoint mainnet_daa_cw144_active_checkpoint{
    "0000000000000000011ebf65b60d0a3de80b8175be709d653b4c1a1beeb6ab9c", 504031};
static
const infrastructure::config::checkpoint testnet_daa_cw144_active_checkpoint{
    "0000000000170ed0918077bde7b4d36cc4c91be69fa09211f748240dabe047fb", 1188697};
static
const infrastructure::config::checkpoint testnet4_daa_cw144_active_checkpoint{
    "0000000000000000000000000000000000000000000000000000000000000000", 0}; //TODO(fernando): testnet4

//2018-May hard fork
static
const infrastructure::config::checkpoint mainnet_monolith_active_checkpoint{
    "0000000000000000011ada8bd08f46074f44a8f155396f43e38acf9501c49103", 530359};
static
const infrastructure::config::checkpoint testnet_monolith_active_checkpoint{
    "0000000000327972b8470c11755adf8f4319796bafae01f5a6650490b98a17db", 1233078};
static
const infrastructure::config::checkpoint testnet4_monolith_active_checkpoint{
    "0000000000000000000000000000000000000000000000000000000000000000", 0}; //TODO(fernando): testnet4

//2018-November hard fork
static
const infrastructure::config::checkpoint mainnet_magnetic_anomaly_active_checkpoint{
    "0000000000000000004626ff6e3b936941d341c5932ece4357eeccac44e6d56c", 556767};
static
const infrastructure::config::checkpoint testnet_magnetic_anomaly_active_checkpoint{
    "00000000000001fae0095cd4bea16f1ce8ab63f3f660a03c6d8171485f484b24", 1267996};
static
const infrastructure::config::checkpoint testnet4_magnetic_anomaly_active_checkpoint{
    "0000000000000000000000000000000000000000000000000000000000000000", 0}; //TODO(fernando): testnet4

//2019-May hard fork
static
const infrastructure::config::checkpoint mainnet_great_wall_active_checkpoint{
    "000000000000000001b4b8e36aec7d4f9671a47872cb9a74dc16ca398c7dcc18", 582680};
static
const infrastructure::config::checkpoint testnet_great_wall_active_checkpoint{
    "00000000000000479138892ef0e4fa478ccc938fb94df862ef5bde7e8dee23d3", 1303885};
static
const infrastructure::config::checkpoint testnet4_great_wall_active_checkpoint{
    "0000000000000000000000000000000000000000000000000000000000000000", 0}; //TODO(fernando): testnet4

//2019-Nov hard fork
static
const infrastructure::config::checkpoint mainnet_graviton_active_checkpoint{
    "000000000000000000b48bb207faac5ac655c313e41ac909322eaa694f5bc5b1", 609136};
static
const infrastructure::config::checkpoint testnet_graviton_active_checkpoint{
    "00000000fffc44ea2e202bd905a9fbbb9491ef9e9d5a9eed4039079229afa35b", 1341712};
static
const infrastructure::config::checkpoint testnet4_graviton_active_checkpoint{
    "0000000000000000000000000000000000000000000000000000000000000000", 0}; //TODO(fernando): testnet4

//2020-May hard fork
static
const infrastructure::config::checkpoint mainnet_phonon_active_checkpoint{
    "00000000000000000033dfef1fc2d6a5d5520b078c55193a9bf498c5b27530f7", 635259};
static
const infrastructure::config::checkpoint testnet_phonon_active_checkpoint{
    "0000000099f5509b5f36b1926bcf82b21d936ebeadee811030dfbbb7fae915d7", 1378461};
static
const infrastructure::config::checkpoint testnet4_phonon_active_checkpoint{
    "0000000000000000000000000000000000000000000000000000000000000000", 0}; //TODO(fernando): testnet4

//2020-Nov hard fork, ASERT Anchor block lock up
//Will be removed once Axion(2020-Nov) update is activated
static
const infrastructure::config::checkpoint mainnet_asert_anchor_lock_up_checkpoint{
    "000000000000000001db4b04393d3cc8da269bb6650011f666bfc17bafcebebf", 648000};
static
const infrastructure::config::checkpoint testnet_asert_anchor_lock_up_checkpoint{
    "0000000000146da9eea6f299ca19ccb81371aa2e9490db229d610e74c4790e08", 1400000};
static
const infrastructure::config::checkpoint testnet4_asert_anchor_lock_up_checkpoint{
    "0000000000000000000000000000000000000000000000000000000000000000", 0}; //TODO(fernando): testnet4

//2020-Nov hard fork
// Complete after the hard fork

} // namespace kth

#endif // KTH_DOMAIN_CONSTANTS_BCH_HPP_

/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
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
#ifndef LIBBITCOIN_CONSTANTS_HPP
#define LIBBITCOIN_CONSTANTS_HPP

#include <cstddef>
#include <cstdint>

#include <bitcoin/bitcoin/built_in_type.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/version.hpp>
#include <bitcoin/infrastructure/compat.hpp>
#include <bitcoin/infrastructure/config/checkpoint.hpp>
#include <bitcoin/infrastructure/constants.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/message/network_address.hpp>

namespace libbitcoin {

// This guards assumptions within the codebase.
static_assert(sizeof(size_t) >= sizeof(uint32_t), "unsupported size_t");

// #define BC_USER_AGENT "/bitprim:" LIBBITCOIN_VERSION "/"

// Generic constants.
//-----------------------------------------------------------------------------

// Defined <bitcoin/infrastructure/constants.hpp>
// BC_CONSTEXPR int64_t min_int64 = MIN_INT64;
// BC_CONSTEXPR int64_t max_int64 = MAX_INT64;
// BC_CONSTEXPR int32_t min_int32 = MIN_INT32;
// BC_CONSTEXPR int32_t max_int32 = MAX_INT32;
// BC_CONSTEXPR uint64_t max_uint64 = MAX_UINT64;
// BC_CONSTEXPR uint32_t max_uint32 = MAX_UINT32;
// BC_CONSTEXPR uint16_t max_uint16 = MAX_UINT16;
// BC_CONSTEXPR uint8_t max_uint8 = MAX_UINT8;
// BC_CONSTEXPR uint64_t max_size_t = BC_MAX_SIZE;
// BC_CONSTEXPR uint8_t byte_bits = 8;

// Consensus sentinels.
//-----------------------------------------------------------------------------

BC_CONSTEXPR uint32_t no_previous_output = max_uint32;
BC_CONSTEXPR uint32_t max_input_sequence = max_uint32;
BC_CONSTEXPR uint64_t sighash_null_value = max_uint64;

// Script/interpreter constants.
//-----------------------------------------------------------------------------

// Consensus
BC_CONSTEXPR size_t max_counted_ops = 201;
BC_CONSTEXPR size_t max_stack_size = 1000;
BC_CONSTEXPR size_t max_script_size = 10000;
// BC_CONSTEXPR size_t max_push_data_size = 520;
BC_CONSTEXPR size_t max_script_public_keys = 20;
BC_CONSTEXPR size_t multisig_default_sigops = 20;
BC_CONSTEXPR size_t max_number_size = 4;
BC_CONSTEXPR size_t max_check_locktime_verify_number_size = 5;
BC_CONSTEXPR size_t max_check_sequence_verify_number_size = 5;

// Policy.
BC_CONSTEXPR size_t max_null_data_size = 80;

// Various validation constants.
//-----------------------------------------------------------------------------

BC_CONSTEXPR size_t min_coinbase_size = 2;
BC_CONSTEXPR size_t max_coinbase_size = 100;

BC_CONSTEXPR size_t median_time_past_interval = 11;

#ifdef BITPRIM_CURRENCY_BCH
BC_CONSTEXPR size_t bitcoin_cash_retarget_blocks = 6;
//BC_CONSTEXPR size_t chain_state_timestamp_count = median_time_past_interval + bitcoin_cash_retarget_blocks;

// Blocks used to calculate the next difficulty
BC_CONSTEXPR size_t new_bitcoin_cash_retarget_algorithm = 147;
BC_CONSTEXPR size_t chain_state_timestamp_count = new_bitcoin_cash_retarget_algorithm;

BC_CONSTEXPR size_t bitcoin_cash_offset_tip = new_bitcoin_cash_retarget_algorithm - 11;  // 147 - 11 = 136
BC_CONSTEXPR size_t bitcoin_cash_offset_tip_minus_6 = bitcoin_cash_offset_tip - 6;

BC_CONSTEXPR size_t max_block_size_old = 8000000;   //eight million bytes
BC_CONSTEXPR size_t max_block_size_new = 32000000;  //thirty two million bytes

BC_CONSTEXPR size_t min_transaction_size = 100;   //one hundred bytes

#else

BC_CONSTEXPR size_t max_block_size = 1000000;  //one million bytes

#endif  //BITPRIM_CURRENCY_BCH

BC_CONSTEXPR size_t coinbase_maturity = 100;
// BC_CONSTEXPR size_t median_time_past_interval = 11;
BC_CONSTEXPR size_t locktime_threshold = 500000000;
// BC_CONSTEXPR size_t max_block_size = 1000000;

#ifdef BITPRIM_CURRENCY_LTC
//0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
BC_CONSTEXPR uint32_t retarget_proof_of_work_limit = 0x1e0fffff;
BC_CONSTEXPR uint32_t no_retarget_proof_of_work_limit = 0x207fffff;  // TODO(libbitcoin): merge version3. check the no_retarget value for LTC
#else                                                                // BITPRIM_CURRENCY_LTC
BC_CONSTEXPR size_t max_work_bits = 0x1d00ffff;
BC_CONSTEXPR uint32_t retarget_proof_of_work_limit = 0x1d00ffff;
BC_CONSTEXPR uint32_t no_retarget_proof_of_work_limit = 0x207fffff;
// This may not be flexible, keep internal.
//0x00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff
#endif                                                               // BITPRIM_CURRENCY_LTC

BC_CONSTFUNC uint32_t work_limit(bool retarget = true) {
    return retarget ? retarget_proof_of_work_limit : no_retarget_proof_of_work_limit;
}

// Derived.
BC_CONSTEXPR size_t max_sigops_factor = 50;

// BC_CONSTEXPR size_t max_block_sigops = max_block_size / max_sigops_factor;
#ifdef BITPRIM_CURRENCY_BCH
BC_CONSTEXPR size_t max_block_sigops_old = max_block_size_old / max_sigops_factor;
BC_CONSTEXPR size_t max_block_sigops_new = max_block_size_new / max_sigops_factor;
#else
BC_CONSTEXPR size_t max_block_sigops = max_block_size / max_sigops_factor;
#endif  //BITPRIM_CURRENCY_BCH

constexpr inline size_t get_max_block_size() noexcept {
#ifdef BITPRIM_CURRENCY_BCH
    return max_block_size_new;
#else
    return max_block_size;
#endif  //BITPRIM_CURRENCY_BCH
}

constexpr inline size_t get_max_block_sigops() {
#ifdef BITPRIM_CURRENCY_BCH
    return max_block_sigops_new;
#else
    return max_block_sigops;
#endif  //BITPRIM_CURRENCY_BCH
}

BC_CONSTEXPR size_t one_million_bytes_block = 1000000;
BC_CONSTEXPR size_t coinbase_reserved_size = 20000;
BC_CONSTEXPR size_t sigops_per_million_bytes = 20000;

BC_CONSTFUNC inline size_t get_allowed_sigops(size_t block_size) {
    return (1 + ((block_size - 1) / one_million_bytes_block)) * sigops_per_million_bytes;
}

// Relative locktime constants.
//-----------------------------------------------------------------------------

BC_CONSTEXPR size_t relative_locktime_min_version = 2;
BC_CONSTEXPR size_t relative_locktime_seconds_shift = 9;
BC_CONSTEXPR uint32_t relative_locktime_mask = 0x0000ffff;
BC_CONSTEXPR uint32_t relative_locktime_disabled = 0x80000000;
BC_CONSTEXPR uint32_t relative_locktime_time_locked = 0x00400000;

// Timespan constants.
//-----------------------------------------------------------------------------

BC_CONSTEXPR uint32_t retargeting_factor = 4;

#ifdef BITPRIM_CURRENCY_LTC
BC_CONSTEXPR uint32_t target_spacing_seconds = 10 * 15;
BC_CONSTEXPR uint32_t target_timespan_seconds = 2 * 7 * 24 * 60 * 15;
#else   //BITPRIM_CURRENCY_LTC
BC_CONSTEXPR uint32_t target_spacing_seconds = 10 * 60;
BC_CONSTEXPR uint32_t target_timespan_seconds = 2 * 7 * 24 * 60 * 60;
#endif  //BITPRIM_CURRENCY_LTC

BC_CONSTEXPR uint32_t timestamp_future_seconds = 2 * 60 * 60;  //TODO(bitprim): New on v3.3.0 merge (September 2017), see how this affects Litecoin
BC_CONSTEXPR uint32_t easy_spacing_factor = 2;

// BC_CONSTEXPR uint32_t easy_spacing_seconds = 20 * 60;
// BC_CONSTEXPR uint32_t retarget_height = easy_spacing_factor * target_spacing_seconds;
BC_CONSTEXPR uint32_t easy_spacing_seconds = easy_spacing_factor * target_spacing_seconds;

// The upper and lower bounds for the retargeting timespan.
BC_CONSTEXPR uint32_t min_timespan = target_timespan_seconds / retargeting_factor;
BC_CONSTEXPR uint32_t max_timespan = target_timespan_seconds * retargeting_factor;

// Bitcoin:  The target number of blocks for 2 weeks of work (2016 blocks).
// Litecoin: The target number of blocks for 3.5 days of work (2016 blocks).
BC_CONSTEXPR size_t retargeting_interval = target_timespan_seconds / target_spacing_seconds;

// Fork constants.
//-----------------------------------------------------------------------------

// Consensus rule change activation and enforcement parameters.
BC_CONSTEXPR size_t first_version = 1;
BC_CONSTEXPR size_t bip34_version = 2;
BC_CONSTEXPR size_t bip66_version = 3;
BC_CONSTEXPR size_t bip65_version = 4;
BC_CONSTEXPR uint32_t bip9_version_bit0 = 1u << 0;
BC_CONSTEXPR uint32_t bip9_version_bit1 = 1u << 1;
BC_CONSTEXPR uint32_t bip9_version_base = 0x20000000;

#ifdef BITPRIM_CURRENCY_BCH

struct monolith_t : built_in_t<uint64_t> {
    using base = built_in_t<uint64_t>;
    using base::base;
};

struct magnetic_anomaly_t : built_in_t<uint64_t> {
    using base = built_in_t<uint64_t>;
    using base::base;
};

struct great_wall_t : built_in_t<uint64_t> {
    using base = built_in_t<uint64_t>;
    using base::base;
};

struct graviton_t : built_in_t<uint64_t> {
    using base = built_in_t<uint64_t>;
    using base::base;
};


// // August 1, 2017 hard fork
// consensus.uahfHeight = 478558;

// // November 13, 2017 hard fork
// consensus.daaHeight = 504031;

// // November 15, 2018 hard fork
// consensus.magneticAnomalyHeight = 556766;

// // Wed, 15 May 2019 12:00:00 UTC hard fork
// consensus.greatWallActivationTime = 1557921600;

// // Nov 15, 2019 12:00:00 UTC protocol upgrade
// consensus.gravitonActivationTime = 1573819200;


// BC_CONSTEXPR size_t          bch_activation_height = 478559 //478558;            //2017-August-01   hard fork
// BC_CONSTEXPR uint32_t        bch_daa_activation_time = 1510600000;               //2017-November-13 hard fork
// BC_CONSTEXPR uint32_t        bch_monolith_activation_time = 1526400000;          //2018-May-15      hard fork
// BC_CONSTEXPR magnetic_anomaly_t bch_magnetic_anomaly_activation_time = 1542300000;  //2018-November-15 hard fork
BC_CONSTEXPR great_wall_t       bch_great_wall_activation_time = 1557921600;        //2019-May-15      hard fork
BC_CONSTEXPR graviton_t         bch_graviton_activation_time = 1573819200;          //2019-Nov-15      hard fork

// inline constexpr
// bool is_uahf_enabled(const Config &config, int nHeight) {
//     return nHeight >= config.GetChainParams().GetConsensus().uahfHeight;
// }

#endif  //BITPRIM_CURRENCY_BCH

#ifdef BITPRIM_CURRENCY_LTC

// Mainnet activation parameters (bip34-style activations).
BC_CONSTEXPR size_t mainnet_active = 750;
BC_CONSTEXPR size_t mainnet_enforce = 950;
BC_CONSTEXPR size_t mainnet_sample = 1000;

// Testnet activation parameters (bip34-style activations).
BC_CONSTEXPR size_t testnet_active = 51;
BC_CONSTEXPR size_t testnet_enforce = 75;
BC_CONSTEXPR size_t testnet_sample = 100;

// Regtest (arbitrary) frozen activation heights (frozen_activations).
BC_CONSTEXPR size_t regtest_bip65_freeze = 1351;
BC_CONSTEXPR size_t regtest_bip66_freeze = 1251;
BC_CONSTEXPR size_t regtest_bip34_freeze = 100000000;

// Mainnet frozen activation heights (frozen_activations).
BC_CONSTEXPR size_t mainnet_bip65_freeze = MAX_UINT32;  //Not Active
BC_CONSTEXPR size_t mainnet_bip66_freeze = MAX_UINT32;  //Not Active
BC_CONSTEXPR size_t mainnet_bip34_freeze = 710000;

// Testnet frozen activation heights (frozen_activations).
BC_CONSTEXPR size_t testnet_bip65_freeze = MAX_UINT32;  //Not Active
BC_CONSTEXPR size_t testnet_bip66_freeze = MAX_UINT32;  //Not Active
BC_CONSTEXPR size_t testnet_bip34_freeze = 76;          //Always Active

// Block 514 is the first testnet block after date-based activation.
// Block 173805 is the first mainnet block after date-based activation.
BC_CONSTEXPR uint32_t bip16_activation_time = 0x4f779a80;

// Block 170060 was mined with an invalid p2sh (code shipped late).
// bitcointalk.org/index.php?topic=63165.msg788832#msg788832
static const config::checkpoint mainnet_bip16_exception_checkpoint{
    "00000000000002dc756eebf4f49723ed8d30cc28a5f108eb94b1ba88ac4f9c22", 170060};

// github.com/bitcoin/bips/blob/master/bip-0030.mediawiki#specification
static const config::checkpoint mainnet_bip30_exception_checkpoint1{
    // TODO(libbitcoin): figure out why this block validates without an exception.
    "00000000000a4d0a398161ffc163c503763b1f4360639393e0e4c8e300e0caec", 91842};

static const config::checkpoint mainnet_bip30_exception_checkpoint2{
    "00000000000743f190a18c5577a3c2d2a1f610ae9601ac046a38084ccb7cd721", 91880};

// Hard fork to stop checking unspent duplicates above fixed bip34 activation.
static const config::checkpoint mainnet_bip34_active_checkpoint{
    "fa09d204a83a768ed5a7c8d441fa62f2043abf420cff1226c7b4329aeb9d51cf", 710000};

static const config::checkpoint testnet_bip34_active_checkpoint{
    "8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573", 76};

static const config::checkpoint regtest_bip34_active_checkpoint{
    "fa09d204a83a768ed5a7c8d441fa62f2043abf420cff1226c7b4329aeb9d51cf", 100000000};

#else  //BITPRIM_CURRENCY_LTC
// Mainnet activation parameters (bip34-style activations).
BC_CONSTEXPR size_t mainnet_active = 750;
BC_CONSTEXPR size_t mainnet_enforce = 950;
BC_CONSTEXPR size_t mainnet_sample = 1000;

// Testnet activation parameters (bip34-style activations).
BC_CONSTEXPR size_t testnet_active = 51;
BC_CONSTEXPR size_t testnet_enforce = 75;
BC_CONSTEXPR size_t testnet_sample = 100;

// Mainnet frozen activation heights (frozen_activations).
BC_CONSTEXPR size_t mainnet_bip65_freeze = 388381;
BC_CONSTEXPR size_t mainnet_bip66_freeze = 363725;
BC_CONSTEXPR size_t mainnet_bip34_freeze = 227931;

// Testnet frozen activation heights (frozen_activations).
BC_CONSTEXPR size_t testnet_bip65_freeze = 581885;
BC_CONSTEXPR size_t testnet_bip66_freeze = 330776;
BC_CONSTEXPR size_t testnet_bip34_freeze = 21111;

// Regtest (arbitrary) frozen activation heights (frozen_activations).
BC_CONSTEXPR size_t regtest_bip65_freeze = 1351;
BC_CONSTEXPR size_t regtest_bip66_freeze = 1251;
BC_CONSTEXPR size_t regtest_bip34_freeze = 0;

// Block 514 is the first testnet block after date-based activation.
// Block 166832 is the first mainnet block after date-based activation.
BC_CONSTEXPR uint32_t bip16_activation_time = 0x4f3af580;

// Block 170060 was mined with an invalid p2sh (code shipped late).
// bitcointalk.org/index.php?topic=63165.msg788832#msg788832
static const config::checkpoint mainnet_bip16_exception_checkpoint{
    "00000000000002dc756eebf4f49723ed8d30cc28a5f108eb94b1ba88ac4f9c22", 170060};

// github.com/bitcoin/bips/blob/master/bip-0030.mediawiki#specification
static const config::checkpoint mainnet_bip30_exception_checkpoint1{
    "00000000000a4d0a398161ffc163c503763b1f4360639393e0e4c8e300e0caec", 91842};

static const config::checkpoint mainnet_bip30_exception_checkpoint2{
    "00000000000743f190a18c5577a3c2d2a1f610ae9601ac046a38084ccb7cd721", 91880};

// bip90 stops checking unspent duplicates above this bip34 activation.
static const config::checkpoint mainnet_bip34_active_checkpoint{
    "000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8", 227931};

static const config::checkpoint testnet_bip34_active_checkpoint{
    "0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8", 21111};
static const config::checkpoint regtest_bip34_active_checkpoint{
    // Since bip90 assumes a historical bip34 activation block, use genesis.
    "06226e46111a0b59caaf126043eb5bbf28c34f3a5e332a1fc7b2b73cf188910f", 0};

#endif  //BITPRIM_CURRENCY_LTC

#ifdef BITPRIM_CURRENCY_LTC

// These cannot be reactivated in a future branch due to window expiration.
static const config::checkpoint mainnet_bip9_bit0_active_checkpoint{
    "b50ce9202c152e481ca509156028af954654ed13e4b0656eb497554aa753db0b", 1201535};
static const config::checkpoint testnet_bip9_bit0_active_checkpoint{
    "4966625a4b2851d9fdee139e56211a0d88575f59ed816ff5e6a63deb4e3e29a0", 0};
static const config::checkpoint regtest_bip9_bit0_active_checkpoint{
    // The activation window is fixed and closed, so assume genesis activation.
    "06226e46111a0b59caaf126043eb5bbf28c34f3a5e332a1fc7b2b73cf188910f", 0};
// These cannot be reactivated in a future branch due to window expiration.
static const config::checkpoint mainnet_bip9_bit1_active_checkpoint{
    "b50ce9202c152e481ca509156028af954654ed13e4b0656eb497554aa753db0b", 1201535};
static const config::checkpoint testnet_bip9_bit1_active_checkpoint{
    "4966625a4b2851d9fdee139e56211a0d88575f59ed816ff5e6a63deb4e3e29a0", 0};
static const config::checkpoint regtest_bip9_bit1_active_checkpoint{
    // The activation window is fixed and closed, so assume genesis activation.
    "06226e46111a0b59caaf126043eb5bbf28c34f3a5e332a1fc7b2b73cf188910f", 0};

#else

// These cannot be reactivated in a future branch due to window expiration.
static const config::checkpoint mainnet_bip9_bit0_active_checkpoint{
    "000000000000000004a1b34462cb8aeebd5799177f7a29cf28f2d1961716b5b5", 419328};
static const config::checkpoint testnet_bip9_bit0_active_checkpoint{
    "00000000025e930139bac5c6c31a403776da130831ab85be56578f3fa75369bb", 770112};
static const config::checkpoint regtest_bip9_bit0_active_checkpoint{
    // The activation window is fixed and closed, so assume genesis activation.
    "06226e46111a0b59caaf126043eb5bbf28c34f3a5e332a1fc7b2b73cf188910f", 0};
// These cannot be reactivated in a future branch due to window expiration.
static const config::checkpoint mainnet_bip9_bit1_active_checkpoint{
    "0000000000000000001c8018d9cb3b742ef25114f27563e3fc4a1902167f9893", 481824};
static const config::checkpoint testnet_bip9_bit1_active_checkpoint{
    "00000000002b980fcd729daaa248fd9316a5200e9b367f4ff2c42453e84201ca", 834624};
static const config::checkpoint regtest_bip9_bit1_active_checkpoint{
    // The activation window is fixed and closed, so assume genesis activation.
    "06226e46111a0b59caaf126043eb5bbf28c34f3a5e332a1fc7b2b73cf188910f", 0};

#endif



#ifdef BITPRIM_CURRENCY_BCH

//2017-August-01 hard fork
//TODO(fernando): remove this commented lines (old code)
// static const config::checkpoint mainnet_uahf_active_checkpoint{
//     "000000000000000000651ef99cb9fcbe0dadde1d424bd9f15ff20136191a5eec", 478559};

// static const config::checkpoint testnet_uahf_active_checkpoint{
//     "00000000000e38fef93ed9582a7df43815d5c2ba9fd37ef70c9a0ea4a285b8f5", 1155876};

static const config::checkpoint mainnet_uahf_active_checkpoint{
    "0000000000000000011865af4122fe3b144e2cbeea86142e8ff2fb4107352d43", 478558};

static const config::checkpoint testnet_uahf_active_checkpoint{
    "00000000000e38fef93ed9582a7df43815d5c2ba9fd37ef70c9a0ea4a285b8f5", 1155876};

//2017-November-13 hard fork
static const config::checkpoint mainnet_daa_active_checkpoint{
    "0000000000000000011ebf65b60d0a3de80b8175be709d653b4c1a1beeb6ab9c", 504031};

static const config::checkpoint testnet_daa_active_checkpoint{
    "0000000000170ed0918077bde7b4d36cc4c91be69fa09211f748240dabe047fb", 1188697};

//2018-May hard fork
static const config::checkpoint mainnet_monolith_active_checkpoint{
    "0000000000000000011ada8bd08f46074f44a8f155396f43e38acf9501c49103", 530359};

static const config::checkpoint testnet_monolith_active_checkpoint{
    "0000000000327972b8470c11755adf8f4319796bafae01f5a6650490b98a17db", 1233078};

//2018-November hard fork
static const config::checkpoint mainnet_magnetic_anomaly_active_checkpoint{
    "0000000000000000004626ff6e3b936941d341c5932ece4357eeccac44e6d56c", 556767};

static const config::checkpoint testnet_magnetic_anomaly_active_checkpoint{
    "00000000000001fae0095cd4bea16f1ce8ab63f3f660a03c6d8171485f484b24", 1267996};


        // // August 1, 2017 hard fork
        // consensus.uahfHeight = 1155875;

        // // November 13, 2017 hard fork
        // consensus.daaHeight = 1188697;

        // // November 15, 2018 hard fork
        // consensus.magneticAnomalyHeight = 1267996;

        // // Wed, 15 May 2019 12:00:00 UTC hard fork
        // consensus.greatWallActivationTime = 1557921600;

        // // Nov 15, 2019 12:00:00 UTC protocol upgrade
        // consensus.gravitonActivationTime = 1573819200;



//2019-May hard fork
// Complete after the hard fork

//2019-November hard fork
// Complete after the hard fork

#endif

// Network protocol constants.
//-----------------------------------------------------------------------------

// Explicit size.
BC_CONSTEXPR size_t command_size = 12;

// Explicit limits.
BC_CONSTEXPR size_t max_address = 1000;
BC_CONSTEXPR size_t max_filter_add = 520;
BC_CONSTEXPR size_t max_filter_functions = 50;
BC_CONSTEXPR size_t max_filter_load = 36000;
BC_CONSTEXPR size_t max_get_blocks = 500;
BC_CONSTEXPR size_t max_get_headers = 2000;
BC_CONSTEXPR size_t max_get_data = 50000;
BC_CONSTEXPR size_t max_inventory = 50000;

BC_CONSTEXPR size_t max_payload_size = 33554432;
/**
 * The minimum safe length of a seed in bits (multiple of 8).
 */
BC_CONSTEXPR size_t minimum_seed_bits = 128;

/**
 * The minimum safe length of a seed in bytes (16).
 */
BC_CONSTEXPR size_t minimum_seed_size = minimum_seed_bits / byte_bits;

// Effective limit given a 32 bit chain height boundary: 10 + log2(2^32) + 1.
BC_CONSTEXPR size_t max_locator = 43;

// Defined <bitcoin/infrastructure/constants.hpp>
// // Variable integer prefix sentinels.
// BC_CONSTEXPR uint8_t varint_two_bytes = 0xfd;
// BC_CONSTEXPR uint8_t varint_four_bytes = 0xfe;
// BC_CONSTEXPR uint8_t varint_eight_bytes = 0xff;

// // String padding sentinel.
// BC_CONSTEXPR uint8_t string_terminator = 0x00;

// Witness serialization values (bip141).
//-----------------------------------------------------------------------------

BC_CONSTEXPR uint8_t witness_marker = 0x00;
BC_CONSTEXPR uint8_t witness_flag = 0x01;
BC_CONSTEXPR uint32_t witness_head = 0xaa21a9ed;
BC_CONSTEXPR size_t fast_sigops_factor = 4;
BC_CONSTEXPR size_t max_fast_sigops = fast_sigops_factor * get_max_block_sigops();
BC_CONSTEXPR size_t light_weight_factor = 4;
BC_CONSTEXPR size_t max_block_weight = light_weight_factor * get_max_block_size();
BC_CONSTEXPR size_t base_size_contribution = 3;
BC_CONSTEXPR size_t total_size_contribution = 1;
BC_CONSTEXPR size_t min_witness_program = 2;
BC_CONSTEXPR size_t max_witness_program = 40;

constexpr inline
size_t get_max_block_weight() noexcept {
#ifdef BITPRIM_CURRENCY_BCH
    return get_max_block_size(); 
#else
    return max_block_weight;
#endif //BITPRIM_CURRENCY_BCH
}


// Currency unit constants (uint64_t).
//-----------------------------------------------------------------------------

BC_CONSTFUNC inline uint64_t max_money_recursive(uint64_t money) {
    return money > 0 ? money + max_money_recursive(money >> 1) : 0;
}

BC_CONSTEXPR uint64_t satoshi_per_bitcoin = 100000000;

BC_CONSTFUNC inline uint64_t bitcoin_to_satoshi(uint64_t bitcoin_uints = 1) {
    return bitcoin_uints * satoshi_per_bitcoin;
}

BC_CONSTEXPR uint64_t initial_block_subsidy_bitcoin = 50;

BC_CONSTFUNC inline uint64_t initial_block_subsidy_satoshi() {
    return bitcoin_to_satoshi(initial_block_subsidy_bitcoin);
}

#ifdef BITPRIM_CURRENCY_LTC
// BC_CONSTEXPR uint64_t reward_interval = 840000;
BC_CONSTEXPR uint64_t retarget_subsidy_interval = 840000;
BC_CONSTEXPR uint64_t no_retarget_subsidy_interval = 150;  // TODO(libbitcoin): merge version3. check the no_retarget (regtest) value for LTC
#else                                                      //BITPRIM_CURRENCY_LTC
// BC_CONSTEXPR uint64_t reward_interval = 210000;
BC_CONSTEXPR uint64_t retarget_subsidy_interval = 210000;
BC_CONSTEXPR uint64_t no_retarget_subsidy_interval = 150;
#endif                                                     //BITPRIM_CURRENCY_LTC

BC_CONSTEXPR uint64_t recursive_money = 0x02540be3f5;

BC_CONSTFUNC uint64_t subsidy_interval(bool retarget = true) {
    return retarget ? retarget_subsidy_interval : no_retarget_subsidy_interval;
}

BC_CONSTFUNC uint64_t max_money(bool retarget = true) {
    ////// Optimize out the derivation of recursive_money.
    ////BITCOIN_ASSERT(recursive_money == max_money_recursive(
    ////    initial_block_subsidy_satoshi()));

    return recursive_money * subsidy_interval(retarget);
}

}  // namespace libbitcoin

#endif

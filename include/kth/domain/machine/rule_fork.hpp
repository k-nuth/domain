// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MACHINE_RULE_FORK_HPP_
#define KTH_MACHINE_RULE_FORK_HPP_

#include <cstdint>

namespace kth {
namespace machine {

enum rule_fork : uint32_t {
    no_rules = 0,

    /// Allow minimum difficulty blocks (hard fork, testnet).
    easy_blocks = 1U << 0,

    /// Pay-to-script-hash enabled (soft fork, feature).
    bip16_rule = 1U << 1,

    /// No duplicated unspent transaction ids (hard fork, security).
    bip30_rule = 1U << 2,

    /// Coinbase must include height (soft fork, security).
    bip34_rule = 1U << 3,

    /// Strict DER signatures required (soft fork, security).
    bip66_rule = 1U << 4,

    /// Operation nop2 becomes check locktime verify (soft fork, feature).
    bip65_rule = 1U << 5,

    /// Hard code bip34-based activation heights (hard fork, optimization).
    bip90_rule = 1U << 6,

    /// Assume hash collisions cannot happen (hard fork, optimization).
    allow_collisions = 1U << 7,

    /// Enforce relative locktime (soft fork, feature).
    bip68_rule = 1U << 8,

    /// Operation nop3 becomes check sequence verify (soft fork, feature).
    bip112_rule = 1U << 9,

    /// Use median time past for locktime (soft fork, feature).
    bip113_rule = 1U << 10,

    //TODO(fernando): check if we have to cancel the following values for coins not equal to BCH
// #ifdef KTH_CURRENCY_BCH
<<<<<<< HEAD:include/bitcoin/bitcoin/machine/rule_fork.hpp
    cash_low_s_rule = 1u << 15,
    // cash_monolith_opcodes = 1u << 16,
    cash_verify_flags_script_enable_sighash_forkid = 1u << 16,
    cash_replay_protection = 1u << 17,
    cash_checkdatasig = 1u << 18,
    cash_schnorr = 1u << 19,
    cash_segwit_recovery = 1u << 20,
=======
    cash_low_s_rule = 1U << 15,
    // cash_monolith_opcodes = 1u << 16,
    cash_verify_flags_script_enable_sighash_forkid = 1U << 16,
    cash_replay_protection = 1U << 17,
    cash_checkdatasig = 1U << 18,
    cash_schnorr = 1U << 19,
    cash_segwit_recovery = 1U << 20,
>>>>>>> dev:include/kth/domain/machine/rule_fork.hpp
// #endif //KTH_CURRENCY_BCH

    /// Segregated witness consensus layer (soft fork, feature).
    bip141_rule = 1U << 11,

    /// Segregated witness v0 verification (soft fork, feature).
    bip143_rule = 1U << 12,

    /// Prevent dummy value malleability (soft fork, feature).
    bip147_rule = 1U << 13,

    /// Perform difficulty retargeting (hard fork, regtest).
    retarget = 1U << 30,

    /// Sentinel bit to indicate tx has not been validated.
    unverified = 1U << 31,

    /// Rules that use bip34-based activation.
    bip34_activations =
        rule_fork::bip34_rule |
        rule_fork::bip65_rule |
        rule_fork::bip66_rule,

    /// Rules that use BIP9 bit zero first time activation.
    bip9_bit0_group =
        rule_fork::bip68_rule |
        rule_fork::bip112_rule |
        rule_fork::bip113_rule,

    /// Rules that use BIP9 bit one first time activation.
    bip9_bit1_group =
        rule_fork::bip141_rule |
        rule_fork::bip143_rule |
        rule_fork::bip147_rule,

    /// Simple mask to set all bits.
    all_rules = 0xffffffff
};

} // namespace machine
} // namespace kth

#endif // KTH_MACHINE_RULE_FORK_HPP_

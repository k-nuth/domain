// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CHAIN_CHAIN_STATE_HPP
#define KTH_DOMAIN_CHAIN_CHAIN_STATE_HPP

#include <cstddef>
#include <cstdint>
#include <deque>
#include <memory>

#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/machine/opcode.hpp>
#include <kth/domain/machine/rule_fork.hpp>
#include <kth/infrastructure/config/checkpoint.hpp>
#include <kth/infrastructure/math/hash.hpp>

namespace kth::domain::chain {

class block;
class header;

class KD_API chain_state {
public:
    using bitss = std::deque<uint32_t>;                 //TODO(fernando): why deque?
    using versions = std::deque<uint32_t>;              //TODO(fernando): why deque?
    using timestamps = std::deque<uint32_t>;            //TODO(fernando): why deque?
    using range = struct {size_t count; size_t high;};
    using ptr = std::shared_ptr<chain_state>;
    using checkpoints = infrastructure::config::checkpoint::list;

    /// Heights used to identify construction requirements.
    /// All values are lower-bounded by the genesis block height.
    /// Obtaining all values even in the case where the set of queries could be
    /// short-circuited allows us to cache results for the next block,
    /// minimizing overall querying.
    struct map {
        // This sentinel indicates that the value was not requested.
        static
        const size_t unrequested = max_size_t;

        /// [block - 1, floor(block - 2016, 0)] mainnet: 1, testnet: 2016|0
        range bits;

        /// (block - 0), used only for populating bits.ordered on increment.
        size_t bits_self;

        /// [block - 1, floor(block - 1000, 0)] mainnet: 1000, testnet: 100
        range version;

        /// (block - 0)
        size_t version_self;

        /// [block - 1, floor(block - 11, 0)]
        range timestamp;

        /// (block - 0)
        size_t timestamp_self;

        /// (block - (block % 2016 == 0 ? 2016 : block % 2016))
        size_t timestamp_retarget;

        /// mainnet: 227931, testnet: 21111 (or map::unrequested)
        size_t allow_collisions_height;

        /// mainnet: 419328, testnet: 770112 (or map::unrequested)
        size_t bip9_bit0_height;

        /// mainnet: 481824, testnet: 834624 (or map::unrequested)
        size_t bip9_bit1_height;
    };

    /// Values used to populate chain state at the target height.
    struct data {
        /// Header values are based on this height.
        size_t height;

        /// Hash of the candidate block or null_hash for memory pool.
        hash_digest hash;

        /// Hash of the allow_collisions block or null_hash if unrequested.
        hash_digest allow_collisions_hash;

        /// Hash of the bip9_bit0 block or null_hash if unrequested.
        hash_digest bip9_bit0_hash;

        /// Hash of the bip9_bit1 block or null_hash if unrequested.
        hash_digest bip9_bit1_hash;

        /// Values must be ordered by height with high (block - 1) last.
        struct {
            uint32_t self;
            bitss ordered;
        } bits;

        /// Values must be ordered by height with high (block - 1) last.
        struct {
            uint32_t self;
            versions ordered;
        } version;

        /// Values must be ordered by height with high (block - 1) last.
        struct {
            uint32_t self;
            uint32_t retarget;
            timestamps ordered;
        } timestamp;
    };

    /// Checkpoints must be ordered by height with greatest at back.
    /// Forks and checkpoints must match those provided for map creation.
    chain_state(data&& values, uint32_t forks, checkpoints const& checkpoints
#ifdef KTH_CURRENCY_BCH
                // , magnetic_anomaly_t magnetic_anomaly_activation_time
                // , great_wall_t great_wall_activation_time
                // , graviton_t graviton_activation_time
                , phonon_t phonon_activation_time
                , axion_t axion_activation_time
#endif  //KTH_CURRENCY_BCH
    );

    // Named constructors

    static
    chain_state from_top(chain_state const& top);

    static
    chain_state from_pool(chain_state const& pool, block const& block);

    static
    chain_state from_parent(chain_state const& parent, header const& header);



    //TODO(fernando): if I delete the copy the Linter complains  
    // // non-copyable and non-movable class
    // chain_state(chain_state const&) = delete;               //NOLINT
    // chain_state& operator=(chain_state const&) = delete;    //NOLINT


    // /// Create pool state from top chain top block state.
    // chain_state(chain_state const& top);

    // /// Create block state from tx pool chain state of same height.
    // chain_state(chain_state const& pool, const chain::block& block);

    // /// Create header state from header pool chain state of previous height.
    // chain_state(chain_state const& parent, chain::header const& header);



    /// Checkpoints must be ordered by height with greatest at back.
    static
    map get_map(size_t height, checkpoints const& checkpoints, uint32_t forks);

    static
    uint32_t signal_version(uint32_t forks);

    /// Properties.
    [[nodiscard]]
    size_t height() const;

    [[nodiscard]]
    uint32_t enabled_forks() const;

    [[nodiscard]]
    uint32_t minimum_version() const;

    [[nodiscard]]
    uint32_t median_time_past() const;

    [[nodiscard]]
    uint32_t work_required() const;

#ifdef KTH_CURRENCY_BCH
    // [[nodiscard]]
    // magnetic_anomaly_t magnetic_anomaly_activation_time() const;
    
    // [[nodiscard]]
    // great_wall_t great_wall_activation_time() const;

    // [[nodiscard]]
    // graviton_t graviton_activation_time() const;
    
    [[nodiscard]]
    phonon_t phonon_activation_time() const;

    [[nodiscard]]
    axion_t axion_activation_time() const;
#endif  //KTH_CURRENCY_BCH

    /// Construction with zero height or any empty array causes invalid state.
    [[nodiscard]]
    bool is_valid() const;

    /// Determine if the fork is set for this block.
    [[nodiscard]]
    bool is_enabled(machine::rule_fork fork) const;

    /// Determine if this block hash fails a checkpoint at this height.
    [[nodiscard]]
    bool is_checkpoint_conflict(hash_digest const& hash) const;

    /// This block height is less than or equal to that of the top checkpoint.
    [[nodiscard]]
    bool is_under_checkpoint() const;

    static
    bool is_retarget_height(size_t height);  //Need to be public, for Litecoin

#ifdef KTH_CURRENCY_BCH
    static
    uint256_t difficulty_adjustment_cash(uint256_t const& target);
#endif  //KTH_CURRENCY_BCH

    uint32_t get_next_work_required(uint32_t time_now);

#ifdef KTH_CURRENCY_BCH
    static
    bool is_mtp_activated(uint32_t median_time_past, uint32_t activation_time);

    [[nodiscard]]
    bool is_monolith_enabled() const;

    [[nodiscard]]
    bool is_magnetic_anomaly_enabled() const;

    [[nodiscard]]
    bool is_great_wall_enabled() const;

    [[nodiscard]]
    bool is_graviton_enabled() const;

    [[nodiscard]]
    bool is_phonon_enabled() const;

    [[nodiscard]]
    bool is_axion_enabled() const;
#endif  //KTH_CURRENCY_BCH

protected:
    struct activations {
        // The forks that are active at this height.
        uint32_t forks;

        // The minimum block version required at this height.
        uint32_t minimum_version;
    };

    static
    activations activation(data const& values, uint32_t forks
#ifdef KTH_CURRENCY_BCH
            // , magnetic_anomaly_t magnetic_anomaly_activation_time
            // , great_wall_t great_wall_activation_time
            // , graviton_t graviton_activation_time
            , phonon_t phonon_activation_time
            , axion_t axion_activation_time
#endif  //KTH_CURRENCY_BCH
    );

    static
    uint32_t median_time_past(data const& values, uint32_t forks, bool tip = true);

    // static
    // uint32_t work_required(data const& values, uint32_t forks, bool bitcoin_cash = false);

    static
    uint32_t work_required(data const& values, uint32_t forks);

private:
    static
    size_t bits_count(size_t height, uint32_t forks);
    
    static
    size_t version_count(size_t height, uint32_t forks);

    static
    size_t timestamp_count(size_t height, uint32_t forks);

    // TODO(kth): make function private again. Moved to public in the litecoin merge
    static
    size_t retarget_height(size_t height, uint32_t forks);

    static
    size_t collision_height(size_t height, uint32_t forks);

    static
    size_t bip9_bit0_height(size_t height, uint32_t forks);

    static
    size_t bip9_bit1_height(size_t height, uint32_t forks);

    // static
    // size_t uahf_height(size_t height, uint32_t forks);

    // static
    // size_t daa_height(size_t height, uint32_t forks);

    static
    bool is_rule_enabled(size_t height, uint32_t forks, size_t mainnet_height, size_t testnet_height);
    
    // ------------------------------------------------------------------------
#ifdef KTH_CURRENCY_BCH
    static
    bool is_uahf_enabled(size_t height, uint32_t forks);
    
    static
    bool is_daa_enabled(size_t height, uint32_t forks);
    
    static
    bool is_monolith_enabled(size_t height, uint32_t forks);
    
    static
    bool is_magnetic_anomaly_enabled(size_t height, uint32_t forks);
    
    static
    bool is_great_wall_enabled(size_t height, uint32_t forks);

    static
    bool is_graviton_enabled(size_t height, uint32_t forks);
    
    // static
    // bool is_phonon_enabled(size_t height, uint32_t forks);

    // static
    // bool is_axion_enabled(size_t height, uint32_t forks);
#endif // KTH_CURRENCY_BCH
    // ------------------------------------------------------------------------

    static
    data to_pool(chain_state const& top);
    
    static
    data to_block(chain_state const& pool, block const& block);
    
    static
    data to_header(chain_state const& parent, header const& header);

    static
    uint32_t work_required_retarget(data const& values);

    static
    uint32_t retarget_timespan(chain_state::data const& values);

    // TODO(kth): make function private again. Moved to public in the litecoin merge
    // static
    // bool is_retarget_height(size_t height);

    // easy blocks
    //TODO(kth):

#ifdef KTH_CURRENCY_BCH
    static
    uint32_t cash_difficulty_adjustment(data const& values);
    
    static
    uint32_t work_required_adjust_cash(data const& values);
#endif  //KTH_CURRENCY_BCH

    static
    uint32_t work_required_easy(data const& values);
    
    static
    uint32_t elapsed_time_limit(chain_state::data const& values);
    
    static
    bool is_retarget_or_non_limit(size_t height, uint32_t bits);

    static
    uint32_t easy_work_required(data const& values, bool daa_active);
    
    static
    uint32_t easy_time_limit(chain_state::data const& values);
    
    static
    size_t retarget_distance(size_t height);

    // This is retained as an optimization for other constructions.
    // A similar height clone can be partially computed, reducing query cost.
    data const data_;

    // Configured forks are saved for state transitions.
    uint32_t const forks_;

    // Checkpoints do not affect the data that is collected or promoted.
    config::checkpoint::list const& checkpoints_;

    // These are computed on construct from sample and checkpoints.
    activations const active_;
    uint32_t const median_time_past_;
    uint32_t const work_required_;

//TODO(fernando): inherit BCH data and functions for a specific BCH class
#ifdef KTH_CURRENCY_BCH
    // magnetic_anomaly_t const magnetic_anomaly_activation_time_;
    // great_wall_t const great_wall_activation_time_;
    // graviton_t const graviton_activation_time_;
    phonon_t const phonon_activation_time_;
    axion_t const axion_activation_time_;
#endif  //KTH_CURRENCY_BCH
};

}  // namespace kth::chain

#endif

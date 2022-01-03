// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CHAIN_HISTORY_HPP
#define KTH_DOMAIN_CHAIN_HISTORY_HPP

#include <cstdint>
#include <vector>

#include <kth/domain/chain/input_point.hpp>
#include <kth/domain/chain/output_point.hpp>
#include <kth/domain/chain/point.hpp>
#include <kth/domain/define.hpp>

namespace kth::domain::chain {

/// Use "kind" for union differentiation.
enum class point_kind : uint32_t {
    output = 0,
    spend = 1
};

/// This structure models the client-server protocol in v1/v2/v3.
struct KD_API history_compact {
    using list = std::vector<history_compact>;

    // The type of point (output or spend).
    point_kind kind;

    /// The point that identifies the record.
    chain::point point;

    /// The height of the point.
    uint32_t height;

    union {
        /// If output, then satoshi value of output.
        uint64_t value;

        /// If spend, then checksum hash of previous output point
        /// To match up this row with the output, recompute the
        /// checksum from the output row with spend_checksum(row.point)
        uint64_t previous_checksum;
    };
};

/// This structure is used between client and API callers in v3.
/// This structure models the client-server protocol in v1/v2.
/// The height values here are 64 bit, but 32 bits on the wire.
struct KD_API history {
    using list = std::vector<history>;

    /// If there is no output this is null_hash:max.
    output_point output;
    uint64_t output_height;

    /// The satoshi value of the output.
    uint64_t value;

    /// If there is no spend this is null_hash:max.
    input_point spend;

    union {
        /// The height of the spend or max if no spend.
        uint64_t spend_height;

        /// During expansion this value temporarily doubles as a checksum.
        uint64_t temporary_checksum;
    };
};

} // namespace kth::domain::chain

#endif

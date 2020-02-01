// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CHAIN_POINTS_VALUE_HPP
#define KTH_CHAIN_POINTS_VALUE_HPP

#include <cstdint>
#include <numeric>

#include <bitcoin/bitcoin/chain/point_value.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>

namespace libbitcoin {
namespace chain {

class BC_API points_value {
public:
    /// A set of valued points.
    point_value::list points;

    /// Total value of the current set of points.
    uint64_t value() const;
};

}  // namespace chain
}  // namespace kth

#endif

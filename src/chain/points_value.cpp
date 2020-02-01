// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <cstdint>
#include <numeric>

#include <bitcoin/bitcoin/chain/point_value.hpp>
#include <bitcoin/bitcoin/chain/points_value.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>

namespace libbitcoin {
namespace chain {

uint64_t points_value::value() const {
    auto const sum = [](uint64_t total, point_value const& point) {
        return safe_add(total, point.value());
    };

    return std::accumulate(points.begin(), points.end(), uint64_t(0), sum);
}

}  // namespace chain
}  // namespace kth

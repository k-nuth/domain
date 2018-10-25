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
#include <bitcoin/bitcoin/wallet/select_outputs.hpp>

#include <algorithm>
#include <cstdint>

#include <bitcoin/bitcoin/chain/points_value.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/infrastructure/utility/assert.hpp>

namespace libbitcoin {
namespace wallet {

using namespace bc::chain;

void select_outputs::greedy(points_value& out, const points_value& unspent, uint64_t minimum_value) {
    out.points.clear();

    // The minimum required value does not exist.
    if (unspent.value() < minimum_value) {
        return;
    }

    // Optimization for simple case not requiring search.
    if (unspent.points.size() == 1) {
        out.points.push_back(unspent.points.front());
        return;
    }

    // Copy the points list for safe manipulation.
    auto copy = unspent.points;

    auto const below = [minimum_value](point_value const& point) {
        return point.value() < minimum_value;
    };

    auto const lesser = [](point_value const& x, point_value const& y) {
        return x.value() < y.value();
    };

    auto const greater = [](point_value const& x, point_value const& y) {
        return x.value() > y.value();
    };

    // Reorder list beteen values that exceed minimum and those that do not.
    auto const sufficient = std::partition(copy.begin(), copy.end(), below);

    // If there are values large enough, return the smallest (of the largest).
    auto const minimum = std::min_element(sufficient, copy.end(), lesser);

    if (minimum != copy.end()) {
        out.points.push_back(*minimum);
        return;
    }

    // Sort all by descending value in order to use the fewest inputs possible.
    std::sort(copy.begin(), copy.end(), greater);

    // This is naive, will not necessarily find the smallest combination.
    // for (auto point = copy.begin(); point != copy.end(); ++point) {
    for (auto const& point : copy) {
        out.points.push_back(point);

        if (out.value() >= minimum_value) {
            return;
        }
    }

    BITCOIN_ASSERT_MSG(false, "unreachable code reached");
}

void select_outputs::individual(points_value& out, const points_value& unspent, uint64_t minimum_value) {
    out.points.clear();
    out.points.reserve(unspent.points.size());

    // Select all individual points that satisfy the minimum.
    for (auto const& point : unspent.points) {
        if (point.value() >= minimum_value) {
            out.points.push_back(point);
        }
    }

    auto const lesser = [](point_value const& x, point_value const& y) {
        return x.value() < y.value();
    };

    // Return in ascending order by value.
    out.points.shrink_to_fit();
    std::sort(out.points.begin(), out.points.end(), lesser);
}

void select_outputs::select(points_value& out, const points_value& unspent, uint64_t minimum_value, algorithm option) {
    switch (option) {
        case algorithm::individual: {
            individual(out, unspent, minimum_value);
            break;
        }
        case algorithm::greedy:
        default: {
            greedy(out, unspent, minimum_value);
            break;
        }
    }
}

}  // namespace wallet
}  // namespace libbitcoin

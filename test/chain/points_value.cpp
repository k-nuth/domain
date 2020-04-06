// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::chain;

// Start Boost Suite: points value tests

TEST_CASE("points value  value  default  zero", "[points value]") {
    static const points_value instance{};
    REQUIRE(instance.value() == 0u);
}

TEST_CASE("points value  value  single point value  expected", "[points value]") {
    static auto const expected = 42u;
    static const points_value instance{{{{null_hash, 0u}, expected}}};
    REQUIRE(instance.value() == expected);
}

TEST_CASE("points value  value  multiple point values  expected", "[points value]") {
    static auto const expected = 10u + 30u + 42u;
    static const points_value instance{
        {
            {{null_hash, 0u}, 10u},
            {{null_hash, 1u}, 30u},
            {{null_hash, 0u}, 42u},
        }};
    BOOST_REQUIRE_EQUAL(instance.value(), expected);
}

BOOST_AUTO_TEST_CASE(points_value__value__multiple_point_values_remove_last__expected) {
    static auto const expected = 10u + 30u;
    points_value instance{
        {
            {{null_hash, 0u}, 10u},
            {{null_hash, 1u}, 30u},
            {{null_hash, 0u}, 42u},
        }};
    instance.points.pop_back();
    BOOST_REQUIRE_EQUAL(instance.value(), expected);
}

BOOST_AUTO_TEST_CASE(points_value__value__multiple_point_values_clear__zero) {
    points_value instance{
        {
            {{null_hash, 0u}, 10u},
            {{null_hash, 1u}, 30u},
            {{null_hash, 0u}, 42u},
        }};
    instance.points.clear();
    BOOST_REQUIRE_EQUAL(instance.value(), 0u);
}

BOOST_AUTO_TEST_SUITE_END()

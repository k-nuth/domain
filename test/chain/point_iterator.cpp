// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::chain;

#define SOURCE "000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f0100"
static auto const valid_raw_point_iterator_source = to_chunk(base16_literal(SOURCE));

BOOST_AUTO_TEST_SUITE(point_iterator_tests)

BOOST_AUTO_TEST_CASE(point_iterator__operator_bool__not_at_end__returns_true) {
    point_iterator instance(point{});
    BOOST_REQUIRE((bool)instance);
}

BOOST_AUTO_TEST_CASE(point_iterator__operator_bool_at_end__returns_false) {
    point value;
    point_iterator instance(value, static_cast<unsigned>(value.serialized_size(false)));
    BOOST_REQUIRE(!instance);
}

BOOST_AUTO_TEST_CASE(point_iterator__operator_asterisk__initialized_point__matches_source) {
    point point;
    BOOST_REQUIRE(entity_from_data(point, valid_raw_point_iterator_source, false));
    point_iterator instance(point);

    for (size_t i = 0; i < valid_raw_point_iterator_source.size(); i++, instance++) {
        BOOST_REQUIRE(instance);
        BOOST_REQUIRE_EQUAL(valid_raw_point_iterator_source[i], (*instance));
    }

    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE_EQUAL(0u, (*instance));
}

BOOST_AUTO_TEST_CASE(point_iterator__operator_arrow__initialized_point__matches_source) {
    point point;
    BOOST_REQUIRE(entity_from_data(point, valid_raw_point_iterator_source, false));
    point_iterator instance(point);
    BOOST_REQUIRE(valid_raw_point_iterator_source.size() > 0);

    for (size_t i = 0; i < valid_raw_point_iterator_source.size(); i++, instance++) {
        BOOST_REQUIRE(instance);
        BOOST_REQUIRE_EQUAL(valid_raw_point_iterator_source[i], instance.operator->());
    }

    BOOST_REQUIRE(!instance);
    BOOST_REQUIRE_EQUAL(0u, instance.operator->());
}

BOOST_AUTO_TEST_CASE(point_iterator__operator_plus_minus_int__roundtrip__success) {
    point point;
    uint8_t offset = 5u;
    BOOST_REQUIRE(entity_from_data(point, valid_raw_point_iterator_source, false));

    point_iterator instance(point, offset);
    point_iterator expected(instance);

    auto initial = instance++;
    BOOST_REQUIRE(instance != expected);
    BOOST_REQUIRE(initial == expected);

    auto modified = instance--;
    BOOST_REQUIRE(instance == expected);
    BOOST_REQUIRE(modified != expected);
}

BOOST_AUTO_TEST_CASE(point_iterator__operator_plus_minus__roundtrip__success) {
    point point;
    uint8_t offset = 5u;
    BOOST_REQUIRE(point.from_data(valid_raw_point_iterator_source, false));

    point_iterator instance(point, offset);
    point_iterator expected(instance);

    ++instance;
    BOOST_REQUIRE(instance != expected);

    --instance;
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(point_iterator__copy_assigment) {
    point point;
    uint8_t offset = 5u;
    BOOST_REQUIRE(point.from_data(valid_raw_point_iterator_source, false));

    point_iterator instance(point, offset);
    point_iterator expected(instance);

    instance = expected;
}

BOOST_AUTO_TEST_SUITE_END()

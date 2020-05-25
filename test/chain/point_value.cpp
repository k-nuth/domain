// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>


using namespace kth;
using namespace kd;
using namespace kth::domain::chain;

BOOST_AUTO_TEST_SUITE(point_value_tests)

static auto const hash1 = hash_literal(
    "000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");

BOOST_AUTO_TEST_CASE(point_value__default_constructor__always__zero_value) {
    static point_value const instance;
    BOOST_REQUIRE(instance.hash() == null_hash);
    BOOST_REQUIRE_EQUAL(instance.index(), 0u);
    BOOST_REQUIRE_EQUAL(instance.value(), 0u);
}

BOOST_AUTO_TEST_CASE(point_value__move_constructor__always__expected) {
    point_value other{{hash1, 42}, 34};
    point_value const instance(std::move(x));
    BOOST_REQUIRE(instance.hash() == hash1);
    BOOST_REQUIRE_EQUAL(instance.index(), 42u);
    BOOST_REQUIRE_EQUAL(instance.value(), 34u);
}

BOOST_AUTO_TEST_CASE(point_value__copy_constructor__always__expected) {
    static point_value const other{{hash1, 42}, 34};
    point_value const instance(x);
    BOOST_REQUIRE(instance.hash() == hash1);
    BOOST_REQUIRE_EQUAL(instance.index(), 42u);
    BOOST_REQUIRE_EQUAL(instance.value(), 34u);
}

BOOST_AUTO_TEST_CASE(point_value__constructor4__always__expected) {
    point foo{hash1, 42};
    static point_value const instance(std::move(foo), 34);
    BOOST_REQUIRE(instance.hash() == hash1);
    BOOST_REQUIRE_EQUAL(instance.index(), 42u);
    BOOST_REQUIRE_EQUAL(instance.value(), 34u);
}

BOOST_AUTO_TEST_CASE(point_value__constructor5__always__expected) {
    static point const foo{hash1, 42};
    static point_value const instance(foo, 34);
    BOOST_REQUIRE(instance.hash() == hash1);
    BOOST_REQUIRE_EQUAL(instance.index(), 42u);
    BOOST_REQUIRE_EQUAL(instance.value(), 34u);
}

BOOST_AUTO_TEST_CASE(point_value__move_assign__always__expected) {
    point_value other{{hash1, 42}, 34};
    auto const instance = std::move(x);
    BOOST_REQUIRE(instance.hash() == hash1);
    BOOST_REQUIRE_EQUAL(instance.index(), 42u);
    BOOST_REQUIRE_EQUAL(instance.value(), 34u);
}

BOOST_AUTO_TEST_CASE(point_value__copy_assign__always__expected) {
    static point_value const other{{hash1, 42}, 34};
    auto const instance = other;
    BOOST_REQUIRE(instance.hash() == hash1);
    BOOST_REQUIRE_EQUAL(instance.index(), 42u);
    BOOST_REQUIRE_EQUAL(instance.value(), 34u);
}

BOOST_AUTO_TEST_CASE(point_value__swap__always__expected_reversal) {
    point_value instance1{{hash1, 42}, 34};
    point_value instance2{{null_hash, 43}, 35};

    // Must be unqualified (no std namespace).
    swap(instance1, instance2);

    BOOST_CHECK(instance2.hash() == hash1);
    BOOST_CHECK_EQUAL(instance2.index(), 42u);
    BOOST_CHECK_EQUAL(instance2.value(), 34u);

    BOOST_CHECK(instance1.hash() == null_hash);
    BOOST_CHECK_EQUAL(instance1.index(), 43u);
    BOOST_CHECK_EQUAL(instance1.value(), 35u);
}

BOOST_AUTO_TEST_CASE(point_value__equality__same__true) {
    static point_value const instance1{{hash1, 42}, 34};
    static point_value const instance2{{hash1, 42}, 34};
    BOOST_REQUIRE(instance1 == instance2);
}

BOOST_AUTO_TEST_CASE(point_value__equality__different_by_hash__false) {
    static point_value const instance1{{hash1, 42}, 34};
    static point_value const instance2{{null_hash, 43}, 34};
    BOOST_REQUIRE(!(instance1 == instance2));
}

BOOST_AUTO_TEST_CASE(point_value__equality__different_by_index__false) {
    static point_value const instance1{{hash1, 42}, 34};
    static point_value const instance2{{hash1, 43}, 34};
    BOOST_REQUIRE(!(instance1 == instance2));
}

BOOST_AUTO_TEST_CASE(point_value__equality__different_by_value__false) {
    static point_value const instance1{{hash1, 42}, 34};
    static point_value const instance2{{hash1, 42}, 35};
    BOOST_REQUIRE(!(instance1 == instance2));
}

BOOST_AUTO_TEST_CASE(point_value__inequality__same__false) {
    static point_value const instance1{{hash1, 42}, 34};
    static point_value const instance2{{hash1, 42}, 34};
    BOOST_REQUIRE(!(instance1 != instance2));
}

BOOST_AUTO_TEST_CASE(point_value__inequality__different_by_hash__true) {
    static point_value const instance1{{hash1, 42}, 34};
    static point_value const instance2{{null_hash, 43}, 34};
    BOOST_REQUIRE(instance1 != instance2);
}

BOOST_AUTO_TEST_CASE(point_value__inequality__different_by_index__true) {
    static point_value const instance1{{hash1, 42}, 34};
    static point_value const instance2{{hash1, 43}, 34};
    BOOST_REQUIRE(instance1 != instance2);
}

BOOST_AUTO_TEST_CASE(point_value__inequality__different_by_value__true) {
    static point_value const instance1{{hash1, 42}, 34};
    static point_value const instance2{{hash1, 42}, 35};
    BOOST_REQUIRE(instance1 != instance2);
}

BOOST_AUTO_TEST_CASE(point_value__set_value__42__42) {
    static auto const expected = 42u;
    point_value instance;
    instance.set_value(expected);
    BOOST_REQUIRE_EQUAL(instance.value(), expected);
}

BOOST_AUTO_TEST_CASE(point_value__set_value__zeroize__zero) {
    point_value instance;
    instance.set_value(42);
    instance.set_value(0);
    BOOST_REQUIRE_EQUAL(instance.value(), 0u);
}

BOOST_AUTO_TEST_SUITE_END()

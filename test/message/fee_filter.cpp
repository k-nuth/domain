// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::message;

BOOST_AUTO_TEST_SUITE(fee_filter_tests)

BOOST_AUTO_TEST_CASE(fee_filter__constructor_1__always__invalid) {
    const fee_filter instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(fee_filter__constructor_2__always__equals_params) {
    uint64_t const value = 6434u;
    const fee_filter instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(value, instance.minimum_fee());
}

BOOST_AUTO_TEST_CASE(fee_filter__constructor_3__always__equals_params) {
    uint64_t const fee = 6434u;
    const fee_filter value(fee);
    const fee_filter instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(fee, instance.minimum_fee());
    BOOST_REQUIRE(value == instance);
}

BOOST_AUTO_TEST_CASE(fee_filter__constructor_4__always__equals_params) {
    uint64_t const fee = 6434u;
    const fee_filter value(fee);
    const fee_filter instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(fee, instance.minimum_fee());
}

BOOST_AUTO_TEST_CASE(fee_filter__from_data__insufficient_bytes_failure) {
    data_chunk const raw = {0xab, 0x11};
    fee_filter instance;
    BOOST_REQUIRE(!entity_from_data(instance, version::level::maximum, raw));
}

BOOST_AUTO_TEST_CASE(fee_filter__from_data__insufficient_version_failure) {
    const fee_filter expected{1};
    auto const data = expected.to_data(fee_filter::version_maximum);
    fee_filter instance;
    BOOST_REQUIRE(!entity_from_data(instance, filter_add::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(fee_filter__factory_from_data_1__roundtrip__success) {
    const fee_filter expected{123};
    auto const data = expected.to_data(fee_filter::version_maximum);
    auto const result = create<fee_filter>(fee_filter::version_maximum, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);

    auto const size = result.serialized_size(version::level::maximum);
    BOOST_REQUIRE_EQUAL(data.size(), size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::maximum), size);
}

BOOST_AUTO_TEST_CASE(fee_filter__factory_from_data_2__roundtrip__success) {
    const fee_filter expected{325};
    auto const data = expected.to_data(fee_filter::version_maximum);
    data_source istream(data);
    auto const result = create<fee_filter>(fee_filter::version_maximum, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);

    auto const size = result.serialized_size(version::level::maximum);
    BOOST_REQUIRE_EQUAL(data.size(), size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::maximum), size);
}

BOOST_AUTO_TEST_CASE(fee_filter__factory_from_data_3__roundtrip__success) {
    const fee_filter expected{58246};
    auto const data = expected.to_data(fee_filter::version_maximum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<fee_filter>(fee_filter::version_maximum, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);

    auto const size = result.serialized_size(version::level::maximum);
    BOOST_REQUIRE_EQUAL(data.size(), size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::maximum), size);
}

BOOST_AUTO_TEST_CASE(fee_filter__minimum_fee__roundtrip__success) {
    uint64_t const value = 42134u;
    fee_filter instance;
    BOOST_REQUIRE_NE(instance.minimum_fee(), value);

    instance.set_minimum_fee(value);
    BOOST_REQUIRE_EQUAL(value, instance.minimum_fee());
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_assign_equals__always__matches_equivalent) {
    fee_filter value(2453u);
    BOOST_REQUIRE(value.is_valid());

    fee_filter instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_boolean_equals__duplicates__returns_true) {
    const fee_filter expected(2453u);
    fee_filter instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_boolean_equals__differs__returns_false) {
    const fee_filter expected(2453u);
    fee_filter instance;
    BOOST_REQUIRE(!(instance == expected));
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_boolean_not_equals__duplicates__returns_false) {
    const fee_filter expected(2453u);
    fee_filter instance(expected);
    BOOST_REQUIRE(!(instance != expected));
}

BOOST_AUTO_TEST_CASE(fee_filter__operator_boolean_not_equals__differs__returns_true) {
    const fee_filter expected(2453u);
    fee_filter instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

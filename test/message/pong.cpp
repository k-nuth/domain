// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;

BOOST_AUTO_TEST_SUITE(pong_tests)

BOOST_AUTO_TEST_CASE(pong__constructor_1__always__invalid) {
    message::pong instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(pong__constructor_2__always__equals_params) {
    uint64_t nonce = 462434u;
    message::pong instance(nonce);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(nonce, instance.nonce());
}

BOOST_AUTO_TEST_CASE(pong__constructor_3__always__equals_params) {
    message::pong expected(24235u);
    BOOST_REQUIRE(expected.is_valid());
    message::pong instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(pong__satoshi_fixed_size__minimum_version__returns_8) {
    auto const size = message::pong::satoshi_fixed_size(message::version::level::minimum);
    BOOST_REQUIRE_EQUAL(size, 8u);
}

BOOST_AUTO_TEST_CASE(pong__factory_from_data_1__minimum_version_empty_data__invalid) {
    static auto const version = message::version::level::minimum;
    auto const result = message::pong::factory_from_data(version, data_chunk{});
    BOOST_REQUIRE(!result.is_valid());
}

BOOST_AUTO_TEST_CASE(pong__factory_from_data_1__round_trip__expected) {
    static const message::pong expected{
        4306550u};

    static auto const version = message::version::level::minimum;
    auto const data = expected.to_data(version);
    auto const result = message::pong::factory_from_data(version, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(pong__factory_from_data_2__round_trip__expected) {
    static const message::pong expected{
        3100693u};

    static auto const version = message::version::level::minimum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    auto const result = message::pong::factory_from_data(version, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(pong__factory_from_data_3__round_trip__expected) {
    static const message::pong expected{
        4642675u};

    static auto const version = message::version::level::minimum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = message::pong::factory_from_data(version, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(pong__nonce_accessor__always__returns_initialized_value) {
    uint64_t value = 43564u;
    message::pong instance(value);
    BOOST_REQUIRE_EQUAL(value, instance.nonce());
}

BOOST_AUTO_TEST_CASE(pong__nonce_setter__roundtrip__success) {
    uint64_t value = 43564u;
    message::pong instance;
    BOOST_REQUIRE(value != instance.nonce());
    instance.set_nonce(value);
    BOOST_REQUIRE_EQUAL(value, instance.nonce());
}

BOOST_AUTO_TEST_CASE(pong__operator_assign_equals__always__matches_equivalent) {
    message::pong value(356234u);
    BOOST_REQUIRE(value.is_valid());
    message::pong instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(pong__operator_boolean_equals__duplicates__returns_true) {
    const message::pong expected(4543234u);
    message::pong instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(pong__operator_boolean_equals__differs__returns_false) {
    const message::pong expected(547553u);
    message::pong instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(pong__operator_boolean_not_equals__duplicates__returns_false) {
    const message::pong expected(653786u);
    message::pong instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(pong__operator_boolean_not_equals__differs__returns_true) {
    const message::pong expected(89764u);
    message::pong instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

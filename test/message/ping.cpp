// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;

BOOST_AUTO_TEST_SUITE(ping_tests)

BOOST_AUTO_TEST_CASE(ping__constructor_1__always__invalid) {
    message::ping instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(ping__constructor_2__always__equals_params) {
    uint64_t nonce = 462434u;
    message::ping instance(nonce);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(nonce, instance.nonce());
}

BOOST_AUTO_TEST_CASE(ping__constructor_3__always__equals_params) {
    message::ping expected(24235u);
    BOOST_REQUIRE(expected.is_valid());
    message::ping instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(ping__satoshi_fixed_size__minimum_version__zero) {
    BOOST_REQUIRE_EQUAL(0u,
                        message::ping::satoshi_fixed_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(ping__satoshi_fixed_size__bip31_version__8) {
    BOOST_REQUIRE_EQUAL(8u,
                        message::ping::satoshi_fixed_size(message::version::level::bip31));
}

BOOST_AUTO_TEST_CASE(ping__factory_from_data_1__maximum_version_empty_data__invalid) {
    static auto const version = message::version::level::maximum;
    auto const result = message::ping::factory_from_data(version, data_chunk{});
    BOOST_REQUIRE(!result.is_valid());
}

BOOST_AUTO_TEST_CASE(ping__factory_from_data_1__minimum_version_empty_data__valid) {
    static auto const version = message::version::level::minimum;
    auto const result = message::ping::factory_from_data(version, data_chunk{});
    BOOST_REQUIRE(result.is_valid());
}

BOOST_AUTO_TEST_CASE(ping__from_data_1__minimum_version__success_zero_nonce) {
    static const message::ping value{
        213153u};

    // This serializes the nonce.
    auto const data = value.to_data(message::version::level::bip31);
    BOOST_REQUIRE_EQUAL(data.size(), 8u);

    // This leaves the nonce on the wire but otherwise succeeds with a zero nonce.
    message::ping instance;
    BOOST_REQUIRE(instance.from_data(message::ping::version_minimum, data));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(instance.nonce(), 0u);
}

BOOST_AUTO_TEST_CASE(ping__factory_from_data_1__minimum_version_round_trip__zero_nonce) {
    static const message::ping value{
        16545612u};

    static auto const version = message::version::level::minimum;
    auto const data = value.to_data(version);
    auto const result = message::ping::factory_from_data(version, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(result.nonce(), 0u);
}

BOOST_AUTO_TEST_CASE(ping__factory_from_data_2__minimum_version_round_trip__zero_nonce) {
    const message::ping value{
        5087222u};

    static auto const version = message::version::level::minimum;
    auto const data = value.to_data(version);
    data_source istream(data);
    auto const result = message::ping::factory_from_data(version, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(result.nonce(), 0u);
}

BOOST_AUTO_TEST_CASE(ping__factory_from_data_3__minimum_version_round_trip__zero_nonce) {
    static const message::ping value{
        6456147u};

    static auto const version = message::version::level::minimum;
    auto const data = value.to_data(version);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = message::ping::factory_from_data(version, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(result.nonce(), 0u);
}

BOOST_AUTO_TEST_CASE(ping__from_data_1__maximum_version__success_expected_nonce) {
    static const message::ping expected{
        213153u};

    // This serializes the nonce.
    auto const data = expected.to_data(message::version::level::bip31);
    BOOST_REQUIRE_EQUAL(data.size(), 8u);

    // This leaves the nonce on the wire but otherwise succeeds with a zero nonce.
    message::ping instance;
    BOOST_REQUIRE(instance.from_data(message::ping::version_maximum, data));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(ping__factory_from_data_1__bip31_version_round_trip__expected_nonce) {
    static const message::ping expected{
        16545612u};

    static auto const version = message::version::level::bip31;
    auto const data = expected.to_data(version);
    auto const result = message::ping::factory_from_data(version, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(result == expected);
}

BOOST_AUTO_TEST_CASE(ping__factory_from_data_2__bip31_version_round_trip__expected_nonce) {
    const message::ping expected{
        5087222u};

    static auto const version = message::version::level::bip31;
    auto const data = expected.to_data(version);
    data_source istream(data);
    auto const result = message::ping::factory_from_data(version, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(result == expected);
}

BOOST_AUTO_TEST_CASE(ping__factory_from_data_3__bip31_version_round_trip__expected_nonce) {
    static const message::ping expected{
        6456147u};

    static auto const version = message::version::level::bip31;
    auto const data = expected.to_data(version);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = message::ping::factory_from_data(version, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(result == expected);
}

BOOST_AUTO_TEST_CASE(ping__nonce_accessor__always__returns_initialized_value) {
    uint64_t value = 43564u;
    message::ping instance(value);
    BOOST_REQUIRE_EQUAL(value, instance.nonce());
}

BOOST_AUTO_TEST_CASE(ping__nonce_setter__roundtrip__success) {
    uint64_t value = 43564u;
    message::ping instance;
    BOOST_REQUIRE(value != instance.nonce());
    instance.set_nonce(value);
    BOOST_REQUIRE_EQUAL(value, instance.nonce());
}

BOOST_AUTO_TEST_CASE(ping__operator_assign_equals__always__matches_equivalent) {
    message::ping value(356234u);
    BOOST_REQUIRE(value.is_valid());
    message::ping instance;
    BOOST_REQUIRE(!instance.is_valid());
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(ping__operator_boolean_equals__duplicates__returns_true) {
    const message::ping expected(4543234u);
    message::ping instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(ping__operator_boolean_equals__differs__returns_false) {
    const message::ping expected(547553u);
    message::ping instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(ping__operator_boolean_not_equals__duplicates__returns_false) {
    const message::ping expected(653786u);
    message::ping instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(ping__operator_boolean_not_equals__differs__returns_true) {
    const message::ping expected(89764u);
    message::ping instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// Start Boost Suite: ping tests

TEST_CASE("ping  constructor 1  always invalid", "[ping]") {
    message::ping instance;
    REQUIRE(!instance.is_valid());
}

TEST_CASE("ping  constructor 2  always  equals params", "[ping]") {
    uint64_t nonce = 462434u;
    message::ping instance(nonce);
    REQUIRE(instance.is_valid());
    REQUIRE(nonce == instance.nonce());
}

TEST_CASE("ping  constructor 3  always  equals params", "[ping]") {
    message::ping expected(24235u);
    REQUIRE(expected.is_valid());
    message::ping instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("ping  satoshi fixed size  minimum version  zero", "[ping]") {
    REQUIRE(0u == message::ping::satoshi_fixed_size(message::version::level::minimum));
}

TEST_CASE("ping  satoshi fixed size  bip31 version  8", "[ping]") {
    REQUIRE(8u == message::ping::satoshi_fixed_size(message::version::level::bip31));
}

TEST_CASE("ping  factory from data 1  maximum version empty data invalid", "[ping]") {
    static auto const version = message::version::level::maximum;
    auto const result = create<message::ping>(version, data_chunk{});
    REQUIRE(!result.is_valid());
}

TEST_CASE("ping  factory from data 1  minimum version empty data valid", "[ping]") {
    static auto const version = message::version::level::minimum;
    auto const result = create<message::ping>(version, data_chunk{});
    REQUIRE(result.is_valid());
}

TEST_CASE("ping  from data 1  minimum version  success zero nonce", "[ping]") {
    static const message::ping value{
        213153u};

    // This serializes the nonce.
    auto const data = value.to_data(message::version::level::bip31);
    REQUIRE(data.size() == 8u);

    // This leaves the nonce on the wire but otherwise succeeds with a zero nonce.
    message::ping instance;
    REQUIRE(entity_from_data(instance, message::ping::version_minimum, data));
    REQUIRE(instance.is_valid());
    REQUIRE(instance.nonce() == 0u);
}

TEST_CASE("ping  factory from data 1  minimum version round trip  zero nonce", "[ping]") {
    static const message::ping value{
        16545612u};

    static auto const version = message::version::level::minimum;
    auto const data = value.to_data(version);
    auto const result = create<message::ping>(version, data);
    REQUIRE(result.is_valid());
    REQUIRE(result.nonce() == 0u);
}

TEST_CASE("ping  factory from data 2  minimum version round trip  zero nonce", "[ping]") {
    const message::ping value{
        5087222u};

    static auto const version = message::version::level::minimum;
    auto const data = value.to_data(version);
    data_source istream(data);
    auto const result = create<message::ping>(version, istream);
    REQUIRE(result.is_valid());
    REQUIRE(result.nonce() == 0u);
}

TEST_CASE("ping  factory from data 3  minimum version round trip  zero nonce", "[ping]") {
    static const message::ping value{
        6456147u};

    static auto const version = message::version::level::minimum;
    auto const data = value.to_data(version);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::ping>(version, source);
    REQUIRE(result.is_valid());
    REQUIRE(result.nonce() == 0u);
}

TEST_CASE("ping  from data 1  maximum version  success expected nonce", "[ping]") {
    static const message::ping expected{
        213153u};

    // This serializes the nonce.
    auto const data = expected.to_data(message::version::level::bip31);
    REQUIRE(data.size() == 8u);

    // This leaves the nonce on the wire but otherwise succeeds with a zero nonce.
    message::ping instance;
    REQUIRE(entity_from_data(instance, message::ping::version_maximum, data));
    REQUIRE(instance.is_valid());
    REQUIRE(instance == expected);
}

TEST_CASE("ping  factory from data 1  bip31 version round trip  expected nonce", "[ping]") {
    static const message::ping expected{
        16545612u};

    static auto const version = message::version::level::bip31;
    auto const data = expected.to_data(version);
    auto const result = create<message::ping>(version, data);
    REQUIRE(result.is_valid());
    REQUIRE(result == expected);
}

TEST_CASE("ping  factory from data 2  bip31 version round trip  expected nonce", "[ping]") {
    const message::ping expected{
        5087222u};

    static auto const version = message::version::level::bip31;
    auto const data = expected.to_data(version);
    data_source istream(data);
    auto const result = create<message::ping>(version, istream);
    REQUIRE(result.is_valid());
    REQUIRE(result == expected);
}

TEST_CASE("ping  factory from data 3  bip31 version round trip  expected nonce", "[ping]") {
    static const message::ping expected{
        6456147u};

    static auto const version = message::version::level::bip31;
    auto const data = expected.to_data(version);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::ping>(version, source);
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

// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// Start Boost Suite: pong tests

TEST_CASE("pong  constructor 1  always invalid", "[pong]") {
    message::pong instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("pong  constructor 2  always  equals params", "[pong]") {
    uint64_t nonce = 462434u;
    message::pong instance(nonce);
    REQUIRE(instance.is_valid());
    REQUIRE(nonce == instance.nonce());
}

TEST_CASE("pong  constructor 3  always  equals params", "[pong]") {
    message::pong expected(24235u);
    REQUIRE(expected.is_valid());
    message::pong instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("pong  satoshi fixed size  minimum version  returns 8", "[pong]") {
    auto const size = message::pong::satoshi_fixed_size(message::version::level::minimum);
    REQUIRE(size == 8u);
}

TEST_CASE("pong  factory from data 1  minimum version empty data invalid", "[pong]") {
    static auto const version = message::version::level::minimum;
    auto const result = create<message::pong>(version, data_chunk{});
    REQUIRE( ! result.is_valid());
}

TEST_CASE("pong  factory from data 1  round trip  expected", "[pong]") {
    static const message::pong expected{
        4306550u};

    static auto const version = message::version::level::minimum;
    auto const data = expected.to_data(version);
    auto const result = create<message::pong>(version, data);

    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
    REQUIRE(data.size() == result.serialized_size(version));
    REQUIRE(expected.serialized_size(version) == result.serialized_size(version));
}

TEST_CASE("pong  factory from data 2  round trip  expected", "[pong]") {
    static const message::pong expected{
        3100693u};

    static auto const version = message::version::level::minimum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    auto const result = create<message::pong>(version, istream);

    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
    REQUIRE(data.size() == result.serialized_size(version));
    REQUIRE(expected.serialized_size(version) == result.serialized_size(version));
}

TEST_CASE("pong  factory from data 3  round trip  expected", "[pong]") {
    static const message::pong expected{
        4642675u};

    static auto const version = message::version::level::minimum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::pong>(version, source);

    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
    REQUIRE(data.size() == result.serialized_size(version));
    REQUIRE(expected.serialized_size(version) == result.serialized_size(version));
}

TEST_CASE("pong  nonce accessor  always  returns initialized value", "[pong]") {
    uint64_t value = 43564u;
    message::pong instance(value);
    REQUIRE(value == instance.nonce());
}

TEST_CASE("pong  nonce setter  roundtrip  success", "[pong]") {
    uint64_t value = 43564u;
    message::pong instance;
    REQUIRE(value != instance.nonce());
    instance.set_nonce(value);
    REQUIRE(value == instance.nonce());
}

TEST_CASE("pong  operator assign equals  always  matches equivalent", "[pong]") {
    message::pong value(356234u);
    REQUIRE(value.is_valid());
    message::pong instance;
    REQUIRE( ! instance.is_valid());
    instance = std::move(value);
    REQUIRE(instance.is_valid());
}

TEST_CASE("pong  operator boolean equals  duplicates  returns true", "[pong]") {
    const message::pong expected(4543234u);
    message::pong instance(expected);
    REQUIRE(instance == expected);
}

TEST_CASE("pong  operator boolean equals  differs  returns false", "[pong]") {
    const message::pong expected(547553u);
    message::pong instance;
    REQUIRE(instance != expected);
}

TEST_CASE("pong  operator boolean not equals  duplicates  returns false", "[pong]") {
    const message::pong expected(653786u);
    message::pong instance(expected);
    REQUIRE(instance == expected);
}

TEST_CASE("pong  operator boolean not equals  differs  returns true", "[pong]") {
    const message::pong expected(89764u);
    message::pong instance;
    REQUIRE(instance != expected);
}

// End Boost Suite

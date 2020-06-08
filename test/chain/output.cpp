// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

data_chunk valid_raw_output = to_chunk(base16_literal("20300500000000001976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac"));

// Start Boost Suite: output tests

TEST_CASE("output  constructor 1  always  returns default initialized", "[output]") {
    chain::output instance;
    REQUIRE(!instance.is_valid());
}

TEST_CASE("output  constructor 2  valid input  returns input initialized", "[output]") {
    uint64_t value = 643u;
    chain::script script;
    auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
    REQUIRE(entity_from_data(script, data, false));

    chain::output instance(value, script);
    REQUIRE(instance.is_valid());
    REQUIRE(script == instance.script());
    REQUIRE(value == instance.value());
}

TEST_CASE("output  constructor 3  valid input  returns input initialized", "[output]") {
    uint64_t value = 643u;
    chain::script script;
    auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
    REQUIRE(entity_from_data(script, data, false));

    // This must be non-const.
    auto dup_script = script;

    chain::output instance(value, std::move(dup_script));

    REQUIRE(instance.is_valid());
    REQUIRE(script == instance.script());
    REQUIRE(value == instance.value());
}

TEST_CASE("output  constructor 4  valid input  returns input initialized", "[output]") {
    chain::output expected;
    REQUIRE(entity_from_data(expected, valid_raw_output));

    chain::output instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("output  constructor 5  valid input  returns input initialized", "[output]") {
    // This must be non-const.
    chain::output expected;
    REQUIRE(entity_from_data(expected, valid_raw_output));

    chain::output instance(std::move(expected));
    REQUIRE(instance.is_valid());
}

TEST_CASE("output  from data  insufficient bytes  failure", "[output]") {
    data_chunk data(2);

    chain::output instance;

    REQUIRE(!entity_from_data(instance, data));
    REQUIRE(!instance.is_valid());
}

TEST_CASE("output  factory from data 1  valid input success", "[output]") {
    auto instance = create<chain::output>(valid_raw_output);
    REQUIRE(instance.is_valid());
    REQUIRE(instance.serialized_size() == valid_raw_output.size());

    // Re-save and compare against original.
    data_chunk resave = instance.to_data();
    REQUIRE(resave.size() == valid_raw_output.size());
    REQUIRE(resave == valid_raw_output);
}

TEST_CASE("output  factory from data 2  valid input success", "[output]") {
    data_source stream(valid_raw_output);
    auto instance = create<chain::output>(stream);
    REQUIRE(instance.is_valid());
    REQUIRE(instance.serialized_size() == valid_raw_output.size());

    // Re-save and compare against original.
    data_chunk resave = instance.to_data();
    REQUIRE(resave.size() == valid_raw_output.size());
    REQUIRE(resave == valid_raw_output);
}

TEST_CASE("output  factory from data 3  valid input success", "[output]") {
    data_source stream(valid_raw_output);
    istream_reader source(stream);
    auto instance = create<chain::output>(source);
    REQUIRE(instance.is_valid());
    REQUIRE(instance.serialized_size() == valid_raw_output.size());

    // Re-save and compare against original.
    data_chunk resave = instance.to_data();
    REQUIRE(resave.size() == valid_raw_output.size());
    REQUIRE(resave == valid_raw_output);
}

TEST_CASE("output  signature operations  always  returns script sigops false", "[output]") {
    chain::output instance;
    REQUIRE(instance.script().sigops(false) == instance.signature_operations(false));
}

TEST_CASE("output  value  roundtrip  success", "[output]") {
    uint64_t expected = 523542u;
    chain::output instance;
    REQUIRE(expected != instance.value());
    instance.set_value(expected);
    REQUIRE(expected == instance.value());
}

TEST_CASE("output  script setter 1  roundtrip  success", "[output]") {
    chain::script value;
    auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
    REQUIRE(entity_from_data(value, data, false));

    chain::output instance;
    REQUIRE(value != instance.script());
    instance.set_script(value);
    REQUIRE(value == instance.script());
    auto const& restricted = instance;
    REQUIRE(value == instance.script());
}

TEST_CASE("output  script setter 2  roundtrip  success", "[output]") {
    chain::script value;
    auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
    REQUIRE(entity_from_data(value, data, false));

    // This must be non-const.
    auto dup_value = value;

    chain::output instance;
    REQUIRE(value != instance.script());
    instance.set_script(std::move(dup_value));
    REQUIRE(value == instance.script());
    auto const& restricted = instance;
    REQUIRE(value == instance.script());
}

TEST_CASE("output  operator assign equals 1  always  matches equivalent", "[output]") {
    chain::output expected;
    REQUIRE(entity_from_data(expected, valid_raw_output));
    chain::output instance;
    instance = create<chain::output>(valid_raw_output);
    REQUIRE(instance == expected);
}

TEST_CASE("output  operator assign equals 2  always  matches equivalent", "[output]") {
    chain::output expected;
    REQUIRE(entity_from_data(expected, valid_raw_output));
    chain::output instance;
    instance = expected;
    REQUIRE(instance == expected);
}

TEST_CASE("output  operator boolean equals  duplicates  returns true", "[output]") {
    chain::output alpha;
    chain::output beta;
    REQUIRE(entity_from_data(alpha, valid_raw_output));
    REQUIRE(entity_from_data(beta, valid_raw_output));
    REQUIRE(alpha == beta);
}

TEST_CASE("output  operator boolean equals  differs  returns false", "[output]") {
    chain::output alpha;
    chain::output beta;
    REQUIRE(entity_from_data(alpha, valid_raw_output));
    REQUIRE(alpha != beta);
}

TEST_CASE("output  operator boolean not equals  duplicates  returns false", "[output]") {
    chain::output alpha;
    chain::output beta;
    REQUIRE(entity_from_data(alpha, valid_raw_output));
    REQUIRE(entity_from_data(beta, valid_raw_output));
    REQUIRE(alpha == beta);
}

TEST_CASE("output  operator boolean not equals  differs  returns true", "[output]") {
    chain::output alpha;
    chain::output beta;
    REQUIRE(entity_from_data(alpha, valid_raw_output));
    REQUIRE(alpha != beta);
}

// End Boost Suite

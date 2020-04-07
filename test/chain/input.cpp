// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::chain;

data_chunk valid_raw_input = to_chunk(base16_literal(
    "54b755c39207d443fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b01000"
    "0006b48304502203267910f55f2297360198fff57a3631be850965344370f732950b4"
    "7795737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d915"
    "da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b"
    "8066c9c585f9ffffffff"));

// Start Boost Suite: input tests

TEST_CASE("input  constructor 1  always  returns default initialized", "[input]") {
    input instance;
    REQUIRE(!instance.is_valid());
}

TEST_CASE("input  constructor 2  valid input  returns input initialized", "[input]") {
    output_point const previous_output{null_hash, 5434u};
    script script;
    REQUIRE(entity_from_data(script, to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1")), false));

    uint32_t sequence = 4568656u;

    input instance(previous_output, script, sequence);
    REQUIRE(instance.is_valid());
    REQUIRE(previous_output == instance.previous_output());
    REQUIRE(script == instance.script());
    REQUIRE(sequence == instance.sequence());
}

TEST_CASE("input  constructor 3  valid input  returns input initialized", "[input]") {
    output_point const previous_output{null_hash, 5434u};
    script script;
    REQUIRE(entity_from_data(script, to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1")), false));

    uint32_t sequence = 4568656u;

    auto dup_previous_output = previous_output;
    auto dup_script = script;
    input instance(std::move(dup_previous_output), std::move(dup_script), sequence);

    REQUIRE(instance.is_valid());
    REQUIRE(previous_output == instance.previous_output());
    REQUIRE(script == instance.script());
    REQUIRE(sequence == instance.sequence());
}

TEST_CASE("input  constructor 4  valid input  returns input initialized", "[input]") {
    input expected;
    REQUIRE(entity_from_data(expected, valid_raw_input));

    input instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("input  constructor 5  valid input  returns input initialized", "[input]") {
    input expected;
    REQUIRE(entity_from_data(expected, valid_raw_input));

    input instance(std::move(expected));
    REQUIRE(instance.is_valid());
}

TEST_CASE("input  from data  insufficient data  failure", "[input]") {
    data_chunk data(2);

    input instance;

    REQUIRE(!entity_from_data(instance, data));
    REQUIRE(!instance.is_valid());
}

TEST_CASE("input  from data  valid data  success", "[input]") {
    auto const junk = base16_literal("000000000000005739943a9c29a1955dfae2b3f37de547005bfb9535192e5fb0000000000000005739943a9c29a1955dfae2b3f37de547005bfb9535192e5fb0");

    // data_chunk_stream_host host(junk);
    byte_source<std::array<uint8_t, 64>> source(junk);
    boost::iostreams::stream<byte_source<std::array<uint8_t, 64>>> stream(source);

    input instance;
    REQUIRE(entity_from_data(instance, stream));
}

TEST_CASE("input  factory from data 1  valid input  success", "[input]") {
    auto const instance = create<input>(valid_raw_input);
    REQUIRE(instance.is_valid());
    REQUIRE(instance.serialized_size() == valid_raw_input.size());

    // Re-save and compare against original.
    auto const resave = instance.to_data();
    REQUIRE(resave.size() == valid_raw_input.size());
    REQUIRE(resave == valid_raw_input);
}

TEST_CASE("input  factory from data 2  valid input  success", "[input]") {
    data_source stream(valid_raw_input);
    auto instance = create<input>(stream);
    REQUIRE(instance.is_valid());
    REQUIRE(instance.serialized_size() == valid_raw_input.size());

    // Re-save and compare against original.
    auto const resave = instance.to_data();
    REQUIRE(resave.size() == valid_raw_input.size());
    REQUIRE(resave == valid_raw_input);
}

TEST_CASE("input  factory from data 3  valid input  success", "[input]") {
    data_source stream(valid_raw_input);
    istream_reader source(stream);
    auto instance = create<input>(source);
    REQUIRE(instance.is_valid());
    REQUIRE(instance.serialized_size() == valid_raw_input.size());

    // Re-save and compare against original.
    auto const resave = instance.to_data();
    REQUIRE(resave.size() == valid_raw_input.size());
    REQUIRE(resave == valid_raw_input);
}

TEST_CASE("input  is final  max input sequence  true", "[input]") {
    input const instance({}, {}, max_input_sequence);
    REQUIRE(instance.is_final());
}

TEST_CASE("input  is final  sequence zero  false", "[input]") {
    input const instance({}, {}, 0);
    REQUIRE(!instance.is_final());
}

TEST_CASE("input  is locked  enabled block type sequence age equals minimum  false", "[input]") {
    static auto const age = 7u;
    static auto const sequence_enabled_block_type_minimum = age;
    input instance({}, {}, sequence_enabled_block_type_minimum);
    auto& prevout = instance.previous_output().validation;
    prevout.height = 42;
    REQUIRE(!instance.is_locked(prevout.height + age, 0));
}

TEST_CASE("input  is locked  enabled block type sequence age above minimum  false", "[input]") {
    static auto const age = 7u;
    static auto const sequence_enabled_block_type_minimum = age - 1;
    input instance({}, {}, sequence_enabled_block_type_minimum);
    auto& prevout = instance.previous_output().validation;
    prevout.height = 42;
    REQUIRE(!instance.is_locked(prevout.height + age, 0));
}

TEST_CASE("input  is locked  enabled block type sequence age below minimum  true", "[input]") {
    static auto const age = 7u;
    static auto const sequence_enabled_block_type_minimum = age + 1;
    input instance({}, {}, sequence_enabled_block_type_minimum);
    auto& prevout = instance.previous_output().validation;
    prevout.height = 42;
    REQUIRE(instance.is_locked(prevout.height + age, 0));
}

TEST_CASE("input  is locked  disabled block type sequence age below minimum  false", "[input]") {
    static auto const age = 7u;
    static auto const sequence_disabled_block_type_minimum = relative_locktime_disabled | (age + 1);
    input instance({}, {}, sequence_disabled_block_type_minimum);
    auto& prevout = instance.previous_output().validation;
    prevout.height = 42;
    REQUIRE(!instance.is_locked(prevout.height + age, 0));
}

TEST_CASE("input  is locked  enabled time type sequence age equals minimum  false", "[input]") {
    static auto const age = 7u;
    static auto const age_seconds = 7u << relative_locktime_seconds_shift;
    static auto const sequence_enabled_time_type_minimum = relative_locktime_time_locked | age;
    input instance({}, {}, sequence_enabled_time_type_minimum);
    auto& prevout = instance.previous_output().validation;
    prevout.median_time_past = 42;
    REQUIRE(!instance.is_locked(0, prevout.median_time_past + age_seconds));
}

TEST_CASE("input  is locked  enabled time type sequence age above minimum  false", "[input]") {
    static auto const age = 7u;
    static auto const age_seconds = 7u << relative_locktime_seconds_shift;
    static auto const sequence_enabled_time_type_minimum = relative_locktime_time_locked | (age - 1);
    input instance({}, {}, sequence_enabled_time_type_minimum);
    auto& prevout = instance.previous_output().validation;
    prevout.median_time_past = 42;
    REQUIRE(!instance.is_locked(0, prevout.median_time_past + age_seconds));
}

TEST_CASE("input  is locked  enabled time type sequence age below minimum  true", "[input]") {
    static auto const age = 7u;
    static auto const age_seconds = 7u << relative_locktime_seconds_shift;
    static auto const sequence_enabled_time_type_minimum = relative_locktime_time_locked | (age + 1);
    input instance({}, {}, sequence_enabled_time_type_minimum);
    auto& prevout = instance.previous_output().validation;
    prevout.median_time_past = 42;
    REQUIRE(instance.is_locked(0, prevout.median_time_past + age_seconds));
}

TEST_CASE("input  is locked  disabled time type sequence age below minimum  false", "[input]") {
    static auto const age = 7u;
    static auto const age_seconds = 7u << relative_locktime_seconds_shift;
    static auto const sequence_disabled_time_type_minimum = relative_locktime_disabled | relative_locktime_time_locked | (age + 1);
    input instance({}, {}, sequence_disabled_time_type_minimum);
    auto& prevout = instance.previous_output().validation;
    prevout.median_time_past = 42;
    REQUIRE(!instance.is_locked(0, prevout.median_time_past + age_seconds));
}

TEST_CASE("input  signature operations  bip16 inactive  returns script sigops", "[input]") {
    auto const raw_script = to_chunk(base16_literal("02acad"));
    script script;
    REQUIRE(entity_from_data(script, raw_script, true));
    input instance;
    instance.set_script(script);
    REQUIRE(script.sigops(false) == instance.signature_operations(false, false));
}

TEST_CASE("input  signature operations  bip16 active cache empty  returns script sigops", "[input]") {
    auto const raw_script = to_chunk(base16_literal("02acad"));
    script script;
    REQUIRE(entity_from_data(script, raw_script, true));
    input instance;
    instance.set_script(script);
    REQUIRE(script.sigops(false) == instance.signature_operations(true, false));
}

TEST_CASE("input  previous output setter 1  roundtrip  success", "[input]") {
    output_point const value{
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        5434u};

    input instance;
    REQUIRE(value != instance.previous_output());
    instance.set_previous_output(value);
    REQUIRE(value == instance.previous_output());
    auto const& restricted = instance;
    REQUIRE(value == restricted.previous_output());
}

TEST_CASE("input  previous output setter 2  roundtrip  success", "[input]") {
    output_point const value{
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        5434u};

    auto dup_value = value;

    input instance;
    BOOST_REQUIRE(value != instance.previous_output());
    instance.set_previous_output(std::move(dup_value));
    BOOST_REQUIRE(value == instance.previous_output());
    auto const& restricted = instance;
    BOOST_REQUIRE(value == restricted.previous_output());
}

BOOST_AUTO_TEST_CASE(input__script_setter_1__roundtrip__success) {
    script value;
    auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
    BOOST_REQUIRE(entity_from_data(value, data, false));

    input instance;
    BOOST_REQUIRE(value != instance.script());
    instance.set_script(value);
    BOOST_REQUIRE(value == instance.script());
    auto const& restricted = instance;
    BOOST_REQUIRE(value == restricted.script());
}

BOOST_AUTO_TEST_CASE(input__script_setter_2__roundtrip__success) {
    script value;
    auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
    BOOST_REQUIRE(entity_from_data(value, data, false));

    auto dup_value = value;
    input instance;
    BOOST_REQUIRE(value != instance.script());
    instance.set_script(std::move(dup_value));
    BOOST_REQUIRE(value == instance.script());
    auto const& restricted = instance;
    BOOST_REQUIRE(value == restricted.script());
}

BOOST_AUTO_TEST_CASE(input__sequence__roundtrip__success) {
    uint32_t value = 1254u;
    input instance;
    BOOST_REQUIRE(value != instance.sequence());
    instance.set_sequence(value);
    BOOST_REQUIRE_EQUAL(value, instance.sequence());
}

BOOST_AUTO_TEST_CASE(input__operator_assign_equals_1__always__matches_equivalent) {
    input expected;
    BOOST_REQUIRE(entity_from_data(expected, valid_raw_input));
    input instance;
    instance = create<input>(valid_raw_input);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(input__operator_assign_equals_2__always__matches_equivalent) {
    input expected;
    BOOST_REQUIRE(entity_from_data(expected, valid_raw_input));
    input instance;
    instance = expected;
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(input__operator_boolean_equals__duplicates__returns_true) {
    input alpha;
    input beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_input));
    BOOST_REQUIRE(entity_from_data(beta, valid_raw_input));
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(input__operator_boolean_equals__differs__returns_false) {
    input alpha;
    input beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_input));
    BOOST_REQUIRE_EQUAL(false, alpha == beta);
}

BOOST_AUTO_TEST_CASE(input__operator_boolean_not_equals__duplicates__returns_false) {
    input alpha;
    input beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_input));
    BOOST_REQUIRE(entity_from_data(beta, valid_raw_input));
    BOOST_REQUIRE_EQUAL(false, alpha != beta);
}

BOOST_AUTO_TEST_CASE(input__operator_boolean_not_equals__differs__returns_true) {
    input alpha;
    input beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_input));
    BOOST_REQUIRE(alpha != beta);
}

BOOST_AUTO_TEST_SUITE_END()

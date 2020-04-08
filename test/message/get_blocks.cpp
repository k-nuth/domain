// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// Start Boost Suite: get blocks tests

TEST_CASE("get blocks  constructor 1  always invalid", "[get blocks]") {
    message::get_blocks instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("get blocks  constructor 2  always  equals params", "[get blocks]") {
    hash_list const starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_blocks instance(starts, stop);
    REQUIRE(instance.is_valid());
    REQUIRE(starts == instance.start_hashes());
    REQUIRE(stop == instance.stop_hash());
}

TEST_CASE("get blocks  constructor 3  always  equals params", "[get blocks]") {
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};
    hash_list starts_duplicate = starts;

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_blocks instance(std::move(starts_duplicate), std::move(stop));
    REQUIRE(instance.is_valid());
    REQUIRE(starts == instance.start_hashes());
    REQUIRE(stop == instance.stop_hash());
}

TEST_CASE("get blocks  constructor 4  always  equals params", "[get blocks]") {
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const message::get_blocks expected(starts, stop);
    message::get_blocks instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
    REQUIRE(starts == instance.start_hashes());
    REQUIRE(stop == instance.stop_hash());
}

TEST_CASE("get blocks  constructor 5  always  equals params", "[get blocks]") {
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_blocks expected(starts, stop);
    message::get_blocks instance(std::move(expected));
    REQUIRE(instance.is_valid());
    REQUIRE(starts == instance.start_hashes());
    REQUIRE(stop == instance.stop_hash());
}

TEST_CASE("get blocks  from data  insufficient bytes  failure", "[get blocks]") {
    data_chunk const raw{0xab, 0xcd};
    message::get_blocks instance;

    REQUIRE( ! entity_from_data(instance, message::version::level::minimum, raw));
}

TEST_CASE("get blocks  factory from data 1  valid input  success", "[get blocks]") {
    const message::get_blocks expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    auto const data = expected.to_data(message::version::level::minimum);
    auto const result = create<message::get_blocks>(
        message::version::level::minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
                        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_blocks__factory_from_data_2__valid_input__success) {
    const message::get_blocks expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    auto const data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    auto const result = create<message::get_blocks>(
        message::version::level::minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
                        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_blocks__factory_from_data_3__valid_input__success) {
    const message::get_blocks expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    auto const data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::get_blocks>(
        message::version::level::minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
                        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_blocks__start_hashes_accessor_1__always__returns_initialized_value) {
    hash_list expected = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    message::get_blocks instance{
        expected,
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    BOOST_REQUIRE(expected == instance.start_hashes());
}

BOOST_AUTO_TEST_CASE(get_blocks__start_hashes_accessor_2__always__returns_initialized_value) {
    hash_list expected = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    const message::get_blocks instance{
        expected,
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    BOOST_REQUIRE(expected == instance.start_hashes());
}

BOOST_AUTO_TEST_CASE(get_blocks__start_hashes_setter_1__roundtrip__success) {
    const hash_list values = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    message::get_blocks instance;
    BOOST_REQUIRE(values != instance.start_hashes());
    instance.set_start_hashes(values);
    BOOST_REQUIRE(values == instance.start_hashes());
}

BOOST_AUTO_TEST_CASE(get_blocks__start_hashes_setter_2__roundtrip__success) {
    hash_list values = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    hash_list values_duplicate = values;

    message::get_blocks instance;
    BOOST_REQUIRE(values != instance.start_hashes());
    instance.set_start_hashes(std::move(values_duplicate));
    BOOST_REQUIRE(values == instance.start_hashes());
}

BOOST_AUTO_TEST_CASE(get_blocks__stop_hash_accessor_1__always__returns_initialized_value) {
    hash_digest expected = hash_literal(
        "7777777777777777777777777777777777777777777777777777777777777777");

    message::get_blocks instance{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        expected};

    BOOST_REQUIRE(expected == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__stop_hash_accessor_2__always__returns_initialized_value) {
    hash_digest expected = hash_literal(
        "7777777777777777777777777777777777777777777777777777777777777777");

    const message::get_blocks instance{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        expected};

    BOOST_REQUIRE(expected == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__stop_hash_setter_1__roundtrip__success) {
    hash_digest value = hash_literal("7777777777777777777777777777777777777777777777777777777777777777");
    message::get_blocks instance;
    BOOST_REQUIRE(value != instance.stop_hash());
    instance.set_stop_hash(value);
    BOOST_REQUIRE(value == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__stop_hash_setter_2__roundtrip__success) {
    hash_digest value = hash_literal("7777777777777777777777777777777777777777777777777777777777777777");
    message::get_blocks instance;
    BOOST_REQUIRE(value != instance.stop_hash());
    instance.set_stop_hash(std::move(value));
    BOOST_REQUIRE(value == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_assign_equals__always__matches_equivalent) {
    hash_list start = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    hash_digest stop = hash_literal("7777777777777777777777777777777777777777777777777777777777777777");

    message::get_blocks value{start, stop};

    BOOST_REQUIRE(value.is_valid());

    message::get_blocks instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(start == instance.start_hashes());
    BOOST_REQUIRE(stop == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_boolean_equals__duplicates__returns_true) {
    const message::get_blocks expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    message::get_blocks instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_boolean_equals__differs__returns_false) {
    const message::get_blocks expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    message::get_blocks instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_boolean_not_equals__duplicates__returns_false) {
    const message::get_blocks expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    message::get_blocks instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_boolean_not_equals__differs__returns_true) {
    const message::get_blocks expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    message::get_blocks instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

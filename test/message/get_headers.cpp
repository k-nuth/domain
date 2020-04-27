// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// Start Boost Suite: get headers tests

TEST_CASE("get headers  constructor 1  always invalid", "[get headers]") {
    message::get_headers instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("get headers  constructor 2  always  equals params", "[get headers]") {
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_headers instance(starts, stop);
    REQUIRE(instance.is_valid());
    REQUIRE(starts == instance.start_hashes());
    REQUIRE(stop == instance.stop_hash());
}

TEST_CASE("get headers  constructor 3  always  equals params", "[get headers]") {
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};
    hash_list starts_duplicate = starts;

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_headers instance(std::move(starts_duplicate), std::move(stop));
    REQUIRE(instance.is_valid());
    REQUIRE(starts == instance.start_hashes());
    REQUIRE(stop == instance.stop_hash());
}

TEST_CASE("get headers  constructor 4  always  equals params", "[get headers]") {
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const message::get_headers expected(starts, stop);
    message::get_headers instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
    REQUIRE(starts == instance.start_hashes());
    REQUIRE(stop == instance.stop_hash());
}

TEST_CASE("get headers  constructor 5  always  equals params", "[get headers]") {
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_headers expected(starts, stop);
    message::get_headers instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(starts == instance.start_hashes());
    BOOST_REQUIRE(stop == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_headers__from_data__insufficient_bytes__failure) {
    data_chunk const raw{0xab, 0xcd};
    message::get_headers instance;

    BOOST_REQUIRE_EQUAL(false, entity_from_data(instance, 
                                   message::get_headers::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(get_headers__from_data__insufficient_version__failure) {
    const message::get_headers expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    auto const data = expected.to_data(message::get_headers::version_minimum);
    message::get_headers instance{};

    BOOST_REQUIRE_EQUAL(false, entity_from_data(instance, 
                                   message::get_headers::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(get_headers__factory_from_data_1__valid_input__success) {
    const message::get_headers expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    auto const data = expected.to_data(message::get_headers::version_minimum);
    auto const result = create<message::get_headers>(
        message::get_headers::version_minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::get_headers::version_minimum));
    BOOST_REQUIRE_EQUAL(
        expected.serialized_size(message::get_headers::version_minimum),
        result.serialized_size(message::get_headers::version_minimum));
}

BOOST_AUTO_TEST_CASE(get_headers__factory_from_data_2__valid_input__success) {
    const message::get_headers expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    auto const data = expected.to_data(message::get_headers::version_minimum);
    data_source istream(data);
    auto const result = create<message::get_headers>(
        message::get_headers::version_minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::get_headers::version_minimum));
    BOOST_REQUIRE_EQUAL(
        expected.serialized_size(message::get_headers::version_minimum),
        result.serialized_size(message::get_headers::version_minimum));
}

BOOST_AUTO_TEST_CASE(get_headers__factory_from_data_3__valid_input__success) {
    const message::get_headers expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    auto const data = expected.to_data(message::get_headers::version_minimum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::get_headers>(
        message::get_headers::version_minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::get_headers::version_minimum));
    BOOST_REQUIRE_EQUAL(
        expected.serialized_size(message::get_headers::version_minimum),
        result.serialized_size(message::get_headers::version_minimum));
}

BOOST_AUTO_TEST_CASE(get_headers__operator_assign_equals__always__matches_equivalent) {
    hash_list start = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")};

    hash_digest stop = hash_literal("7777777777777777777777777777777777777777777777777777777777777777");

    message::get_headers value{start, stop};

    BOOST_REQUIRE(value.is_valid());

    message::get_headers instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(start == instance.start_hashes());
    BOOST_REQUIRE(stop == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_headers__operator_boolean_equals__duplicates__returns_true) {
    const message::get_headers expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    message::get_headers instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(get_headers__operator_boolean_equals__differs__returns_false) {
    const message::get_headers expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    message::get_headers instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(get_headers__operator_boolean_not_equals__duplicates__returns_false) {
    const message::get_headers expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    message::get_headers instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(get_headers__operator_boolean_not_equals__differs__returns_true) {
    const message::get_headers expected{
        {hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
         hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
         hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
         hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
         hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")},
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")};

    message::get_headers instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

auto const hash1 = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
auto const valid_raw_output_point = to_chunk(base16_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f00000015"));

// Start Boost Suite: output point tests

TEST_CASE("output point  constructor 1  always  returns default initialized", "[output point]") {
    const chain::point instance;
    REQUIRE(!instance.is_valid());
}

TEST_CASE("output point  constructor 2  valid input  returns input initialized", "[output point]") {
    static uint32_t const index = 1234u;
    const chain::point value(hash1, index);
    chain::output_point instance(value);
    REQUIRE(instance.is_valid());
    REQUIRE(value == instance);
    REQUIRE(hash1 == instance.hash());
    REQUIRE(index == instance.index());
}

TEST_CASE("output point  constructor 3  valid input  returns input initialized", "[output point]") {
    static uint32_t const index = 1234u;
    chain::point value(hash1, index);
    chain::output_point instance(std::move(value));
    REQUIRE(instance.is_valid());
    REQUIRE(hash1 == instance.hash());
    REQUIRE(index == instance.index());
}

TEST_CASE("output point  constructor 4  valid input  returns input initialized", "[output point]") {
    static uint32_t const index = 1234u;
    chain::output_point instance(hash1, index);
    REQUIRE(instance.is_valid());
    REQUIRE(hash1 == instance.hash());
    REQUIRE(index == instance.index());
}

TEST_CASE("output point  constructor 5  valid input  returns input initialized", "[output point]") {
    static uint32_t const index = 1234u;
    auto dup_hash = hash1;
    chain::output_point instance(std::move(dup_hash), index);
    REQUIRE(instance.is_valid());
    REQUIRE(hash1 == instance.hash());
    REQUIRE(index == instance.index());
}

TEST_CASE("output point  constructor 6  valid input  returns input initialized", "[output point]") {
    const chain::output_point expected(hash1, 524342u);
    chain::output_point instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("output point  constructor 7  valid input  returns input initialized", "[output point]") {
    chain::output_point expected(hash1, 524342u);
    chain::output_point instance(std::move(expected));
    REQUIRE(instance.is_valid());
}

TEST_CASE("output point  begin end  initialized  begin not equal end", "[output point]") {
    static const chain::output_point instance{null_hash, 0};
    REQUIRE(instance.begin() != instance.end());
}

TEST_CASE("output point  from data  insufficient bytes  failure", "[output point]") {
    static data_chunk const data(10);
    chain::output_point instance;
    REQUIRE(!entity_from_data(instance, data));
    REQUIRE(!instance.is_valid());
}

TEST_CASE("output point  from data  roundtrip  success", "[output point]") {
    static uint32_t const index = 53213u;
    static hash_digest const hash{
        {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
         0x01, 0x01, 0xab, 0x11, 0x11, 0xcd, 0x11, 0x11,
         0x01, 0x10, 0x11, 0xab, 0x11, 0x11, 0xcd, 0x11,
         0x01, 0x11, 0x11, 0x11, 0xab, 0x11, 0x11, 0xcd}};

    chain::output_point initial{hash, index};

    BOOST_REQUIRE(initial.is_valid());
    BOOST_REQUIRE(hash == initial.hash());
    BOOST_REQUIRE(index == initial.index());

    chain::output_point point;

    BOOST_REQUIRE(point != initial);
    BOOST_REQUIRE(entity_from_data(point, initial.to_data()));
    BOOST_REQUIRE(point.is_valid());
    BOOST_REQUIRE(point == initial);
}

BOOST_AUTO_TEST_CASE(output_point__factory_from_data_1__roundtrip__success) {
    static auto const data = to_chunk(base16_literal("46682488f0a721124a3905a1bb72445bf13493e2cd46c5c0c8db1c15afa0d58e00000000"));
    BOOST_REQUIRE(data == (data_chunk{
                              0x46, 0x68, 0x24, 0x88, 0xf0, 0xa7, 0x21, 0x12, 0x4a, 0x39, 0x05, 0xa1,
                              0xbb, 0x72, 0x44, 0x5b, 0xf1, 0x34, 0x93, 0xe2, 0xcd, 0x46, 0xc5, 0xc0,
                              0xc8, 0xdb, 0x1c, 0x15, 0xaf, 0xa0, 0xd5, 0x8e, 0x00, 0x00, 0x00, 0x00}));

    auto point = create<chain::output_point>(data);

    BOOST_REQUIRE(point.is_valid());
    BOOST_REQUIRE_EQUAL(encode_hash(point.hash()), "8ed5a0af151cdbc8c0c546cde29334f15b4472bba105394a1221a7f088246846");
    BOOST_REQUIRE(point.index() == 0);

    data_chunk output = point.to_data();
    BOOST_REQUIRE(output == data);
}

BOOST_AUTO_TEST_CASE(output_point__factory_from_data_2__roundtrip__success) {
    static auto const data = to_chunk(base16_literal("46682488f0a721124a3905a1bb72445bf13493e2cd46c5c0c8db1c15afa0d58e00000000"));
    BOOST_REQUIRE(data == (data_chunk{
                              0x46, 0x68, 0x24, 0x88, 0xf0, 0xa7, 0x21, 0x12, 0x4a, 0x39, 0x05, 0xa1,
                              0xbb, 0x72, 0x44, 0x5b, 0xf1, 0x34, 0x93, 0xe2, 0xcd, 0x46, 0xc5, 0xc0,
                              0xc8, 0xdb, 0x1c, 0x15, 0xaf, 0xa0, 0xd5, 0x8e, 0x00, 0x00, 0x00, 0x00}));

    data_source istream(data);
    auto point = create<chain::output_point>(istream);

    BOOST_REQUIRE(point.is_valid());
    BOOST_REQUIRE_EQUAL(encode_hash(point.hash()), "8ed5a0af151cdbc8c0c546cde29334f15b4472bba105394a1221a7f088246846");
    BOOST_REQUIRE(point.index() == 0);

    data_chunk output = point.to_data();
    BOOST_REQUIRE(output == data);
}

BOOST_AUTO_TEST_CASE(output_point__factory_from_data_3__roundtrip__success) {
    static auto const data = to_chunk(base16_literal("46682488f0a721124a3905a1bb72445bf13493e2cd46c5c0c8db1c15afa0d58e00000000"));
    BOOST_REQUIRE(data == (data_chunk{
                              0x46, 0x68, 0x24, 0x88, 0xf0, 0xa7, 0x21, 0x12, 0x4a, 0x39, 0x05, 0xa1,
                              0xbb, 0x72, 0x44, 0x5b, 0xf1, 0x34, 0x93, 0xe2, 0xcd, 0x46, 0xc5, 0xc0,
                              0xc8, 0xdb, 0x1c, 0x15, 0xaf, 0xa0, 0xd5, 0x8e, 0x00, 0x00, 0x00, 0x00}));

    data_source istream(data);
    istream_reader source(istream);
    auto point = create<chain::output_point>(source);

    BOOST_REQUIRE(point.is_valid());
    BOOST_REQUIRE_EQUAL(encode_hash(point.hash()), "8ed5a0af151cdbc8c0c546cde29334f15b4472bba105394a1221a7f088246846");
    BOOST_REQUIRE(point.index() == 0);

    data_chunk output = point.to_data();
    BOOST_REQUIRE(output == data);
}

BOOST_AUTO_TEST_CASE(output_point__is_mature__mature_coinbase_prevout__returns_true) {
    size_t target_height = 162u;
    chain::output_point instance(hash1, 42);
    instance.validation.height = 50u;
    instance.validation.coinbase = true;
    BOOST_REQUIRE(!instance.is_null());
    BOOST_REQUIRE(instance.is_mature(target_height));
}

BOOST_AUTO_TEST_CASE(output_point__is_mature__immature_coinbase_prevout__returns_false) {
    size_t target_height = 162u;
    chain::output_point instance(hash1, 42);
    instance.validation.height = 100u;
    instance.validation.coinbase = true;
    BOOST_REQUIRE(!instance.is_null());
    BOOST_REQUIRE(!instance.is_mature(target_height));
}

BOOST_AUTO_TEST_CASE(output_point__is_mature__immature_coinbase_prevout_null_input__returns_true) {
    size_t target_height = 162u;
    chain::output_point instance(null_hash, chain::point::null_index);
    instance.validation.height = 100u;
    instance.validation.coinbase = true;
    BOOST_REQUIRE(instance.is_null());
    BOOST_REQUIRE(instance.is_mature(target_height));
}

BOOST_AUTO_TEST_CASE(output_point__is_mature__mature_non_coinbase_prevout__returns_true) {
    size_t target_height = 162u;
    chain::output_point instance(hash1, 42);
    instance.validation.height = 50u;
    instance.validation.coinbase = false;
    BOOST_REQUIRE(!instance.is_null());
    BOOST_REQUIRE(instance.is_mature(target_height));
}

BOOST_AUTO_TEST_CASE(output_point__is_mature__immature_non_coinbase_prevout__returns_true) {
    size_t target_height = 162u;
    chain::output_point instance(hash1, 42);
    instance.validation.height = 100u;
    instance.validation.coinbase = false;
    BOOST_REQUIRE(!instance.is_null());
    BOOST_REQUIRE(instance.is_mature(target_height));
}

BOOST_AUTO_TEST_CASE(output_point__operator_assign_equals_1__always__matches_equivalent) {
    chain::output_point expected;
    BOOST_REQUIRE(entity_from_data(expected, valid_raw_output_point));
    chain::output_point instance;
    chain::output_point value;
    BOOST_REQUIRE(entity_from_data(value, valid_raw_output_point));
    instance = std::move(value);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(output_point__operator_assign_equals_2__always__matches_equivalent) {
    chain::output_point expected;
    BOOST_REQUIRE(entity_from_data(expected, valid_raw_output_point));
    chain::output_point instance;
    instance = expected;
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(output_point__operator_assign_equals_3__always__matches_equivalent) {
    chain::point expected;
    BOOST_REQUIRE(entity_from_data(expected, valid_raw_output_point));
    chain::output_point instance;
    chain::point value;
    BOOST_REQUIRE(entity_from_data(value, valid_raw_output_point));
    instance = std::move(value);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(output_point__operator_assign_equals_4__always__matches_equivalent) {
    chain::point expected;
    BOOST_REQUIRE(entity_from_data(expected, valid_raw_output_point));
    chain::output_point instance;
    instance = expected;
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(output_point__operator_boolean_equals_1__duplicates__returns_true) {
    chain::output_point alpha;
    chain::output_point beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_output_point));
    BOOST_REQUIRE(entity_from_data(beta, valid_raw_output_point));
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(output_point__operator_boolean_equals_1__differs__returns_false) {
    chain::output_point alpha;
    chain::output_point beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_output_point));
    BOOST_REQUIRE_EQUAL(false, alpha == beta);
}

BOOST_AUTO_TEST_CASE(output_point__operator_boolean_equals_2__duplicates__returns_true) {
    chain::output_point alpha;
    chain::point beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_output_point));
    BOOST_REQUIRE(entity_from_data(beta, valid_raw_output_point));
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(output_point__operator_boolean_equals_2__differs__returns_false) {
    chain::output_point alpha;
    chain::point beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_output_point));
    BOOST_REQUIRE_EQUAL(false, alpha == beta);
}

BOOST_AUTO_TEST_CASE(output_point__operator_boolean_not_equals_1__duplicates__returns_false) {
    chain::output_point alpha;
    chain::output_point beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_output_point));
    BOOST_REQUIRE(entity_from_data(beta, valid_raw_output_point));
    BOOST_REQUIRE_EQUAL(false, alpha != beta);
}

BOOST_AUTO_TEST_CASE(output_point__operator_boolean_not_equals_1__differs__returns_true) {
    chain::output_point alpha;
    chain::output_point beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_output_point));
    BOOST_REQUIRE(alpha != beta);
}

BOOST_AUTO_TEST_CASE(output_point__operator_boolean_not_equals_2__duplicates__returns_false) {
    chain::output_point alpha;
    chain::point beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_output_point));
    BOOST_REQUIRE(entity_from_data(beta, valid_raw_output_point));
    BOOST_REQUIRE_EQUAL(false, alpha != beta);
}

BOOST_AUTO_TEST_CASE(output_point__operator_boolean_not_equals_2__differs__returns_true) {
    chain::output_point alpha;
    chain::point beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_output_point));
    BOOST_REQUIRE(alpha != beta);
}

BOOST_AUTO_TEST_SUITE_END()

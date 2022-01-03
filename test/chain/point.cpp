// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

auto const valid_raw_point = to_chunk(base16_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f00000015"));

// Start Boost Suite: point tests

TEST_CASE("point  constructor 1  always  returns default initialized", "[point]") {
    chain::point instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("point  constructor 2  valid input  returns input initialized", "[point]") {
    auto const hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    uint32_t index = 1234u;

    chain::point instance(hash, index);
    REQUIRE(instance.is_valid());
    REQUIRE(hash == instance.hash());
    REQUIRE(index == instance.index());
}

TEST_CASE("point  constructor 3  valid input  returns input initialized", "[point]") {
    auto const hash = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    uint32_t index = 1234u;

    // This must be non-const.
    auto dup_hash = hash;

    chain::point instance(std::move(dup_hash), index);
    REQUIRE(instance.is_valid());
    REQUIRE(hash == instance.hash());
    REQUIRE(index == instance.index());
}

TEST_CASE("point  constructor 4  valid input  returns input initialized", "[point]") {
    const chain::point expected{
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        524342u};

    chain::point instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("point  constructor 5  valid input  returns input initialized", "[point]") {
    // This must be non-const.
    chain::point expected{
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        524342u};

    chain::point instance(std::move(expected));
    REQUIRE(instance.is_valid());
}

TEST_CASE("point  begin end  initialized  begin not equal end", "[point]") {
    chain::point instance{null_hash, 0};

    REQUIRE(instance.begin() != instance.end());
}

TEST_CASE("point  from data  insufficient bytes  failure", "[point]") {
    data_chunk data(10);
    chain::point instance;

    REQUIRE( ! entity_from_data(instance, data));
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("point  from data  roundtrip  success", "[point]") {
    uint32_t index = 53213;
    hash_digest const hash{
        {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
         0x01, 0x01, 0xab, 0x11, 0x11, 0xcd, 0x11, 0x11,
         0x01, 0x10, 0x11, 0xab, 0x11, 0x11, 0xcd, 0x11,
         0x01, 0x11, 0x11, 0x11, 0xab, 0x11, 0x11, 0xcd}};

    chain::point initial{hash, index};

    REQUIRE(initial.is_valid());
    REQUIRE(hash == initial.hash());
    REQUIRE(index == initial.index());

    chain::point point;

    REQUIRE(point != initial);
    REQUIRE(entity_from_data(point, initial.to_data()));
    REQUIRE(point.is_valid());
    REQUIRE(point == initial);
}

TEST_CASE("point  factory from data 1  roundtrip  success", "[point]") {
    auto const raw = to_chunk(base16_literal("46682488f0a721124a3905a1bb72445bf13493e2cd46c5c0c8db1c15afa0d58e00000000"));
    auto const data = data_chunk{
        0x46, 0x68, 0x24, 0x88, 0xf0, 0xa7, 0x21, 0x12, 0x4a, 0x39, 0x05, 0xa1,
        0xbb, 0x72, 0x44, 0x5b, 0xf1, 0x34, 0x93, 0xe2, 0xcd, 0x46, 0xc5, 0xc0,
        0xc8, 0xdb, 0x1c, 0x15, 0xaf, 0xa0, 0xd5, 0x8e, 0x00, 0x00, 0x00, 0x00};

    REQUIRE(raw == data);

    auto point = create<chain::point>(raw);

    REQUIRE(point.is_valid());
    REQUIRE(encode_hash(point.hash()) == "8ed5a0af151cdbc8c0c546cde29334f15b4472bba105394a1221a7f088246846");
    REQUIRE(point.index() == 0);

    data_chunk output = point.to_data();
    REQUIRE(output == raw);
}

TEST_CASE("point  factory from data 2  roundtrip  success", "[point]") {
    auto const raw = to_chunk(base16_literal("46682488f0a721124a3905a1bb72445bf13493e2cd46c5c0c8db1c15afa0d58e00000000"));
    data_chunk const data{
        0x46, 0x68, 0x24, 0x88, 0xf0, 0xa7, 0x21, 0x12, 0x4a, 0x39, 0x05, 0xa1,
        0xbb, 0x72, 0x44, 0x5b, 0xf1, 0x34, 0x93, 0xe2, 0xcd, 0x46, 0xc5, 0xc0,
        0xc8, 0xdb, 0x1c, 0x15, 0xaf, 0xa0, 0xd5, 0x8e, 0x00, 0x00, 0x00, 0x00};

    REQUIRE(raw == data);

    data_source istream(raw);
    auto point = create<chain::point>(istream);

    REQUIRE(point.is_valid());
    REQUIRE(encode_hash(point.hash()) == "8ed5a0af151cdbc8c0c546cde29334f15b4472bba105394a1221a7f088246846");
    REQUIRE(point.index() == 0);

    data_chunk output = point.to_data();
    REQUIRE(output == raw);
}

TEST_CASE("point  factory from data 3  roundtrip  success", "[point]") {
    auto const raw = to_chunk(base16_literal("46682488f0a721124a3905a1bb72445bf13493e2cd46c5c0c8db1c15afa0d58e00000000"));
    data_chunk const data{
        0x46, 0x68, 0x24, 0x88, 0xf0, 0xa7, 0x21, 0x12, 0x4a, 0x39, 0x05, 0xa1,
        0xbb, 0x72, 0x44, 0x5b, 0xf1, 0x34, 0x93, 0xe2, 0xcd, 0x46, 0xc5, 0xc0,
        0xc8, 0xdb, 0x1c, 0x15, 0xaf, 0xa0, 0xd5, 0x8e, 0x00, 0x00, 0x00, 0x00};

    REQUIRE(raw == data);

    data_source istream(raw);
    istream_reader source(istream);
    auto point = create<chain::point>(source);

    REQUIRE(point.is_valid());
    REQUIRE(encode_hash(point.hash()) == "8ed5a0af151cdbc8c0c546cde29334f15b4472bba105394a1221a7f088246846");
    REQUIRE(point.index() == 0);

    data_chunk output = point.to_data();
    REQUIRE(output == raw);
}

TEST_CASE("point  hash setter 1  roundtrip  success", "[point]") {
    auto const value = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    chain::point instance;
    REQUIRE(value != instance.hash());
    instance.set_hash(value);
    REQUIRE(value == instance.hash());
}

TEST_CASE("point  hash setter 2  roundtrip  success", "[point]") {
    auto const value = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    // This must be non-const.
    auto dup_value = value;

    chain::point instance;
    REQUIRE(value != instance.hash());
    instance.set_hash(std::move(dup_value));
    REQUIRE(value == instance.hash());
}

TEST_CASE("point  index  roundtrip  success", "[point]") {
    uint32_t value = 1254u;
    chain::point instance;
    REQUIRE(value != instance.index());
    instance.set_index(value);
    REQUIRE(value == instance.index());
}

TEST_CASE("point  operator assign equals 1  always  matches equivalent", "[point]") {
    chain::point expected;
    REQUIRE(entity_from_data(expected, valid_raw_point));
    chain::point instance;

    // This must be non-const.
    chain::point value;

    REQUIRE(entity_from_data(value, valid_raw_point));
    instance = std::move(value);
    REQUIRE(instance == expected);
}

TEST_CASE("point  operator assign equals 2  always  matches equivalent", "[point]") {
    chain::point expected;
    REQUIRE(entity_from_data(expected, valid_raw_point));
    chain::point instance;
    instance = expected;
    REQUIRE(instance == expected);
}

TEST_CASE("point  operator boolean equals  duplicates  returns true", "[point]") {
    chain::point alpha;
    chain::point beta;
    REQUIRE(entity_from_data(alpha, valid_raw_point));
    REQUIRE(entity_from_data(beta, valid_raw_point));
    REQUIRE(alpha == beta);
}

TEST_CASE("point  operator boolean equals  differs  returns false", "[point]") {
    chain::point alpha;
    chain::point beta;
    REQUIRE(entity_from_data(alpha, valid_raw_point));
    REQUIRE(alpha != beta);
}

TEST_CASE("point  operator boolean not equals  duplicates  returns false", "[point]") {
    chain::point alpha;
    chain::point beta;
    REQUIRE(entity_from_data(alpha, valid_raw_point));
    REQUIRE(entity_from_data(beta, valid_raw_point));
    REQUIRE(alpha == beta);
}

TEST_CASE("point  operator boolean not equals  differs  returns true", "[point]") {
    chain::point alpha;
    chain::point beta;
    REQUIRE(entity_from_data(alpha, valid_raw_point));
    REQUIRE(alpha != beta);
}

TEST_CASE("point  checksum  all ones  returns all ones", "[point]") {
    static auto const tx_hash = hash_literal("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    chain::point instance(tx_hash, 0xffffffff);
    REQUIRE(instance.checksum() == 0xffffffffffffffff);
}

TEST_CASE("point  checksum  all zeroes  returns all zeroes", "[point]") {
    static auto const tx_hash = hash_literal("0000000000000000000000000000000000000000000000000000000000000000");
    chain::point instance(tx_hash, 0x00000000);
    REQUIRE(instance.checksum() == 0x0000000000000000);
}

TEST_CASE("point  checksum  pattern one  returns expected", "[point]") {
    static auto const tx_hash = hash_literal("000000000000000000000000aaaaaaaaaaaaaaaa000000000000000000000000");
    chain::point instance(tx_hash, 0x00000001);
    REQUIRE(instance.checksum() == 0xaaaaaaaaaaaa8001);
}

TEST_CASE("point  checksum  pattern high  returns expected", "[point]") {
    static auto const tx_hash = hash_literal("ffffffffffffffffffffffff01234567aaaaaaaaffffffffffffffffffffffff");
    chain::point instance(tx_hash, 0x89abcdef);
    REQUIRE(instance.checksum() == 0x1234567aaaacdef);
}

// End Boost Suite

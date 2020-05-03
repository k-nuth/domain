// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chrono>

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// Start Boost Suite: chain header tests

TEST_CASE("chain header constructor 1 always initialized invalid", "[chain header]") {
    chain::header instance;
    REQUIRE(!instance.is_valid());
}

TEST_CASE("chain header  constructor 2  always  equals params", "[chain header]") {
    uint32_t const version = 10u;
    auto const previous = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    auto const merkle = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    uint32_t const timestamp = 531234u;
    uint32_t const bits = 6523454u;
    uint32_t const nonce = 68644u;

    chain::header instance(version, previous, merkle, timestamp, bits, nonce);
    REQUIRE(instance.is_valid());
    REQUIRE(version == instance.version());
    REQUIRE(timestamp == instance.timestamp());
    REQUIRE(bits == instance.bits());
    REQUIRE(nonce == instance.nonce());
    REQUIRE(previous == instance.previous_block_hash());
    REQUIRE(merkle == instance.merkle());
}

TEST_CASE("chain header  constructor 3  always  equals params", "[chain header]") {
    uint32_t const version = 10u;
    uint32_t const timestamp = 531234u;
    uint32_t const bits = 6523454u;
    uint32_t const nonce = 68644u;

    // These must be non-const.
    auto previous = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    auto merkle = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    chain::header instance(version, std::move(previous), std::move(merkle), timestamp, bits, nonce);
    REQUIRE(instance.is_valid());
    REQUIRE(version == instance.version());
    REQUIRE(timestamp == instance.timestamp());
    REQUIRE(bits == instance.bits());
    REQUIRE(nonce == instance.nonce());
    REQUIRE(previous == instance.previous_block_hash());
    REQUIRE(merkle == instance.merkle());
}

TEST_CASE("chain header  constructor 4  always  equals params", "[chain header]") {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("chain header  constructor 5  always  equals params", "[chain header]") {
    // This must be non-const.
    chain::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance(std::move(expected));
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("chain header  from data  insufficient bytes  failure", "[chain header]") {
    data_chunk data(10);

    chain::header header;

    REQUIRE(!entity_from_data(header, data));
    REQUIRE(!header.is_valid());
}

TEST_CASE("chain header  factory from data 1  valid input  success", "[chain header]") {
    chain::header expected{
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644};

    auto const data = expected.to_data();

    auto const result = create<chain::header>(data);

    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
}

TEST_CASE("chain header  factory from data 2  valid input  success", "[chain header]") {
    chain::header expected{
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644};

    auto const data = expected.to_data();
    data_source istream(data);

    auto const result = create<chain::header>(istream);

    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
}

TEST_CASE("chain header  factory from data 3  valid input  success", "[chain header]") {
    chain::header const expected{
        10,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234,
        6523454,
        68644};

    auto const data = expected.to_data();
    data_source istream(data);
    istream_reader source(istream);

    auto const result = create<chain::header>(source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(header__version_accessor__always__returns_initialized_value) {
    uint32_t const value = 11234u;
    chain::header const instance(
        value,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE_EQUAL(value, instance.version());
}

BOOST_AUTO_TEST_CASE(header__version_setter__roundtrip__success) {
    uint32_t expected = 4521u;
    chain::header instance;
    BOOST_REQUIRE(expected != instance.version());
    instance.set_version(expected);
    BOOST_REQUIRE(expected == instance.version());
}

BOOST_AUTO_TEST_CASE(header__previous_block_hash_accessor_1__always__returns_initialized_value) {
    auto const value = hash_literal("abababababababababababababababababababababababababababababababab");
    chain::header instance(
        11234u,
        value,
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE(value == instance.previous_block_hash());
}

BOOST_AUTO_TEST_CASE(header__previous_block_hash_accessor_2__always__returns_initialized_value) {
    auto const value = hash_literal("abababababababababababababababababababababababababababababababab");
    chain::header const instance(
        11234u,
        value,
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE(value == instance.previous_block_hash());
}

BOOST_AUTO_TEST_CASE(header__previous_block_hash_setter_1__roundtrip__success) {
    auto const expected = hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe");
    chain::header instance;
    BOOST_REQUIRE(expected != instance.previous_block_hash());
    instance.set_previous_block_hash(expected);
    BOOST_REQUIRE(expected == instance.previous_block_hash());
}

BOOST_AUTO_TEST_CASE(header__previous_block_hash_setter_2__roundtrip__success) {
    auto const expected = hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe");

    // This must be non-const.
    auto duplicate = expected;

    chain::header instance;
    BOOST_REQUIRE(expected != instance.previous_block_hash());
    instance.set_previous_block_hash(std::move(duplicate));
    BOOST_REQUIRE(expected == instance.previous_block_hash());
}

BOOST_AUTO_TEST_CASE(header__merkle_accessor_1__always__returns_initialized_value) {
    auto const value = hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe");
    chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        value,
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE(value == instance.merkle());
}

BOOST_AUTO_TEST_CASE(header__merkle_accessor_2__always__returns_initialized_value) {
    auto const value = hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe");
    chain::header const instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        value,
        753234u,
        4356344u,
        34564u);

    BOOST_REQUIRE(value == instance.merkle());
}

BOOST_AUTO_TEST_CASE(header__merkle_setter_1__roundtrip__success) {
    auto const expected = hash_literal("abababababababababababababababababababababababababababababababab");
    chain::header instance;
    BOOST_REQUIRE(expected != instance.merkle());
    instance.set_merkle(expected);
    BOOST_REQUIRE(expected == instance.merkle());
}

BOOST_AUTO_TEST_CASE(header__merkle_setter_2__roundtrip__success) {
    auto const expected = hash_literal("abababababababababababababababababababababababababababababababab");

    // This must be non-const.
    hash_digest duplicate = expected;

    chain::header instance;
    BOOST_REQUIRE(expected != instance.merkle());
    instance.set_merkle(std::move(duplicate));
    BOOST_REQUIRE(expected == instance.merkle());
}

BOOST_AUTO_TEST_CASE(header__timestamp_accessor__always__returns_initialized_value) {
    uint32_t value = 753234u;
    chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        value,
        4356344u,
        34564u);

    BOOST_REQUIRE_EQUAL(value, instance.timestamp());
}

BOOST_AUTO_TEST_CASE(header__timestamp_setter__roundtrip__success) {
    uint32_t expected = 4521u;
    chain::header instance;
    BOOST_REQUIRE(expected != instance.timestamp());
    instance.set_timestamp(expected);
    BOOST_REQUIRE(expected == instance.timestamp());
}

BOOST_AUTO_TEST_CASE(header__bits_accessor__always__returns_initialized_value) {
    uint32_t value = 4356344u;
    chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        value,
        34564u);

    BOOST_REQUIRE_EQUAL(value, instance.bits());
}

BOOST_AUTO_TEST_CASE(header__bits_setter__roundtrip__success) {
    uint32_t expected = 4521u;
    chain::header instance;
    BOOST_REQUIRE(expected != instance.bits());
    instance.set_bits(expected);
    BOOST_REQUIRE(expected == instance.bits());
}

BOOST_AUTO_TEST_CASE(header__nonce_accessor__always__returns_initialized_value) {
    uint32_t value = 34564u;
    chain::header instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        4356344u,
        value);

    BOOST_REQUIRE_EQUAL(value, instance.nonce());
}

BOOST_AUTO_TEST_CASE(header__nonce_setter__roundtrip__success) {
    uint32_t expected = 4521u;
    chain::header instance;
    BOOST_REQUIRE(expected != instance.nonce());
    instance.set_nonce(expected);
    BOOST_REQUIRE(expected == instance.nonce());
}

BOOST_AUTO_TEST_CASE(header__is_valid_timestamp__timestamp_less_than_2_hours_from_now__returns_true) {
    chain::header instance;
    auto const now = std::chrono::system_clock::now();
    auto const now_time = std::chrono::system_clock::to_time_t(now);
    instance.set_timestamp(static_cast<uint32_t>(now_time));
    BOOST_REQUIRE(instance.is_valid_timestamp());
}

BOOST_AUTO_TEST_CASE(header__is_valid_timestamp__timestamp_greater_than_2_hours_from_now__returns_false) {
    chain::header instance;
    auto const now = std::chrono::system_clock::now();
    auto const duration = std::chrono::hours(3);
    auto const future = std::chrono::system_clock::to_time_t(now + duration);
    instance.set_timestamp(static_cast<uint32_t>(future));
    BOOST_REQUIRE(!instance.is_valid_timestamp());
}

BOOST_AUTO_TEST_CASE(header__proof1__genesis_mainnet__expected) {
    BOOST_REQUIRE_EQUAL(chain::header::proof(0x1d00ffff), 0x0000000100010001);
}

BOOST_AUTO_TEST_CASE(header__is_valid_proof_of_work__bits_exceeds_maximum__returns_false) {
    chain::header instance;
    instance.set_bits(retarget_proof_of_work_limit + 1);
    BOOST_REQUIRE(!instance.is_valid_proof_of_work());
}

BOOST_AUTO_TEST_CASE(header__is_valid_proof_of_work__retarget_bits_exceeds_maximum__returns_false) {
    chain::header instance;
    instance.set_bits(no_retarget_proof_of_work_limit + 1);
    BOOST_REQUIRE(!instance.is_valid_proof_of_work(false));
}

BOOST_AUTO_TEST_CASE(header__is_valid_proof_of_work__hash_greater_bits__returns_false) {
    chain::header const instance(
        11234u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
        753234u,
        0u,
        34564u);

    BOOST_REQUIRE(!instance.is_valid_proof_of_work());
}

BOOST_AUTO_TEST_CASE(header__is_valid_proof_of_work__hash_less_than_bits__returns_true) {
    chain::header const instance(
        4u,
        hash_literal("000000000000000003ddc1e929e2944b8b0039af9aa0d826c480a83d8b39c373"),
        hash_literal("a6cb0b0d6531a71abe2daaa4a991e5498e1b6b0b51549568d0f9d55329b905df"),
        1474388414u,
        402972254u,
        2842832236u);

    BOOST_REQUIRE(instance.is_valid_proof_of_work());
}

BOOST_AUTO_TEST_CASE(header__operator_assign_equals__always__matches_equivalent) {
    // This must be non-const.
    chain::header value(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    BOOST_REQUIRE(value.is_valid());

    chain::header instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_equals__duplicates__returns_true) {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_equals__differs__returns_false) {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_not_equals__duplicates__returns_false) {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_not_equals__differs__returns_true) {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::header instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

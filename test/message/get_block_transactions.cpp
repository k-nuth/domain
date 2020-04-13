// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// Start Boost Suite: get block transactions tests

TEST_CASE("get block transactions  constructor 1  always invalid", "[get block transactions]") {
    message::get_block_transactions instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("get block transactions  constructor 2  always  equals params", "[get block transactions]") {
    hash_digest const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};

    message::get_block_transactions instance(hash, indexes);
    REQUIRE(instance.is_valid());
    REQUIRE(hash == instance.block_hash());
    REQUIRE(indexes == instance.indexes());
}

TEST_CASE("get block transactions  constructor 3  always  equals params", "[get block transactions]") {
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    auto hash_dup = hash;
    std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};
    auto indexes_dup = indexes;

    message::get_block_transactions instance(std::move(hash_dup), std::move(indexes_dup));
    REQUIRE(instance.is_valid());
    REQUIRE(hash == instance.block_hash());
    REQUIRE(indexes == instance.indexes());
}

TEST_CASE("get block transactions  constructor 4  always  equals params", "[get block transactions]") {
    message::get_block_transactions value(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {1u, 3454u, 4234u, 75123u, 455323u});

    message::get_block_transactions instance(value);
    REQUIRE(instance.is_valid());
    REQUIRE(value == instance);
}

TEST_CASE("get block transactions  constructor 5  always  equals params", "[get block transactions]") {
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};

    message::get_block_transactions value(hash, indexes);
    message::get_block_transactions instance(std::move(value));
    REQUIRE(instance.is_valid());
    REQUIRE(hash == instance.block_hash());
    REQUIRE(indexes == instance.indexes());
}

TEST_CASE("get block transactions  from data  insufficient bytes  failure", "[get block transactions]") {
    data_chunk const raw{0xab, 0xcd};
    message::get_block_transactions instance{};

    REQUIRE( ! entity_from_data(instance, message::version::level::minimum, raw));
}

TEST_CASE("get block transactions  factory from data 1  valid input  success", "[get block transactions]") {
    const message::get_block_transactions expected{
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {16,
         32,
         37,
         44}};

    auto const data = expected.to_data(message::version::level::minimum);
    auto const result = create<message::get_block_transactions>(
        message::version::level::minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
                        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_block_transactions__factory_from_data_2__valid_input__success) {
    const message::get_block_transactions expected{
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {16,
         32,
         37,
         44}};

    auto const data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    auto result = create<message::get_block_transactions>(
        message::version::level::minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
                        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_block_transactions__factory_from_data_3__valid_input__success) {
    const message::get_block_transactions expected{
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {16,
         32,
         37,
         44}};

    auto const data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::get_block_transactions>(
        message::version::level::minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
                        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_block_transactions__block_hash_accessor_1__always__returns_initialized_value) {
    hash_digest const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};
    message::get_block_transactions instance(hash, indexes);
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__block_hash_accessor_2__always__returns_initialized_value) {
    hash_digest const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};
    const message::get_block_transactions instance(hash, indexes);
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__block_hash_setter_1__roundtrip__success) {
    hash_digest const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::get_block_transactions instance;
    BOOST_REQUIRE(hash != instance.block_hash());
    instance.set_block_hash(hash);
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__block_hash_setter_2__roundtrip__success) {
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    auto dup = hash;
    message::get_block_transactions instance;
    BOOST_REQUIRE(hash != instance.block_hash());
    instance.set_block_hash(std::move(dup));
    BOOST_REQUIRE(hash == instance.block_hash());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__indexes_accessor_1__always__returns_initialized_value) {
    hash_digest const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};
    message::get_block_transactions instance(hash, indexes);
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__indexes_accessor_2__always__returns_initialized_value) {
    hash_digest const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};
    const message::get_block_transactions instance(hash, indexes);
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__indexes_setter_1__roundtrip__success) {
    const std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};
    message::get_block_transactions instance;
    BOOST_REQUIRE(indexes != instance.indexes());
    instance.set_indexes(indexes);
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__indexes_setter_2__roundtrip__success) {
    std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};
    auto dup = indexes;
    message::get_block_transactions instance;
    BOOST_REQUIRE(indexes != instance.indexes());
    instance.set_indexes(std::move(dup));
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_assign_equals__always__matches_equivalent) {
    hash_digest const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const std::vector<uint64_t> indexes = {1u, 3454u, 4234u, 75123u, 455323u};
    message::get_block_transactions value(hash, indexes);

    BOOST_REQUIRE(value.is_valid());

    message::get_block_transactions instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(hash == instance.block_hash());
    BOOST_REQUIRE(indexes == instance.indexes());
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_boolean_equals__duplicates__returns_true) {
    const message::get_block_transactions expected(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {1u, 3454u, 4234u, 75123u, 455323u});

    message::get_block_transactions instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_boolean_equals__differs__returns_false) {
    const message::get_block_transactions expected(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {1u, 3454u, 4234u, 75123u, 455323u});

    message::get_block_transactions instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_boolean_not_equals__duplicates__returns_false) {
    const message::get_block_transactions expected(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {1u, 3454u, 4234u, 75123u, 455323u});

    message::get_block_transactions instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(get_block_transactions__operator_boolean_not_equals__differs__returns_true) {
    const message::get_block_transactions expected(
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        {1u, 3454u, 4234u, 75123u, 455323u});

    message::get_block_transactions instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

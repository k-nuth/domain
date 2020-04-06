// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::message;

// Start Boost Suite: inventory vector tests

TEST_CASE("inventory vector  to number  error  returns 0", "[inventory vector]") {
    REQUIRE(inventory_vector::to_number(inventory_vector::type_id::error) == 0u);
}

TEST_CASE("inventory vector  to number  transaction  returns 1", "[inventory vector]") {
    REQUIRE(inventory_vector::to_number(inventory_vector::type_id::transaction) == 1u);
}

TEST_CASE("inventory vector  to number  block  returns 2", "[inventory vector]") {
    REQUIRE(inventory_vector::to_number(inventory_vector::type_id::block) == 2u);
}

TEST_CASE("inventory vector  to number  filtered block  returns 3", "[inventory vector]") {
    REQUIRE(inventory_vector::to_number(inventory_vector::type_id::filtered_block) == 3u);
}

TEST_CASE("inventory vector  to number  compact block  returns 4", "[inventory vector]") {
    REQUIRE(inventory_vector::to_number(inventory_vector::type_id::compact_block) == 4u);
}

TEST_CASE("inventory vector  to type  0  returns error", "[inventory vector]") {
    REQUIRE(inventory_vector::type_id::error == inventory_vector::to_type(0));
}

TEST_CASE("inventory vector  to type  1  returns transaction", "[inventory vector]") {
    REQUIRE(inventory_vector::type_id::transaction == inventory_vector::to_type(1));
}

TEST_CASE("inventory vector  to type  2  returns block", "[inventory vector]") {
    REQUIRE(inventory_vector::type_id::block == inventory_vector::to_type(2));
}

TEST_CASE("inventory vector  to type  3  returns filtered block", "[inventory vector]") {
    REQUIRE(inventory_vector::type_id::filtered_block == inventory_vector::to_type(3));
}

TEST_CASE("inventory vector  to type  4  returns compact block", "[inventory vector]") {
    REQUIRE(inventory_vector::type_id::compact_block == inventory_vector::to_type(4));
}

TEST_CASE("inventory vector  constructor 1  always invalid", "[inventory vector]") {
    inventory_vector instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("inventory vector  constructor 2  always  equals params", "[inventory vector]") {
    inventory_vector::type_id type = inventory_vector::type_id::block;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance(type, hash);
    REQUIRE(instance.is_valid());
    REQUIRE(type == instance.type());
    REQUIRE(hash == instance.hash());
}

TEST_CASE("inventory vector  constructor 3  always  equals params", "[inventory vector]") {
    inventory_vector::type_id type = inventory_vector::type_id::block;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance(type, std::move(hash));
    REQUIRE(instance.is_valid());
}

TEST_CASE("inventory vector  constructor 4  always  equals params", "[inventory vector]") {
    inventory_vector::type_id type = inventory_vector::type_id::block;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector const expected(type, hash);
    REQUIRE(expected.is_valid());
    inventory_vector instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("inventory vector  constructor 5  always  equals params", "[inventory vector]") {
    inventory_vector::type_id type = inventory_vector::type_id::block;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector expected(type, hash);
    REQUIRE(expected.is_valid());
    inventory_vector instance(std::move(expected));
    REQUIRE(instance.is_valid());
    REQUIRE(type == instance.type());
    REQUIRE(hash == instance.hash());
}

TEST_CASE("inventory vector  from data  insufficient bytes  failure", "[inventory vector]") {
    static data_chunk const raw{1};
    inventory_vector instance;
    REQUIRE( ! entity_from_data(instance, version::level::minimum, raw));
}

TEST_CASE("inventory vector  factory from data 1  valid input  success", "[inventory vector]") {
    static inventory_vector const expected{
        inventory_vector::type_id::error,
        {{0x44, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
          0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
          0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
          0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf}}};

    static auto const version = version::level::minimum;
    auto const data = expected.to_data(version);
    auto const result = create<inventory_vector>(version, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(inventory_vector__factory_from_data_2__valid_input__success) {
    static inventory_vector const expected{
        inventory_vector::type_id::transaction,
        {{0x44, 0x9a, 0x0d, 0xee, 0x9a, 0xd5, 0x39, 0xee,
          0xee, 0x85, 0x0a, 0x3d, 0xee, 0x24, 0xed, 0x0f,
          0xc3, 0xee, 0x6f, 0x55, 0x7d, 0xee, 0x12, 0x1a,
          0x37, 0xc0, 0xee, 0x32, 0xf0, 0xd6, 0xee, 0xdf}}};

    static auto const version = version::level::minimum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    auto const result = create<inventory_vector>(version, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(inventory_vector__factory_from_data_3__valid_input__success) {
    static inventory_vector const expected{
        inventory_vector::type_id::block,
        {{0x66, 0x9a, 0x0d, 0x24, 0x66, 0xd5, 0x39, 0x89,
          0xbb, 0x66, 0x0a, 0x3d, 0x79, 0x66, 0xed, 0x0f,
          0xc3, 0x0d, 0x66, 0x55, 0x7d, 0x71, 0x66, 0x1a,
          0x37, 0xc0, 0xb0, 0x66, 0xf0, 0xd6, 0x6e, 0x66}}};

    static auto const version = version::level::minimum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<inventory_vector>(version, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

#if defined(KTH_SEGWIT_ENABLED)
BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__error__unchanged) {
    static auto const expected = inventory_vector::type_id::error;
    inventory_vector instance{expected, {}};
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__filtered_block__unchanged) {
    static auto const expected = inventory_vector::type_id::filtered_block;
    inventory_vector instance{expected, {}};
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__compact_block__unchanged) {
    static auto const expected = inventory_vector::type_id::compact_block;
    inventory_vector instance{expected, {}};
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__witness_transaction__unchanged) {
    static auto const expected = inventory_vector::type_id::witness_transaction;
    inventory_vector instance{expected, {}};
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__witness_block__unchanged) {
    static auto const expected = inventory_vector::type_id::witness_block;
    inventory_vector instance{expected, {}};
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__block__expected) {
    inventory_vector instance{inventory_vector::type_id::block, {}};
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == inventory_vector::type_id::witness_block);
}

BOOST_AUTO_TEST_CASE(inventory_vector__to_witness__transaction__expected) {
    inventory_vector instance{inventory_vector::type_id::transaction, {}};
    instance.to_witness();
    BOOST_REQUIRE(instance.type() == inventory_vector::type_id::witness_transaction);
}
#endif // KTH_CURRENCY_BCH

BOOST_AUTO_TEST_CASE(inventory_vector__is_block_type__block__returns_true) {
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::block);
    BOOST_REQUIRE(instance.is_block_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_block_type__compact_block__returns_true) {
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::compact_block);
    BOOST_REQUIRE(instance.is_block_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_block_type__filtered_block__returns_true) {
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::filtered_block);
    BOOST_REQUIRE(instance.is_block_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_block_type__non_block_type__returns_false) {
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::transaction);
    BOOST_REQUIRE_EQUAL(false, instance.is_block_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_transaction_type__transaction__returns_true) {
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::transaction);
    BOOST_REQUIRE(instance.is_transaction_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__is_transaction_type__non_transaction_type__returns_false) {
    inventory_vector instance;
    instance.set_type(inventory_vector::type_id::block);
    BOOST_REQUIRE_EQUAL(false, instance.is_transaction_type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__type_accessor__always__returns_initialized_value) {
    inventory_vector::type_id type = inventory_vector::type_id::transaction;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance(type, hash);
    BOOST_REQUIRE(type == instance.type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__type_setter__roundtrip__success) {
    inventory_vector::type_id type = inventory_vector::type_id::transaction;
    inventory_vector instance;
    BOOST_REQUIRE(type != instance.type());
    instance.set_type(type);
    BOOST_REQUIRE(type == instance.type());
}

BOOST_AUTO_TEST_CASE(inventory_vector__hash_accessor__always__returns_initialized_value) {
    inventory_vector::type_id type = inventory_vector::type_id::transaction;
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance(type, hash);
    BOOST_REQUIRE(hash == instance.hash());
}

BOOST_AUTO_TEST_CASE(inventory_vector__hash_setter_1__roundtrip__success) {
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance;
    BOOST_REQUIRE(hash != instance.hash());
    instance.set_hash(hash);
    BOOST_REQUIRE(hash == instance.hash());
}

BOOST_AUTO_TEST_CASE(inventory_vector__hash_setter_2__roundtrip__success) {
    hash_digest duplicate = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    hash_digest hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector instance;
    BOOST_REQUIRE(duplicate != instance.hash());
    instance.set_hash(std::move(hash));
    BOOST_REQUIRE(duplicate == instance.hash());
}

BOOST_AUTO_TEST_CASE(inventory_vector__operator_assign_equals_1__always__matches_equivalent) {
    inventory_vector value(
        inventory_vector::type_id::compact_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    BOOST_REQUIRE(value.is_valid());

    inventory_vector instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(inventory_vector__operator_assign_equals_2__always__matches_equivalent) {
    inventory_vector value(
        inventory_vector::type_id::compact_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    BOOST_REQUIRE(value.is_valid());

    inventory_vector instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = value;
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
}

BOOST_AUTO_TEST_CASE(inventory_vector__operator_boolean_equals__duplicates__returns_true) {
    inventory_vector const expected(
        inventory_vector::type_id::filtered_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    inventory_vector instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(inventory_vector__operator_boolean_equals__differs__returns_false) {
    inventory_vector const expected(
        inventory_vector::type_id::filtered_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    inventory_vector instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(reject__operator_boolean_not_equals__duplicates__returns_false) {
    inventory_vector const expected(
        inventory_vector::type_id::filtered_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    inventory_vector instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(reject__operator_boolean_not_equals__differs__returns_true) {
    inventory_vector const expected(
        inventory_vector::type_id::filtered_block,
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    inventory_vector instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

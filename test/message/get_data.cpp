// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::message;

// Start Boost Suite: get data tests

TEST_CASE("get data  constructor 1  always invalid", "[get data]") {
    const get_data instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("get data  constructor 2  always  equals params", "[get data]") {
    static inventory_vector::list const values =
        {
            inventory_vector{
                inventory::type_id::error,
                {{0x44, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
                  0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
                  0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
                  0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf}}}};

    const get_data instance(values);
    REQUIRE(instance.is_valid());
    REQUIRE(values == instance.inventories());
}

TEST_CASE("get data  constructor 3  always  equals params", "[get data]") {
    static auto const type = inventory_vector::type_id::error;
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    inventory_vector::list const values{
        inventory_vector(type, hash)};

    const get_data instance(std::move(values));
    REQUIRE(instance.is_valid());
    auto const inventories = instance.inventories();
    REQUIRE(1u == inventories.size());
    REQUIRE(type == inventories[0].type());
    REQUIRE(hash == inventories[0].hash());
}

TEST_CASE("get data  constructor 4  always  equals params", "[get data]") {
    static auto const type = inventory_vector::type_id::error;
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    static hash_list const hashes{hash};

    const get_data instance(hashes, type);
    REQUIRE(instance.is_valid());
    auto inventories = instance.inventories();
    REQUIRE(1u == inventories.size());
    REQUIRE(type == inventories[0].type());
    REQUIRE(hash == inventories[0].hash());
}

TEST_CASE("get data  constructor 5  always  equals params", "[get data]") {
    static auto const type = inventory_vector::type_id::error;
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const get_data instance{{type, hash}};
    REQUIRE(instance.is_valid());
    auto inventories = instance.inventories();
    REQUIRE(1u == inventories.size());
    REQUIRE(type == inventories[0].type());
    REQUIRE(hash == inventories[0].hash());
}

TEST_CASE("get data  constructor 6  always  equals params", "[get data]") {
    static auto const type = inventory_vector::type_id::error;
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const get_data value{{type, hash}};
    REQUIRE(value.is_valid());
    const get_data instance(value);
    auto const inventories = instance.inventories();
    REQUIRE(1u == inventories.size());
    REQUIRE(type == inventories[0].type());
    REQUIRE(hash == inventories[0].hash());
    REQUIRE(value == instance);
}

TEST_CASE("get data  constructor 7  always  equals params", "[get data]") {
    static auto const type = inventory_vector::type_id::error;
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const get_data value{{type, hash}};
    REQUIRE(value.is_valid());
    const get_data instance(std::move(value));
    auto const inventories = instance.inventories();
    REQUIRE(1u == inventories.size());
    REQUIRE(type == inventories[0].type());
    REQUIRE(hash == inventories[0].hash());
}

TEST_CASE("get data  from data  insufficient bytes  failure", "[get data]") {
    static data_chunk const raw{0xab, 0xcd};
    get_data instance;
    static auto const version = version::level::maximum;
    REQUIRE( ! entity_from_data(instance, version, raw));
}

TEST_CASE("get data  from data  insufficient version  failure", "[get data]") {
    static const get_data expected{
        {inventory_vector::type_id::error,
         hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")}};

    auto const raw = expected.to_data(version::level::maximum);
    get_data instance;
    REQUIRE( ! entity_from_data(instance, get_data::version_minimum - 1, raw));
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("get data  factory from data 1  valid input  success", "[get data]") {
    static const get_data expected{
        {inventory_vector::type_id::error,
         hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")}};

    static auto const version = version::level::maximum;
    auto const data = expected.to_data(version);
    auto const result = create<get_data>(version, data);
    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
    REQUIRE(data.size() == result.serialized_size(version));
    REQUIRE(expected.serialized_size(version) == result.serialized_size(version));
}

TEST_CASE("get data  factory from data 2  valid input  success", "[get data]") {
    static const get_data expected{
        {{inventory_vector::type_id::transaction,
          hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")}}};

    static auto const version = version::level::maximum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    auto const result = create<get_data>(version, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(get_data__factory_from_data_3__valid_input__success) {
    static const get_data expected{
        {{inventory_vector::type_id::block,
          hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b")}}};

    static auto const version = version::level::maximum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<get_data>(version, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

#if defined(KTH_SEGWIT_ENABLED)
BOOST_AUTO_TEST_CASE(get_data__to_witness__error__unchanged) {
    static auto const expected = inventory_vector::type_id::error;
    get_data instance{{expected, {}}};
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__filtered_block__unchanged) {
    static auto const expected = inventory_vector::type_id::filtered_block;
    get_data instance{{expected, {}}};
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__compact_block__unchanged) {
    static auto const expected = inventory_vector::type_id::compact_block;
    get_data instance{{expected, {}}};
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__witness_transaction__unchanged) {
    static auto const expected = inventory_vector::type_id::witness_transaction;
    get_data instance{{expected, {}}};
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__witness_block__unchanged) {
    static auto const expected = inventory_vector::type_id::witness_block;
    get_data instance{{expected, {}}};
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == expected);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__block__expected) {
    get_data instance{{inventory_vector::type_id::block, {}}};
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == inventory_vector::type_id::witness_block);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__transaction__expected) {
    get_data instance{{inventory_vector::type_id::transaction, {}}};
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == inventory_vector::type_id::witness_transaction);
}

BOOST_AUTO_TEST_CASE(get_data__to_witness__block_error_transaction__expected) {
    get_data instance{
        {inventory_vector::type_id::block, {}},
        {inventory_vector::type_id::error, {}},
        {inventory_vector::type_id::transaction, {}}};
    instance.to_witness();
    BOOST_REQUIRE(instance.inventories()[0].type() == inventory_vector::type_id::witness_block);
    BOOST_REQUIRE(instance.inventories()[1].type() == inventory_vector::type_id::error);
    BOOST_REQUIRE(instance.inventories()[2].type() == inventory_vector::type_id::witness_transaction);
}
#endif // KTH_CURRENCY_BCH

BOOST_AUTO_TEST_CASE(get_data__operator_assign_equals__always__matches_equivalent) {
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    static inventory_vector::list const elements{
        inventory_vector(inventory_vector::type_id::error, hash)};

    get_data value(elements);
    BOOST_REQUIRE(value.is_valid());

    get_data instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(elements == instance.inventories());
}

BOOST_AUTO_TEST_CASE(get_data__operator_boolean_equals__duplicates__returns_true) {
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const get_data expected{
        inventory_vector(inventory_vector::type_id::error, hash)};

    const get_data instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(get_data__operator_boolean_equals__differs__returns_false) {
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const get_data expected{
        inventory_vector(inventory_vector::type_id::error, hash)};

    const get_data instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(get_data__operator_boolean_not_equals__duplicates__returns_false) {
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const get_data expected{
        inventory_vector(inventory_vector::type_id::error, hash)};

    const get_data instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(get_data__operator_boolean_not_equals__differs__returns_true) {
    static auto const hash = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const get_data expected{
        inventory_vector(inventory_vector::type_id::error, hash)};

    const get_data instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

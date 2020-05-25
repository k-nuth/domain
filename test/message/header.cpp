// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;

BOOST_AUTO_TEST_SUITE(message_header_tests)

BOOST_AUTO_TEST_CASE(header__constructor_1__always__initialized_invalid) {
    message::header instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(header__constructor_2__always__equals_params) {
    uint32_t version = 10u;
    hash_digest previous = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    hash_digest merkle = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    uint32_t timestamp = 531234u;
    uint32_t bits = 6523454u;
    uint32_t nonce = 68644u;

    message::header instance(version, previous, merkle, timestamp, bits, nonce);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(timestamp, instance.timestamp());
    BOOST_REQUIRE_EQUAL(bits, instance.bits());
    BOOST_REQUIRE_EQUAL(nonce, instance.nonce());
    BOOST_REQUIRE(previous == instance.previous_block_hash());
    BOOST_REQUIRE(merkle == instance.merkle());
}

BOOST_AUTO_TEST_CASE(header__constructor_3__always__equals_params) {
    uint32_t version = 10u;
    hash_digest previous = hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f");
    hash_digest merkle = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    uint32_t timestamp = 531234u;
    uint32_t bits = 6523454u;
    uint32_t nonce = 68644u;

    message::header instance(version, std::move(previous), std::move(merkle), timestamp, bits, nonce);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(timestamp, instance.timestamp());
    BOOST_REQUIRE_EQUAL(bits, instance.bits());
    BOOST_REQUIRE_EQUAL(nonce, instance.nonce());
    BOOST_REQUIRE(previous == instance.previous_block_hash());
    BOOST_REQUIRE(merkle == instance.merkle());
}

BOOST_AUTO_TEST_CASE(header__constructor_4__always__equals_params) {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        1234u);

    message::header instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(header__constructor_5__always__equals_params) {
    chain::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        123u);

    message::header instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(header__constructor_6__always__equals_params) {
    const message::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    message::header instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(header__constructor_7__always__equals_params) {
    message::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    message::header instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(header__from_data__insufficient_bytes__failure) {
    data_chunk data(10);
    message::header header;
    BOOST_REQUIRE_EQUAL(false, header.from_data(message::header::version_maximum, data));
    BOOST_REQUIRE_EQUAL(false, header.is_valid());
}

BOOST_AUTO_TEST_CASE(header__factory_from_data_1__valid_input_canonical_version__no_transaction_count) {
    auto const version = message::version::level::canonical;
    message::header expected{
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u};

    auto const data = expected.to_data(version);

    auto const result = message::header::factory_from_data(version, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), chain::header::satoshi_fixed_size());
}

BOOST_AUTO_TEST_CASE(header__factory_from_data_1__valid_input__success) {
    auto const version = message::header::version_minimum;
    message::header expected{
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u};

    auto const data = expected.to_data(version);

    auto const result = message::header::factory_from_data(version, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(header__factory_from_data_2__valid_input__success) {
    auto const version = message::header::version_minimum;
    message::header expected{
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u};

    auto const data = expected.to_data(version);
    data_source istream(data);

    auto const result = message::header::factory_from_data(version, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(header__factory_from_data_3__valid_input__success) {
    auto const version = message::header::version_minimum;
    message::header expected{
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u};

    auto const data = expected.to_data(version);
    data_source istream(data);
    istream_reader source(istream);

    auto const result = message::header::factory_from_data(version, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version), result.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(header__operator_assign_equals_1__always__matches_equivalent) {
    chain::header value(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    BOOST_REQUIRE(value.is_valid());

    message::header instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
}

BOOST_AUTO_TEST_CASE(header__operator_assign_equals_2__always__matches_equivalent) {
    message::header value(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    BOOST_REQUIRE(value.is_valid());

    message::header instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_equals_1__duplicates__returns_true) {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        3565u);

    message::header instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_equals_1__differs__returns_false) {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        6523454u,
        68644u,
        4453u);

    message::header instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_not_equals_1__duplicates__returns_false) {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        6523454u,
        68644u,
        2345u);

    message::header instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_not_equals_1__differs__returns_true) {
    chain::header const expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        68644u,
        47476u);

    message::header instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_equals_2__duplicates__returns_true) {
    const message::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    message::header instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_equals_2__differs__returns_false) {
    const message::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    message::header instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_not_equals_2__duplicates__returns_false) {
    const message::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    message::header instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(header__operator_boolean_not_equals_2__differs__returns_true) {
    const message::header expected(
        10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    message::header instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

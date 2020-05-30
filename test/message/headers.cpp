// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::message;

BOOST_AUTO_TEST_SUITE(headers_tests)

BOOST_AUTO_TEST_CASE(headers__constructor_1__always__initialized_invalid) {
    headers instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(headers__constructor_2__always__equals_params) {
    header::list const expected{
        header(
            10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        header(
            11234u,
            hash_literal("abababababababababababababababababababababababababababababababab"),
            hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
            753234u,
            4356344u,
            34564u)};

    headers instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(instance.elements() == expected);
}

BOOST_AUTO_TEST_CASE(headers__constructor_3__always__equals_params) {
    header::list const expected{
        header(
            10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        header(
            11234u,
            hash_literal("abababababababababababababababababababababababababababababababab"),
            hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
            753234u,
            4356344u,
            34564u)};

    headers instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(instance.elements().size(), 2u);
}

BOOST_AUTO_TEST_CASE(headers__constructor_4__always__equals_params) {
    headers instance(
        {header(
             10u,
             hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
             hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
             531234u,
             6523454u,
             68644u),
         header(
             11234u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
             753234u,
             4356344u,
             34564u)});

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(instance.elements().size(), 2u);
}

BOOST_AUTO_TEST_CASE(headers__constructor_5__always__equals_params) {
    headers const expected(
        {header(
             10u,
             hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
             hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
             531234u,
             6523454u,
             68644u),
         header(
             11234u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
             753234u,
             4356344u,
             34564u)});

    headers instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(headers__constructor_6__always__equals_params) {
    headers expected(
        {header(
             10u,
             hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
             hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
             531234u,
             6523454u,
             68644u),
         header(
             11234u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
             753234u,
             4356344u,
             34564u)});

    headers instance(std::move(expected));
    BOOST_REQUIRE_EQUAL(instance.elements().size(), 2u);
}

BOOST_AUTO_TEST_CASE(headers__from_data__insufficient_bytes__failure) {
    data_chunk const raw{0xab, 0xcd};
    headers instance{};
    BOOST_REQUIRE_EQUAL(false, entity_from_data(instance, headers::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(headers__from_data__insufficient_version__failure) {
    static headers const expected{
        {10,
         hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
         hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
         531234,
         6523454,
         68644}};

    data_chunk const data = expected.to_data(headers::version_minimum);
    headers instance{};
    BOOST_REQUIRE(!entity_from_data(instance, headers::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(headers__factory_from_data_1__valid_input__success) {
    static headers const expected{
        {10,
         hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
         hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
         531234,
         6523454,
         68644}};

    static auto const version = headers::version_minimum;
    auto const data = expected.to_data(version);
    auto const result = create<headers>(version, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(result == expected);
    BOOST_REQUIRE_EQUAL(result.serialized_size(version), data.size());
    BOOST_REQUIRE_EQUAL(result.serialized_size(version), expected.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(headers__factory_from_data_2__valid_input__success) {
    static headers const expected{
        {15,
         hash_literal("00acadae0019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
         hash_literal("4a5e1e4bbbccddee32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
         542344,
         1247742,
         34632}};

    static auto const version = headers::version_minimum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    auto result = create<headers>(version, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(result == expected);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(result.serialized_size(version), expected.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(headers__factory_from_data_3__valid_input__success) {
    static headers const expected{
        {7,
         hash_literal("1234123412341234123412341234123412341234123412341234123412341234"),
         hash_literal("4321432143214321432143214321432143214321432143214321432143214321"),
         83221,
         4353212,
         54234}};

    static auto const version = headers::version_minimum;
    auto const data = expected.to_data(version);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = headers::factory_from_data(version, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(result == expected);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version));
    BOOST_REQUIRE_EQUAL(result.serialized_size(version), expected.serialized_size(version));
}

BOOST_AUTO_TEST_CASE(headers__elements_accessor_1__always__returns_initialized_value) {
    header::list const expected{
        header(
            10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        header(
            11234u,
            hash_literal("abababababababababababababababababababababababababababababababab"),
            hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
            753234u,
            4356344u,
            34564u)};

    message::headers instance(expected);
    BOOST_REQUIRE(instance.elements() == expected);
}

BOOST_AUTO_TEST_CASE(headers__elements_accessor_2__always__returns_initialized_value) {
    header::list const expected{
        header(
            10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        header(
            11234u,
            hash_literal("abababababababababababababababababababababababababababababababab"),
            hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
            753234u,
            4356344u,
            34564u)};

    const message::headers instance(expected);
    BOOST_REQUIRE(instance.elements() == expected);
}

BOOST_AUTO_TEST_CASE(headers__command_setter_1__roundtrip__success) {
    header::list const expected{
        header(
            10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        header(
            11234u,
            hash_literal("abababababababababababababababababababababababababababababababab"),
            hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
            753234u,
            4356344u,
            34564u)};

    message::headers instance;
    BOOST_REQUIRE(instance.elements() != expected);
    instance.set_elements(expected);
    BOOST_REQUIRE(instance.elements() == expected);
}

BOOST_AUTO_TEST_CASE(headers__command_setter_2__roundtrip__success) {
    header::list values{
        header(
            10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        header(
            11234u,
            hash_literal("abababababababababababababababababababababababababababababababab"),
            hash_literal("fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe"),
            753234u,
            4356344u,
            34564u)};

    message::headers instance;
    BOOST_REQUIRE(instance.elements().empty());
    instance.set_elements(std::move(values));
    BOOST_REQUIRE_EQUAL(instance.elements().size(), 2u);
}

BOOST_AUTO_TEST_CASE(headers__operator_assign_equals__always__matches_equivalent) {
    message::headers value(
        {header{
             1u,
             hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
             hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
             10u,
             100u,
             1000u},
         header{
             2u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("babababababababababababababababababababababababababababababababa"),
             20u,
             200u,
             2000u},
         header{
             3u,
             hash_literal("e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2"),
             hash_literal("7373737373737373737373737373737373737373737373737373737373737373"),
             30u,
             300u,
             3000u}});

    BOOST_REQUIRE(value.is_valid());
    message::headers instance;
    BOOST_REQUIRE(!instance.is_valid());
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(headers__operator_boolean_equals__duplicates__returns_true) {
    const message::headers expected(
        {header{
             1u,
             hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
             hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
             10u,
             100u,
             1000u},
         header{
             2u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("babababababababababababababababababababababababababababababababa"),
             20u,
             200u,
             2000u},
         header{
             3u,
             hash_literal("e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2"),
             hash_literal("7373737373737373737373737373737373737373737373737373737373737373"),
             30u,
             300u,
             3000u}});

    message::headers instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(headers__operator_boolean_equals__differs__returns_false) {
    const message::headers expected(
        {header{
             1u,
             hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
             hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
             10u,
             100u,
             1000u},
         header{
             2u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("babababababababababababababababababababababababababababababababa"),
             20u,
             200u,
             2000u},
         header{
             3u,
             hash_literal("e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2"),
             hash_literal("7373737373737373737373737373737373737373737373737373737373737373"),
             30u,
             300u,
             3000u}});

    message::headers instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(headers__operator_boolean_not_equals__duplicates__returns_false) {
    const message::headers expected(
        {header{
             1u,
             hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
             hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
             10u,
             100u,
             1000u},
         header{
             2u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("babababababababababababababababababababababababababababababababa"),
             20u,
             200u,
             2000u},
         header{
             3u,
             hash_literal("e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2"),
             hash_literal("7373737373737373737373737373737373737373737373737373737373737373"),
             30u,
             300u,
             3000u}});

    message::headers instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(headers__operator_boolean_not_equals__differs__returns_true) {
    const message::headers expected(
        {header{
             1u,
             hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
             hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
             10u,
             100u,
             1000u},
         header{
             2u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("babababababababababababababababababababababababababababababababa"),
             20u,
             200u,
             2000u},
         header{
             3u,
             hash_literal("e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2"),
             hash_literal("7373737373737373737373737373737373737373737373737373737373737373"),
             30u,
             300u,
             3000u}});

    message::headers instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_CASE(headers__to_hashes__empty__returns_empty_list) {
    message::headers instance;
    hash_list result;
    instance.to_hashes(result);
    BOOST_REQUIRE(result.empty());
}

BOOST_AUTO_TEST_CASE(headers__to_hashes__non_empty__returns_header_hash_list) {
    const hash_list expected{
        hash_literal("108127a4f5955a546b78807166d8cb9cd3eee1ed530c14d51095bc798685f4d6"),
        hash_literal("37ec64a548b6419769b152d70efc4c356f74c7fda567711d98cac3c55c34a890"),
        hash_literal("d9bbb4b47ca45ec8477cba125262b07b17daae944b54d1780e0a6373d2eed879")};

    const message::headers instance(
        {header{
             1u,
             hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
             hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
             10u,
             100u,
             1000u},
         header{
             2u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("babababababababababababababababababababababababababababababababa"),
             20u,
             200u,
             2000u},
         header{
             3u,
             hash_literal("e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2"),
             hash_literal("7373737373737373737373737373737373737373737373737373737373737373"),
             30u,
             300u,
             3000u}});

    hash_list result;
    instance.to_hashes(result);
    BOOST_REQUIRE_EQUAL(result.size(), expected.size());
    BOOST_REQUIRE(result == expected);
}

BOOST_AUTO_TEST_CASE(headers__to_inventory__empty__returns_empty_list) {
    message::headers instance;
    inventory_vector::list result;
    instance.to_inventory(result, inventory_vector::type_id::block);
    BOOST_REQUIRE_EQUAL(0, result.size());
}

BOOST_AUTO_TEST_CASE(headers__to_inventory__non_empty__returns_header_hash_inventory_list) {
    inventory_vector::list const expected{
        inventory_vector(inventory_vector::type_id::block, hash_literal("108127a4f5955a546b78807166d8cb9cd3eee1ed530c14d51095bc798685f4d6")),
        inventory_vector(inventory_vector::type_id::block, hash_literal("37ec64a548b6419769b152d70efc4c356f74c7fda567711d98cac3c55c34a890")),
        inventory_vector(inventory_vector::type_id::block, hash_literal("d9bbb4b47ca45ec8477cba125262b07b17daae944b54d1780e0a6373d2eed879"))};

    headers const instance(
        {header{
             1u,
             hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
             hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
             10u,
             100u,
             1000u},
         header{
             2u,
             hash_literal("abababababababababababababababababababababababababababababababab"),
             hash_literal("babababababababababababababababababababababababababababababababa"),
             20u,
             200u,
             2000u},
         header{
             3u,
             hash_literal("e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2"),
             hash_literal("7373737373737373737373737373737373737373737373737373737373737373"),
             30u,
             300u,
             3000u}});

    inventory_vector::list result;
    instance.to_inventory(result, inventory_vector::type_id::block);
    BOOST_REQUIRE_EQUAL(result.size(), expected.size());
    BOOST_REQUIRE(result == expected);
}

BOOST_AUTO_TEST_CASE(headers__is_sequential__empty__true) {
    static headers const instance;
    BOOST_REQUIRE(instance.elements().empty());
    BOOST_REQUIRE(instance.is_sequential());
}

BOOST_AUTO_TEST_CASE(headers__is_sequential__single__true) {
    static header const first{
        1u,
        hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
        hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
        10u,
        100u,
        1000u};

    headers const instance({first});
    BOOST_REQUIRE(instance.is_sequential());
}

BOOST_AUTO_TEST_CASE(headers__is_sequential__sequential__true) {
    static header const first{
        1u,
        hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
        hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
        10u,
        100u,
        1000u};

    static header const second{
        2u,
        first.hash(),
        hash_literal("babababababababababababababababababababababababababababababababa"),
        20u,
        200u,
        2000u};

    static header const third{
        3u,
        second.hash(),
        hash_literal("7373737373737373737373737373737373737373737373737373737373737373"),
        30u,
        300u,
        3000u};

    headers const instance({first, second, third});
    BOOST_REQUIRE(instance.is_sequential());
}

BOOST_AUTO_TEST_CASE(headers__is_sequential__disordered__false) {
    static header const first{
        1u,
        hash_literal("f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0"),
        hash_literal("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f"),
        10u,
        100u,
        1000u};

    static header const second{
        2u,
        hash_literal("abababababababababababababababababababababababababababababababab"),
        hash_literal("babababababababababababababababababababababababababababababababa"),
        20u,
        200u,
        2000u};

    static header const third{
        3u,
        hash_literal("e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2"),
        hash_literal("7373737373737373737373737373737373737373737373737373737373737373"),
        30u,
        300u,
        3000u};

    headers const instance({first, second, third});
    BOOST_REQUIRE(!instance.is_sequential());
}

BOOST_AUTO_TEST_SUITE_END()

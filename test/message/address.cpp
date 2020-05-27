// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::message;

bool equal(address const& x, address const& y) {
    auto const left_addresses = x.addresses();
    auto const right_addresses = y.addresses();

    bool same = (left_addresses.size() == right_addresses.size());

    for (size_t i = 0; (i < left_addresses.size()) && same; i++) {
        same = (left_addresses[i] == right_addresses[i]) && (left_addresses[i].timestamp() == right_addresses[i].timestamp());
    }

    return same;
}

BOOST_AUTO_TEST_SUITE(address_tests)

BOOST_AUTO_TEST_CASE(address__constructor_1__always__invalid) {
    address instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(address__constructor_2__always__equals_params) {
    infrastructure::message::network_address::list const addresses{
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)};

    address instance(addresses);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__constructor_3__always__equals_params) {
    network_address::list const addresses{
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)};

    auto dup_addresses = addresses;

    address instance(std::move(dup_addresses));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__constructor_4__always__equals_params) {
    network_address::list const addresses{
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)};

    address value(addresses);
    address instance(value);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__constructor_5__always__equals_params) {
    network_address::list const addresses{
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)};

    address value(addresses);
    address instance(std::move(value));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__from_data__insufficient_bytes__failure) {
    data_chunk const raw{0xab};
    address instance;

    BOOST_REQUIRE(!instance.from_data(version::level::minimum, raw));
}

BOOST_AUTO_TEST_CASE(address__factory_from_data_1__roundtrip__success) {
    const address expected(
        {{734678u,
          5357534u,
          base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
          123u}});

    auto const data = expected.to_data(version::level::minimum);
    auto const result = address::factory_from_data(version::level::minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(equal(expected, result));
    auto const serialized_size = result.serialized_size(version::level::minimum);
    BOOST_REQUIRE_EQUAL(data.size(), serialized_size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::minimum), serialized_size);
}

BOOST_AUTO_TEST_CASE(address__factory_from_data_2__roundtrip__success) {
    const address expected(
        {{734678u,
          5357534u,
          base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
          123u}});

    auto const data = expected.to_data(version::level::minimum);
    data_source istream(data);
    auto const result = address::factory_from_data(version::level::minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(equal(expected, result));
    auto const serialized_size = result.serialized_size(version::level::minimum);
    BOOST_REQUIRE_EQUAL(data.size(), serialized_size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::minimum), serialized_size);
}

BOOST_AUTO_TEST_CASE(address__factory_from_data_3__roundtrip__success) {
    const address expected(
        {{734678u,
          5357534u,
          base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
          123u}});

    data_chunk const data = expected.to_data(version::level::minimum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = address::factory_from_data(version::level::minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(equal(expected, result));
    auto const serialized_size = result.serialized_size(version::level::minimum);
    BOOST_REQUIRE_EQUAL(data.size(), serialized_size);
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version::level::minimum), serialized_size);
}

BOOST_AUTO_TEST_CASE(address__addresses_setter_1__roundtrip__success) {
    network_address::list const value{
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)};

    address instance;
    BOOST_REQUIRE(instance.addresses() != value);
    instance.set_addresses(value);
    BOOST_REQUIRE(value == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__addresses_setter_2__roundtrip__success) {
    network_address::list const value{
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)};

    auto dup_value = value;
    address instance;
    BOOST_REQUIRE(instance.addresses() != value);
    instance.set_addresses(std::move(dup_value));
    BOOST_REQUIRE(value == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__operator_assign_equals__always__matches_equivalent) {
    network_address::list const addresses{
        network_address(
            734678u,
            5357534u,
            base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
            123u),
        network_address(
            34654u,
            47653u,
            base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            222u),
        network_address(
            265453u,
            2115325u,
            base16_literal("19573257168426842319857321595126"),
            159u)};

    address value(addresses);

    BOOST_REQUIRE(value.is_valid());

    address instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(addresses == instance.addresses());
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_equals__duplicates__returns_true) {
    const address expected(
        {network_address(
             734678u,
             5357534u,
             base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
             123u),
         network_address(
             34654u,
             47653u,
             base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
             222u),
         network_address(
             265453u,
             2115325u,
             base16_literal("19573257168426842319857321595126"),
             159u)});

    address instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_equals__differs__returns_false) {
    const address expected(
        {network_address(
             734678u,
             5357534u,
             base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
             123u),
         network_address(
             34654u,
             47653u,
             base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
             222u),
         network_address(
             265453u,
             2115325u,
             base16_literal("19573257168426842319857321595126"),
             159u)});

    address instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_not_equals__duplicates__returns_false) {
    const address expected(
        {network_address(
             734678u,
             5357534u,
             base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
             123u),
         network_address(
             34654u,
             47653u,
             base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
             222u),
         network_address(
             265453u,
             2115325u,
             base16_literal("19573257168426842319857321595126"),
             159u)});

    address instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(address__operator_boolean_not_equals__differs__returns_true) {
    const address expected(
        {network_address(
             734678u,
             5357534u,
             base16_literal("47816a40bb92bdb4e0b8256861f96a55"),
             123u),
         network_address(
             34654u,
             47653u,
             base16_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
             222u),
         network_address(
             265453u,
             2115325u,
             base16_literal("19573257168426842319857321595126"),
             159u)});

    address instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

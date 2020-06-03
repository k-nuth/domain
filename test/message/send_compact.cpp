// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;

BOOST_AUTO_TEST_SUITE(send_compact_tests)

BOOST_AUTO_TEST_CASE(send_compact__constructor_1__always__invalid) {
    message::send_compact instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(send_compact__constructor_2__always__equals_params) {
    bool mode = true;
    uint64_t version = 1245436u;
    message::send_compact instance(mode, version);
    BOOST_REQUIRE_EQUAL(mode, instance.high_bandwidth_mode());
    BOOST_REQUIRE_EQUAL(version, instance.version());
}

BOOST_AUTO_TEST_CASE(send_compact__constructor_3__always__equals_params) {
    const message::send_compact expected(true, 1245436u);
    message::send_compact instance(expected);
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(send_compact__constructor_4__always__equals_params) {
    bool mode = true;
    uint64_t version = 1245436u;
    message::send_compact expected(mode, version);
    BOOST_REQUIRE(expected.is_valid());

    message::send_compact instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(mode, instance.high_bandwidth_mode());
    BOOST_REQUIRE_EQUAL(version, instance.version());
}

BOOST_AUTO_TEST_CASE(send_compact__factory_from_data_1__valid_input__success) {
    const message::send_compact expected{true, 164};
    auto const data = expected.to_data(message::send_compact::version_minimum);
    auto const result = create<message::send_compact>(
        message::send_compact::version_minimum, data);

    BOOST_REQUIRE_EQUAL(
        message::send_compact::satoshi_fixed_size(
            message::send_compact::version_minimum),
        data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(send_compact__factory_from_data_2__valid_input__success) {
    const message::send_compact expected{false, 5};
    auto const data = expected.to_data(message::send_compact::version_minimum);
    data_source istream(data);
    auto const result = create<message::send_compact>(
        message::send_compact::version_minimum, istream);

    BOOST_REQUIRE_EQUAL(
        message::send_compact::satoshi_fixed_size(
            message::send_compact::version_minimum),
        data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(send_compact__factory_from_data_3__valid_input__success) {
    const message::send_compact expected{true, 257};
    auto const data = expected.to_data(message::send_compact::version_minimum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::send_compact>(
        message::send_compact::version_minimum, source);

    BOOST_REQUIRE_EQUAL(
        message::send_compact::satoshi_fixed_size(
            message::send_compact::version_minimum),
        data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(send_compact__from_data_1__invalid_mode_byte__failure) {
    data_chunk raw_data{0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    message::send_compact msg;
    bool result = entity_from_data(msg, message::send_compact::version_minimum, raw_data);
    BOOST_REQUIRE(!result);
}

BOOST_AUTO_TEST_CASE(send_compact__from_data_1__insufficient_version__failure) {
    const message::send_compact expected{true, 257};
    data_chunk raw_data = expected.to_data(message::send_compact::version_minimum);
    message::send_compact msg;
    bool result = entity_from_data(msg, message::send_compact::version_minimum - 1, raw_data);
    BOOST_REQUIRE(!result);
}

BOOST_AUTO_TEST_CASE(send_compact__high_bandwidth_mode_accessor__always__returns_initialized_value) {
    bool const expected = true;
    const message::send_compact instance(expected, 210u);
    BOOST_REQUIRE_EQUAL(expected, instance.high_bandwidth_mode());
}

BOOST_AUTO_TEST_CASE(send_compact__high_bandwidth_mode_setter__roundtrip__success) {
    bool const expected = true;
    message::send_compact instance;
    BOOST_REQUIRE(expected != instance.high_bandwidth_mode());
    instance.set_high_bandwidth_mode(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.high_bandwidth_mode());
}

BOOST_AUTO_TEST_CASE(send_compact__version_accessor__always__returns_initialized_value) {
    uint64_t const expected = 6548u;
    const message::send_compact instance(false, expected);
    BOOST_REQUIRE_EQUAL(expected, instance.version());
}

BOOST_AUTO_TEST_CASE(send_compact__version_setter__roundtrip__success) {
    uint64_t const expected = 6548u;
    message::send_compact instance;
    BOOST_REQUIRE(expected != instance.version());
    instance.set_version(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.version());
}

BOOST_AUTO_TEST_CASE(send_compact__operator_assign_equals__always__matches_equivalent) {
    bool mode = false;
    uint64_t version = 210u;
    message::send_compact value(mode, version);
    BOOST_REQUIRE(value.is_valid());

    message::send_compact instance;
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(mode, instance.high_bandwidth_mode());
    BOOST_REQUIRE_EQUAL(version, instance.version());
}

BOOST_AUTO_TEST_CASE(send_compact__operator_boolean_equals__duplicates__returns_true) {
    const message::send_compact expected(false, 15234u);
    message::send_compact instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(send_compact__operator_boolean_equals__differs__returns_false) {
    const message::send_compact expected(true, 979797u);
    message::send_compact instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(send_compact__operator_boolean_not_equals__duplicates__returns_false) {
    const message::send_compact expected(true, 734678u);
    message::send_compact instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(send_compact__operator_boolean_not_equals__differs__returns_true) {
    const message::send_compact expected(false, 5357534u);
    message::send_compact instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

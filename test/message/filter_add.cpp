// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// Start Boost Suite: filter add tests

TEST_CASE("filter add  constructor 1  always invalid", "[filter add]") {
    message::filter_add instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("filter add  constructor 2  always  equals params", "[filter add]") {
    data_chunk const data = {0x0f, 0xf0, 0x55, 0xaa};
    message::filter_add instance(data);
    REQUIRE(instance.is_valid());
    REQUIRE(data == instance.data());
}

TEST_CASE("filter add  constructor 3  always  equals params", "[filter add]") {
    data_chunk const data = {0x0f, 0xf0, 0x55, 0xaa};
    auto dup = data;
    message::filter_add instance(std::move(dup));
    REQUIRE(instance.is_valid());
    REQUIRE(data == instance.data());
}

TEST_CASE("filter add  constructor 4  always  equals params", "[filter add]") {
    data_chunk const data = {0x0f, 0xf0, 0x55, 0xaa};
    const message::filter_add value(data);
    message::filter_add instance(value);
    REQUIRE(instance.is_valid());
    REQUIRE(value == instance);
    REQUIRE(data == instance.data());
}

TEST_CASE("filter add  constructor 5  always  equals params", "[filter add]") {
    data_chunk const data = {0x0f, 0xf0, 0x55, 0xaa};
    message::filter_add value(data);
    message::filter_add instance(std::move(value));
    REQUIRE(instance.is_valid());
    REQUIRE(data == instance.data());
}

TEST_CASE("filter add  from data  insufficient bytes  failure", "[filter add]") {
    data_chunk raw = {0xab, 0x11};
    message::filter_add instance;

    REQUIRE( ! entity_from_data(instance, message::version::level::maximum, raw));
}

TEST_CASE("filter add  from data  insufficient version  failure", "[filter add]") {
    const message::filter_add expected{
        {0x1F, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
         0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
         0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
         0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf}};

    auto const data = expected.to_data(message::version::level::maximum);
    message::filter_add instance;

    BOOST_REQUIRE_EQUAL(false, entity_from_data(instance, 
                                   message::filter_add::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(filter_add__factory_from_data_1__valid_input__success) {
    const message::filter_add expected{
        {0x1F, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
         0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
         0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
         0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf}};

    auto const data = expected.to_data(message::version::level::maximum);
    auto const result = create<message::filter_add>(
        message::version::level::maximum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::maximum),
                        result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(filter_add__factory_from_data_2__valid_input__success) {
    const message::filter_add expected{
        {0x1F, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
         0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
         0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
         0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf}};

    auto const data = expected.to_data(message::version::level::maximum);
    data_source istream(data);
    auto const result = create<message::filter_add>(
        message::version::level::maximum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::maximum),
                        result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(filter_add__factory_from_data_3__valid_input__success) {
    const message::filter_add expected{
        {0x1F, 0x9a, 0x0d, 0x24, 0x9a, 0xd5, 0x39, 0x89,
         0xbb, 0x85, 0x0a, 0x3d, 0x79, 0x24, 0xed, 0x0f,
         0xc3, 0x0d, 0x6f, 0x55, 0x7d, 0x71, 0x12, 0x1a,
         0x37, 0xc0, 0xb0, 0x32, 0xf0, 0xd6, 0x6e, 0xdf}};

    auto const data = expected.to_data(message::version::level::maximum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::filter_add>(
        message::version::level::maximum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::maximum),
                        result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(filter_add__data_accessor_1__always__returns_initialized_value) {
    data_chunk const value = {0x0f, 0xf0, 0x55, 0xaa};
    message::filter_add instance(value);
    BOOST_REQUIRE(value == instance.data());
}

BOOST_AUTO_TEST_CASE(filter_add__data_accessor_2__always__returns_initialized_value) {
    data_chunk const value = {0x0f, 0xf0, 0x55, 0xaa};
    const message::filter_add instance(value);
    BOOST_REQUIRE(value == instance.data());
}

BOOST_AUTO_TEST_CASE(filter_add__data_setter_1__roundtrip__success) {
    data_chunk const value = {0x0f, 0xf0, 0x55, 0xaa};
    message::filter_add instance;
    BOOST_REQUIRE(value != instance.data());
    instance.set_data(value);
    BOOST_REQUIRE(value == instance.data());
}

BOOST_AUTO_TEST_CASE(filter_add__data_setter_2__roundtrip__success) {
    data_chunk const value = {0x0f, 0xf0, 0x55, 0xaa};
    data_chunk dup = value;
    message::filter_add instance;
    BOOST_REQUIRE(value != instance.data());
    instance.set_data(std::move(dup));
    BOOST_REQUIRE(value == instance.data());
}

BOOST_AUTO_TEST_CASE(filter_add__operator_assign_equals__always__matches_equivalent) {
    data_chunk const data = {0x0f, 0xf0, 0x55, 0xaa};
    message::filter_add value(data);
    BOOST_REQUIRE(value.is_valid());
    message::filter_add instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(data == instance.data());
}

BOOST_AUTO_TEST_CASE(filter_add__operator_boolean_equals__duplicates__returns_true) {
    const message::filter_add expected({0x0f, 0xf0, 0x55, 0xaa});
    message::filter_add instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(filter_add__operator_boolean_equals__differs__returns_false) {
    const message::filter_add expected({0x0f, 0xf0, 0x55, 0xaa});
    message::filter_add instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(filter_add__operator_boolean_not_equals__duplicates__returns_false) {
    const message::filter_add expected({0x0f, 0xf0, 0x55, 0xaa});
    message::filter_add instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(filter_add__operator_boolean_not_equals__differs__returns_true) {
    const message::filter_add expected({0x0f, 0xf0, 0x55, 0xaa});
    message::filter_add instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

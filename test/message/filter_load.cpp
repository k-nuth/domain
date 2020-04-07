// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// Start Boost Suite: filter load tests

TEST_CASE("filter load  constructor 1  always invalid", "[filter load]") {
    message::filter_load instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("filter load  constructor 2  always  equals params", "[filter load]") {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    REQUIRE(instance.is_valid());
    REQUIRE(filter == instance.filter());
    REQUIRE(hash_functions == instance.hash_functions());
    REQUIRE(tweak == instance.tweak());
    REQUIRE(flags == instance.flags());
}

TEST_CASE("filter load  constructor 3  always  equals params", "[filter load]") {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    auto dup_filter = filter;
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(std::move(dup_filter), hash_functions, tweak, flags);
    REQUIRE(instance.is_valid());
    REQUIRE(filter == instance.filter());
    REQUIRE(hash_functions == instance.hash_functions());
    REQUIRE(tweak == instance.tweak());
    REQUIRE(flags == instance.flags());
}

TEST_CASE("filter load  constructor 4  always  equals params", "[filter load]") {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    const message::filter_load value(filter, hash_functions, tweak, flags);
    message::filter_load instance(value);
    REQUIRE(instance.is_valid());
    REQUIRE(value == instance);
    REQUIRE(filter == instance.filter());
    REQUIRE(hash_functions == instance.hash_functions());
    REQUIRE(tweak == instance.tweak());
    REQUIRE(flags == instance.flags());
}

TEST_CASE("filter load  constructor 5  always  equals params", "[filter load]") {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load value(filter, hash_functions, tweak, flags);
    message::filter_load instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter == instance.filter());
    BOOST_REQUIRE_EQUAL(hash_functions, instance.hash_functions());
    BOOST_REQUIRE_EQUAL(tweak, instance.tweak());
    BOOST_REQUIRE_EQUAL(flags, instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__from_data__insufficient_bytes__failure) {
    data_chunk const raw{0xab, 0x11};
    message::filter_load instance;

    BOOST_REQUIRE_EQUAL(false, entity_from_data(instance, message::version::level::maximum, raw));
}

BOOST_AUTO_TEST_CASE(filter_load__from_data__insufficient_version__failure) {
    const message::filter_load expected{
        {0x05, 0xaa, 0xbb, 0xcc, 0xdd, 0xee},
        25,
        10,
        0xab};

    data_chunk const data = expected.to_data(message::version::level::maximum);
    message::filter_load instance;

    BOOST_REQUIRE_EQUAL(false, entity_from_data(instance, 
                                   message::filter_load::version_minimum - 1, data));
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(filter_load__factory_from_data_1__valid_input__success) {
    const message::filter_load expected{
        {0x05, 0xaa, 0xbb, 0xcc, 0xdd, 0xee},
        25,
        10,
        0xab};

    auto const data = expected.to_data(message::version::level::maximum);
    auto const result = create<message::filter_load>(
        message::version::level::maximum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::maximum),
                        result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(filter_load__factory_from_data_2__valid_input__success) {
    const message::filter_load expected{
        {0x05, 0xaa, 0xbb, 0xcc, 0xdd, 0xee},
        25,
        10,
        0xab};

    auto const data = expected.to_data(message::version::level::maximum);
    data_source istream(data);
    auto const result = create<message::filter_load>(
        message::version::level::maximum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::version::level::maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::maximum),
                        result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(filter_load__factory_from_data_3__valid_input__success) {
    const message::filter_load expected{
        {0x05, 0xaa, 0xbb, 0xcc, 0xdd, 0xee},
        25,
        10,
        0xab};

    auto const data = expected.to_data(message::version::level::maximum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::filter_load>(
        message::version::level::maximum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(message::version::level::maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::maximum),
                        result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(filter_load__filter_accessor_1__always__returns_initialized_value) {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(filter_load__filter_accessor_2__always__returns_initialized_value) {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    const message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(filter_load__filter_setter_1__roundtrip__success) {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    message::filter_load instance;
    BOOST_REQUIRE(filter != instance.filter());
    instance.set_filter(filter);
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(filter_load__filter_setter_2__roundtrip__success) {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    data_chunk dup = filter;

    message::filter_load instance;
    BOOST_REQUIRE(filter != instance.filter());
    instance.set_filter(std::move(dup));
    BOOST_REQUIRE(filter == instance.filter());
}

BOOST_AUTO_TEST_CASE(filter_load__hash_functions_accessor__always__returns_initialized_value) {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE_EQUAL(hash_functions, instance.hash_functions());
}

BOOST_AUTO_TEST_CASE(filter_load__hash_functions_setter__roundtrip__success) {
    uint32_t hash_functions = 48u;
    message::filter_load instance;
    BOOST_REQUIRE(hash_functions != instance.hash_functions());
    instance.set_hash_functions(hash_functions);
    BOOST_REQUIRE(hash_functions == instance.hash_functions());
}

BOOST_AUTO_TEST_CASE(filter_load__tweak_accessor__always__returns_initialized_value) {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE_EQUAL(tweak, instance.tweak());
}

BOOST_AUTO_TEST_CASE(filter_load__tweak_setter__roundtrip__success) {
    uint32_t tweak = 36u;
    message::filter_load instance;
    BOOST_REQUIRE(tweak != instance.tweak());
    instance.set_tweak(tweak);
    BOOST_REQUIRE(tweak == instance.tweak());
}

BOOST_AUTO_TEST_CASE(filter_load__flags_accessor__always__returns_initialized_value) {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;

    message::filter_load instance(filter, hash_functions, tweak, flags);
    BOOST_REQUIRE_EQUAL(flags, instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__flags_setter__roundtrip__success) {
    uint8_t flags = 0xae;
    message::filter_load instance;
    BOOST_REQUIRE(flags != instance.flags());
    instance.set_flags(flags);
    BOOST_REQUIRE(flags == instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__operator_assign_equals__always__matches_equivalent) {
    data_chunk const filter = {0x0f, 0xf0, 0x55, 0xaa};
    uint32_t hash_functions = 48u;
    uint32_t tweak = 36u;
    uint8_t flags = 0xae;
    message::filter_load value(filter, hash_functions, tweak, flags);

    BOOST_REQUIRE(value.is_valid());

    message::filter_load instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(filter == instance.filter());
    BOOST_REQUIRE_EQUAL(hash_functions, instance.hash_functions());
    BOOST_REQUIRE_EQUAL(tweak, instance.tweak());
    BOOST_REQUIRE_EQUAL(flags, instance.flags());
}

BOOST_AUTO_TEST_CASE(filter_load__operator_boolean_equals__duplicates__returns_true) {
    const message::filter_load expected(
        {0x0f, 0xf0, 0x55, 0xaa}, 643u, 575u, 0xaa);

    message::filter_load instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(filter_load__operator_boolean_equals__differs__returns_false) {
    const message::filter_load expected(
        {0x0f, 0xf0, 0x55, 0xaa}, 643u, 575u, 0xaa);

    message::filter_load instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(filter_load__operator_boolean_not_equals__duplicates__returns_false) {
    const message::filter_load expected(
        {0x0f, 0xf0, 0x55, 0xaa}, 643u, 575u, 0xaa);

    message::filter_load instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(filter_load__operator_boolean_not_equals__differs__returns_true) {
    const message::filter_load expected(
        {0x0f, 0xf0, 0x55, 0xaa}, 643u, 575u, 0xaa);

    message::filter_load instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

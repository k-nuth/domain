// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <string>

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::message;

// Start Boost Suite: heading tests

TEST_CASE("heading  constructor 1  always  initialized invalid", "[heading]") {
    heading instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("heading  constructor 2  always  equals params", "[heading]") {
    uint32_t magic = 123u;
    std::string const command = "foo";
    uint32_t payload_size = 3454u;
    uint32_t checksum = 35746u;
    heading instance(magic, command, payload_size, checksum);
    REQUIRE(instance.is_valid());
    REQUIRE(magic == instance.magic());
    REQUIRE(command == instance.command());
    REQUIRE(payload_size == instance.payload_size());
    REQUIRE(checksum == instance.checksum());
}

TEST_CASE("heading  constructor 3  always  equals params", "[heading]") {
    uint32_t magic = 123u;
    std::string const command = "foo";
    uint32_t payload_size = 3454u;
    uint32_t checksum = 35746u;
    heading instance(magic, "foo", payload_size, checksum);
    REQUIRE(instance.is_valid());
    REQUIRE(magic == instance.magic());
    REQUIRE(command == instance.command());
    REQUIRE(payload_size == instance.payload_size());
    REQUIRE(checksum == instance.checksum());
}

TEST_CASE("heading  constructor 4  always  equals params", "[heading]") {
    heading expected(453u, "bar", 436u, 5743u);
    heading instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("heading  constructor 5  always  equals params", "[heading]") {
    uint32_t magic = 123u;
    std::string const command = "foo";
    uint32_t payload_size = 3454u;
    uint32_t checksum = 35746u;
    heading value(magic, command, payload_size, checksum);
    heading instance(std::move(value));
    REQUIRE(instance.is_valid());
    REQUIRE(magic == instance.magic());
    REQUIRE(command == instance.command());
    REQUIRE(payload_size == instance.payload_size());
    REQUIRE(checksum == instance.checksum());
}

TEST_CASE("heading  to data  checksum variations  success", "[heading]") {
    heading instance{
        32414u,
        "foo",
        56731u,
        0u};

    auto const zero_checksum = instance.to_data();
    REQUIRE(zero_checksum.size() == heading::satoshi_fixed_size());

    instance.set_checksum(123u);
    auto const nonzero_checksum = instance.to_data();
    REQUIRE(nonzero_checksum.size() == heading::satoshi_fixed_size());
}

TEST_CASE("heading  from data  insufficient bytes  failure", "[heading]") {
    static data_chunk const raw{
        0xab, 0xcd};

    heading instance;
    REQUIRE(!entity_from_data(instance, raw));
}

TEST_CASE("heading  factory from data 1  valid input  success", "[heading]") {
    static const heading expected{
        32414u,
        "foo",
        56731u,
        0u};

    auto const data = expected.to_data();
    auto const result = create<heading>(data);
    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
    REQUIRE(data.size() == heading::satoshi_fixed_size());
}

TEST_CASE("heading  factory from data 2  valid input  success", "[heading]") {
    static const heading expected{
        29145u,
        "bar",
        79531u,
        0u};

    auto const data = expected.to_data();
    data_source istream(data);
    auto const result = create<heading>(istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), heading::satoshi_fixed_size());
}

BOOST_AUTO_TEST_CASE(heading__factory_from_data_3__valid_input__success) {
    static const heading expected{
        1u,
        "bazbazbazbaz",
        2u,
        0u};

    auto const data = expected.to_data();
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<heading>(source);
    BOOST_REQUIRE_EQUAL(data.size(), heading::satoshi_fixed_size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(heading__magic_accessor__always__returns_initialized_value) {
    uint32_t expected = 3574u;
    message::heading instance(expected, "baz", 4356u, 7923u);
    BOOST_REQUIRE_EQUAL(expected, instance.magic());
}

BOOST_AUTO_TEST_CASE(heading__magic_setter__roundtrip__success) {
    uint32_t const expected = 3574u;
    message::heading instance;
    BOOST_REQUIRE_EQUAL(0, instance.magic());
    instance.set_magic(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.magic());
}

BOOST_AUTO_TEST_CASE(heading__command_accessor_1__always__returns_initialized_value) {
    std::string const expected = "asdge";
    message::heading instance(545u, expected, 4356u, 7923u);
    BOOST_REQUIRE_EQUAL(expected, instance.command());
}

BOOST_AUTO_TEST_CASE(heading__command_accessor_2__always__returns_initialized_value) {
    std::string const expected = "asdge";
    const message::heading instance(545u, expected, 4356u, 7923u);
    BOOST_REQUIRE_EQUAL(expected, instance.command());
}

BOOST_AUTO_TEST_CASE(heading__command_setter_1__roundtrip__success) {
    std::string const expected = "gdasd";
    message::heading instance;
    BOOST_REQUIRE(expected != instance.command());
    instance.set_command(expected);
    BOOST_REQUIRE(expected == instance.command());
}

BOOST_AUTO_TEST_CASE(heading__command_setter_2__roundtrip__success) {
    std::string expected = "eytyry";
    message::heading instance;
    BOOST_REQUIRE_EQUAL(false, expected == instance.command());
    instance.set_command("eytyry");
    BOOST_REQUIRE_EQUAL(expected, instance.command());
}

BOOST_AUTO_TEST_CASE(heading__payload_size_accessor__always__returns_initialized_value) {
    uint32_t const expected = 4356u;
    message::heading instance(3574u, "baz", expected, 7923u);
    BOOST_REQUIRE_EQUAL(expected, instance.payload_size());
}

BOOST_AUTO_TEST_CASE(heading__payload_size_setter__roundtrip__success) {
    uint32_t const expected = 3574u;
    message::heading instance;
    BOOST_REQUIRE_EQUAL(0, instance.payload_size());
    instance.set_payload_size(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.payload_size());
}

BOOST_AUTO_TEST_CASE(heading__checksum_accessor__always__returns_initialized_value) {
    uint32_t expected = 7923u;
    message::heading instance(3574u, "baz", 4356u, expected);
    BOOST_REQUIRE_EQUAL(expected, instance.checksum());
}

BOOST_AUTO_TEST_CASE(heading__checksum_setter__roundtrip__success) {
    uint32_t const expected = 3574u;
    message::heading instance;
    BOOST_REQUIRE_EQUAL(0, instance.checksum());
    instance.set_checksum(expected);
    BOOST_REQUIRE_EQUAL(expected, instance.checksum());
}

BOOST_AUTO_TEST_CASE(heading__operator_assign_equals__always__matches_equivalent) {
    message::heading value(1u, "foobar", 2u, 3u);
    BOOST_REQUIRE(value.is_valid());
    message::heading instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(heading__operator_boolean_equals__duplicates__returns_true) {
    const message::heading expected(1u, "foobar", 2u, 3u);
    message::heading instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(heading__operator_boolean_equals__differs__returns_false) {
    const message::heading expected(1u, "foobar", 2u, 3u);
    message::heading instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(heading__operator_boolean_not_equals__duplicates__returns_false) {
    const message::heading expected(1u, "foobar", 2u, 3u);
    message::heading instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(heading__operator_boolean_not_equals__differs__returns_true) {
    const message::heading expected(1u, "foobar", 2u, 3u);
    message::heading instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_CASE(heading__type__all_cases__match_expected) {
    message::heading instance;
    BOOST_REQUIRE(message::message_type::unknown == instance.type());
    instance.set_command(message::address::command);
    BOOST_REQUIRE(message::message_type::address == instance.type());
    instance.set_command(message::alert::command);
    BOOST_REQUIRE(message::message_type::alert == instance.type());
    instance.set_command(message::block::command);
    BOOST_REQUIRE(message::message_type::block == instance.type());
    instance.set_command(message::block_transactions::command);
    BOOST_REQUIRE(message::message_type::block_transactions == instance.type());
    instance.set_command(message::compact_block::command);
    BOOST_REQUIRE(message::message_type::compact_block == instance.type());
    instance.set_command(message::filter_add::command);
    BOOST_REQUIRE(message::message_type::filter_add == instance.type());
    instance.set_command(message::filter_clear::command);
    BOOST_REQUIRE(message::message_type::filter_clear == instance.type());
    instance.set_command(message::filter_load::command);
    BOOST_REQUIRE(message::message_type::filter_load == instance.type());
    instance.set_command(message::get_address::command);
    BOOST_REQUIRE(message::message_type::get_address == instance.type());
    instance.set_command(message::get_block_transactions::command);
    BOOST_REQUIRE(message::message_type::get_block_transactions == instance.type());
    instance.set_command(message::get_blocks::command);
    BOOST_REQUIRE(message::message_type::get_blocks == instance.type());
    instance.set_command(message::get_data::command);
    BOOST_REQUIRE(message::message_type::get_data == instance.type());
    instance.set_command(message::get_headers::command);
    BOOST_REQUIRE(message::message_type::get_headers == instance.type());
    instance.set_command(message::headers::command);
    BOOST_REQUIRE(message::message_type::headers == instance.type());
    instance.set_command(message::inventory::command);
    BOOST_REQUIRE(message::message_type::inventory == instance.type());
    instance.set_command(message::memory_pool::command);
    BOOST_REQUIRE(message::message_type::memory_pool == instance.type());
    instance.set_command(message::merkle_block::command);
    BOOST_REQUIRE(message::message_type::merkle_block == instance.type());
    instance.set_command(message::not_found::command);
    BOOST_REQUIRE(message::message_type::not_found == instance.type());
    instance.set_command(message::ping::command);
    BOOST_REQUIRE(message::message_type::ping == instance.type());
    instance.set_command(message::pong::command);
    BOOST_REQUIRE(message::message_type::pong == instance.type());
    instance.set_command(message::reject::command);
    BOOST_REQUIRE(message::message_type::reject == instance.type());
    instance.set_command(message::send_compact::command);
    BOOST_REQUIRE(message::message_type::send_compact == instance.type());
    instance.set_command(message::send_headers::command);
    BOOST_REQUIRE(message::message_type::send_headers == instance.type());
    instance.set_command(message::transaction::command);
    BOOST_REQUIRE(message::message_type::transaction == instance.type());
    instance.set_command(message::verack::command);
    BOOST_REQUIRE(message::message_type::verack == instance.type());
    instance.set_command(message::version::command);
    BOOST_REQUIRE(message::message_type::version == instance.type());
    instance.set_command(message::xverack::command);
    BOOST_REQUIRE(message::message_type::xverack == instance.type());
    instance.set_command(message::xversion::command);
    BOOST_REQUIRE(message::message_type::xversion == instance.type());
}

BOOST_AUTO_TEST_CASE(heading__maximum_size__always__matches_satoshi_fixed_size) {
    BOOST_REQUIRE_EQUAL(heading::satoshi_fixed_size(), heading::maximum_size());
}

#if defined(KTH_SEGWIT_ENABLED)
// TODO(kth): This test is broken for networks bigger than 4Mbs
//BOOST_AUTO_TEST_CASE(heading__maximum_payload_size__non_witness__matches_expected)
//{
//    static const size_t expected = 3u + (sizeof(uint32_t) + hash_size) * 50000u;
//    BOOST_REQUIRE_EQUAL(expected, heading::maximum_payload_size(0u, false));
//}

BOOST_AUTO_TEST_CASE(heading__maximum_payload_size__witness__matches_expected) {
    BOOST_REQUIRE_EQUAL(max_block_weight, heading::maximum_payload_size(0u, true));
}
#endif

BOOST_AUTO_TEST_SUITE_END()

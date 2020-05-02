// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <string>

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// /Satoshi:0.12.1/
// Invalid reject payload from [46.101.110.115:8333] bad data stream
// This contradicts docs in that it is tx with readable text vs. hash.
// tx : nonstandard : too-long-mempool-chain : <empty>
#define MALFORMED_REJECT "0274784016746f6f2d6c6f6e672d6d656d706f6f6c2d636861696e"

static std::string const reason_text = "My Reason...";
static auto const version_maximum = message::version::level::maximum;

static hash_digest const data{
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
     0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
     0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
     0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}};

// Start Boost Suite: reject tests

TEST_CASE("reject  factory from data  tx nonstandard empty data valid", "[reject]") {
    data_chunk payload;
    REQUIRE(decode_base16(payload, MALFORMED_REJECT));
    auto const reject = create<message::reject>(version_maximum, payload);
    REQUIRE(reject.is_valid());
}

TEST_CASE("reject  constructor 1  always invalid", "[reject]") {
    message::reject instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("reject  constructor 2  always  equals params", "[reject]") {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject instance(code, message, reason, data);
    REQUIRE(instance.is_valid());
    REQUIRE(code == instance.code());
    REQUIRE(message == instance.message());
    REQUIRE(reason == instance.reason());
    REQUIRE(data == instance.data());
}

TEST_CASE("reject  constructor 3  always  equals params", "[reject]") {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "sadfasdgd";
    std::string reason = "jgfghkggfsr";
    hash_digest data = hash_literal("ce8f4b713ffdd2658900845251890f30371856be201cd1f5b3d970f793634333");
    message::reject instance(code, std::move(message), std::move(reason), std::move(data));
    REQUIRE(instance.is_valid());
}

TEST_CASE("reject  constructor 4  always  equals params", "[reject]") {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject expected(code, message, reason, data);
    message::reject instance(expected);
    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
    REQUIRE(code == instance.code());
    REQUIRE(message == instance.message());
    REQUIRE(reason == instance.reason());
    REQUIRE(data == instance.data());
}

TEST_CASE("reject  constructor 5  always  equals params", "[reject]") {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject expected(code, message, reason, data);
    message::reject instance(std::move(expected));
    REQUIRE(instance.is_valid());
    REQUIRE(code == instance.code());
    REQUIRE(message == instance.message());
    REQUIRE(reason == instance.reason());
    REQUIRE(data == instance.data());
}

TEST_CASE("reject  from data  insufficient bytes  failure", "[reject]") {
    static data_chunk const raw{0xab};
    message::reject instance{};
    REQUIRE( ! entity_from_data(instance, version_maximum, raw));
}

TEST_CASE("reject  from data  insufficient version  failure", "[reject]") {
    const message::reject expected(
        message::reject::reason_code::dust,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};
    REQUIRE( ! entity_from_data(instance, message::reject::version_minimum - 1, raw));
}

TEST_CASE("reject  from data  code malformed  success", "[reject]") {
    const message::reject expected(
        message::reject::reason_code::malformed,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};
    REQUIRE(entity_from_data(instance, message::reject::version_minimum, raw));
    REQUIRE(expected == instance);
}

TEST_CASE("reject  from data  code invalid  success", "[reject]") {
    const message::reject expected(
        message::reject::reason_code::invalid,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};

    REQUIRE(entity_from_data(instance, message::reject::version_minimum, raw));
    REQUIRE(expected == instance);
}

TEST_CASE("reject  from data  code obsolete  success", "[reject]") {
    const message::reject expected(
        message::reject::reason_code::obsolete,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};
    REQUIRE(entity_from_data(instance, message::reject::version_minimum, raw));
    REQUIRE(expected == instance);
}

TEST_CASE("reject  from data  code duplicate  success", "[reject]") {
    const message::reject expected(
        message::reject::reason_code::duplicate,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};
    BOOST_REQUIRE(entity_from_data(instance, message::reject::version_minimum, raw));
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(reject__from_data__code_nonstandard__success) {
    const message::reject expected(
        message::reject::reason_code::nonstandard,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};
    BOOST_REQUIRE(entity_from_data(instance, message::reject::version_minimum, raw));
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(reject__from_data__code_dust__success) {
    const message::reject expected(
        message::reject::reason_code::dust,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};
    BOOST_REQUIRE(entity_from_data(instance, message::reject::version_minimum, raw));
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(reject__from_data__code_insufficient_fee__success) {
    const message::reject expected(
        message::reject::reason_code::insufficient_fee,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};
    BOOST_REQUIRE(entity_from_data(instance, message::reject::version_minimum, raw));
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(reject__from_data__code_checkpoint__success) {
    const message::reject expected(
        message::reject::reason_code::checkpoint,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};

    BOOST_REQUIRE(entity_from_data(instance, message::reject::version_minimum, raw));
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(reject__from_data__code_undefined__success) {
    const message::reject expected(
        message::reject::reason_code::undefined,
        message::block::command,
        reason_text,
        data);

    data_chunk const raw = expected.to_data(version_maximum);
    message::reject instance{};

    BOOST_REQUIRE(entity_from_data(instance, message::reject::version_minimum, raw));
    BOOST_REQUIRE(expected == instance);
}

BOOST_AUTO_TEST_CASE(reject__factory_from_data_1__valid_input__success) {
    const message::reject expected(
        message::reject::reason_code::dust,
        message::block::command,
        reason_text,
        data);

    auto const data = expected.to_data(version_maximum);
    auto const result = create<message::reject>(version_maximum, data);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version_maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version_maximum), result.serialized_size(version_maximum));
}

BOOST_AUTO_TEST_CASE(reject__factory_from_data_2__valid_input__success) {
    const message::reject expected(
        message::reject::reason_code::insufficient_fee,
        message::block::command,
        reason_text,
        data);

    auto const data = expected.to_data(version_maximum);
    data_source istream(data);
    auto const result = create<message::reject>(version_maximum, istream);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version_maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version_maximum), result.serialized_size(version_maximum));
}

BOOST_AUTO_TEST_CASE(reject__factory_from_data_3__valid_input__success) {
    const message::reject expected(
        message::reject::reason_code::duplicate,
        message::transaction::command,
        reason_text,
        data);

    auto const data = expected.to_data(version_maximum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::reject>(version_maximum, source);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(), result.serialized_size(version_maximum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(version_maximum), result.serialized_size(version_maximum));
}

BOOST_AUTO_TEST_CASE(reject__code_accessor__always__returns_initialized_value) {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject instance(code, message, reason, data);
    BOOST_REQUIRE(code == instance.code());
}

BOOST_AUTO_TEST_CASE(reject__code_setter__roundtrip__success) {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject instance;
    BOOST_REQUIRE(code != instance.code());
    instance.set_code(code);
    BOOST_REQUIRE(code == instance.code());
}

BOOST_AUTO_TEST_CASE(reject__message_accessor_1__always__returns_initialized_value) {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject instance(code, message, reason, data);
    BOOST_REQUIRE_EQUAL(message, instance.message());
}

BOOST_AUTO_TEST_CASE(reject__message_accessor_2__always__returns_initialized_value) {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const message::reject instance(code, message, reason, data);
    BOOST_REQUIRE_EQUAL(message, instance.message());
}

BOOST_AUTO_TEST_CASE(reject__message_setter_1__roundtrip__success) {
    std::string message = "Alpha Beta";
    message::reject instance;
    BOOST_REQUIRE(message != instance.message());
    instance.set_message(message);
    BOOST_REQUIRE_EQUAL(message, instance.message());
}

BOOST_AUTO_TEST_CASE(reject__message_setter_2__roundtrip__success) {
    std::string duplicate = "Gamma";
    std::string message = "Gamma";
    message::reject instance;
    BOOST_REQUIRE(duplicate != instance.message());
    instance.set_message(std::move(message));
    BOOST_REQUIRE_EQUAL(duplicate, instance.message());
}

BOOST_AUTO_TEST_CASE(reject__reason_accessor_1__always__returns_initialized_value) {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject instance(code, message, reason, data);
    BOOST_REQUIRE_EQUAL(reason, instance.reason());
}

BOOST_AUTO_TEST_CASE(reject__reason_accessor_2__always__returns_initialized_value) {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const message::reject instance(code, message, reason, data);
    BOOST_REQUIRE_EQUAL(reason, instance.reason());
}

BOOST_AUTO_TEST_CASE(reject__reason_setter_1__roundtrip__success) {
    std::string reason = "Alpha Beta";
    message::reject instance;
    BOOST_REQUIRE(reason != instance.reason());
    instance.set_reason(reason);
    BOOST_REQUIRE_EQUAL(reason, instance.reason());
}

BOOST_AUTO_TEST_CASE(reject__reason_setter_2__roundtrip__success) {
    std::string duplicate = "Gamma";
    std::string reason = "Gamma";
    message::reject instance;
    BOOST_REQUIRE(duplicate != instance.reason());
    instance.set_reason(std::move(reason));
    BOOST_REQUIRE_EQUAL(duplicate, instance.reason());
}

BOOST_AUTO_TEST_CASE(reject__data_accessor_1__always__returns_initialized_value) {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject instance(code, message, reason, data);
    BOOST_REQUIRE(data == instance.data());
}

BOOST_AUTO_TEST_CASE(reject__data_accessor_2__always__returns_initialized_value) {
    auto code = message::reject::reason_code::nonstandard;
    std::string message = "Alpha Beta";
    std::string reason = "Gamma Delta";
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    const message::reject instance(code, message, reason, data);
    BOOST_REQUIRE(data == instance.data());
}

BOOST_AUTO_TEST_CASE(reject__data_setter_1__roundtrip__success) {
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject instance;
    BOOST_REQUIRE(data != instance.data());
    instance.set_data(data);
    BOOST_REQUIRE(data == instance.data());
}

BOOST_AUTO_TEST_CASE(reject__data_setter_2__roundtrip__success) {
    hash_digest duplicate = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    hash_digest data = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");
    message::reject instance;
    BOOST_REQUIRE(duplicate != instance.data());
    instance.set_data(std::move(data));
    BOOST_REQUIRE(duplicate == instance.data());
}

BOOST_AUTO_TEST_CASE(reject__operator_assign_equals__always__matches_equivalent) {
    message::reject value(
        message::reject::reason_code::dust,
        "My Message",
        "My Reason",
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    BOOST_REQUIRE(value.is_valid());

    message::reject instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_CASE(reject__operator_boolean_equals__duplicates__returns_true) {
    const message::reject expected(
        message::reject::reason_code::dust,
        "My Message",
        "My Reason",
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    message::reject instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(reject__operator_boolean_equals__differs__returns_false) {
    const message::reject expected(
        message::reject::reason_code::dust,
        "My Message",
        "My Reason",
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    message::reject instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(reject__operator_boolean_not_equals__duplicates__returns_false) {
    const message::reject expected(
        message::reject::reason_code::dust,
        "My Message",
        "My Reason",
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    message::reject instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(reject__operator_boolean_not_equals__differs__returns_true) {
    const message::reject expected(
        message::reject::reason_code::dust,
        "My Message",
        "My Reason",
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

    message::reject instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

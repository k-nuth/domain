// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

// Start Boost Suite: alert payload tests

TEST_CASE("alert payload  constructor 1  always invalid", "[alert payload]") {
    message::alert_payload instance;
    REQUIRE(!instance.is_valid());
}

TEST_CASE("alert payload  constructor 2  always  equals params", "[alert payload]") {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
                                    cancel, set_cancel, min_version, max_version, set_sub_version,
                                    priority, comment, status_bar, reserved);

    REQUIRE(instance.is_valid());
    REQUIRE(version == instance.version());
    REQUIRE(relay_until == instance.relay_until());
    REQUIRE(expiration == instance.expiration());
    REQUIRE(id == instance.id());
    REQUIRE(cancel == instance.cancel());
    REQUIRE(set_cancel == instance.set_cancel());
    REQUIRE(min_version == instance.min_version());
    REQUIRE(max_version == instance.max_version());
    REQUIRE(set_sub_version == instance.set_sub_version());
    REQUIRE(priority == instance.priority());
    REQUIRE(comment == instance.comment());
    REQUIRE(status_bar == instance.status_bar());
    REQUIRE(reserved == instance.reserved());
}

TEST_CASE("alert payload  constructor 3  always  equals params", "[alert payload]") {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    auto dup_set_cancel = set_cancel;
    auto dup_set_sub_version = set_sub_version;
    auto dup_comment = comment;
    auto dup_status_bar = status_bar;
    auto dup_reserved = reserved;

    message::alert_payload instance(version, relay_until, expiration, id,
                                    cancel, std::move(dup_set_cancel), min_version, max_version,
                                    std::move(dup_set_sub_version), priority, std::move(dup_comment),
                                    std::move(dup_status_bar), std::move(dup_reserved));

    REQUIRE(instance.is_valid());
    REQUIRE(version == instance.version());
    REQUIRE(relay_until == instance.relay_until());
    REQUIRE(expiration == instance.expiration());
    REQUIRE(id == instance.id());
    REQUIRE(cancel == instance.cancel());
    REQUIRE(set_cancel == instance.set_cancel());
    REQUIRE(min_version == instance.min_version());
    REQUIRE(max_version == instance.max_version());
    REQUIRE(set_sub_version == instance.set_sub_version());
    REQUIRE(priority == instance.priority());
    REQUIRE(comment == instance.comment());
    REQUIRE(status_bar == instance.status_bar());
    REQUIRE(reserved == instance.reserved());
}

TEST_CASE("alert payload  constructor 4  always  equals params", "[alert payload]") {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    message::alert_payload value(version, relay_until, expiration, id,
                                 cancel, set_cancel, min_version, max_version, set_sub_version,
                                 priority, comment, status_bar, reserved);

    message::alert_payload instance(value);

    REQUIRE(instance.is_valid());
    REQUIRE(version == instance.version());
    REQUIRE(relay_until == instance.relay_until());
    REQUIRE(expiration == instance.expiration());
    REQUIRE(id == instance.id());
    REQUIRE(cancel == instance.cancel());
    REQUIRE(set_cancel == instance.set_cancel());
    REQUIRE(min_version == instance.min_version());
    REQUIRE(max_version == instance.max_version());
    REQUIRE(set_sub_version == instance.set_sub_version());
    REQUIRE(priority == instance.priority());
    REQUIRE(comment == instance.comment());
    REQUIRE(status_bar == instance.status_bar());
    REQUIRE(reserved == instance.reserved());
}

TEST_CASE("alert payload  constructor 5  always  equals params", "[alert payload]") {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    message::alert_payload value(version, relay_until, expiration, id,
                                 cancel, set_cancel, min_version, max_version, set_sub_version,
                                 priority, comment, status_bar, reserved);

    message::alert_payload instance(std::move(value));

    REQUIRE(instance.is_valid());
    REQUIRE(version == instance.version());
    REQUIRE(relay_until == instance.relay_until());
    REQUIRE(expiration == instance.expiration());
    REQUIRE(id == instance.id());
    REQUIRE(cancel == instance.cancel());
    REQUIRE(set_cancel == instance.set_cancel());
    REQUIRE(min_version == instance.min_version());
    REQUIRE(max_version == instance.max_version());
    REQUIRE(set_sub_version == instance.set_sub_version());
    REQUIRE(priority == instance.priority());
    REQUIRE(comment == instance.comment());
    REQUIRE(status_bar == instance.status_bar());
    REQUIRE(reserved == instance.reserved());
}

TEST_CASE("alert payload  from data  insufficient bytes  failure", "[alert payload]") {
    data_chunk raw{0xab, 0x11};
    message::alert_payload instance;

    REQUIRE(!entity_from_data(instance, message::version::level::minimum, raw));
    REQUIRE(!instance.is_valid());
}

TEST_CASE("alert payload  factory from data 1  wiki sample test  success", "[alert payload]") {
    data_chunk const raw{
        0x01, 0x00, 0x00, 0x00, 0x37, 0x66, 0x40, 0x4f, 0x00, 0x00,
        0x00, 0x00, 0xb3, 0x05, 0x43, 0x4f, 0x00, 0x00, 0x00, 0x00,
        0xf2, 0x03, 0x00, 0x00, 0xf1, 0x03, 0x00, 0x00, 0x00, 0x10,
        0x27, 0x00, 0x00, 0x48, 0xee, 0x00, 0x00, 0x00, 0x64, 0x00,
        0x00, 0x00, 0x00, 0x46, 0x53, 0x65, 0x65, 0x20, 0x62, 0x69,
        0x74, 0x63, 0x6f, 0x69, 0x6e, 0x2e, 0x6f, 0x72, 0x67, 0x2f,
        0x66, 0x65, 0x62, 0x32, 0x30, 0x20, 0x69, 0x66, 0x20, 0x79,
        0x6f, 0x75, 0x20, 0x68, 0x61, 0x76, 0x65, 0x20, 0x74, 0x72,
        0x6f, 0x75, 0x62, 0x6c, 0x65, 0x20, 0x63, 0x6f, 0x6e, 0x6e,
        0x65, 0x63, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x61, 0x66, 0x74,
        0x65, 0x72, 0x20, 0x32, 0x30, 0x20, 0x46, 0x65, 0x62, 0x72,
        0x75, 0x61, 0x72, 0x79, 0x00};

    const message::alert_payload expected{
        1,
        1329620535,
        1329792435,
        1010,
        1009,
        std::vector<uint32_t>{},
        10000,
        61000,
        std::vector<std::string>{},
        100,
        "",
        "See bitcoin.org/feb20 if you have trouble connecting after 20 February",
        ""};

    auto const result = create<message::alert_payload>(
        message::version::level::minimum, raw);

    REQUIRE(result.is_valid());
    REQUIRE(raw.size() == result.serialized_size(message::version::level::minimum));
    REQUIRE(result == expected);

    auto const data = expected.to_data(message::version::level::minimum);

    REQUIRE(raw.size() == data.size());
    REQUIRE(data.size() == expected.serialized_size(message::version::level::minimum));
}

TEST_CASE("alert payload  factory from data 1  roundtrip  success", "[alert payload]") {
    message::alert_payload expected{
        5,
        105169,
        723544,
        1779,
        1678,
        {10, 25256, 37, 98485, 250},
        75612,
        81354,
        {"alpha", "beta", "gamma", "delta"},
        781,
        "My Comment",
        "My Status Bar",
        "RESERVED?"};

    auto const data = expected.to_data(message::version::level::minimum);
    auto const result = create<message::alert_payload>(
        message::version::level::minimum, data);

    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
    REQUIRE(data.size() == result.serialized_size(message::version::level::minimum));
    REQUIRE(expected.serialized_size(message::version::level::minimum) == result.serialized_size(message::version::level::minimum));
}

TEST_CASE("alert payload  factory from data 2  roundtrip  success", "[alert payload]") {
    message::alert_payload expected{
        5,
        105169,
        723544,
        1779,
        1678,
        {10, 25256, 37, 98485, 250},
        75612,
        81354,
        {"alpha", "beta", "gamma", "delta"},
        781,
        "My Comment",
        "My Status Bar",
        "RESERVED?"};

    auto const data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    auto const result = create<message::alert_payload>(
        message::version::level::minimum, istream);

    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
    REQUIRE(data.size() == result.serialized_size(message::version::level::minimum));
    REQUIRE(expected.serialized_size(message::version::level::minimum) == result.serialized_size(message::version::level::minimum));
}

TEST_CASE("alert payload  factory from data 3  roundtrip  success", "[alert payload]") {
    const message::alert_payload expected{
        5,
        105169,
        723544,
        1779,
        1678,
        {10, 25256, 37, 98485, 250},
        75612,
        81354,
        {"alpha", "beta", "gamma", "delta"},
        781,
        "My Comment",
        "My Status Bar",
        "RESERVED?"};

    auto const data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = create<message::alert_payload>(
        message::version::level::minimum, source);

    REQUIRE(result.is_valid());
    REQUIRE(expected == result);
    REQUIRE(data.size() == result.serialized_size(message::version::level::minimum));
    REQUIRE(expected.serialized_size(message::version::level::minimum) == result.serialized_size(message::version::level::minimum));
}

TEST_CASE("alert payload  version  roundtrip  success", "[alert payload]") {
    uint32_t value = 1234u;
    message::alert_payload instance;
    REQUIRE(instance.version() != value);
    instance.set_version(value);
    REQUIRE(value == instance.version());
}

TEST_CASE("alert payload  relay until  roundtrip  success", "[alert payload]") {
    uint64_t value = 5121234u;
    message::alert_payload instance;
    REQUIRE(instance.relay_until() != value);
    instance.set_relay_until(value);
    REQUIRE(value == instance.relay_until());
}

TEST_CASE("alert payload  expiration  roundtrip  success", "[alert payload]") {
    uint64_t value = 5121234u;
    message::alert_payload instance;
    REQUIRE(instance.expiration() != value);
    instance.set_expiration(value);
    REQUIRE(value == instance.expiration());
}

TEST_CASE("alert payload  id  roundtrip  success", "[alert payload]") {
    uint32_t value = 68215u;
    message::alert_payload instance;
    REQUIRE(instance.id() != value);
    instance.set_id(value);
    REQUIRE(value == instance.id());
}

TEST_CASE("alert payload  cancel  roundtrip  success", "[alert payload]") {
    uint32_t value = 68215u;
    message::alert_payload instance;
    REQUIRE(instance.cancel() != value);
    instance.set_cancel(value);
    REQUIRE(value == instance.cancel());
}

TEST_CASE("alert payload  set cancel accessor 1  always  returns initialized", "[alert payload]") {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
                                    cancel, set_cancel, min_version, max_version, set_sub_version,
                                    priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_cancel_accessor_2__always__returns_initialized) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
                                          cancel, set_cancel, min_version, max_version, set_sub_version,
                                          priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_cancel_setter_1__roundtrip__success) {
    const std::vector<uint32_t> value = {68215u, 34542u, 4756u};
    message::alert_payload instance;
    BOOST_REQUIRE(instance.set_cancel() != value);
    instance.set_set_cancel(value);
    BOOST_REQUIRE(value == instance.set_cancel());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_cancel_setter_2__roundtrip__success) {
    const std::vector<uint32_t> value = {68215u, 34542u, 4756u};
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.set_cancel() != value);
    instance.set_set_cancel(std::move(dup_value));
    BOOST_REQUIRE(value == instance.set_cancel());
}

BOOST_AUTO_TEST_CASE(alert_payload__min_version__roundtrip__success) {
    uint32_t value = 68215u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.min_version() != value);
    instance.set_min_version(value);
    BOOST_REQUIRE_EQUAL(value, instance.min_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__max_version__roundtrip__success) {
    uint32_t value = 68215u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.max_version() != value);
    instance.set_max_version(value);
    BOOST_REQUIRE_EQUAL(value, instance.max_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_sub_version_accessor_1__always__returns_initialized) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
                                    cancel, set_cancel, min_version, max_version, set_sub_version,
                                    priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_sub_version_accessor_2__always__returns_initialized) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
                                          cancel, set_cancel, min_version, max_version, set_sub_version,
                                          priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_sub_version_setter_1__roundtrip__success) {
    const std::vector<std::string> value = {"asdfa", "sgfdf", "Tryertsd"};
    message::alert_payload instance;
    BOOST_REQUIRE(instance.set_sub_version() != value);
    instance.set_set_sub_version(value);
    BOOST_REQUIRE(value == instance.set_sub_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__set_sub_version_setter_2__roundtrip__success) {
    const std::vector<std::string> value = {"asdfa", "sgfdf", "Tryertsd"};
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.set_sub_version() != value);
    instance.set_set_sub_version(std::move(dup_value));
    BOOST_REQUIRE(value == instance.set_sub_version());
}

BOOST_AUTO_TEST_CASE(alert_payload__priority__roundtrip__success) {
    uint32_t value = 68215u;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.priority() != value);
    instance.set_priority(value);
    BOOST_REQUIRE_EQUAL(value, instance.priority());
}

BOOST_AUTO_TEST_CASE(alert_payload__comment_accessor_1__always__returns_initialized) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
                                    cancel, set_cancel, min_version, max_version, set_sub_version,
                                    priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
}

BOOST_AUTO_TEST_CASE(alert_payload__comment_accessor_2__always__returns_initialized) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
                                          cancel, set_cancel, min_version, max_version, set_sub_version,
                                          priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
}

BOOST_AUTO_TEST_CASE(alert_payload__comment_setter_1__roundtrip__success) {
    std::string const value = "asdfa";
    message::alert_payload instance;
    BOOST_REQUIRE(instance.comment() != value);
    instance.set_comment(value);
    BOOST_REQUIRE(value == instance.comment());
}

BOOST_AUTO_TEST_CASE(alert_payload__comment_setter_2__roundtrip__success) {
    std::string const value = "Tryertsd";
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.comment() != value);
    instance.set_comment(std::move(dup_value));
    BOOST_REQUIRE(value == instance.comment());
}

BOOST_AUTO_TEST_CASE(alert_payload__status_bar_accessor_1__always__returns_initialized) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
                                    cancel, set_cancel, min_version, max_version, set_sub_version,
                                    priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
}

BOOST_AUTO_TEST_CASE(alert_payload__status_bar_accessor_2__always__returns_initialized) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
                                          cancel, set_cancel, min_version, max_version, set_sub_version,
                                          priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
}

BOOST_AUTO_TEST_CASE(alert_payload__status_bar_setter_1__roundtrip__success) {
    std::string const value = "asdfa";
    message::alert_payload instance;
    BOOST_REQUIRE(instance.status_bar() != value);
    instance.set_status_bar(value);
    BOOST_REQUIRE(value == instance.status_bar());
}

BOOST_AUTO_TEST_CASE(alert_payload__status_bar_setter_2__roundtrip__success) {
    std::string const value = "Tryertsd";
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.status_bar() != value);
    instance.set_status_bar(std::move(dup_value));
    BOOST_REQUIRE(value == instance.status_bar());
}

BOOST_AUTO_TEST_CASE(alert_payload__reserved_accessor_1__always__returns_initialized) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    message::alert_payload instance(version, relay_until, expiration, id,
                                    cancel, set_cancel, min_version, max_version, set_sub_version,
                                    priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__reserved_accessor_2__always__returns_initialized) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    const message::alert_payload instance(version, relay_until, expiration, id,
                                          cancel, set_cancel, min_version, max_version, set_sub_version,
                                          priority, comment, status_bar, reserved);

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__reserved_setter_1__roundtrip__success) {
    std::string const value = "asdfa";
    message::alert_payload instance;
    BOOST_REQUIRE(instance.reserved() != value);
    instance.set_reserved(value);
    BOOST_REQUIRE(value == instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__reserved_setter_2__roundtrip__success) {
    std::string const value = "Tryertsd";
    auto dup_value = value;
    message::alert_payload instance;
    BOOST_REQUIRE(instance.reserved() != value);
    instance.set_reserved(std::move(dup_value));
    BOOST_REQUIRE(value == instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_assign_equals__always__matches_equivalent) {
    uint32_t const version = 3452u;
    uint64_t const relay_until = 64556u;
    uint64_t const expiration = 78545u;
    uint32_t const id = 43547u;
    uint32_t const cancel = 546562345u;
    const std::vector<uint32_t> set_cancel = {2345u, 346754u, 234u, 4356u};
    uint32_t const min_version = 4644u;
    uint32_t const max_version = 89876u;
    const std::vector<std::string> set_sub_version = {"foo", "bar", "baz"};
    uint32_t const priority = 34323u;
    std::string const comment = "asfgsddsa";
    std::string const status_bar = "fgjdfhjg";
    std::string const reserved = "utyurtevc";

    message::alert_payload value(version, relay_until, expiration, id,
                                 cancel, set_cancel, min_version, max_version, set_sub_version,
                                 priority, comment, status_bar, reserved);

    BOOST_REQUIRE(value.is_valid());

    message::alert_payload instance;
    BOOST_REQUIRE(!instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE_EQUAL(version, instance.version());
    BOOST_REQUIRE_EQUAL(relay_until, instance.relay_until());
    BOOST_REQUIRE_EQUAL(expiration, instance.expiration());
    BOOST_REQUIRE_EQUAL(id, instance.id());
    BOOST_REQUIRE_EQUAL(cancel, instance.cancel());
    BOOST_REQUIRE(set_cancel == instance.set_cancel());
    BOOST_REQUIRE_EQUAL(min_version, instance.min_version());
    BOOST_REQUIRE_EQUAL(max_version, instance.max_version());
    BOOST_REQUIRE(set_sub_version == instance.set_sub_version());
    BOOST_REQUIRE_EQUAL(priority, instance.priority());
    BOOST_REQUIRE_EQUAL(comment, instance.comment());
    BOOST_REQUIRE_EQUAL(status_bar, instance.status_bar());
    BOOST_REQUIRE_EQUAL(reserved, instance.reserved());
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_boolean_equals__duplicates__returns_true) {
    const message::alert_payload expected(3452u, 64556u, 78545u, 43547u,
                                          546562345u, {2345u, 346754u, 234u, 4356u}, 4644u, 89876u,
                                          {"foo", "bar", "baz"}, 34323u, "asfgsddsa", "fgjdfhjg", "utyurtevc");

    message::alert_payload instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_boolean_equals__differs__returns_false) {
    const message::alert_payload expected(3452u, 64556u, 78545u, 43547u,
                                          546562345u, {2345u, 346754u, 234u, 4356u}, 4644u, 89876u,
                                          {"foo", "bar", "baz"}, 34323u, "asfgsddsa", "fgjdfhjg", "utyurtevc");

    message::alert_payload instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_boolean_not_equals__duplicates__returns_false) {
    const message::alert_payload expected(3452u, 64556u, 78545u, 43547u,
                                          546562345u, {2345u, 346754u, 234u, 4356u}, 4644u, 89876u,
                                          {"foo", "bar", "baz"}, 34323u, "asfgsddsa", "fgjdfhjg", "utyurtevc");

    message::alert_payload instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(alert_payload__operator_boolean_not_equals__differs__returns_true) {
    const message::alert_payload expected(3452u, 64556u, 78545u, 43547u,
                                          546562345u, {2345u, 346754u, 234u, 4356u}, 4644u, 89876u,
                                          {"foo", "bar", "baz"}, 34323u, "asfgsddsa", "fgjdfhjg", "utyurtevc");

    message::alert_payload instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

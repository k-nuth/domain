// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <optional>

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::wallet;

// Start Boost Suite: bitcoin uri tests

// Constructors
// ----------------------------------------------------------------------------

TEST_CASE("bitcoin uri  construct  uninitialized  false", "[bitcoin uri]") {
    REQUIRE(!bitcoin_uri());
}

TEST_CASE("bitcoin uri  construct  initialized  true", "[bitcoin uri]") {
    REQUIRE(bitcoin_uri("bitcoin:"));
}

TEST_CASE("bitcoin uri  construct  scheme mixed case  normalized", "[bitcoin uri]") {
    REQUIRE(bitcoin_uri("bitcOin:").encoded() == "bitcoin:");
}

TEST_CASE("bitcoin uri  construct  invalid scheme  false", "[bitcoin uri]") {
    REQUIRE(!bitcoin_uri("fedcoin:"));
}

TEST_CASE("bitcoin uri  construct  payment address only  false", "[bitcoin uri]") {
    REQUIRE(!bitcoin_uri("113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD"));
}

TEST_CASE("bitcoin uri  construct  stealth address only  false", "[bitcoin uri]") {
    REQUIRE(!bitcoin_uri("hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i"));
}

TEST_CASE("bitcoin uri  construct  fragment  false", "[bitcoin uri]") {
    REQUIRE(!bitcoin_uri("bitcoin:#satoshi"));
}

TEST_CASE("bitcoin uri  construct  strict  test", "[bitcoin uri]") {
    REQUIRE(!bitcoin_uri("bitcoin:?label=Some テスト"));
}

TEST_CASE("bitcoin uri  construct  not strict  test", "[bitcoin uri]") {
    REQUIRE(bitcoin_uri("bitcoin:?label=Some テスト", false));
}

// Setters
// ----------------------------------------------------------------------------

TEST_CASE("bitcoin uri  set path  payment address  expected encoding", "[bitcoin uri]") {
    auto const expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    auto const expected_uri = std::string("bitcoin:") + expected_payment;

    bitcoin_uri uri;
    REQUIRE(uri.set_path(expected_payment));
    REQUIRE(uri.encoded() == expected_uri);
}

TEST_CASE("bitcoin uri  set path  stealth address  expected encoding", "[bitcoin uri]") {
    auto const expected_payment = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    auto const expected_uri = std::string("bitcoin:") + expected_payment;

    bitcoin_uri uri;
    REQUIRE(uri.set_path(expected_payment));
    REQUIRE(uri.encoded() == expected_uri);
}

TEST_CASE("bitcoin uri  set path  reset stealth after payment  expected encoding", "[bitcoin uri]") {
    auto const expected_stealth = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    auto const expected_uri = std::string("bitcoin:") + expected_stealth;

    bitcoin_uri uri;
    auto const payment = payment_address("113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    REQUIRE(payment);
    uri.set_address(payment);
    uri.set_address(stealth_address(expected_stealth));
    REQUIRE(uri.encoded() == expected_uri);
}

TEST_CASE("bitcoin uri  set path  reset payment after stealth  expected encoding", "[bitcoin uri]") {
    auto const expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    auto const expected_uri = std::string("bitcoin:") + expected_payment;

    bitcoin_uri uri;
    auto const stealth = stealth_address("hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i");
    REQUIRE(stealth);
    uri.set_address(stealth);
    uri.set_address(payment_address(expected_payment));
    REQUIRE(uri.encoded() == expected_uri);
}

TEST_CASE("bitcoin uri  set path  reset path  false", "[bitcoin uri]") {
    auto const expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    auto const expected_uri = std::string("bitcoin:") + expected_payment;

    bitcoin_uri uri;
    uri.set_address(stealth_address("hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i"));

    // The set_path will not reset a path. This is necessary to catch failures in non-strict parsing.
    REQUIRE(!uri.set_path(expected_payment));
}

TEST_CASE("bitcoin uri  set amount  reset amount  latter amount", "[bitcoin uri]") {
    bitcoin_uri uri;
    uri.set_amount(10000000000);
    uri.set_amount(120000);
    REQUIRE(uri.encoded() == "bitcoin:?amount=0.0012");
}

TEST_CASE("bitcoin uri  all setters  complex uri  expected encoding", "[bitcoin uri]") {
    bitcoin_uri uri;
    uri.set_path("113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    uri.set_amount(120000);
    uri.set_label("&=\n");
    uri.set_message("hello bitcoin");
    uri.set_r("http://example.com?purchase=shoes&user=bob");

    BOOST_REQUIRE_EQUAL(uri.encoded(),
                        "bitcoin:113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD?"
                        "amount=0.0012&"
                        "label=%26%3D%0A&"
                        "message=hello%20bitcoin&"
                        "r=http://example.com?purchase%3Dshoes%26user%3Dbob");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__set_parameter__amount_denormalized__normalized) {
    bitcoin_uri uri;
    BOOST_REQUIRE(uri.set_parameter("amount", ".0012"));
    BOOST_REQUIRE_EQUAL(uri.encoded(), "bitcoin:?amount=0.0012");
}

// Getters
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(bitcoin_uri__amount__set__expected) {
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?amount=0.0012").amount(), 120000u);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__label__escaped__expected) {
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?label=%26%3D%0A").label(), "&=\n");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__message__escaped__expected) {
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?message=hello%20bitcoin").message(), "hello bitcoin");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__r__escaped__expected) {
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?r=http://example.com?purchase%3Dshoes%26user%3Dbob").r(), "http://example.com?purchase=shoes&user=bob");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__payment__valid__expected) {
    auto const expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    auto const expected_uri = std::string("bitcoin:") + expected_payment;
    BOOST_REQUIRE_EQUAL(bitcoin_uri(expected_uri).payment().encoded(), expected_payment);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__stealth__valid__expected) {
    auto const expected_stealth = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    auto const expected_uri = std::string("bitcoin:") + expected_stealth;
    BOOST_REQUIRE_EQUAL(bitcoin_uri(expected_uri).stealth().encoded(), expected_stealth);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__address__payment__expected) {
    auto const expected_payment = "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD";
    auto const expected_uri = std::string("bitcoin:") + expected_payment;
    BOOST_REQUIRE_EQUAL(bitcoin_uri(expected_uri).address(), expected_payment);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__address__stealth__expected) {
    auto const expected_stealth = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    auto const expected_uri = std::string("bitcoin:") + expected_stealth;
    BOOST_REQUIRE_EQUAL(bitcoin_uri(expected_uri).address(), expected_stealth);
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__parameter_amount__denormalized__normalized) {
    BOOST_REQUIRE_EQUAL(bitcoin_uri("bitcoin:?amount=.0012").parameter("amount"), "0.0012");
}

BOOST_AUTO_TEST_CASE(bitcoin_uri__parameters_all__complex_uri__expected) {
    bitcoin_uri uri(
        "bitcoin:113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD?"
        "amount=0.0012&"
        "label=%26%3D%0A&"
        "message=hello%20bitcoin&"
        "r=http://example.com?purchase%3Dshoes%26user%3Dbob");

    BOOST_REQUIRE_EQUAL(uri.address(), "113Pfw4sFqN1T5kXUnKbqZHMJHN9oyjtgD");
    BOOST_REQUIRE_EQUAL(uri.parameter("amount"), "0.0012");
    BOOST_REQUIRE_EQUAL(uri.parameter("label"), "&=\n");
    BOOST_REQUIRE_EQUAL(uri.parameter("message"), "hello bitcoin");
    BOOST_REQUIRE_EQUAL(uri.parameter("r"), "http://example.com?purchase=shoes&user=bob");
}

BOOST_AUTO_TEST_SUITE_END()

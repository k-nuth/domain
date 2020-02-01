// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

<<<<<<< HEAD
#include <bitcoin/bitcoin.hpp>
=======
#include <kth/domain.hpp>
>>>>>>> dev
#include <boost/test/unit_test.hpp>

using namespace bc;
using namespace bc::wallet;

BOOST_AUTO_TEST_SUITE(stealth_address_tests)

#define SCAN_KEY "03d9e876028f4fc062c19f7097762e4affc2ce4edfffa7d42e3c17cd157ec6d1bc"
#define SPEND_KEY1 "0215a49b55a2ed2a02569cb6c018644211d408caab3aca86d2cc7d6a9e5789b1d2"
#define STEALTH_ADDRESS "vJmzLu29obZcUGXXgotapfQLUpz7dfnZpbr4xg1R75qctf8xaXAteRdi3ZUk3T2ZMSad5KyPbve7uyH6eswYAxLHRVSbWgNUeoGuXp"

BOOST_AUTO_TEST_CASE(stealth_address__construct__string__expected_encoding) {
    ec_compressed scan;
    ec_compressed spend1;
    BOOST_REQUIRE(decode_base16(scan, SCAN_KEY));
    BOOST_REQUIRE(decode_base16(spend1, SPEND_KEY1));
    stealth_address address({}, scan, {spend1}, 0, 42);
    BOOST_REQUIRE(address);
    BOOST_REQUIRE_EQUAL(address.encoded(), STEALTH_ADDRESS);
}

BOOST_AUTO_TEST_CASE(stealth_address__construct__decoded__expected_properties) {
    stealth_address address(STEALTH_ADDRESS);
    BOOST_REQUIRE(address);
    BOOST_REQUIRE_EQUAL((size_t)address.version(), 42);
    BOOST_REQUIRE_EQUAL(encode_base16(address.scan_key()), SCAN_KEY);
    BOOST_REQUIRE_EQUAL(address.spend_keys().size(), 1u);
    BOOST_REQUIRE_EQUAL(encode_base16(address.spend_keys()[0]), SPEND_KEY1);
    BOOST_REQUIRE_EQUAL((size_t)address.signatures(), 1u);
    BOOST_REQUIRE_EQUAL(address.filter().size(), 0u);
    BOOST_REQUIRE_EQUAL(address.encoded(), STEALTH_ADDRESS);
}

BOOST_AUTO_TEST_CASE(stealth_address__encoding__scan_mainnet__round_trips) {
    auto const encoded = "vJmzLu29obZcUGXXgotapfQLUpz7dfnZpbr4xg1R75qctf8xaXAteRdi3ZUk3T2ZMSad5KyPbve7uyH6eswYAxLHRVSbWgNUeoGuXp";
    stealth_address address(encoded);
    BOOST_REQUIRE_EQUAL(address.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(address.version(), 42u);
}

BOOST_AUTO_TEST_CASE(stealth_address__encoding__scan_testnet__round_trips) {
    std::string const encoded = "waPXhQwQE9tDugfgLkvpDs3dnkPx1RsfDjFt4zBq7EeWeATRHpyQpYrFZR8T4BQy91Vpvshm2TDER8b9ZryuZ8VSzz8ywzNzX8NqF4";
    stealth_address address(encoded);
    BOOST_REQUIRE_EQUAL(address.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(address.version(), 43u);
}

BOOST_AUTO_TEST_CASE(stealth_address__encoding__scan_pub_mainnet__round_trips) {
    auto const encoded = "hfFGUXFPKkQ5M6LC6aEUKMsURdhw93bUdYdacEtBA8XttLv7evZkira2i";
    stealth_address address(encoded);
    BOOST_REQUIRE_EQUAL(address.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(address.version(), 42u);
}

BOOST_AUTO_TEST_CASE(stealth_address__encoding__scan_pub_testnet__round_trip) {
    auto const encoded = "idPayBqZUpZH7Y5GTaoEyGxDsEmU377JUmhtqG8yoHCkfGfhnAHmGUJbL";
    stealth_address address(encoded);
    BOOST_REQUIRE_EQUAL(address.encoded(), encoded);
    BOOST_REQUIRE_EQUAL(address.version(), 43u);
}

BOOST_AUTO_TEST_SUITE_END()

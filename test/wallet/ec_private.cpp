// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::wallet;

BOOST_AUTO_TEST_SUITE(ec_private_tests)

// TODO(legacy): add version tests

#define SECRET "8010b1bb119ad37d4b65a1022a314897b1b3614b345974332cb1b9582cf03536"
#define WIF_COMPRESSED "L1WepftUBemj6H4XQovkiW1ARVjxMqaw4oj2kmkYqdG1xTnBcHfC"
#define WIF_UNCOMPRESSED "5JngqQmHagNTknnCshzVUysLMWAjT23FWs1TgNU5wyFH5SB3hrP"

// TODO(legacy): implement testnet version tests
//#define WIF_COMPRESSED_TESTNET "cRseHatKciTzFiXnoDjt5pWE3j3N2Hgd8qsVsCD4Ljv2DCwuD1V6"
//#define WIF_UNCOMPRESSED_TESTNET "92ZKR9aqAuSbirHVW3tQMaRJ1AXScBaSrosQkzpbHhzKrVBsZBL"

BOOST_AUTO_TEST_CASE(ec_private__compressed_wif__compressed__test) {
    BOOST_REQUIRE(ec_private(WIF_COMPRESSED).compressed());
}

BOOST_AUTO_TEST_CASE(ec_private__uncompressed_wif__not_compressed__test) {
    BOOST_REQUIRE(!ec_private(WIF_UNCOMPRESSED).compressed());
}

BOOST_AUTO_TEST_CASE(ec_private__encode_wif__compressed__test) {
    BOOST_REQUIRE_EQUAL(ec_private(base16_literal(SECRET)).encoded(), WIF_COMPRESSED);
}

BOOST_AUTO_TEST_CASE(ec_private__encode_wif__uncompressed__test) {
    BOOST_REQUIRE_EQUAL(ec_private(base16_literal(SECRET), 0x8000, false).encoded(), WIF_UNCOMPRESSED);
}

BOOST_AUTO_TEST_CASE(ec_private__decode_wif__compressed__test) {
    ec_private secret(WIF_COMPRESSED);
    BOOST_REQUIRE_EQUAL(encode_base16(secret.secret()), SECRET);
    BOOST_REQUIRE_EQUAL(secret.version(), 0x8000);
    BOOST_REQUIRE(secret.compressed());
}

BOOST_AUTO_TEST_CASE(ec_private__decode_wif__uncompressed__test) {
    ec_private secret(WIF_UNCOMPRESSED);
    BOOST_REQUIRE_EQUAL(encode_base16(secret.secret()), SECRET);
    BOOST_REQUIRE_EQUAL(secret.version(), 0x8000);
    BOOST_REQUIRE(!secret.compressed());
}

BOOST_AUTO_TEST_SUITE_END()

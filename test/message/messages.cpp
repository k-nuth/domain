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

BOOST_AUTO_TEST_SUITE(messages_tests)

BOOST_AUTO_TEST_CASE(messages__variable_uint_size__one_byte__expected) {
    static uint64_t const value = 1;
    BOOST_REQUIRE_EQUAL(message::variable_uint_size(value), 1u);
}

BOOST_AUTO_TEST_CASE(messages__variable_uint_size__two_byte__expected) {
    static uint64_t const value = 0xfe;
    BOOST_REQUIRE_EQUAL(message::variable_uint_size(value), 3u);
}

BOOST_AUTO_TEST_CASE(messages__variable_uint_size__four_byte__expected) {
    static uint64_t const value = 0x10000;
    BOOST_REQUIRE_EQUAL(message::variable_uint_size(value), 5u);
}

BOOST_AUTO_TEST_CASE(messages__variable_uint_size__eight_byte__expected) {
    static uint64_t const value = 0x100000000;
    BOOST_REQUIRE_EQUAL(message::variable_uint_size(value), 9u);
}

BOOST_AUTO_TEST_SUITE_END()

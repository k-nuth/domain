// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace bc;

BOOST_AUTO_TEST_SUITE(get_address_tests)

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_chunk) {
    const message::get_address expected{};
    auto const data = expected.to_data(message::version::level::minimum);
    auto const result = message::get_address::factory_from_data(
        message::version::level::minimum, data);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_stream) {
    const message::get_address expected{};
    auto const data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    auto const result = message::get_address::factory_from_data(
        message::version::level::minimum, istream);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_reader) {
    const message::get_address expected{};
    auto const data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = message::get_address::factory_from_data(
        message::version::level::minimum, source);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_SUITE_END()

// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin/bitcoin.hpp>
#include <boost/test/unit_test.hpp>

using namespace bc;

BOOST_AUTO_TEST_SUITE(send_headers_tests)

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_chunk) {
    const message::send_headers expected{};
    auto const data = expected.to_data(message::version::level::maximum);
    auto const result = message::send_headers::factory_from_data(
        message::version::level::maximum, data);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_stream) {
    const message::send_headers expected{};
    auto const data = expected.to_data(message::version::level::maximum);
    data_source istream(data);
    auto const result = message::send_headers::factory_from_data(
        message::version::level::maximum, istream);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_reader) {
    const message::send_headers expected{};
    auto const data = expected.to_data(message::version::level::maximum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = message::send_headers::factory_from_data(
        message::version::level::maximum, source);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(from_data_reader_version_prior_bip130_failure) {
    data_chunk data{};
    data_source istream(data);
    istream_reader source(istream);
    message::send_headers instance{};
    auto const result = instance.from_data(message::version::level::bip130 - 1, source);

    BOOST_REQUIRE(!result);
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(from_data_reader_version_at_least_bip130_success) {
    data_chunk data{};
    data_source istream(data);
    istream_reader source(istream);
    message::send_headers instance{};
    auto const result = instance.from_data(message::version::level::bip130, source);

    BOOST_REQUIRE(result);
    BOOST_REQUIRE(instance.is_valid());
}

BOOST_AUTO_TEST_SUITE_END()

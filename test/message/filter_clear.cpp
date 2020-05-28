// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::message;

BOOST_AUTO_TEST_SUITE(filter_clear_tests)

BOOST_AUTO_TEST_CASE(from_data_insufficient_version_failure) {
    static const filter_clear expected{};
    auto const raw = expected.to_data(version::level::maximum);
    filter_clear instance{};

    BOOST_REQUIRE(!entity_from_data(instance, filter_clear::version_minimum - 1, raw));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_chunk) {
    static const filter_clear expected{};
    auto const data = expected.to_data(version::level::maximum);
    auto const result = create<filter_clear>(version::level::maximum, data);

    BOOST_REQUIRE_EQUAL(data.size(), 0u);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(result.serialized_size(version::level::maximum), 0u);
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_stream) {
    static const filter_clear expected{};
    auto const data = expected.to_data(version::level::maximum);
    data_source istream(data);
    auto const result = create<filter_clear>(version::level::maximum, istream);

    BOOST_REQUIRE_EQUAL(data.size(), 0u);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(result.serialized_size(version::level::maximum), 0u);
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_reader) {
    static const filter_clear expected{};
    auto const data = expected.to_data(version::level::maximum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = filter_clear::factory_from_data(version::level::maximum, source);

    BOOST_REQUIRE_EQUAL(data.size(), 0u);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(result.serialized_size(version::level::maximum), 0u);
}

BOOST_AUTO_TEST_SUITE_END()

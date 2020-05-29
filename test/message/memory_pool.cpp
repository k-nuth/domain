// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain.hpp>
#include <boost/test/unit_test.hpp>

using namespace kth;
using namespace kd;

BOOST_AUTO_TEST_SUITE(memory_pool_tests)

BOOST_AUTO_TEST_CASE(from_data_insufficient_version_failure) {
    const message::memory_pool expected;
    data_chunk const data = expected.to_data(message::version::level::maximum);
    message::memory_pool instance{};

    BOOST_REQUIRE_EQUAL(false, entity_from_data(instance, 
                                   message::memory_pool::version_minimum - 1, data));
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_chunk) {
    const message::memory_pool expected{};
    auto const data = expected.to_data(message::version::level::maximum);
    auto const result = create<message::memory_pool>(
        message::version::level::maximum, data);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_stream) {
    const message::memory_pool expected{};
    auto const data = expected.to_data(message::version::level::maximum);
    data_source istream(data);
    auto const result = create<message::memory_pool>(
        message::version::level::maximum, istream);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_reader) {
    const message::memory_pool expected{};
    auto const data = expected.to_data(message::version::level::maximum);
    data_source istream(data);
    istream_reader source(istream);
    auto const result = message::memory_pool::factory_from_data(
        message::version::level::maximum, source);

    BOOST_REQUIRE_EQUAL(0u, data.size());
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(0u, result.serialized_size(message::version::level::maximum));
}

BOOST_AUTO_TEST_SUITE_END()

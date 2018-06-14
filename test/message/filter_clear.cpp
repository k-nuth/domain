/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;
using namespace bc::message;

BOOST_AUTO_TEST_SUITE(filter_clear_tests)

BOOST_AUTO_TEST_CASE(from_data_insufficient_version_failure)
{
    static const filter_clear expected{};
    const auto raw = expected.to_data(version::level::maximum);
    filter_clear instance{};

    BOOST_REQUIRE(!instance.from_data(filter_clear::version_minimum - 1, raw));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_chunk)
{
    static const filter_clear expected{};
    const auto data = expected.to_data(version::level::maximum);
    const auto result = filter_clear::factory_from_data(version::level::maximum, data);

    BOOST_REQUIRE_EQUAL(data.size(), 0u);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(result.serialized_size(version::level::maximum), 0u);
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_stream)
{
    static const filter_clear expected{};
    const auto data = expected.to_data(version::level::maximum);
    data_source istream(data);
    const auto result = filter_clear::factory_from_data(version::level::maximum, istream);

    BOOST_REQUIRE_EQUAL(data.size(), 0u);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(result.serialized_size(version::level::maximum), 0u);
}

BOOST_AUTO_TEST_CASE(roundtrip_to_data_factory_from_data_reader)
{
    static const filter_clear expected{};
    const auto data = expected.to_data(version::level::maximum);
    data_source istream(data);
    istream_reader source(istream);
    const auto result = filter_clear::factory_from_data(version::level::maximum, source);

    BOOST_REQUIRE_EQUAL(data.size(), 0u);
    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE_EQUAL(result.serialized_size(version::level::maximum), 0u);
}

BOOST_AUTO_TEST_SUITE_END()

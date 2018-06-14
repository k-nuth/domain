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

BOOST_AUTO_TEST_SUITE(get_blocks_tests)

BOOST_AUTO_TEST_CASE(get_blocks__constructor_1__always__invalid)
{
    message::get_blocks instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

BOOST_AUTO_TEST_CASE(get_blocks__constructor_2__always__equals_params)
{
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    };

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_blocks instance(starts, stop);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(starts == instance.start_hashes());
    BOOST_REQUIRE(stop == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__constructor_3__always__equals_params)
{
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    };
    hash_list starts_duplicate = starts;

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_blocks instance(std::move(starts_duplicate), std::move(stop));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(starts == instance.start_hashes());
    BOOST_REQUIRE(stop == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__constructor_4__always__equals_params)
{
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    };

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    const message::get_blocks expected(starts, stop);
    message::get_blocks instance(expected);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(expected == instance);
    BOOST_REQUIRE(starts == instance.start_hashes());
    BOOST_REQUIRE(stop == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__constructor_5__always__equals_params)
{
    hash_list starts = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    };

    hash_digest stop = hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b");

    message::get_blocks expected(starts, stop);
    message::get_blocks instance(std::move(expected));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(starts == instance.start_hashes());
    BOOST_REQUIRE(stop == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__from_data__insufficient_bytes__failure)
{
    const data_chunk raw{ 0xab, 0xcd };
    message::get_blocks instance;

    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::version::level::minimum, raw));
}

BOOST_AUTO_TEST_CASE(get_blocks__factory_from_data_1__valid_input__success)
{
    const message::get_blocks expected
    {
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
            hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
            hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
            hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
        },
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")
    };

    const auto data = expected.to_data(message::version::level::minimum);
    const auto result = message::get_blocks::factory_from_data(
        message::version::level::minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_blocks__factory_from_data_2__valid_input__success)
{
    const message::get_blocks expected
    {
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
            hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
            hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
            hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
        },
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")
    };

    const auto data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    const auto result = message::get_blocks::factory_from_data(
        message::version::level::minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_blocks__factory_from_data_3__valid_input__success)
{
    const message::get_blocks expected
    {
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
            hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
            hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
            hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
        },
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")
    };

    const auto data = expected.to_data(message::version::level::minimum);
    data_source istream(data);
    istream_reader source(istream);
    const auto result = message::get_blocks::factory_from_data(
        message::version::level::minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
        result.serialized_size(message::version::level::minimum));
    BOOST_REQUIRE_EQUAL(expected.serialized_size(message::version::level::minimum),
        result.serialized_size(message::version::level::minimum));
}

BOOST_AUTO_TEST_CASE(get_blocks__start_hashes_accessor_1__always__returns_initialized_value)
{
    hash_list expected = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    };

    message::get_blocks instance
    {
        expected,
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")
    };

    BOOST_REQUIRE(expected == instance.start_hashes());
}

BOOST_AUTO_TEST_CASE(get_blocks__start_hashes_accessor_2__always__returns_initialized_value)
{
    hash_list expected = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    };

    const message::get_blocks instance
    {
        expected,
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")
    };

    BOOST_REQUIRE(expected == instance.start_hashes());
}

BOOST_AUTO_TEST_CASE(get_blocks__start_hashes_setter_1__roundtrip__success)
{
    const hash_list values = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    };

    message::get_blocks instance;
    BOOST_REQUIRE(values != instance.start_hashes());
    instance.set_start_hashes(values);
    BOOST_REQUIRE(values == instance.start_hashes());
}

BOOST_AUTO_TEST_CASE(get_blocks__start_hashes_setter_2__roundtrip__success)
{
    hash_list values = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    };

    hash_list values_duplicate = values;

    message::get_blocks instance;
    BOOST_REQUIRE(values != instance.start_hashes());
    instance.set_start_hashes(std::move(values_duplicate));
    BOOST_REQUIRE(values == instance.start_hashes());
}

BOOST_AUTO_TEST_CASE(get_blocks__stop_hash_accessor_1__always__returns_initialized_value)
{
    hash_digest expected = hash_literal(
        "7777777777777777777777777777777777777777777777777777777777777777");

    message::get_blocks instance
    {
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
            hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
            hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
            hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
        },
        expected
    };

    BOOST_REQUIRE(expected == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__stop_hash_accessor_2__always__returns_initialized_value)
{
    hash_digest expected = hash_literal(
        "7777777777777777777777777777777777777777777777777777777777777777");

    const message::get_blocks instance
    {
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
            hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
            hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
            hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
        },
        expected
    };

    BOOST_REQUIRE(expected == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__stop_hash_setter_1__roundtrip__success)
{
    hash_digest value = hash_literal("7777777777777777777777777777777777777777777777777777777777777777");
    message::get_blocks instance;
    BOOST_REQUIRE(value != instance.stop_hash());
    instance.set_stop_hash(value);
    BOOST_REQUIRE(value == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__stop_hash_setter_2__roundtrip__success)
{
    hash_digest value = hash_literal("7777777777777777777777777777777777777777777777777777777777777777");
    message::get_blocks instance;
    BOOST_REQUIRE(value != instance.stop_hash());
    instance.set_stop_hash(std::move(value));
    BOOST_REQUIRE(value == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_assign_equals__always__matches_equivalent)
{
    hash_list start = {
        hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
        hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
        hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
        hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
        hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    };

    hash_digest stop = hash_literal("7777777777777777777777777777777777777777777777777777777777777777");

    message::get_blocks value{ start, stop };

    BOOST_REQUIRE(value.is_valid());

    message::get_blocks instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());

    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(start == instance.start_hashes());
    BOOST_REQUIRE(stop == instance.stop_hash());
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_boolean_equals__duplicates__returns_true)
{
    const message::get_blocks expected
    {
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
            hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
            hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
            hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
        },
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")
    };

    message::get_blocks instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_boolean_equals__differs__returns_false)
{
    const message::get_blocks expected
    {
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
            hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
            hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
            hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
        },
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")
    };

    message::get_blocks instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_boolean_not_equals__duplicates__returns_false)
{
    const message::get_blocks expected
    {
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
            hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
            hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
            hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
        },
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")
    };

    message::get_blocks instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(get_blocks__operator_boolean_not_equals__differs__returns_true)
{
    const message::get_blocks expected
    {
        {
            hash_literal("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"),
            hash_literal("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
            hash_literal("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"),
            hash_literal("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"),
            hash_literal("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
        },
        hash_literal("7777777777777777777777777777777777777777777777777777777777777777")
    };

    message::get_blocks instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

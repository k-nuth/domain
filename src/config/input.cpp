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
#include <bitcoin/bitcoin/config/input.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <boost/program_options.hpp>
#include <bitcoin/bitcoin/chain/input.hpp>
#include <bitcoin/bitcoin/chain/input_point.hpp>
#include <bitcoin/bitcoin/config/point.hpp>
#include <bitcoin/infrastructure/utility/string.hpp>

namespace libbitcoin {
namespace config {

using namespace boost::program_options;

// input is currently a private encoding in bx.
static bool decode_input(chain::input& input, const std::string& tuple)
{
    const auto tokens = split(tuple, point::delimeter);
    if (tokens.size() != 2 && tokens.size() != 3)
        return false;

    input.set_sequence(max_input_sequence);
    input.set_previous_output(point(tokens[0] + ":" + tokens[1]));

    if (tokens.size() == 3)
        input.set_sequence(deserialize<uint32_t>(tokens[2], true));

    return true;
}

// input is currently a private encoding in bx.
static std::string encode_input(const chain::input& input)
{
    std::stringstream result;
    result << point(input.previous_output()) << point::delimeter
        << input.sequence();

    return result.str();
}

input::input()
  : value_()
{
}

input::input(const std::string& tuple)
{
    std::stringstream(tuple) >> *this;
}

input::input(const chain::input& value)
  : value_(value)
{
}

input::input(const input& other)
  : input(other.value_)
{
}

input::input(const chain::input_point& value)
  : value_({value, {}, max_input_sequence})
{
}

input::operator const chain::input&() const
{
    return value_;
}

std::istream& operator>>(std::istream& stream, input& argument)
{
    std::string tuple;
    stream >> tuple;

    if (!decode_input(argument.value_, tuple))
    {
        BOOST_THROW_EXCEPTION(invalid_option_value(tuple));
    }

    return stream;
}

std::ostream& operator<<(std::ostream& output, const input& argument)
{
    output << encode_input(argument.value_);
    return output;
}

} // namespace config
} // namespace libbitcoin

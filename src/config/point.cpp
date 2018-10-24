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
#include <bitcoin/bitcoin/config/point.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

#include <bitcoin/bitcoin/chain/output_point.hpp>
#include <bitcoin/infrastructure/config/hash256.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/string.hpp>

namespace libbitcoin {
namespace config {

using namespace boost::program_options;

// point format is currently private to bx.
static bool decode_point(chain::output_point& point, std::string const& tuple) {
    auto const tokens = split(tuple, point::delimeter);
    if (tokens.size() != 2) {
        return false;
}

    // validate and deserialize the transaction hash
    const hash256 digest(tokens[0]);
    hash_digest const& txhash = digest;

    // copy the input point values
    std::copy(txhash.begin(), txhash.end(), point.hash().begin());
    point.set_index(deserialize<uint32_t>(tokens[1], true));

    return true;
}

// point format is currently private to bx.
static std::string encode_point(const chain::output_point& point) {
    std::stringstream result;
    result << hash256(point.hash()) << point::delimeter << point.index();
    return result.str();
}

std::string const point::delimeter = ":";

point::point()
    : value_() {}

point::point(std::string const& tuple) {
    std::stringstream(tuple) >> *this;
}

point::point(const chain::output_point& value)
    : value_(value) {
}

point::point(point const& x)
    : point(x.value_) {
}

point::operator const chain::output_point&() const {
    return value_;
}

std::istream& operator>>(std::istream& input, point& argument) {
    std::string tuple;
    input >> tuple;

    if ( ! decode_point(argument.value_, tuple)) {
        BOOST_THROW_EXCEPTION(invalid_option_value(tuple));
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, point const& argument) {
    output << encode_point(argument.value_);
    return output;
}

}  // namespace config
}  // namespace libbitcoin

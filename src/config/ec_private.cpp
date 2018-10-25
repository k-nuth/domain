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
#include <bitcoin/bitcoin/config/ec_private.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/define.hpp>

namespace libbitcoin {
namespace config {

// ec_secret base16 format is private to bx.
static bool decode_secret(ec_secret& secret, std::string const& encoded) {
    return decode_base16(secret, encoded) && verify(secret);
}

ec_private::ec_private(std::string const& hexcode) {
    //TODO(fernando): Eliminate std::stringstream everywhere (performance)
    std::stringstream(hexcode) >> *this;
}

ec_private::ec_private(ec_secret const& secret)
    : value_(secret) {
}

ec_private::operator ec_secret const&() const {
    return value_;
}

std::istream& operator>>(std::istream& input, ec_private& argument) {
    std::string hexcode;
    input >> hexcode;

    if ( ! decode_secret(argument.value_, hexcode)) {
        BOOST_THROW_EXCEPTION(boost::program_options::invalid_option_value(hexcode));
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, ec_private const& argument) {
    output << encode_base16(argument.value_);
    return output;
}

}  // namespace config
}  // namespace libbitcoin
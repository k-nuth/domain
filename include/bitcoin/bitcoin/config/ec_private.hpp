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
#ifndef LIBBITCOIN_CONFIG_EC_PRIVATE_HPP
#define LIBBITCOIN_CONFIG_EC_PRIVATE_HPP

#include <iostream>
#include <string>

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/define.hpp>

namespace libbitcoin {
namespace config {

/**
 * Serialization helper to convert between base16 string and ec_secret.
 */
class BC_API ec_private {
public:
    /**
     * Default constructor.
     */
    ec_private() {
    }

    /**
     * Initialization constructor.
     * @param[in]  hexcode  The value to initialize with.
     */
    ec_private(std::string const& hexcode);

    /**
     * Initialization constructor.
     * @param[in]  secret  The value to initialize with.
     */
    ec_private(ec_secret const& secret);

    /**
     * Overload cast to internal type.
     * @return  This object's value cast to internal type.
     */
    operator ec_secret const&() const;

    /**
     * Overload stream in. Throws if input is invalid.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input,
                                    ec_private& argument);

    /**
     * Overload stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
                                    const ec_private& argument);

private:
    /**
     * The state of this object.
     */
    ec_secret value_;
};

}  // namespace config
}  // namespace libbitcoin

#endif
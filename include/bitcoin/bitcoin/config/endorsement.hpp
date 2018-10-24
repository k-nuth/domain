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
#ifndef LIBBITCOIN_ENDORSEMENT_HPP
#define LIBBITCOIN_ENDORSEMENT_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <string>

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/bitcoin/define.hpp>

namespace libbitcoin {
namespace config {

/**
 * Serialization helper to convert between endorsement string and data_chunk.
 */
class BC_API endorsement {
   public:
    /**
     * Default constructor.
     */
    endorsement();

    /**
     * Initialization constructor.
     * @param[in]  hexcode  The value to initialize with.
     */
    endorsement(std::string const& hexcode);

    /**
     * Initialization constructor.
     * @param[in]  value  The value to initialize with.
     */
    endorsement(data_chunk const& value);

    /**
     * Initialization constructor.
     * @param[in]  value  The value to initialize with.
     */
    template <size_t Size>
    endorsement(const byte_array<Size>& value)
        : value_(value.begin(), value.end()) {
    }

    /**
     * Copy constructor.
     * @param[in]  other  The object to copy into self on construct.
     */
    endorsement(const endorsement& x);

    /**
     * Overload cast to internal type.
     * @return  This object's value cast to internal type.
     */
    operator data_chunk const&() const;

    /**
     * Overload cast to generic data reference.
     * @return  This object's value cast to a generic data reference.
     */
    operator data_slice() const;

    /**
     * Overload stream in. If input is invalid sets no bytes in argument.
     * @param[in]   input     The input stream to read the value from.
     * @param[out]  argument  The object to receive the read value.
     * @return                The input stream reference.
     */
    friend std::istream& operator>>(std::istream& input,
                                    endorsement& argument);

    /**
     * Overload stream out.
     * @param[in]   output    The output stream to write the value to.
     * @param[out]  argument  The object from which to obtain the value.
     * @return                The output stream reference.
     */
    friend std::ostream& operator<<(std::ostream& output,
                                    const endorsement& argument);

   private:
    /**
     * The state of this object.
     */
    data_chunk value_;
};

}  // namespace config
}  // namespace libbitcoin

#endif
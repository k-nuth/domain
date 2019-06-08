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
#include <bitcoin/bitcoin/wallet/ek_private.hpp>

#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/formats/base_58.hpp>
#include <bitcoin/infrastructure/math/checksum.hpp>

namespace libbitcoin {
namespace wallet {

ek_private::ek_private()
    : private_() 
{}

ek_private::ek_private(std::string const& encoded)
    : ek_private(from_string(encoded)) {
}

// ek_private::ek_private(ek_private const& x)
//     : valid_(x.valid_), private_(x.private_) {
// }

ek_private::ek_private(encrypted_private const& value)
    : valid_(true), private_(value) {
}

// ek_private& ek_private::operator=(ek_private const& x) {
//     valid_ = x.valid_;
//     private_ = x.private_;
//     return *this;
// }

// Factories.
// ----------------------------------------------------------------------------

ek_private ek_private::from_string(std::string const& encoded) {
    // TODO(libbitcoin): incorporate existing parser here, setting new members.

    encrypted_private key;
    return decode_base58(key, encoded) && verify_checksum(key) ? ek_private(key) : ek_private();
}

// Cast operators.
// ----------------------------------------------------------------------------

ek_private::operator bool const() const {
    return valid_;
}

ek_private::operator encrypted_private const&() const {
    return private_;
}

// Serializer.
// ----------------------------------------------------------------------------

std::string ek_private::encoded() const {
    return encode_base58(private_);
}

// Accessors.
// ----------------------------------------------------------------------------

encrypted_private const& ek_private::private_key() const {
    return private_;
}

// Operators.
// ----------------------------------------------------------------------------

bool ek_private::operator==(ek_private const& x) const {
    return valid_ == x.valid_ && private_ == x.private_;
}

bool ek_private::operator!=(ek_private const& x) const {
    return !(*this == x);
}

bool ek_private::operator<(ek_private const& x) const {
    return encoded() < x.encoded();
}

std::istream& operator>>(std::istream& in, ek_private& to) {
    std::string value;
    in >> value;
    to = ek_private(value);

    if ( ! to) {
        using namespace boost::program_options;
        BOOST_THROW_EXCEPTION(invalid_option_value(value));
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, ek_private const& of) {
    out << of.encoded();
    return out;
}

}  // namespace wallet
}  // namespace libbitcoin

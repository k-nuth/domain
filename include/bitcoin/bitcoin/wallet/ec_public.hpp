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
#ifndef LIBBITCOIN_EC_PUBLIC_HPP
#define LIBBITCOIN_EC_PUBLIC_HPP

#include <cstdint>
#include <iostream>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>

namespace libbitcoin {
namespace wallet {

class ec_private;
class payment_address;

/// Use to pass an ec point as either ec_compressed or ec_uncompressed.
/// ec_public doesn't carry a version for address creation or base58 encoding.
class BC_API ec_public {
public:
    static uint8_t const compressed_even;
    static uint8_t const compressed_odd;
    static uint8_t const uncompressed;
    static uint8_t const mainnet_p2kh;

    /// Constructors.
    ec_public();
    ec_public(ec_public const& x);
    ec_public(ec_private const& secret);
    ec_public(data_chunk const& decoded);
    ec_public(std::string const& base16);
    ec_public(ec_compressed const& point, bool compress = true);
    ec_public(ec_uncompressed const& point, bool compress = false);

    ec_public& operator=(ec_public const& x) = default;

    /// Operators.
    bool operator==(ec_public const& x) const;
    bool operator!=(ec_public const& x) const;
    bool operator<(ec_public const& x) const;

    friend std::istream& operator>>(std::istream& in, ec_public& to);
    friend std::ostream& operator<<(std::ostream& out, ec_public const& of);

    /// Cast operators.
    operator bool const() const;
    operator ec_compressed const&() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    ec_compressed const& point() const;
    uint16_t const version() const;
    uint8_t const payment_version() const;
    uint8_t const wif_version() const;
    bool const compressed() const;

    /// Methods.
    bool to_data(data_chunk& out) const;
    bool to_uncompressed(ec_uncompressed& out) const;
    payment_address to_payment_address(uint8_t version = mainnet_p2kh) const;

private:
    /// Validators.
    static bool is_point(data_slice decoded);

    /// Factories.
    static ec_public from_data(data_chunk const& decoded);
    static ec_public from_private(ec_private const& secret);
    static ec_public from_string(std::string const& base16);
    static ec_public from_point(ec_uncompressed const& point, bool compress);

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_{false};
    bool compress_{true};
    uint8_t version_;
    ec_compressed point_;
};

}  // namespace wallet
}  // namespace libbitcoin

#endif

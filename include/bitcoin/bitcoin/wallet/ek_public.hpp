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
#ifndef LIBBITCOIN_WALLET_EK_PUBLIC_HPP
#define LIBBITCOIN_WALLET_EK_PUBLIC_HPP

#include <iostream>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/wallet/encrypted_keys.hpp>

namespace libbitcoin {
namespace wallet {

/// Use to pass an encrypted public key.
class BC_API ek_public {
public:
    /// Constructors.
    ek_public();
    ek_public(std::string const& encoded);
    ek_public(encrypted_public const& value);
    ek_public(ek_public const& x) = default;

    ek_public& operator=(ek_public const& x) = default;

    /// Operators.

    bool operator==(ek_public const& x) const;
    bool operator!=(ek_public const& x) const;
    bool operator<(ek_public const& x) const;
    friend std::istream& operator>>(std::istream& in, ek_public& to);
    friend std::ostream& operator<<(std::ostream& out, ek_public const& of);

    /// Cast operators.
    operator bool const() const;
    operator encrypted_public const&() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    encrypted_public const& public_key() const;

private:
    /// Factories.
    static ek_public from_string(std::string const& encoded);

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_{false};
    encrypted_public public_;
};

}  // namespace wallet
}  // namespace libbitcoin

#endif

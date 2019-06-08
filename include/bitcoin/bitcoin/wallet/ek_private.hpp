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
#ifndef LIBBITCOIN_WALLET_EK_PRIVATE_HPP
#define LIBBITCOIN_WALLET_EK_PRIVATE_HPP

#include <iostream>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/wallet/encrypted_keys.hpp>

namespace libbitcoin {
namespace wallet {

/// Use to pass an encrypted private key.
class BC_API ek_private {
public:
    /// Constructors.
    ek_private();
    ek_private(std::string const& encoded);
    ek_private(encrypted_private const& value);
    ek_private(ek_private const& x) = default;

    ek_private& operator=(ek_private const& x) = default;

    /// Operators.
    bool operator==(ek_private const& x) const;
    bool operator!=(ek_private const& x) const;
    bool operator<(ek_private const& x) const;

    friend std::istream& operator>>(std::istream& in, ek_private& to);
    friend std::ostream& operator<<(std::ostream& out, ek_private const& of);

    /// Cast operators.
    operator bool() const;
    operator encrypted_private const&() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    encrypted_private const& private_key() const;

private:
    /// Factories.
    static ek_private from_string(std::string const& encoded);

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_{false};
    encrypted_private private_;
};

}  // namespace wallet
}  // namespace libbitcoin

#endif

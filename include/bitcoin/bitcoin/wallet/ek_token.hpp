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
#ifndef LIBBITCOIN_WALLET_EK_TOKEN_HPP
#define LIBBITCOIN_WALLET_EK_TOKEN_HPP

#include <iostream>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/wallet/encrypted_keys.hpp>

namespace libbitcoin {
namespace wallet {

/**
 * Serialization helper to convert between base58 string and bip38 token.
 */
class BC_API ek_token {
public:
    /// Constructors.
    ek_token();
    ek_token(std::string const& encoded);
    ek_token(encrypted_token const& value);
    ek_token(ek_token const& x) = default;


    ek_token& operator=(ek_token const& x) = default;

    /// Operators.
    bool operator==(ek_token const& x) const;
    bool operator!=(ek_token const& x) const;
    bool operator<(ek_token const& x) const;

    friend std::istream& operator>>(std::istream& in, ek_token& to);
    friend std::ostream& operator<<(std::ostream& out, ek_token const& of);

    /// Cast operators.
    operator bool() const;
    operator encrypted_token const&() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    encrypted_token const& token() const;

private:
    /// Factories.
    static ek_token from_string(std::string const& encoded);

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_{false};
    encrypted_token token_;
};

}  // namespace wallet
}  // namespace libbitcoin

#endif

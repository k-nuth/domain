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
#ifndef LIBBITCOIN_WALLET_BITCOIN_URI_HPP
#define LIBBITCOIN_WALLET_BITCOIN_URI_HPP

#include <cstdint>
#include <iostream>
#include <map>
#include <string>

#include <boost/optional.hpp>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/wallet/payment_address.hpp>
#include <bitcoin/bitcoin/wallet/stealth_address.hpp>
// #include <bitcoin/bitcoin/wallet/uri_reader.hpp>

namespace libbitcoin {
namespace wallet {

/// A bitcoin URI corresponding to BIP 21 and BIP 72.
/// The object is not constant, setters can change state after construction.
// class BC_API bitcoin_uri : public uri_reader {
class BC_API bitcoin_uri {
public:
    /// Constructors.
    bitcoin_uri() = default;
    bitcoin_uri(std::string const& uri, bool strict = true);

    bitcoin_uri(bitcoin_uri const& x) = default;
    bitcoin_uri& operator=(bitcoin_uri const& x) = default;

    /// Operators.
    bool operator==(bitcoin_uri const& x) const;
    bool operator!=(bitcoin_uri const& x) const;

    bool operator<(bitcoin_uri const& x) const;

    friend std::istream& operator>>(std::istream& in, bitcoin_uri& to);
    friend std::ostream& operator<<(std::ostream& out, bitcoin_uri const& from);

    /// Test whether the URI has been initialized.
    operator const bool() const;

    /// Get the serialized URI representation.
    std::string encoded() const;

    /// Property getters.
    uint64_t amount() const;
    std::string label() const;
    std::string message() const;
    std::string r() const;
    std::string address() const;
    payment_address payment() const;
    stealth_address stealth() const;
    std::string parameter(std::string const& key) const;

    /// Property setters.
    void set_amount(uint64_t satoshis);
    void set_label(std::string const& label);
    void set_message(std::string const& message);
    void set_r(std::string const& r);
    bool set_address(std::string const& address);
    void set_address(payment_address const& payment);
    void set_address(stealth_address const& stealth);

    /// uri_reader implementation.
    void set_strict(bool strict);
    bool set_scheme(std::string const& scheme);
    bool set_authority(std::string const& authority);
    bool set_path(std::string const& path);
    bool set_fragment(std::string const& fragment);
    bool set_parameter(std::string const& key, std::string const& value);

private:
    /// Private helpers.
    bool set_amount(std::string const& satoshis);

    /// Member state.
    bool strict_{true};
    std::string scheme_;
    std::string address_;
    std::map<std::string, std::string> query_;
};

}  // namespace wallet
}  // namespace libbitcoin

#endif

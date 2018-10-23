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
#include <bitcoin/bitcoin/wallet/bitcoin_uri.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <boost/program_options.hpp>
#include <bitcoin/infrastructure/formats/base_10.hpp>
#include <bitcoin/bitcoin/wallet/payment_address.hpp>
#include <bitcoin/bitcoin/wallet/stealth_address.hpp>
#include <bitcoin/infrastructure/wallet/uri.hpp>
#include <bitcoin/bitcoin/wallet/uri_reader.hpp>

namespace libbitcoin {
namespace wallet {

static auto const bitcoin_scheme = "bitcoin";
static auto const parameter_amount = "amount";
static auto const parameter_label = "label";
static auto const parameter_message = "message";
static auto const parameter_r = "r";
static auto const parameter_req_ = "req-";
static constexpr size_t parameter_req_length = 4;

bitcoin_uri::bitcoin_uri()
  : strict_(true)
{
}

bitcoin_uri::bitcoin_uri(const bitcoin_uri& other)
  : strict_(other.strict_), scheme_(other.scheme_), address_(other.address_),
    query_(other.query_)
{
}

bitcoin_uri::bitcoin_uri(std::string const& uri, bool strict)
  : bitcoin_uri(uri_reader::parse<bitcoin_uri>(uri, strict))
{
}

// Cast operators.
// ----------------------------------------------------------------------------

bitcoin_uri::operator const bool() const
{
    // An uninitialized URI returns false.
    return !address_.empty() || !query_.empty() || !scheme_.empty();
}

// Serializer.
// ----------------------------------------------------------------------------

std::string bitcoin_uri::encoded() const
{
    // Bitcoin URIs don't use the authority or fragment components.
    uri out;
    out.set_scheme(bitcoin_scheme);
    out.set_path(address_);
    out.encode_query(query_);
    return out.encoded();
}

// Property getters.
// ----------------------------------------------------------------------------

uint64_t bitcoin_uri::amount() const
{
    uint64_t satoshis;
    decode_base10(satoshis, parameter(parameter_amount), btc_decimal_places);
    return satoshis;
}

std::string bitcoin_uri::label() const
{
    return parameter(parameter_label);
}

std::string bitcoin_uri::message() const
{
    return parameter(parameter_message);
}

std::string bitcoin_uri::r() const
{
    return parameter(parameter_r);
}

payment_address bitcoin_uri::payment() const
{
    return payment_address(address_);
}

stealth_address bitcoin_uri::stealth() const
{
    return stealth_address(address_);
}

std::string bitcoin_uri::parameter(std::string const& key) const
{
    auto const value = query_.find(key);
    return value == query_.end() ? std::string() : value->second;
}

std::string bitcoin_uri::address() const
{
    return address_;
}

// Property setters.
// ----------------------------------------------------------------------------

void bitcoin_uri::set_amount(uint64_t satoshis)
{
    auto const amount = encode_base10(satoshis, btc_decimal_places);
    query_[parameter_amount] = amount;
}

void bitcoin_uri::set_label(std::string const& label)
{
    query_[parameter_label] = label;
}

void bitcoin_uri::set_message(std::string const& message)
{
    query_[parameter_message] = message;
}

void bitcoin_uri::set_r(std::string const& r)
{
    query_[parameter_r] = r;
}

bool bitcoin_uri::set_address(std::string const& address)
{
    payment_address payment(address);
    if (payment)
    {
        address_ = address;
        return true;
    }

    stealth_address stealth(address);
    if (stealth)
    {
        address_ = address;
        return true;
    }

    return false;
}

void bitcoin_uri::set_address(const payment_address& payment)
{
    address_ = payment.encoded();
}

void bitcoin_uri::set_address(const stealth_address& stealth)
{
    address_ = stealth.encoded();
}

bool bitcoin_uri::set_amount(std::string const& satoshis)
{
    uint64_t decoded;
    if ( ! decode_base10(decoded, satoshis, btc_decimal_places, strict_))
        return false;

    // Normalize the encoding for string-based getter (parameter).
    set_amount(decoded);
    return true;
}

// uri_reader implementation.
// ----------------------------------------------------------------------------

void bitcoin_uri::set_strict(bool strict)
{
    strict_ = strict;
}

bool bitcoin_uri::set_scheme(std::string const& scheme)
{
    if (scheme == bitcoin_scheme)
    {
        scheme_ = scheme;
        return true;
    }

    return false;
}

bool bitcoin_uri::set_authority(std::string const& authority)
{
    // Using "bitcoin://" instead of "bitcoin:" is a common mistake, so we
    // allow the authority in place of the path when not strict.
    return !strict_ && set_path(authority);
}

bool bitcoin_uri::set_path(std::string const& path)
{
    // Guard against the path having been set via authority (or second set).
    return address_.empty() && set_address(path);
}

bool bitcoin_uri::set_fragment(std::string const& fragment)
{
    return false;
}

bool bitcoin_uri::set_parameter(std::string const& key,
    std::string const& value)
{
    auto const required = [](std::string const& key)
    {
        return key.substr(0, parameter_req_length) == parameter_req_;
    };

    if (key == parameter_amount)
        return set_amount(value);

    if (key == parameter_label)
        set_label(value);
    else if (key == parameter_message)
        set_message(value);
    else if (key == parameter_r)
        set_r(value);

    // Fail on any required parameter that we don't support.
    return !required(key);
}

// Operators.
// ----------------------------------------------------------------------------

bitcoin_uri& bitcoin_uri::operator=(const bitcoin_uri& other)
{
    strict_ = other.strict_;
    scheme_ = other.scheme_;
    address_ = other.address_;
    query_ = other.query_;
    return *this;
}

bool bitcoin_uri::operator<(const bitcoin_uri& other) const
{
    return encoded() < other.encoded();
}

bool bitcoin_uri::operator==(const bitcoin_uri& other) const
{
    return strict_ == other.strict_ && scheme_ == other.scheme_ &&
        address_ == other.address_ && query_ == other.query_;
}

bool bitcoin_uri::operator!=(const bitcoin_uri& other) const
{
    return !(*this == other);
}

// This is always strict.
std::istream& operator>>(std::istream& in, bitcoin_uri& to)
{
    std::string value;
    in >> value;
    to = bitcoin_uri(value);

    if ( ! to)
    {
        using namespace boost::program_options;
        BOOST_THROW_EXCEPTION(invalid_option_value(value));
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const bitcoin_uri& from)
{
    out << from.encoded();
    return out;
}

} // namespace wallet
} // namespace libbitcoin

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
#ifndef LIBBITCOIN_WALLET_PAYMENT_ADDRESS_HPP
#define LIBBITCOIN_WALLET_PAYMENT_ADDRESS_HPP

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/infrastructure/compat.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/math/checksum.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/bitcoin/wallet/ec_private.hpp>
#include <bitcoin/bitcoin/wallet/ec_public.hpp>

namespace libbitcoin {
namespace wallet {

static BC_CONSTEXPR size_t payment_size = 1u + short_hash_size + checksum_size; // 1 + 20 + sizeof(uint32_t) = 1 + 20 + 4 = 25
typedef byte_array<payment_size> payment;

/// A class for working with non-stealth payment addresses.
class BC_API payment_address
{
public:
    static const uint8_t mainnet_p2kh;
    static const uint8_t mainnet_p2sh;

    static const uint8_t testnet_p2kh;
    static const uint8_t testnet_p2sh;

#ifdef BITPRIM_CURRENCY_BCH
    static std::string const cashaddr_prefix_mainnet;
    static std::string const cashaddr_prefix_testnet;
#endif

    typedef std::vector<payment_address> list;
    typedef std::shared_ptr<payment_address> ptr;

    /// Extract a payment address list from an input or output script.
    static list extract(chain::script const& script,
        uint8_t p2kh_version=mainnet_p2kh, uint8_t p2sh_version=mainnet_p2sh);
    static list extract_input(chain::script const& script,
        uint8_t p2kh_version=mainnet_p2kh, uint8_t p2sh_version=mainnet_p2sh);
    static list extract_output(chain::script const& script,
        uint8_t p2kh_version=mainnet_p2kh, uint8_t p2sh_version=mainnet_p2sh);

    /// Constructors.
    payment_address();
    payment_address(payment_address&& x);
    payment_address(const payment_address& x);
    payment_address(const payment& decoded);
    payment_address(const ec_private& secret);
    payment_address(std::string const& address);
    payment_address(short_hash&& hash, uint8_t version=mainnet_p2kh);
    payment_address(short_hash const& hash, uint8_t version=mainnet_p2kh);
    payment_address(const ec_public& point, uint8_t version=mainnet_p2kh);
    payment_address(chain::script const& script, uint8_t version=mainnet_p2sh);

    /// Operators.
    bool operator<(const payment_address& x) const;
    bool operator==(const payment_address& x) const;
    bool operator!=(const payment_address& x) const;
    payment_address& operator=(const payment_address& x);
    friend std::istream& operator>>(std::istream& in, payment_address& to);
    friend std::ostream& operator<<(std::ostream& out,
        const payment_address& of);

    /// Cast operators.
    operator const bool() const;
    operator short_hash const&() const;

    /// Serializer.
    std::string encoded() const;

#ifdef BITPRIM_CURRENCY_BCH
    std::string encoded_cashaddr() const;
#endif //BITPRIM_CURRENCY_BCH
    
    /// Accessors.
    uint8_t version() const;
    short_hash const& hash() const;

    /// Methods.
    payment to_payment() const;

private:
    /// Validators.
    static bool is_address(data_slice decoded);

    /// Factories.
    static payment_address from_string(std::string const& address);

#ifdef BITPRIM_CURRENCY_BCH
    static payment_address from_string_cashaddr(std::string const& address);
#endif //BITPRIM_CURRENCY_BCH
    
    static payment_address from_payment(const payment& decoded);
    static payment_address from_private(const ec_private& secret);
    static payment_address from_public(const ec_public& point, uint8_t version);
    static payment_address from_script(chain::script const& script,
        uint8_t version);

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_;
    uint8_t version_;
    short_hash hash_;
};

/// The pre-encoded structure of a payment address or other similar data.
struct BC_API wrapped_data
{
    uint8_t version;
    data_chunk payload;
    uint32_t checksum;
};

} // namespace wallet
} // namespace libbitcoin

// Allow payment_address to be in indexed in std::*map classes.
namespace std
{
template <>
struct hash<bc::wallet::payment_address>
{
    size_t operator()(const bc::wallet::payment_address& address) const
    {
        return std::hash<bc::short_hash>()(address.hash());
    }
};

} // namespace std

#endif
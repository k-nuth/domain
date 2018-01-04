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
#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/compat.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/math/checksum.hpp>
#include <bitcoin/bitcoin/math/elliptic_curve.hpp>
#include <bitcoin/bitcoin/math/hash.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
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

    typedef std::shared_ptr<payment_address> ptr;

    /// Extract a payment address from an input or output script.
    /// The address will be invalid if and only if the script type is not
    /// supported or the script is itself invalid.
    static payment_address extract(const chain::script& script,
        uint8_t p2kh_version=mainnet_p2kh, uint8_t p2sh_version=mainnet_p2sh);

    /// Constructors.
    payment_address();
    payment_address(const payment& decoded);
    payment_address(const ec_private& secret);
    payment_address(const std::string& address);
    payment_address(const payment_address& other);
    payment_address(const short_hash& hash, uint8_t version=mainnet_p2kh);
    payment_address(const ec_public& point, uint8_t version=mainnet_p2kh);
    payment_address(const chain::script& script, uint8_t version=mainnet_p2sh);

    /// Operators.
    bool operator<(const payment_address& other) const;
    bool operator==(const payment_address& other) const;
    bool operator!=(const payment_address& other) const;
    payment_address& operator=(const payment_address& other);
    friend std::istream& operator>>(std::istream& in, payment_address& to);
    friend std::ostream& operator<<(std::ostream& out,
        const payment_address& of);

    /// Cast operators.
    operator const bool() const;
    operator const short_hash&() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    uint8_t version() const;
    const short_hash& hash() const;

    /// Methods.
    payment to_payment() const;

private:
    /// Validators.
    static bool is_address(data_slice decoded);

    /// Factories.
    static payment_address from_string(const std::string& address);
    static payment_address from_string_cashaddr(std::string const& address);
    static payment_address from_payment(const payment& decoded);
    static payment_address from_private(const ec_private& secret);
    static payment_address from_public(const ec_public& point, uint8_t version);
    static payment_address from_script(const chain::script& script,
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


// Unit Tests ----------------------------------------------------
#ifdef DOCTEST_LIBRARY_INCLUDED
using namespace libbitcoin::wallet;
using namespace std;


TEST_CASE("[cashaddr_testvectors_valid] cashaddr_testvectors_valid") {
    static const std::string CASES[] = {
        "prefix:x64nx6hz",
        "PREFIX:X64NX6HZ",
        "p:gpf8m4h7",
        "bitcoincash:qpzry9x8gf2tvdw0s3jn54khce6mua7lcw20ayyn",
        "bchtest:testnetaddress4d6njnut",
        "bchreg:555555555555555555555555555555555555555555555udxmlmrz",
    };

    for (std::string const& str : CASES) {
        auto ret = cashdddr_decode(str);
        CHECK( ! ret.first.empty());
        std::string recode = cashaddr::encode(ret.first, ret.second);
        CHECK( ! recode.empty());
        CHECK(case_insensitive_equal(str, recode));
    }
}

TEST_CASE("[cashaddr_testvectors_invalid] cashaddr_testvectors_invalid") {
    static const std::string CASES[] = {
        "prefix:x32nx6hz",
        "prEfix:x64nx6hz",
        "prefix:x64nx6Hz",
        "pref1x:6m8cxv73",
        "prefix:",
        ":u9wsx07j",
        "bchreg:555555555555555555x55555555555555555555555555udxmlmrz",
        "bchreg:555555555555555555555555555555551555555555555udxmlmrz",
        "pre:fix:x32nx6hz",
        "prefixx64nx6hz",
    };

    for (std::string const& str : CASES) {
        auto ret = cashdddr_decode(str);
        CHECK(ret.first.empty());
    }
}

TEST_CASE("[cashaddr_rawencode] cashaddr_rawencode") {
    using raw = std::pair<std::string, std::vector<uint8_t>>;

    raw toEncode;
    toEncode.first = "helloworld";
    toEncode.second = {0x1f, 0x0d};

    std::string encoded = cashaddr::encode(toEncode.first, toEncode.second);
    raw decoded = cashdddr_decode(encoded);

    CHECK(toEncode.first == decoded.first);

    // BOOST_CHECK_EQUAL_COLLECTIONS(begin(toEncode.second), end(toEncode.second),
    //                               begin(decoded.second), end(decoded.second));

    CHECK(std::equal(begin(toEncode.second), end(toEncode.second),
                     begin(decoded.second), end(decoded.second)));

}

TEST_CASE("[cashaddr_testvectors_noprefix] cashaddr_testvectors_noprefix") {
    static const std::pair<std::string, std::string> CASES[] = {
        {"bitcoincash", "qpzry9x8gf2tvdw0s3jn54khce6mua7lcw20ayyn"},
        {"prefix", "x64nx6hz"},
        {"PREFIX", "X64NX6HZ"},
        {"p", "gpf8m4h7"},
        {"bitcoincash", "qpzry9x8gf2tvdw0s3jn54khce6mua7lcw20ayyn"},
        {"bchtest", "testnetaddress4d6njnut"},
        {"bchreg", "555555555555555555555555555555555555555555555udxmlmrz"},
    };

    for (std::pair<std::string, std::string> const& c : CASES) {
        std::string prefix = c.first;
        std::string payload = c.second;
        std::string addr = prefix + ":" + payload;
        auto ret = cashaddr::decode(payload, prefix);

        CHECK(case_insensitive_equal(ret.first, prefix));
        std::string recode = cashaddr::encode(ret.first, ret.second);
        CHECK( ! recode.empty());
        CHECK(case_insensitive_equal(addr, recode));
    }
}
#endif /*DOCTEST_LIBRARY_INCLUDED*/
// Unit Tests ----------------------------------------------------

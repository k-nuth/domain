// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_WALLET_PAYMENT_ADDRESS_HPP
#define KTH_WALLET_PAYMENT_ADDRESS_HPP

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/wallet/ec_private.hpp>
#include <bitcoin/bitcoin/wallet/ec_public.hpp>
#include <bitcoin/infrastructure/compat.hpp>
#include <bitcoin/infrastructure/math/checksum.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>

namespace libbitcoin {
namespace wallet {

static constexpr size_t payment_size = 1u + short_hash_size + checksum_size;  // 1 + 20 + sizeof(uint32_t) = 1 + 20 + 4 = 25
using payment = byte_array<payment_size>;

/// A class for working with non-stealth payment addresses.
class BC_API payment_address {
public:
    static uint8_t const mainnet_p2kh;
    static uint8_t const mainnet_p2sh;

    static uint8_t const testnet_p2kh;
    static uint8_t const testnet_p2sh;

#ifdef KTH_CURRENCY_BCH
    static std::string const cashaddr_prefix_mainnet;
    static std::string const cashaddr_prefix_testnet;
#endif

    using list = std::vector<payment_address>;
    using ptr = std::shared_ptr<payment_address>;

    /// Extract a payment address list from an input or output script.
    static list extract(chain::script const& script, uint8_t p2kh_version = mainnet_p2kh, uint8_t p2sh_version = mainnet_p2sh);
    static list extract_input(chain::script const& script, uint8_t p2kh_version = mainnet_p2kh, uint8_t p2sh_version = mainnet_p2sh);
    static list extract_output(chain::script const& script, uint8_t p2kh_version = mainnet_p2kh, uint8_t p2sh_version = mainnet_p2sh);

    /// Constructors.
    payment_address();
    payment_address(payment_address const& x) = default;
    payment_address(payment_address&& x) noexcept;
    
    payment_address(payment const& decoded);
    payment_address(ec_private const& secret);
    payment_address(std::string const& address);
    payment_address(short_hash const& hash, uint8_t version = mainnet_p2kh);
    payment_address(ec_public const& point, uint8_t version = mainnet_p2kh);
    payment_address(chain::script const& script, uint8_t version = mainnet_p2sh);

    payment_address& operator=(payment_address const& x) = default;

    /// Operators.
    bool operator==(payment_address const& x) const;
    bool operator!=(payment_address const& x) const;
    bool operator<(payment_address const& x) const;

    friend std::istream& operator>>(std::istream& in, payment_address& to);
    friend std::ostream& operator<<(std::ostream& out, payment_address const& of);

    /// Cast operators.
    operator bool() const;
    operator short_hash const&() const;

    /// Serializer.
    std::string encoded() const;

#ifdef KTH_CURRENCY_BCH
    std::string encoded_cashaddr() const;
#endif  //KTH_CURRENCY_BCH

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

#ifdef KTH_CURRENCY_BCH
    static payment_address from_string_cashaddr(std::string const& address);
#endif  //KTH_CURRENCY_BCH

    static payment_address from_payment(payment const& decoded);
    static payment_address from_private(ec_private const& secret);
    static payment_address from_public(ec_public const& point, uint8_t version);
    static payment_address from_script(chain::script const& script,
                                       uint8_t version);

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_{false};
    uint8_t version_{0};
    short_hash hash_{null_short_hash};
};

/// The pre-encoded structure of a payment address or other similar data.
struct BC_API wrapped_data {
    uint8_t version;
    data_chunk payload;
    uint32_t checksum;
};

}  // namespace wallet
}  // namespace kth

// Allow payment_address to be in indexed in std::*map classes.
namespace std {
template <>
struct hash<bc::wallet::payment_address> {
    size_t operator()(const bc::wallet::payment_address& address) const {
        return std::hash<bc::short_hash>()(address.hash());
    }
};

}  // namespace std

#endif
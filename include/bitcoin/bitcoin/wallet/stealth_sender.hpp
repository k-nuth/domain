// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_WALLET_STEALTH_SENDER_HPP
#define KTH_WALLET_STEALTH_SENDER_HPP

#include <cstdint>

#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/wallet/payment_address.hpp>
#include <bitcoin/bitcoin/wallet/stealth_address.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
#include <bitcoin/infrastructure/utility/binary.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>

namespace libbitcoin {
namespace wallet {

/// This class does not support multisignature stealth addresses.
class BC_API stealth_sender {
public:
    /// Constructors.
    /// Generate a send address from the stealth address.
    stealth_sender(stealth_address const& address, data_chunk const& seed, binary const& filter, uint8_t version = payment_address::mainnet_p2kh);

    /// Generate a send address from the stealth address.
    stealth_sender(ec_secret const& ephemeral_private,
                   stealth_address const& address,
                   data_chunk const& seed,
                   binary const& filter,
                   uint8_t version = payment_address::mainnet_p2kh);

    /// Caller must test after construct.
    operator bool() const;

    /// Attach this script to the output before the send output.
    chain::script const& stealth_script() const;

    /// The bitcoin payment address to which the payment will be made.
    const wallet::payment_address& payment_address() const;

private:
    void initialize(ec_secret const& ephemeral_private,
                    stealth_address const& address,
                    data_chunk const& seed,
                    binary const& filter);

    uint8_t const version_;
    chain::script script_;
    wallet::payment_address address_;
};

}  // namespace wallet
}  // namespace kth

#endif

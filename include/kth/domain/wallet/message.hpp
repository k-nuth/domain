// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_WALLET_MESSAGE_HPP
#define KTH_WALLET_MESSAGE_HPP

#include <string>

#include <kth/domain/define.hpp>
#include <kth/domain/wallet/payment_address.hpp>
#include <kth/infrastructure/compat.hpp>
#include <kth/infrastructure/math/elliptic_curve.hpp>
#include <kth/infrastructure/math/hash.hpp>

namespace kth::wallet {

/**
 * A message signature is an EC signature with one prefix byte.
 */
static constexpr 
size_t message_signature_size = 1 + ec_signature_size;

using message_signature = byte_array<message_signature_size>;

/**
 * Hashes a messages in preparation for signing.
 */
BC_API hash_digest hash_message(data_slice message);

/**
 * Signs a message using deterministic signature.
 * @param[in] out_signature The signature in Bitcoin's own format.
 * This should be base64 encoded for presentation to the user.
 * @return true if wif is valid and signature encoding is successful.
 */
BC_API bool sign_message(message_signature& out_signature, data_slice message, ec_private const& secret);

/**
 * Signs a message using deterministic signature.
 * @param[in] out_signature The signature in Bitcoin's own format.
 * This should be base64 encoded for presentation to the user.
 * @return true if wif is valid and signature encoding is successful.
 */
BC_API bool sign_message(message_signature& out_signature, data_slice message, std::string const& wif);

/**
 * Signs a message using deterministic signature.
 * @param[in] out_signature The signature in Bitcoin's own format.
 * This should be base64 encoded for presentation to the user.
 * @param[in] compressed true if the bitcoin address derived from the
 * private key is in compressed format.
 * @return true if signature encoding is successful.
 */
BC_API bool sign_message(message_signature& out_signature, data_slice message, ec_secret const& secret, bool compressed = true);

/**
 * Verifies a message.
 * @param signature a message signature in Bitcoin's own format.
 * The user will generally provide this as a base64 string,
 * which the user interface must decode.
 * @return false if the signature does not match the address or if there are
 * any errors in the signature encoding.
 */
BC_API bool verify_message(data_slice message, payment_address const& address, const message_signature& signature);

/// Exposed primarily for independent testability.
BC_API bool recovery_id_to_magic(uint8_t& out_magic, uint8_t recovery_id, bool compressed);

/// Exposed primarily for independent testability.
BC_API bool magic_to_recovery_id(uint8_t& out_recovery_id, bool& out_compressed, uint8_t magic);

}  // namespace wallet
}  // namespace kth

#endif

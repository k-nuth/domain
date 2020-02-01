// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MATH_STEALTH_HPP
#define KTH_MATH_STEALTH_HPP

#include <cstdint>

#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/infrastructure/compat.hpp>
#include <bitcoin/infrastructure/define.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
#include <bitcoin/infrastructure/utility/binary.hpp>

namespace libbitcoin {

/// Determine if the script is a null-data script of at least 32 data bytes.
BI_API bool is_stealth_script(chain::script const& script);

/// Convert a stealth info script to a prefix usable for stealth.
BI_API bool to_stealth_prefix(uint32_t& out_prefix,
                              chain::script const& script);

/// Create a valid stealth ephemeral private key from the provided seed.
BI_API bool create_ephemeral_key(ec_secret& out_secret,
                                 data_chunk const& seed);

/// Create a stealth null data script the specified filter prefix.
/// Create an ephemeral secret key generated from the seed.
BI_API bool create_stealth_data(chain::script& out_null_data,
                                ec_secret& out_secret,
                                binary const& filter,
                                data_chunk const& seed);

/// Create a stealth null data script the specified filter prefix.
/// Use the ephemeral secret key provided by parameter.
BI_API bool create_stealth_script(chain::script& out_null_data,
                                  ec_secret const& secret,
                                  binary const& filter,
                                  data_chunk const& seed);

/// Extract the stealth ephemeral public key from an output script.
BI_API bool extract_ephemeral_key(ec_compressed& out_ephemeral_public_key,
                                  chain::script const& script);

/// Extract the unsigned stealth ephemeral public key from an output script.
BI_API bool extract_ephemeral_key(hash_digest& out_unsigned_ephemeral_key,
                                  chain::script const& script);

/// Calculate the shared secret.
BI_API bool shared_secret(ec_secret& out_shared, ec_secret const& secret, ec_compressed const& point);

/// Uncover the stealth public key.
BI_API bool uncover_stealth(ec_compressed& out_stealth,
                            ec_compressed const& ephemeral_or_scan,
                            ec_secret const& scan_or_ephemeral,
                            ec_compressed const& spend);

/// Uncover the stealth secret.
BI_API bool uncover_stealth(ec_secret& out_stealth,
                            ec_compressed const& ephemeral_or_scan,
                            ec_secret const& scan_or_ephemeral,
                            ec_secret const& spend);

}  // namespace kth

#endif  //KTH_MATH_STEALTH_HPP

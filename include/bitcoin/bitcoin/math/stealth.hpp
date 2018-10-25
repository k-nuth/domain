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
#ifndef LIBBITCOIN_MATH_STEALTH_HPP
#define LIBBITCOIN_MATH_STEALTH_HPP

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

}  // namespace libbitcoin

#endif  //LIBBITCOIN_MATH_STEALTH_HPP

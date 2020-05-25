// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_PARSE_ENCRYPTED_KEY_IPP
#define KTH_PARSE_ENCRYPTED_KEY_IPP

#include <cstddef>
#include <cstdint>

#include <kth/domain/wallet/encrypted_keys.hpp>
#include <kth/infrastructure/utility/data.hpp>

#include "parse_encrypted_prefix.hpp"

namespace kth::domain::wallet {

template <size_t PrefixSize>
parse_encrypted_key<PrefixSize>::parse_encrypted_key(
    const byte_array<PrefixSize>& prefix,
    const one_byte& flags,
    const ek_salt& salt,
    const ek_entropy& entropy)
    : parse_encrypted_prefix<PrefixSize>(prefix),
      flags_(flags),
      salt_(salt),
      entropy_(entropy) {
}

template <size_t PrefixSize>
bool parse_encrypted_key<PrefixSize>::compressed() const {
    return (flags() & ek_flag::ec_compressed_key) != 0;
}

template <size_t PrefixSize>
ek_entropy parse_encrypted_key<PrefixSize>::entropy() const {
    // The owner salt + lot-sequence or owner entropy.
    return entropy_;
}

template <size_t PrefixSize>
uint8_t parse_encrypted_key<PrefixSize>::flags() const {
    return flags_.front();
}

template <size_t PrefixSize>
bool parse_encrypted_key<PrefixSize>::lot_sequence() const {
    return (flags() & ek_flag::lot_sequence_key) != 0;
}

template <size_t PrefixSize>
data_chunk parse_encrypted_key<PrefixSize>::owner_salt() const {
    // Either 4 or 8 bytes, depending on the lot sequence flags.
    if (lot_sequence())
        return to_chunk(slice<0, ek_salt_size>(entropy()));
    else
        return to_chunk(entropy());
}

template <size_t PrefixSize>
ek_salt parse_encrypted_key<PrefixSize>::salt() const {
    // The address hash salt.
    return salt_;
}

}  // namespace wallet
}  // namespace kth

#endif

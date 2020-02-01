// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_PARSE_ENCRYPTED_TOKEN_HPP
#define KTH_PARSE_ENCRYPTED_TOKEN_HPP

#include <cstddef>
#include <cstdint>

#include <bitcoin/bitcoin/wallet/encrypted_keys.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>

#include "parse_encrypted_key.hpp"

namespace libbitcoin {
namespace wallet {

// Swap not defined.
class parse_encrypted_token
    : public parse_encrypted_prefix<8u> {
public:
    static byte_array<prefix_size> prefix_factory(bool lot_sequence);

    explicit parse_encrypted_token(encrypted_token const& value);

    bool lot_sequence() const;
    hash_digest data() const;
    ek_entropy entropy() const;
    one_byte sign() const;

private:
    bool verify_context() const;
    bool verify_magic() const;

    static constexpr uint8_t lot_context_ = 0x51;
    static constexpr uint8_t default_context_ = 0x53;
    static const byte_array<magic_size> magic_;

    const ek_entropy entropy_;
    const one_byte sign_;
    hash_digest const data_;
};

}  // namespace wallet
}  // namespace kth

#endif

// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <kth/domain/chain/script.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/multi_crypto_settings.hpp>

#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/thread.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::chain {

static constexpr size_t MAX_CONSENSUS_COMMITMENT_LENGTH = 40;

using token_id_t = hash_digest;
enum class amount_t : int64_t {};
using nft_commitment_t = std::vector<uint8_t>;

// The values assigned to the low-order nibble of the bitfield byte represent the "permissions" of an NFT.
// For pure-fungible tokens, the value must be none (0x0).
// It's important to note that only the three specified bitpatterns are valid for this nibble.
enum class capability : uint8_t {
    none = 0x00,     // The token lacks any capability or permission and is either a pure-fungible token or an immutable non-fungible token.
    mut = 0x01,      // If the mutable capability is present, it means that the encoded token is a non-fungible token that can be altered.
    minting = 0x02,  // If the minting capability is present, it indicates that the encoded token is a non-fungible token used for minting.
};

// The high-order nibble of the bitfield byte specifies the structure of the token's data payload that follows.
// The data that follows is described by combining the specified bitpatterns using bitwise-OR.
// However, it's important to note that the nibble cannot be equal to 0 or have the `reserved` bit set.
enum class structure : uint8_t {
    has_amount = 0x10,            // The payload encodes an amount of fungible tokens.
    has_nft = 0x20,               // The payload encodes a non-fungible token.
    has_commitment_length = 0x40, // The payload encodes a commitment-length and a commitment (has_nft must also be set).
    reserved = 0x80,              // Must be unset.
};

// Data that gets serialized/deserialized to/from a scriptPubKey in a transaction output prefixed with PREFIX_BYTE
struct token_data {
    token_data() noexcept = default;

    token_data(token_id_t const& id, amount_t amount, nft_commitment_t const& commitment = {}, bool has_nft = false, bool is_mutable_nft = false, bool is_minting_nft = false, bool unchecked_nft = false)
        : id_(id)
    {
        set_amount(amount, true);
        set_commitment(commitment, true);
        set_nft(has_nft, is_mutable_nft, is_minting_nft, unchecked_nft);
    }

    token_id_t const& id() const {
        return id_;
    }

    amount_t amount() const {
        return amount_;
    }

    nft_commitment_t const& commitment() const {
        return commitment_;
    }

    uint8_t bitfield() const {
        return bitfield_;
    }

private:

    // check the class invariants
    bool is_valid() const {
        return true;
    }

    // Setters: These should only be called when setting up CTxOuts for CmutableTransaction, and never on in-memory
    //          "Coin" instances e.g. from the coins cache.
    void set_id(token_id_t const& id) {
        id_ = id;
    }

    void set_amount(amount_t amount, bool set_bitfield = true) {
        amount_ = amount;
        if (set_bitfield) {
            bitfield_ = amount_ != amount_t{0} ? bitfield_ | uint8_t(structure::has_amount)
                                               : bitfield_ & ~uint8_t(structure::has_amount);
        }
    }

    void set_commitment(nft_commitment_t const& c, bool set_bitfield = true) {
        commitment_ = c;
        if (set_bitfield) {
            bitfield_ = ! commitment_.empty() ? bitfield_ | uint8_t(structure::has_commitment_length)
                                              : bitfield_ & ~uint8_t(structure::has_commitment_length);
        }
    }
public:

    bool has_nft() const {
        return bitfield_ & uint8_t(structure::has_nft);
    }

    capability get_capability() const {
        return static_cast<capability>(bitfield_ & 0x0fu);
    }

    bool is_minting_nft() const {
        return has_nft() && get_capability() == capability::minting;
    }

    //! Note that technically "minting" also has the mutable capability, but in this low-level API we differentiate them
    bool is_mutable_nft() const {
        return has_nft() && get_capability() == capability::mut;
    }

    bool is_immutable_nft() const {
        return has_nft() && get_capability() == capability::none;
    }

    bool is_fungible_only() const {
        return ! has_nft();
    }

    bool has_amount() const {
        return bitfield_ & uint8_t(structure::has_amount);
    }

    bool has_commitment_length() const {
        return bitfield_ & uint8_t(structure::has_commitment_length);
    }

    bool is_valid_bitfield() const {
        // At least 1 bit must be set in the structure nibble, but the structure::reserved bit must not be set.
        if (const uint8_t s = bitfield_ & 0xf0u; s >= 0x80u || s == 0x00u) return false;
        // capability nibble > 2 is invalid (that is, only valid bit-patterns for this nibble are: 0x00, 0x01, 0x02).
        if ((bitfield_ & 0x0fu) > 2u) return false;
        // A token prefix encoding no tokens (both has_nft and has_amount are unset) is invalid.
        if (!has_nft() && !has_amount()) return false;
        // A token prefix where has_nft is unset must encode capability nibble of 0x00.
        if (!has_nft() && (bitfield_ & 0x0fu) != 0u) return false;
        // A token prefix encoding has_commitment_length without has_nft is invalid.
        if (!has_nft() && has_commitment_length()) return false;

        return true;
    }



    /// NB: param `unchecked` is for tests only and enforces no rules if true.
    void set_nft(bool has_nft, bool is_mutable = false, bool is_minting = false, bool unchecked = false) {
        bitfield_ = has_nft ? (bitfield_ | uint8_t(structure::has_nft)) : (bitfield_ & ~uint8_t(structure::has_nft));
        if (!unchecked) {
            // enforce sanity if not using "unchecked" mode
            is_minting = has_nft && is_minting;
            is_mutable = has_nft && is_mutable && !is_minting;
            bitfield_ &= 0xf0u; // clear low-order nibble now (will be set below)
        }
        bitfield_ = is_minting ? bitfield_ | uint8_t(capability::minting) : (bitfield_ & ~uint8_t(capability::minting));
        bitfield_ = is_mutable ? bitfield_ | uint8_t(capability::mut) : (bitfield_ & ~uint8_t(capability::mut));
    }

    // /// Note: Supplied bitfield is *not* checked for correctness! (This is for tests only)
    // void SetBitfieldUnchecked(uint8_t bf) {
    //     bitfield_ = bf;
    // }

    // SERIALIZE_METHODS(token_data, obj) {
    //     READWRITE(obj.id); // 32-byte hash
    //     READWRITE(obj.bitfield_);
    //     if ( ! obj.is_valid_bitfield()) {
    //         throw InvalidBitfieldError(strprintf("Invalid token bitfield: 0x%02x", obj.bitfield_));
    //     }

    //     if (obj.has_commitment_length()) {
    //         // This will throw if we are (un)serializing an empty vector. If the has_commitment_length bit is set, the
    //         // commitment itself cannot be empty
    //         READWRITE(obj.commitment_);
    //     } else {
    //         // Skip commitment field if HasCommitment bit is not set, but ensure prevector is cleared if unserializing
    //         SER_READ(obj, obj.commitment_.clear());
    //     }

    //     if (obj.has_amount()) {
    //         READWRITE(obj.amount_); // may throw if out of range, e.g. > INT64_MAX (negative), or if 0
    //     } else {
    //         // Skip amount field if has_amount bit is not set, but ensure we clear it to 0 if unserializing
    //         SER_READ(obj, obj.amount_ = amount_t::fromInt(0).value());
    //     }
    // }

    bool operator==(const token_data &o) const {
        return amount_ == o.amount_ && bitfield_ == o.bitfield_ && id_ == o.id_ && commitment_ == o.commitment_;
    }

    bool operator!=(const token_data &o) const {
        return !this->operator==(o);
    }

    bool operator<(const token_data &o) const {
        // Note this ordering is used for BIP69 sorting. See: https://github.com/bitjson/cashtokens
        return   std::tuple(  amount_,   has_nft(),   get_capability(),   commitment_,   id_)
               < std::tuple(o.amount_, o.has_nft(), o.get_capability(), o.commitment_, o.id_);
    }

    // If verbose is true, print the full token id hex and commitment hex, otherwise print only the first
    // 30 characters of each.
    std::string to_string(bool verbose = false) const;

    // This is a rough estimate and actual size may be smaller in the average case or larger in some cases.
    static constexpr
    size_t serialized_size_estimated() {
        return sizeof(token_id_t) +
               1 +                          // bitfield
               sizeof(amount_t) +
               1 +                          // compact size for commitment length
               MAX_CONSENSUS_COMMITMENT_LENGTH;
    }
private:
    token_id_t id_;
    uint8_t bitfield_ = 0;        // Token bitfield byte. High order nibble is one of the structure enum values and low order nibble is capability.
    amount_t amount_;             // token amount (FT & NFT tokens). May not be negative.
    nft_commitment_t commitment_; // may be empty
};


} // namespace kth::domain::chain


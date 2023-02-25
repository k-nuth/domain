// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include <kth/domain/chain/script.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/machine/opcode.hpp>
#include <kth/domain/multi_crypto_settings.hpp>

#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/thread.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::chain {

namespace detail {
template<class> inline constexpr bool always_false_v = false;
} // namespace detail

namespace encoding {
static constexpr uint8_t PREFIX_BYTE = uint8_t(::kth::domain::machine::opcode::special_token_prefix);
}

using token_id_t = hash_digest;
enum class amount_t : int64_t {};
using commitment_t = std::vector<uint8_t>;

// The values assigned to the low-order nibble of the bitfield byte represent the "permissions" of an NFT.
// For pure-fungible tokens, the value must be none (0x0).
// It's important to note that only the three specified bitpatterns are valid for this nibble.
enum class capability_t : uint8_t {
    none    = 0x00,  // The token lacks any capability or permission and is either a pure-fungible token or an immutable non-fungible token.
    mut     = 0x01,  // If the mutable capability is present, it means that the encoded token is a non-fungible token that can be altered.
    minting = 0x02,  // If the minting capability is present, it indicates that the encoded token is a non-fungible token used for minting.
};

struct fungible {
    amount_t amount;

    friend constexpr
    auto operator<=>(fungible const&, fungible const&) = default;
};

struct non_fungible {
    capability_t capability;
    commitment_t commitment;

    friend constexpr
    auto operator<=>(non_fungible const&, non_fungible const&) = default;
};

using both_kinds = std::pair<fungible, non_fungible>;

struct token_data_t {
    token_id_t id;
    std::variant<fungible, non_fungible, both_kinds> data;

    friend constexpr
    auto operator<=>(token_data_t const&, token_data_t const&) = default;
};

using token_data_opt = std::optional<token_data_t>;

inline constexpr
bool is_valid(fungible const& x) {
    return x.amount > amount_t{0};
}

inline constexpr
bool is_valid(non_fungible const& x) {
    //TODO(fernando): ??
    return true;
}

inline constexpr
bool is_valid(both_kinds const& x) {
    return is_valid(x.first) && is_valid(x.second);
}

inline constexpr
bool is_valid(token_data_t const& x) {
    auto const visitor = [](auto&& arg) {
        return is_valid(arg);
    };

    //TODO(fernando): id validation

    return std::visit(visitor, x.data);
}

inline constexpr
bool is_valid(token_data_opt const& x) {
    return x.has_value() ? is_valid(x.value()) : true;
}


namespace encoding {

inline constexpr
size_t serialized_size(token_data_t const& x) {
    return std::size(x.id) +
           1 + // bitfield
           0 //TODO(fernando): the rest
           ;
}

inline constexpr
size_t serialized_size(token_data_opt const& x) {
    return x.has_value() ? serialized_size(x.value()) : 0;
}

inline constexpr
uint8_t bitfield(fungible const& x) {
    //TODO(fernando): this is for HAS_AMOUNT fungible data
    return 0b0001'0000;      //0x10
}

inline constexpr
uint8_t bitfield(non_fungible const& x) {
    //TODO(fernando): siempre tienen HAS_COMMITMENT y HAS_NFT ?
    return uint8_t{0b0100'0000} | // 0x40 - HAS_COMMITMENT_LENGTH, the prefix encodes a commitment length and commitment.
           uint8_t{0b0010'0000} | // 0x20 - HAS_NFT, the prefix encodes a non-fungible token.
           uint8_t(x.capability);
}

inline constexpr
uint8_t bitfield(both_kinds const& x) {
    return bitfield(x.first) | bitfield(x.second);
}

inline constexpr
uint8_t bitfield(token_data_t const& x) {
    auto const visitor = [](auto&& arg) {
        return bitfield(arg);
    };
    return std::visit(visitor, x.data);
}

template <typename W>
inline constexpr
void to_data(W& sink, fungible const& x) {
    //TODO(fernando): revisar en el spec cómo se debe serializar este amount. Si compact o de alguna otra forma.
    sink.write_8_bytes_little_endian(int64_t(x.amount));
}

template <typename W>
inline constexpr
void to_data(W& sink, non_fungible const& x) {
    sink.write_bytes(x.commitment);
}

template <typename W>
inline constexpr
void to_data(W& sink, both_kinds const& x) {
    to_data(sink, x.second);
    to_data(sink, x.first);
}

template <typename W>
inline constexpr
void to_data(W& sink, token_data_t const& x) {
    sink.write_bytes(std::begin(x.id), std::size(x.id));
    sink.write_byte(bitfield(x));

    auto const visitor = [&sink](auto&& arg) {
        to_data(sink, arg);
    };
    std::visit(visitor, x.data);
}

// The high-order nibble of the bitfield byte specifies the structure of the token's data payload that follows.
// The data that follows is described by combining the specified bitpatterns using bitwise-OR.
// However, it's important to note that the nibble cannot be equal to 0 or have the `reserved` bit set.
enum class structure_t : uint8_t {
    has_amount = 0x10,            // The payload encodes an amount of fungible tokens.
    has_nft = 0x20,               // The payload encodes a non-fungible token.
    has_commitment = 0x40,        // The payload encodes a commitment-length and a commitment (has_nft must also be set).
    reserved = 0x80,              // Must be unset.
};

// enum class capability_t : uint8_t {
//     none    = 0x00,  // The token lacks any capability or permission and is either a pure-fungible token or an immutable non-fungible token.
//     mut     = 0x01,  // If the mutable capability is present, it means that the encoded token is a non-fungible token that can be altered.
//     minting = 0x02,  // If the minting capability is present, it indicates that the encoded token is a non-fungible token used for minting.
// };

inline constexpr
bool has_nft(uint8_t bitfield) {
    return bitfield & uint8_t(structure_t::has_nft);
}

inline constexpr
bool has_amount(uint8_t bitfield) {
    return bitfield & uint8_t(structure_t::has_amount);
}

inline constexpr
bool has_commitment(uint8_t bitfield) {
    return bitfield & uint8_t(structure_t::has_commitment);
}

inline constexpr
std::pair<uint8_t, uint8_t> nibbles(uint8_t bitfield) {
    uint8_t const higher = bitfield & 0xf0u;
    uint8_t const lower  = bitfield & 0x0fu;
    return {higher, lower};
};

inline constexpr
capability_t capability(uint8_t bitfield) {
    auto const [_, c] = nibbles(bitfield);
    return capability_t{c};
};


inline constexpr
bool is_valid_bitfield(uint8_t bitfield) {
    //TODO(fernando): comparar la logica con la que escribimos en el spec interno

    auto const [structure, capability] = nibbles(bitfield);

    // At least 1 bit must be set in the structure nibble, but the Structure::Reserved bit must not be set.
    if (structure >= 0x80u || structure == 0x00u) return false;

    // Capability nibble > 2 is invalid (that is, only valid bit-patterns for this nibble are: 0x00, 0x01, 0x02).
    if (capability > 2u) return false;

    // A token prefix encoding no tokens (both has_nft and has_amount are unset) is invalid.
    if ( ! has_nft(bitfield) && ! has_amount(bitfield)) return false;

    // A token prefix where has_nft is unset must encode capability nibble of 0x00.
    if ( ! has_nft(bitfield) && capability != 0u) return false;

    // A token prefix encoding has_commitment without has_nft is invalid.
    if ( ! has_nft(bitfield) && has_commitment(bitfield)) return false;
    return true;
}

template <typename R, KTH_IS_READER(R)>
inline constexpr
bool from_data(R& source, fungible& x, bool /*has_commitment*/) {
    std::cout << "from_data - fungible" << std::endl;
    x.amount = amount_t(source.read_variable_little_endian());
    return source;
}

template <typename R, KTH_IS_READER(R)>
inline constexpr
bool from_data(R& source, non_fungible& x, bool has_commitment) {
    std::cout << "from_data - non_fungible" << std::endl;
    auto const size = source.read_size_little_endian();

    if (has_commitment) {
        //TODO(fernando): check size is valid
        x.commitment = source.read_bytes(size);
    }
    return source;
}

template <typename R, KTH_IS_READER(R)>
inline constexpr
bool from_data(R& source, both_kinds& x, bool has_commitment) {
    std::cout << "from_data - both_kinds" << std::endl;
    from_data(source, x.second, has_commitment);
    from_data(source, x.first, has_commitment);
    return source;
}

template <typename R, KTH_IS_READER(R)>
inline constexpr
bool from_data(R& source, token_data_opt& x) {
    x.reset();
    auto const id = source.read_hash();
    auto const bitfield = source.read_byte();

    if ( ! is_valid_bitfield(bitfield)) {
        source.invalidate();
        return false;
    }

    if (has_nft(bitfield) && has_amount(bitfield)) {
        x.emplace(id, both_kinds { {}, {capability(bitfield), {}} });
    } else if (has_nft(bitfield)) {
        x.emplace(id, non_fungible { .capability = capability(bitfield) });
    } else if (has_amount(bitfield)) {
        x.emplace(token_data_t {id, fungible {}});
    }

    auto const visitor = [&source, bitfield](auto&& arg) {
        return from_data(source, arg, has_commitment(bitfield));
    };
    std::visit(visitor, x->data);

    return source;
}

} // namespace encoding

} // namespace kth::domain::chain


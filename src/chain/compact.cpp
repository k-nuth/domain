// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/compact.hpp>

#include <cstdint>

#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/assert.hpp>

namespace kth::chain {

// Bitcoin compact for represents a value in base 256 notation as follows:
// value = (-1^sign) * mantissa * 256^(exponent-3)

// [exponent:8 | sign:1 | mantissa:31]
static constexpr uint32_t exp_byte = 0xff000000;
static constexpr uint32_t sign_bit = 0x00800000;
static constexpr uint32_t mantissa_max = ~(exp_byte | sign_bit);
static constexpr uint32_t mantissa_bits = (sizeof(uint32_t) - 1) * 8;

// assertions
static constexpr uint32_t mantissa_mask = ~mantissa_max;
static constexpr uint32_t first_byte_mask = 0xffffff00;

// Inlines
//-----------------------------------------------------------------------------

inline 
bool is_negated(uint32_t compact) {
    return (compact & sign_bit) != 0;
}

inline 
bool is_nonzero(uint32_t compact) {
    return (compact & mantissa_max) != 0;
}

inline 
uint8_t log_256(uint32_t mantissa) {
    BITCOIN_ASSERT_MSG(mantissa <= 0x00ffffff, "mantissa log256 is 4");
    return (mantissa > 0x0000ffff ? 3 : (mantissa > 0x000000ff ? 2 : (mantissa > 0x00000000 ? 1 : 0)));     //NOLINT
}

inline 
bool is_overflow(uint8_t exponent, uint32_t mantissa) {
    // Overflow if exponent would shift the mantissa more than 32 bytes.
    return (mantissa > 0) && (exponent > 32 + 3 - log_256(mantissa));       //NOLINT
}

inline 
uint32_t shift_low(uint8_t exponent) {
    BITCOIN_ASSERT(exponent <= 3);
    return 8 * (3 - exponent);  //NOLINT
}

inline 
uint32_t shift_high(uint8_t exponent) {
    BITCOIN_ASSERT(exponent > 3);
    return 8 * (exponent - 3);  //NOLINT
}

inline 
size_t logical_size(uint256_t value) {
    auto byte = 0;

    for (; value != 0; ++byte) {
        value >>= 8;              //NOLINT
    }

    return byte;
}

// Constructors
//-----------------------------------------------------------------------------

compact::compact(uint32_t compact) {
    overflowed_ = !from_compact(big_, compact);
    normal_ = from_big(big_);
}

compact::compact(uint256_t const& big)
    : big_(big), overflowed_(false) {
    normal_ = from_big(big_);
}

bool compact::is_overflowed() const {
    return overflowed_;
}

uint32_t compact::normal() const {
    return normal_;
}

compact::operator uint256_t const&() const {
    return big_;
}

uint256_t const& compact::big() const {
    return big_;
}

// Returns false on overflow, negatives are converted to zero.
bool compact::from_compact(uint256_t& out, uint32_t compact) {
    //*************************************************************************
    // CONSENSUS: The sign bit is not honored and it instead produces zero.
    // This results from having used a signed data structure for unsigned data.
    //*************************************************************************
    if (is_negated(compact)) {
        out = 0;
        return true;
    }

    // Mask off the exponent byte and sign to get the mantissa.
    auto mantissa = compact & mantissa_max;

    // Shift off the mantissa and sign to get the exponent byte.
    auto const exponent = static_cast<uint8_t>(compact >> mantissa_bits);

    // Shift the mantissa into the big number.
    if (exponent <= 3) {
        mantissa >>= shift_low(exponent);
        out = mantissa;
        return true;
    }

    // Compact has space for more exponent bits than can be represented in a
    // base 256 number represented in 32 bytes, so we trap overflow here.
    if (is_overflow(exponent, mantissa)) {
        return false;
    }

    out = mantissa;
    out <<= shift_high(exponent);
    return true;
}

uint32_t compact::from_big(uint256_t const& big) {
    // This value is limited to 32, so exponent cannot overflow.
    auto exponent = static_cast<uint8_t>(logical_size(big));

    // Shift the big number significant digits into the mantissa.
    auto const mantissa64 = exponent <= 3 ? static_cast<uint64_t>(big) << shift_low(exponent) : static_cast<uint64_t>(big >> shift_high(exponent));

    auto mantissa = static_cast<uint32_t>(mantissa64);

    //*************************************************************************
    // CONSENSUS: Satoshi used a signed implementation to represent unsigned.
    // If the sign bit is set drop it from the mantissa into the exponent.
    // This is intended to prevent the appearance of a negative sign bit.
    //*************************************************************************
    if (is_negated(mantissa)) {
        exponent++;
        mantissa >>= 8u;
    }

    BITCOIN_ASSERT_MSG((exponent & first_byte_mask) == 0, "size exceess");
    BITCOIN_ASSERT_MSG((mantissa & mantissa_mask) == 0, "value exceess");

    // Assemble the compact notation.
    return (static_cast<uint32_t>(exponent) << mantissa_bits) | mantissa;
}

}  // namespace kth

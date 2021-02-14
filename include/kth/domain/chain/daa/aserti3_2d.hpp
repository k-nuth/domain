// Copyright (c) 2016-2021 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CHAIN_DAA_ASERTI3_2D_HPP_
#define KTH_DOMAIN_CHAIN_DAA_ASERTI3_2D_HPP_
#if defined(KTH_CURRENCY_BCH)

#include <cstdint>
#include <utility>

#include <kth/infrastructure/utility/operators.hpp>

namespace kth::domain::chain::daa {

namespace {

// precondition: TODO
// postcondition: exponent == ((return.first + 16) * 65536) + return.second
inline
std::pair<int64_t, uint16_t> shifts_frac(int64_t time_diff, int64_t height_diff, uint32_t target_spacing_seconds, uint32_t half_life) noexcept {
    int64_t const exponent = ((time_diff - target_spacing_seconds * height_diff) * 65536) / half_life;
    return {sar(exponent, 16) - 16, uint16_t(exponent)};
}

inline
std::pair<int64_t, uint32_t> shifts_factor(int64_t time_diff, int64_t height_diff, uint32_t target_spacing_seconds, uint32_t half_life) noexcept {
    auto const [shifts, frac] = shifts_frac(time_diff, height_diff, target_spacing_seconds, half_life);

    // multiply target by 65536 * 2^(fractional part)
    // 2^x ~= (1 + 0.695502049*x + 0.2262698*x**2 + 0.0782318*x**3) for 0 <= x < 1
    // Error versus actual 2^x is less than 0.013%.
    uint32_t const factor = 65536 + ((
        + 195766423245049ull * frac
        + 971821376ull * frac * frac
        + 5127ull * frac * frac * frac
        + (1ull << 47)
        ) >> 48);

    return {shifts, factor};
}

inline
uint256_t shift_2way_safe(uint256_t x, int64_t n, uint256_t const& def) noexcept {
    if (n <= 0) {
        x >>= -n;
        // 0 is not a valid target, but 1 is.
        if (x == 0) return uint256_t(1);
        if (x > def) return def;
        return x;
    }

    // Predetect overflow that would silently discard high bits.
    if (msb(x) + n > 255) return def;
    x <<= n;
    if (x > def) return def;
    return x;
} 

} // namespace

// // Input target must never be zero nor exceed pow_limit.
// assert(refTarget > 0 && refTarget <= pow_limit);

// // We need some leading zero bits in pow_limit in order to have room to handle
// // overflows easily. 32 leading zero bits is more than enough.
// assert((pow_limit >> 224) == 0);

// // Height diff should NOT be negative.
// assert(height_diff >= 0); TODO(fernando)

// // It will be helpful when reading what follows, to remember that
// // nextTarget is adapted from anchor block target value.

// // Ultimately, we want to approximate the following ASERT formula, using only integer (fixed-point) math:
// //     new_target = old_target * 2^((blocks_time - IDEAL_BLOCK_TIME * (height_diff + 1)) / half_life)

// // First, we'll calculate the exponent:
// assert( llabs(time_diff - target_spacing_seconds * height_diff) < (1ll << (63 - 16)) );

uint256_t aserti3_2d(uint256_t const& anchor_target,
                     uint32_t target_spacing_seconds,
                     int64_t time_diff,
                     int64_t height_diff,
                     uint256_t const& pow_limit,
                     uint32_t half_life) noexcept {

    // precondition: TODO
    // postcondition:

    auto const [shifts, factor] = shifts_factor(time_diff, height_diff, target_spacing_seconds, half_life);
    return shift_2way_safe(anchor_target * factor, shifts, pow_limit);
}

} // namespace kth::domain::chain::daa

#endif // defined(KTH_CURRENCY_BCH)
#endif // KTH_DOMAIN_CHAIN_DAA_ASERTI3_2D_HPP_




// // ASERT calculation function.
// // Clamps to powLimit.
// arith_uint256 CalculateASERT(const arith_uint256 &refTarget,
//                              const int64_t nPowTargetSpacing,
//                              const int64_t nTimeDiff,
//                              const int64_t nHeightDiff,
//                              const arith_uint256 &powLimit,
//                              const int64_t nHalfLife) noexcept {

//     // Input target must never be zero nor exceed powLimit.
//     assert(refTarget > 0 && refTarget <= powLimit);

//     // We need some leading zero bits in powLimit in order to have room to handle
//     // overflows easily. 32 leading zero bits is more than enough.
//     assert((powLimit >> 224) == 0);

//     // Height diff should NOT be negative.
//     assert(nHeightDiff >= 0);

//     // It will be helpful when reading what follows, to remember that
//     // nextTarget is adapted from anchor block target value.

//     // Ultimately, we want to approximate the following ASERT formula, using only integer (fixed-point) math:
//     //     new_target = old_target * 2^((blocks_time - IDEAL_BLOCK_TIME * (height_diff + 1)) / nHalfLife)

//     // First, we'll calculate the exponent:
//     assert( llabs(nTimeDiff - nPowTargetSpacing * nHeightDiff) < (1ll << (63 - 16)) );
//     const int64_t exponent = ((nTimeDiff - nPowTargetSpacing * (nHeightDiff + 1)) * 65536) / nHalfLife;

//     // Next, we use the 2^x = 2 * 2^(x-1) identity to shift our exponent into the [0, 1) interval.
//     // The truncated exponent tells us how many shifts we need to do
//     // Note1: This needs to be a right shift. Right shift rounds downward (floored division),
//     //        whereas integer division in C++ rounds towards zero (truncated division).
//     // Note2: This algorithm uses arithmetic shifts of negative numbers. This
//     //        is unpecified but very common behavior for C++ compilers before
//     //        C++20, and standard with C++20. We must check this behavior e.g.
//     //        using static_assert.
//     static_assert(int64_t(-1) >> 1 == int64_t(-1),
//                   "ASERT algorithm needs arithmetic shift support");

//     // Now we compute an approximated target * 2^(exponent/65536.0)

//     // First decompose exponent into 'integer' and 'fractional' parts:
//     int64_t shifts = exponent >> 16;
//     const auto frac = uint16_t(exponent);
//     assert(exponent == (shifts * 65536) + frac);

//     // multiply target by 65536 * 2^(fractional part)
//     // 2^x ~= (1 + 0.695502049*x + 0.2262698*x**2 + 0.0782318*x**3) for 0 <= x < 1
//     // Error versus actual 2^x is less than 0.013%.
//     const uint32_t factor = 65536 + ((
//         + 195766423245049ull * frac
//         + 971821376ull * frac * frac
//         + 5127ull * frac * frac * frac
//         + (1ull << 47)
//         ) >> 48);
//     // this is always < 2^241 since refTarget < 2^224
//     arith_uint256 nextTarget = refTarget * factor;

//     // multiply by 2^(integer part) / 65536
//     shifts -= 16;
//     if (shifts <= 0) {
//         nextTarget >>= -shifts;
//     } else {
//         // Predetect overflow that would silently discard high bits.
//         if (int64_t(nextTarget.bits()) + shifts > 255) {
//             // If we had wider integers, the final value of nextTarget would be
//             // >= 2^256 so it would have just ended up as powLimit anyway.
//             return powLimit;
//         }
//         nextTarget <<= shifts;
//     }

//     if (nextTarget == 0) {
//         // 0 is not a valid target, but 1 is.
//         return arith_uint256(1);
//     }
//     if (nextTarget > powLimit) {
//         return powLimit;
//     }

//     return nextTarget;
// }

// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CHAIN_COMPACT_HPP
#define KTH_CHAIN_COMPACT_HPP

#include <cstdint>

#include <kth/domain/define.hpp>
#include <kth/infrastructure/math/hash.hpp>

namespace kth::chain {

/// A signed but zero-floored scientific notation in 32 bits.
class BC_API compact {
public:
    /// Construct a normal form compact number from a 32 bit compact number.
    explicit 
    compact(uint32_t compact);

    /// Construct a normal form compact number from a 256 bit number
    explicit 
    compact(uint256_t const& big);

    compact(compact&& x) noexcept;

    /// Copy constructor.
    compact(compact const& x);

    /// True if construction overflowed.
    [[nodiscard]] 
    bool is_overflowed() const;

    /// Consensus-normalized compact number value.
    /// This is derived from the construction parameter.
    [[nodiscard]] 
    uint32_t normal() const;

    /// Big number that the compact number represents.
    /// This is either saved or generated from the construction parameter.
    operator uint256_t const&() const;
    [[nodiscard]] 
    uint256_t const& big() const;

private:
    static
    bool from_compact(uint256_t& out, uint32_t compact);
    
    static
    uint32_t from_big(uint256_t const& big);

    uint256_t big_;
    uint32_t normal_;
    bool overflowed_;
};

}  // namespace kth::chain

#endif

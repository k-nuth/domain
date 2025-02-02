// Copyright (c) 2016-2025 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CHAIN_UTXO_HPP
#define KTH_DOMAIN_CHAIN_UTXO_HPP

#include <cstdint>

#include <kth/domain/chain/output_point.hpp>
#include <kth/domain/define.hpp>

namespace kth::domain::chain {

class KD_API utxo {
public:
    utxo() = default;

    utxo(output_point const& point, uint64_t amount);
    utxo(output_point&& point, uint64_t amount);

    // Getters
    output_point& point();
    output_point const& point() const;
    uint64_t amount() const;

    // Setters
    void set_point(output_point const& point);
    void set_amount(uint64_t amount);

private:
    output_point point_;
    uint64_t amount_;
};

} // namespace kth::domain::chain

#endif
// Copyright (c) 2016-2025 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/utxo.hpp>

namespace kth::domain::chain {

utxo::utxo(output_point const& point, uint64_t amount)
    : point_(point), amount_(amount)
{}

utxo::utxo(output_point&& point, uint64_t amount)
    : point_(std::move(point)), amount_(amount)
{}

output_point& utxo::point() {
    return point_;
}

output_point const& utxo::point() const {
    return point_;
}

uint64_t utxo::amount() const {
    return amount_;
}

void utxo::set_point(output_point const& point) {
    point_ = point;
}

void utxo::set_amount(uint64_t amount) {
    amount_ = amount;
}

} // namespace kth::domain::chain
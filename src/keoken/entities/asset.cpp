// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/keoken/entities/asset.hpp>

#include <kth/domain/keoken/primitives.hpp>

namespace kth {
namespace keoken {
namespace entities {

// using namespace bc;
using bc::wallet::payment_address;

// Constructors.
//-----------------------------------------------------------------------------

asset::asset(asset_id_t id, std::string name, amount_t amount, bc::wallet::payment_address owner)
    : id_(id), name_(std::move(name)), amount_(amount), owner_(std::move(owner)) {}

// Operators.
//-----------------------------------------------------------------------------

// friend
bool operator==(asset const& a, asset const& b) {
    return a.id_ == b.id_;
}

// friend
bool operator!=(asset const& a, asset const& b) {
    return !(a == b);
}

// Properties (size, accessors, cache).
//-----------------------------------------------------------------------------

asset_id_t asset::id() const {
    return id_;
}

// void asset::set_id(asset_id_t x) {
//     id_ = x;
// }

std::string const& asset::name() const {
    return name_;
}

// void asset::set_name(std::string x) {
//     name_ = std::move(x);
// }

amount_t asset::amount() const {
    return amount_;
}

// void asset::set_amount(amount_t x) {
//     amount_ = x;
// }

payment_address const& asset::owner() const {
    return owner_;
}

// void asset::set_owner(bc::wallet::payment_address x) {
//     owner_ = std::move(x);
// }

}  // namespace entities
}  // namespace keoken
}  // namespace kth

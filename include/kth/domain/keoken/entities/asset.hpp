// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_ENTITIES_ASSET_HPP_
#define KTH_KEOKEN_ENTITIES_ASSET_HPP_

#include <string>

#include <kth/domain/define.hpp>
#include <kth/domain/wallet/payment_address.hpp>

#include <kth/domain/keoken/primitives.hpp>

namespace knuth {
namespace keoken {
namespace entities {

class BC_API asset {
public:
    asset(asset_id_t id, std::string name, amount_t amount, bc::wallet::payment_address owner);

    // Semiregular.
    //-------------------------------------------------------------------------

    asset() = default;
    // asset(asset const& x) = default;
    // asset(asset&& x) = default;
    // asset& operator=(asset const& x) = default;
    // asset& operator=(asset&& x) = default;

    // Regular.
    //-------------------------------------------------------------------------

    friend bool operator==(asset const& a, asset const& b);

    friend bool operator!=(asset const& a, asset const& b);

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    asset_id_t id() const;
    // void set_id(asset_id_t x);

    std::string const& name() const;
    // void set_name(std::string x);

    amount_t amount() const;
    // void set_amount(amount_t x);

    bc::wallet::payment_address const& owner() const;
    // void set_owner(bc::::wallet::payment_address x);

private:
    asset_id_t id_;
    std::string name_;
    amount_t amount_;
    bc::wallet::payment_address owner_;
};

}  // namespace entities
}  // namespace keoken
}  // namespace knuth

#endif  //KTH_KEOKEN_ENTITIES_ASSET_HPP_

// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_TRANSACTION_HPP
#define KTH_TRANSACTION_HPP

#ifdef KTH_INFRASTUCTURE_CONFIG_ENABLED

#include <iostream>
#include <string>

#include <kth/domain/chain/transaction.hpp>
#include <kth/domain/define.hpp>

namespace kth::domain::config {

/**
 * Serialization helper to convert between serialized and deserialized satoshi
 * transaction.
 */
class KD_API transaction {
public:
    transaction() = default;

    transaction(std::string const& hexcode);
    transaction(chain::transaction const& value);

    transaction(transaction const& x);

    chain::transaction& data();

    operator chain::transaction const&() const;

    friend
    std::istream& operator>>(std::istream& input,
                                    transaction& argument);

    friend
    std::ostream& operator<<(std::ostream& output, transaction const& argument);

private:
    chain::transaction value_;
};

} // namespace kth::domain::config

#endif // KTH_INFRASTUCTURE_CONFIG_ENABLED

#endif


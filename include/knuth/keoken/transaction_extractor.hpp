// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_TRANSACTION_EXTRACTOR_HPP_
#define KTH_KEOKEN_TRANSACTION_EXTRACTOR_HPP_

#include <kth/domain/chain/transaction.hpp>
#include <kth/infrastructure/utility/data.hpp>

namespace knuth {
namespace keoken {

bc::data_chunk first_keoken_output(bc::chain::transaction const& tx);

}  // namespace keoken
}  // namespace knuth

#endif  //KTH_KEOKEN_TRANSACTION_EXTRACTOR_HPP_

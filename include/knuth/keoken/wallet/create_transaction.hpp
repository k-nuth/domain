/**
 * Copyright (c) 2016-2018 Knuth Project.
 *
 * This file is part of Knuth Project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KTH_KEOKEN_WALLET_CREATE_TRANSACTION_HPP_
#define KTH_KEOKEN_WALLET_CREATE_TRANSACTION_HPP_

#include <kth/domain/chain/input.hpp>
#include <kth/domain/chain/input_point.hpp>
#include <kth/domain/chain/output.hpp>
#include <kth/domain/chain/output_point.hpp>
#include <kth/domain/chain/transaction.hpp>
#include <kth/domain/config/input.hpp>
#include <kth/domain/config/output.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/wallet/transaction_functions.hpp>

#include <knuth/keoken/message/base.hpp>
#include <knuth/keoken/primitives.hpp>

namespace knuth {
namespace keoken {
namespace wallet {

// TODO(guillermo): calculate the fees instead of using a static value
static constexpr uint64_t fees = 2000;

using result_t = std::pair<kth::error::error_code_t, kth::chain::transaction>;

BC_API
result_t tx_encode_create_asset(kth::chain::input_point::list const& outputs_to_spend,
                                kth::wallet::payment_address const& asset_owner,
                                uint64_t utxo_satoshis,
                                std::string const& asset_name,
                                amount_t asset_amount,
                                uint32_t locktime = 0,
                                uint32_t tx_version = 1,
                                uint8_t script_version = 5);

BC_API
result_t tx_encode_send_token(kth::chain::input_point::list const& outputs_to_spend,
                              kth::wallet::payment_address const& asset_owner,
                              uint64_t utxo_satoshis,
                              kth::wallet::payment_address const& asset_receiver,
                              uint64_t dust,
                              asset_id_t asset_id,
                              amount_t asset_amount,
                              uint32_t locktime = 0,
                              uint32_t tx_version = 1,
                              uint8_t script_version = 5);

BC_API
result_t create_asset_tx_complete(kth::chain::input_point const& output_to_spend,
                                  kth::chain::script const& output_script,
                                  kth::ec_secret const& private_key,
                                  kth::wallet::ec_public const& public_key,
                                  uint64_t amount,
                                  kth::wallet::payment_address const& addr,
                                  std::string const& asset_name,
                                  amount_t asset_amount);

BC_API
result_t send_token_tx_complete(kth::chain::input_point const& output_to_spend,
                                kth::chain::script const& output_script,
                                kth::ec_secret const& private_key,
                                kth::wallet::ec_public const& public_key,
                                uint64_t amount,
                                kth::wallet::payment_address const& addr_origin,
                                kth::wallet::payment_address const& addr_dest,
                                uint64_t dust,
                                asset_id_t asset_id,
                                amount_t asset_amount);

}  // namespace wallet
}  // namespace keoken
}  // namespace knuth

#endif  //KTH_KEOKEN_WALLET_CREATE_TRANSACTION_HPP_

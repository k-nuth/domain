// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

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

#include <kth/domain/keoken/message/base.hpp>
#include <kth/domain/keoken/primitives.hpp>

namespace kth::keoken::wallet {

// TODO(guillermo): calculate the fees instead of using a static value
static constexpr 
uint64_t fees = 2000;

using result_t = std::pair<kth::error::error_code_t, kth::domain::chain::transaction>;

KD_API
result_t tx_encode_create_asset(kth::domain::chain::input_point::list const& outputs_to_spend,
                                kth::domain::wallet::payment_address const& asset_owner,
                                uint64_t utxo_satoshis,
                                std::string const& asset_name,
                                amount_t asset_amount,
                                uint32_t locktime = 0,
                                uint32_t tx_version = 1,
                                uint8_t script_version = 5);

KD_API
result_t tx_encode_send_token(kth::domain::chain::input_point::list const& outputs_to_spend,
                              kth::domain::wallet::payment_address const& asset_owner,
                              uint64_t utxo_satoshis,
                              kth::domain::wallet::payment_address const& asset_receiver,
                              uint64_t dust,
                              asset_id_t asset_id,
                              amount_t asset_amount,
                              uint32_t locktime = 0,
                              uint32_t tx_version = 1,
                              uint8_t script_version = 5);

KD_API
result_t create_asset_tx_complete(kth::domain::chain::input_point const& output_to_spend,
                                  kth::domain::chain::script const& output_script,
                                  kth::ec_secret const& private_key,
                                  kth::domain::wallet::ec_public const& public_key,
                                  uint64_t amount,
                                  kth::domain::wallet::payment_address const& addr,
                                  std::string const& asset_name,
                                  amount_t asset_amount);

KD_API
result_t send_token_tx_complete(kth::domain::chain::input_point const& output_to_spend,
                                kth::domain::chain::script const& output_script,
                                kth::ec_secret const& private_key,
                                kth::domain::wallet::ec_public const& public_key,
                                uint64_t amount,
                                kth::domain::wallet::payment_address const& addr_origin,
                                kth::domain::wallet::payment_address const& addr_dest,
                                uint64_t dust,
                                asset_id_t asset_id,
                                amount_t asset_amount);

} // namespace wallet
} // namespace keoken
} // namespace kth

#endif  //KTH_KEOKEN_WALLET_CREATE_TRANSACTION_HPP_

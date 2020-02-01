// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_WALLET_CREATE_TRANSACTION_HPP_
#define KTH_KEOKEN_WALLET_CREATE_TRANSACTION_HPP_

#include <bitcoin/bitcoin/chain/input.hpp>
#include <bitcoin/bitcoin/chain/input_point.hpp>
#include <bitcoin/bitcoin/chain/output.hpp>
#include <bitcoin/bitcoin/chain/output_point.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/config/input.hpp>
#include <bitcoin/bitcoin/config/output.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/wallet/transaction_functions.hpp>

#include <knuth/keoken/message/base.hpp>
#include <knuth/keoken/primitives.hpp>

namespace knuth {
namespace keoken {
namespace wallet {

// TODO(guillermo): calculate the fees instead of using a static value
static constexpr uint64_t fees = 2000;

using result_t = std::pair<libbitcoin::error::error_code_t, libbitcoin::chain::transaction>;

BC_API
result_t tx_encode_create_asset(libbitcoin::chain::input_point::list const& outputs_to_spend,
                                libbitcoin::wallet::payment_address const& asset_owner,
                                uint64_t utxo_satoshis,
                                std::string const& asset_name,
                                amount_t asset_amount,
                                uint32_t locktime = 0,
                                uint32_t tx_version = 1,
                                uint8_t script_version = 5);

BC_API
result_t tx_encode_send_token(libbitcoin::chain::input_point::list const& outputs_to_spend,
                              libbitcoin::wallet::payment_address const& asset_owner,
                              uint64_t utxo_satoshis,
                              libbitcoin::wallet::payment_address const& asset_receiver,
                              uint64_t dust,
                              asset_id_t asset_id,
                              amount_t asset_amount,
                              uint32_t locktime = 0,
                              uint32_t tx_version = 1,
                              uint8_t script_version = 5);

BC_API
result_t create_asset_tx_complete(libbitcoin::chain::input_point const& output_to_spend,
                                  libbitcoin::chain::script const& output_script,
                                  libbitcoin::ec_secret const& private_key,
                                  libbitcoin::wallet::ec_public const& public_key,
                                  uint64_t amount,
                                  libbitcoin::wallet::payment_address const& addr,
                                  std::string const& asset_name,
                                  amount_t asset_amount);

BC_API
result_t send_token_tx_complete(libbitcoin::chain::input_point const& output_to_spend,
                                libbitcoin::chain::script const& output_script,
                                libbitcoin::ec_secret const& private_key,
                                libbitcoin::wallet::ec_public const& public_key,
                                uint64_t amount,
                                libbitcoin::wallet::payment_address const& addr_origin,
                                libbitcoin::wallet::payment_address const& addr_dest,
                                uint64_t dust,
                                asset_id_t asset_id,
                                amount_t asset_amount);

}  // namespace wallet
}  // namespace keoken
}  // namespace knuth

#endif  //KTH_KEOKEN_WALLET_CREATE_TRANSACTION_HPP_

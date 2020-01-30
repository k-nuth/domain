// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef KTH_TRANSACTION_FUNCTIONS_HPP
#define KTH_TRANSACTION_FUNCTIONS_HPP

#include <string>
#include <utility>
#include <vector>

#include <kth/domain.hpp>

namespace kth {
namespace wallet {

using raw_output = std::pair<payment_address, uint64_t>;
using raw_output_list = std::vector<raw_output>;

BC_API std::pair<error::error_code_t, chain::transaction> tx_encode(chain::input_point::list const& outputs_to_spend,
                                                                    raw_output_list const& destiny_and_amount,
                                                                    std::vector<kth::chain::output> const& extra_outputs,
                                                                    uint32_t locktime = 0,
                                                                    uint32_t tx_version = 1,
                                                                    uint8_t script_version = 5);

BC_API std::pair<error::error_code_t, chain::transaction> tx_encode(chain::input_point::list const& outputs_to_spend,
                                                                    raw_output_list const& destiny_and_amount,
                                                                    uint32_t locktime = 0,
                                                                    uint32_t tx_version = 1,
                                                                    uint8_t script_version = 5);

BC_API std::pair<error::error_code_t, data_chunk> input_signature_old(kth::ec_secret const& private_key,
                                                                      chain::script const& output_script,
                                                                      chain::transaction const& tx,
                                                                      uint32_t index,
                                                                      uint8_t sign_type = 0x01,
                                                                      bool anyone_can_pay = false);

BC_API std::pair<error::error_code_t, data_chunk> input_signature_btc(kth::ec_secret const& private_key,
                                                                      chain::script const& output_script,
                                                                      chain::transaction const& tx,
                                                                      uint64_t amount,
                                                                      uint32_t index,
                                                                      uint8_t sign_type = 0x01,
                                                                      bool anyone_can_pay = false);

BC_API std::pair<error::error_code_t, data_chunk> input_signature_bch(kth::ec_secret const& private_key,
                                                                      chain::script const& output_script,
                                                                      chain::transaction const& tx,
                                                                      uint64_t amount,
                                                                      uint32_t index,
                                                                      uint8_t sign_type = 0x01,
                                                                      bool anyone_can_pay = false);

BC_API std::pair<error::error_code_t, chain::transaction> input_set(chain::script const& script,
                                                                    chain::transaction const& raw_tx,
                                                                    uint32_t index = 0);

BC_API std::pair<error::error_code_t, chain::transaction> input_set(data_chunk const& signature,
                                                                    wallet::ec_public const& public_key,
                                                                    chain::transaction const& raw_tx,
                                                                    uint32_t index = 0);

}  //namespace wallet
}  //namespace kth

#endif  //KTH_TRANSACTION_FUNCTIONS_HPP
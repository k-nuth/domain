// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// #include <kth/domain.hpp>
#include <kth/domain/chain/input.hpp>
#include <kth/domain/chain/input_point.hpp>
#include <kth/domain/chain/output.hpp>
#include <kth/domain/chain/output_point.hpp>
#include <kth/domain/chain/transaction.hpp>
#include <kth/domain/config/input.hpp>
#include <kth/domain/config/output.hpp>
#include <kth/domain/wallet/transaction_functions.hpp>
#include <kth/infrastructure/formats/base_16.hpp>
#include <kth/domain/keoken/message/base.hpp>

#include <kth/domain/keoken/message/base.hpp>
#include <kth/domain/keoken/message/create_asset.hpp>
#include <kth/domain/keoken/message/send_tokens.hpp>
#include <kth/domain/keoken/wallet/create_transaction.hpp>

using namespace bc;
using namespace knuth::keoken::wallet;

#define SEED "fffb587496cc54912bbcef874fa9a61a"
#define WALLET "mwx2YDHgpdfHUmCpFjEi9LarXf7EkQN6YG"
#define WALLETDESTINY "ms9qrYaJ468QCwgX6v7ittgX3vBk3mg6PM"
#define TX_ENCODE "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000"
#define SIGNATURE "30440220433c405e4cb7698ad5f58e0ea162c3c3571d46d96ff1b3cb9232a06eba3b444d02204bc5f48647c0f052ade7cf85eac3911f7afbfa69fa5ebd92084191a5da33f88d41"
#define COMPLETE_TX "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d98010000006a4730440220433c405e4cb7698ad5f58e0ea162c3c3571d46d96ff1b3cb9232a06eba3b444d02204bc5f48647c0f052ade7cf85eac3911f7afbfa69fa5ebd92084191a5da33f88d4121027a45d4abb6ebb00214796e2c7cf61d18c9185ba771fe9ed75b303eb7a8e9028bffffffff0118beeb0b000000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000"

TEST_CASE("[keoken__send_token__expected]") {
    std::vector<chain::input_point> outputs_to_spend;
    bc::hash_digest hash_to_spend;
    bc::decode_hash(hash_to_spend, "980de6ce12c29698d54323c6b0f358e1a9ae867598b840ee0094b9df22b07393");
    uint32_t const index_to_spend = 1;
    outputs_to_spend.push_back({hash_to_spend, index_to_spend});
    knuth::keoken::asset_id_t asset_id = 1;
    knuth::keoken::amount_t amount_tokens = 1;
    auto result_tx = tx_encode_send_token(outputs_to_spend, bc::wallet::payment_address(WALLET), 21647102398, bc::wallet::payment_address(WALLETDESTINY), 20000, asset_id, amount_tokens);
    std::string result = "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff03204e0000000000001976a9147fa36605e302ed00aeca0da8e2743772df11290188acce3b440a050000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac0000000000000000176a0400004b50100000000100000001000000000000000100000000";
    REQUIRE(result == kth::encode_base16(result_tx.second.to_data()));
}

TEST_CASE("[keoken__create_asset__expected]") {
    std::vector<chain::input_point> outputs_to_spend;
    bc::hash_digest hash_to_spend;
    bc::decode_hash(hash_to_spend, "980de6ce12c29698d54323c6b0f358e1a9ae867598b840ee0094b9df22b07393");
    uint32_t const index_to_spend = 1;
    outputs_to_spend.push_back({hash_to_spend, index_to_spend});
    std::string name = "testcoin";
    knuth::keoken::amount_t amount_tokens = 1;
    auto result_tx = tx_encode_create_asset(outputs_to_spend, bc::wallet::payment_address(WALLET), 21647102398, name, amount_tokens);
    std::string result = "01000000019373b022dfb99400ee40b8987586aea9e158f3b0c62343d59896c212cee60d980100000000ffffffff02ee89440a050000001976a914b43ff4532569a00bcab4ce60f87cdeebf985b69a88ac00000000000000001c6a0400004b50150000000074657374636f696e00000000000000000100000000";

    REQUIRE(result == kth::encode_base16(result_tx.second.to_data()));
}

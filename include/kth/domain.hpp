// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

///////////////////////////////////////////////////////////////////////////////
//
//        GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
//
///////////////////////////////////////////////////////////////////////////////
#ifndef KTH_DOMAIN_HPP
#define KTH_DOMAIN_HPP

/**
 * API Users: Include only this header. Direct use of other headers is fragile
 * and unsupported as header organization is subject to change.
 *
 * Maintainers: Do not include this header internal to this library.
 */

#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/chain/block.hpp>
#include <kth/domain/chain/chain_state.hpp>
#include <kth/domain/chain/compact.hpp>
#include <kth/domain/chain/header.hpp>
#include <kth/domain/chain/history.hpp>
#include <kth/domain/chain/input.hpp>
#include <kth/domain/chain/input_point.hpp>
#include <kth/domain/chain/output.hpp>
#include <kth/domain/chain/output_point.hpp>
#include <kth/domain/chain/point.hpp>
#include <kth/domain/chain/point_iterator.hpp>
#include <kth/domain/chain/point_value.hpp>
#include <kth/domain/chain/points_value.hpp>
#include <kth/domain/chain/script.hpp>
#include <kth/domain/chain/stealth.hpp>
#include <kth/domain/chain/transaction.hpp>

#if defined(KTH_SEGWIT_ENABLED)
#include <kth/domain/chain/witness.hpp>
#endif

#include <kth/domain/version.hpp>
#include <kth/domain/config/parser.hpp>
#include <kth/domain/machine/interpreter.hpp>
#include <kth/domain/machine/opcode.hpp>
#include <kth/domain/machine/operation.hpp>
#include <kth/domain/machine/program.hpp>
#include <kth/domain/machine/rule_fork.hpp>

// #include <kth/infrastructure/machine/number.hpp>
// #include <kth/infrastructure/machine/script_pattern.hpp>
// #include <kth/infrastructure/machine/script_version.hpp>
// #include <kth/infrastructure/machine/sighash_algorithm.hpp>
// #include <kth/infrastructure/math/checksum.hpp>
// #include <kth/infrastructure/math/crypto.hpp>
// #include <kth/infrastructure/math/elliptic_curve.hpp>

#include <kth/domain/math/stealth.hpp>
#include <kth/domain/message/address.hpp>
#include <kth/domain/message/alert.hpp>
#include <kth/domain/message/alert_payload.hpp>
#include <kth/domain/message/block.hpp>
#include <kth/domain/message/block_transactions.hpp>
#include <kth/domain/message/compact_block.hpp>
#include <kth/domain/message/fee_filter.hpp>
#include <kth/domain/message/filter_add.hpp>
#include <kth/domain/message/filter_clear.hpp>
#include <kth/domain/message/filter_load.hpp>
#include <kth/domain/message/get_address.hpp>
#include <kth/domain/message/get_block_transactions.hpp>
#include <kth/domain/message/get_blocks.hpp>
#include <kth/domain/message/get_data.hpp>
#include <kth/domain/message/get_headers.hpp>
#include <kth/domain/message/header.hpp>
#include <kth/domain/message/headers.hpp>
#include <kth/domain/message/heading.hpp>
#include <kth/domain/message/inventory.hpp>
#include <kth/domain/message/inventory_vector.hpp>
#include <kth/domain/message/memory_pool.hpp>
#include <kth/domain/message/merkle_block.hpp>

// #include <kth/infrastructure/math/uint256.hpp>
// #include <kth/infrastructure/message/message_tools.hpp>

#include <kth/domain/message/messages.hpp>
#include <kth/domain/message/not_found.hpp>
#include <kth/domain/message/ping.hpp>
#include <kth/domain/message/pong.hpp>
#include <kth/domain/message/prefilled_transaction.hpp>
#include <kth/domain/message/reject.hpp>
#include <kth/domain/message/send_compact.hpp>
#include <kth/domain/message/send_headers.hpp>
#include <kth/domain/message/transaction.hpp>
#include <kth/domain/message/verack.hpp>
#include <kth/domain/message/version.hpp>
#include <kth/domain/message/xverack.hpp>
#include <kth/domain/message/xversion.hpp>

#include <kth/domain/wallet/bitcoin_uri.hpp>
#include <kth/domain/wallet/ec_private.hpp>
#include <kth/domain/wallet/ec_public.hpp>
#include <kth/domain/wallet/ek_private.hpp>
#include <kth/domain/wallet/ek_public.hpp>
#include <kth/domain/wallet/ek_token.hpp>
#include <kth/domain/wallet/encrypted_keys.hpp>
#include <kth/domain/wallet/message.hpp>
#include <kth/domain/wallet/payment_address.hpp>
#include <kth/domain/wallet/select_outputs.hpp>
#include <kth/domain/wallet/stealth_address.hpp>
#include <kth/domain/wallet/stealth_receiver.hpp>
#include <kth/domain/wallet/stealth_sender.hpp>
#include <kth/domain/wallet/uri_reader.hpp>

// #include <kth/infrastructure.hpp>

#endif //KTH_DOMAIN_HPP

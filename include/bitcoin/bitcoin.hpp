///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2015 libbitcoin developers (see COPYING).
//
//        GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
//
///////////////////////////////////////////////////////////////////////////////
#ifndef LIBBITCOIN_BITCOIN_HPP
#define LIBBITCOIN_BITCOIN_HPP

/**
 * API Users: Include only this header. Direct use of other headers is fragile
 * and unsupported as header organization is subject to change.
 *
 * Maintainers: Do not include this header internal to this library.
 */

// #include <bitcoin/bitcoin/compat.h>
// #include <bitcoin/bitcoin/compat.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
// #include <bitcoin/bitcoin/error.hpp>
// #include <bitcoin/bitcoin/handlers.hpp>
#include <bitcoin/bitcoin/chain/block.hpp>
#include <bitcoin/bitcoin/chain/chain_state.hpp>
#include <bitcoin/bitcoin/chain/compact.hpp>
#include <bitcoin/bitcoin/chain/header.hpp>
#include <bitcoin/bitcoin/chain/history.hpp>
#include <bitcoin/bitcoin/chain/input.hpp>
#include <bitcoin/bitcoin/chain/input_point.hpp>
#include <bitcoin/bitcoin/chain/output.hpp>
#include <bitcoin/bitcoin/chain/output_point.hpp>
#include <bitcoin/bitcoin/chain/point.hpp>
#include <bitcoin/bitcoin/chain/point_iterator.hpp>
#include <bitcoin/bitcoin/chain/point_value.hpp>
#include <bitcoin/bitcoin/chain/points_value.hpp>
#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/chain/stealth.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/chain/witness.hpp>
#include <bitcoin/bitcoin/version.hpp>

// #include <bitcoin/bitcoin/config/authority.hpp>
// #include <bitcoin/infrastructure/config/base16.hpp>
// #include <bitcoin/bitcoin/config/base2.hpp>
// #include <bitcoin/bitcoin/config/base58.hpp>
// #include <bitcoin/bitcoin/config/base64.hpp>
// #include <bitcoin/infrastructure/config/checkpoint.hpp>
// #include <bitcoin/bitcoin/config/directory.hpp>
// #include <bitcoin/bitcoin/config/endpoint.hpp>
// #include <bitcoin/bitcoin/config/ec_private.hpp>
// #include <bitcoin/bitcoin/config/endorsement.hpp>
// #include <bitcoin/infrastructure/config/hash160.hpp>
// #include <bitcoin/infrastructure/config/hash256.hpp>
// #include <bitcoin/bitcoin/config/header.hpp>
// #include <bitcoin/bitcoin/config/input.hpp>
// #include <bitcoin/bitcoin/config/output.hpp>
// #include <bitcoin/bitcoin/config/parameter.hpp>
// #include <bitcoin/bitcoin/config/parser.hpp>
// #include <bitcoin/bitcoin/config/point.hpp>
// #include <bitcoin/bitcoin/config/printer.hpp>
// #include <bitcoin/bitcoin/config/script.hpp>
// #include <bitcoin/bitcoin/config/settings.hpp>
// #include <bitcoin/bitcoin/config/sodium.hpp>
// #include <bitcoin/bitcoin/config/transaction.hpp>

// #include <bitcoin/bitcoin/formats/base_10.hpp>
// #include <bitcoin/infrastructure/formats/base_16.hpp>
// #include <bitcoin/bitcoin/formats/base_58.hpp>
// #include <bitcoin/bitcoin/formats/base_64.hpp>
// #include <bitcoin/bitcoin/formats/base_85.hpp>

// #include <bitcoin/bitcoin/log/attributes.hpp>
// #include <bitcoin/bitcoin/log/file_char_traits.hpp>
// #include <bitcoin/bitcoin/log/file_collector.hpp>
// #include <bitcoin/bitcoin/log/file_collector_repository.hpp>
// #include <bitcoin/bitcoin/log/file_counter_formatter.hpp>
// #include <bitcoin/bitcoin/log/rotable_file.hpp>
// #include <bitcoin/bitcoin/log/severity.hpp>
// #include <bitcoin/bitcoin/log/sink.hpp>
// #include <bitcoin/bitcoin/log/source.hpp>
// #include <bitcoin/bitcoin/log/statsd_sink.hpp>
// #include <bitcoin/bitcoin/log/statsd_source.hpp>
// #include <bitcoin/bitcoin/log/udp_client_sink.hpp>
// #include <bitcoin/bitcoin/log/features/counter.hpp>
// #include <bitcoin/bitcoin/log/features/gauge.hpp>
// #include <bitcoin/bitcoin/log/features/metric.hpp>
// #include <bitcoin/bitcoin/log/features/rate.hpp>
// #include <bitcoin/bitcoin/log/features/timer.hpp>
#include <bitcoin/bitcoin/machine/interpreter.hpp>
#include <bitcoin/bitcoin/machine/opcode.hpp>
#include <bitcoin/bitcoin/machine/operation.hpp>
#include <bitcoin/bitcoin/machine/program.hpp>
#include <bitcoin/bitcoin/machine/rule_fork.hpp>
#include <bitcoin/infrastructure/machine/number.hpp>
#include <bitcoin/infrastructure/machine/script_pattern.hpp>
#include <bitcoin/infrastructure/machine/script_version.hpp>
#include <bitcoin/infrastructure/machine/sighash_algorithm.hpp>
#include <bitcoin/infrastructure/math/checksum.hpp>
#include <bitcoin/infrastructure/math/crypto.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
// #include <bitcoin/infrastructure/math/hash.hpp>
// #include <bitcoin/infrastructure/math/limits.hpp>
#include <bitcoin/bitcoin/math/stealth.hpp>
#include <bitcoin/bitcoin/message/address.hpp>
#include <bitcoin/bitcoin/message/alert.hpp>
#include <bitcoin/bitcoin/message/alert_payload.hpp>
#include <bitcoin/bitcoin/message/block.hpp>
#include <bitcoin/bitcoin/message/block_transactions.hpp>
#include <bitcoin/bitcoin/message/compact_block.hpp>
#include <bitcoin/bitcoin/message/fee_filter.hpp>
#include <bitcoin/bitcoin/message/filter_add.hpp>
#include <bitcoin/bitcoin/message/filter_clear.hpp>
#include <bitcoin/bitcoin/message/filter_load.hpp>
#include <bitcoin/bitcoin/message/get_address.hpp>
#include <bitcoin/bitcoin/message/get_block_transactions.hpp>
#include <bitcoin/bitcoin/message/get_blocks.hpp>
#include <bitcoin/bitcoin/message/get_data.hpp>
#include <bitcoin/bitcoin/message/get_headers.hpp>
#include <bitcoin/bitcoin/message/header.hpp>
#include <bitcoin/bitcoin/message/headers.hpp>
#include <bitcoin/bitcoin/message/heading.hpp>
#include <bitcoin/bitcoin/message/inventory.hpp>
#include <bitcoin/bitcoin/message/inventory_vector.hpp>
#include <bitcoin/bitcoin/message/memory_pool.hpp>
#include <bitcoin/bitcoin/message/merkle_block.hpp>
#include <bitcoin/infrastructure/math/uint256.hpp>
#include <bitcoin/infrastructure/message/message_tools.hpp>
// #include <bitcoin/bitcoin/message/network_address.hpp>
#include <bitcoin/bitcoin/message/messages.hpp>
#include <bitcoin/bitcoin/message/not_found.hpp>
#include <bitcoin/bitcoin/message/ping.hpp>
#include <bitcoin/bitcoin/message/pong.hpp>
#include <bitcoin/bitcoin/message/prefilled_transaction.hpp>
#include <bitcoin/bitcoin/message/reject.hpp>
#include <bitcoin/bitcoin/message/send_compact.hpp>
#include <bitcoin/bitcoin/message/send_headers.hpp>
#include <bitcoin/bitcoin/message/transaction.hpp>
#include <bitcoin/bitcoin/message/verack.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
// #include <bitcoin/infrastructure/unicode/console_streambuf.hpp>
// #include <bitcoin/infrastructure/unicode/file_lock.hpp>
// #include <bitcoin/infrastructure/unicode/ifstream.hpp>
// #include <bitcoin/infrastructure/unicode/ofstream.hpp>
// #include <bitcoin/infrastructure/unicode/unicode.hpp>
// #include <bitcoin/infrastructure/unicode/unicode_istream.hpp>
// #include <bitcoin/infrastructure/unicode/unicode_ostream.hpp>
// #include <bitcoin/infrastructure/unicode/unicode_streambuf.hpp>
// #include <bitcoin/infrastructure/utility/array_slice.hpp>
// #include <bitcoin/infrastructure/utility/asio.hpp>
// #include <bitcoin/infrastructure/utility/assert.hpp>
// #include <bitcoin/infrastructure/utility/atomic.hpp>
// #include <bitcoin/infrastructure/utility/binary.hpp>
// #include <bitcoin/infrastructure/utility/collection.hpp>
// #include <bitcoin/infrastructure/utility/color.hpp>
// #include <bitcoin/infrastructure/utility/conditional_lock.hpp>
// #include <bitcoin/infrastructure/utility/container_sink.hpp>
// #include <bitcoin/infrastructure/utility/container_source.hpp>
// #include <bitcoin/infrastructure/utility/data.hpp>
// #include <bitcoin/infrastructure/utility/deadline.hpp>
// #include <bitcoin/infrastructure/utility/decorator.hpp>
// #include <bitcoin/infrastructure/utility/delegates.hpp>
// #include <bitcoin/infrastructure/utility/deserializer.hpp>
// #include <bitcoin/infrastructure/utility/dispatcher.hpp>
// #include <bitcoin/infrastructure/utility/enable_shared_from_base.hpp>
// #include <bitcoin/infrastructure/utility/endian.hpp>
// #include <bitcoin/infrastructure/utility/exceptions.hpp>
// #include <bitcoin/infrastructure/utility/flush_lock.hpp>
// #include <bitcoin/infrastructure/utility/interprocess_lock.hpp>
// #include <bitcoin/infrastructure/utility/istream_reader.hpp>
// #include <bitcoin/infrastructure/utility/monitor.hpp>
// #include <bitcoin/infrastructure/utility/noncopyable.hpp>
// #include <bitcoin/infrastructure/utility/ostream_writer.hpp>
// #include <bitcoin/infrastructure/utility/pending.hpp>
// #include <bitcoin/infrastructure/utility/png.hpp>
// #include <bitcoin/infrastructure/utility/prioritized_mutex.hpp>
// #include <bitcoin/infrastructure/utility/pseudo_random.hpp>
// #include <bitcoin/infrastructure/utility/reader.hpp>
// #include <bitcoin/infrastructure/utility/resubscriber.hpp>
// #include <bitcoin/infrastructure/utility/scope_lock.hpp>
// #include <bitcoin/infrastructure/utility/sequencer.hpp>
// #include <bitcoin/infrastructure/utility/sequential_lock.hpp>
// #include <bitcoin/infrastructure/utility/serializer.hpp>
// #include <bitcoin/infrastructure/utility/socket.hpp>
// #include <bitcoin/infrastructure/utility/string.hpp>
// #include <bitcoin/infrastructure/utility/subscriber.hpp>
// #include <bitcoin/infrastructure/utility/synchronizer.hpp>
// #include <bitcoin/infrastructure/utility/thread.hpp>
// #include <bitcoin/infrastructure/utility/threadpool.hpp>
// #include <bitcoin/infrastructure/utility/timer.hpp>
// #include <bitcoin/infrastructure/utility/track.hpp>
// #include <bitcoin/infrastructure/utility/work.hpp>
// #include <bitcoin/infrastructure/utility/writer.hpp>

// #include <bitcoin/bitcoin/utility/socket.hpp>

#include <bitcoin/bitcoin/wallet/bitcoin_uri.hpp>
// #include <bitcoin/bitcoin/wallet/dictionary.hpp>
#include <bitcoin/bitcoin/wallet/ec_private.hpp>
#include <bitcoin/bitcoin/wallet/ec_public.hpp>
#include <bitcoin/bitcoin/wallet/ek_private.hpp>
#include <bitcoin/bitcoin/wallet/ek_public.hpp>
#include <bitcoin/bitcoin/wallet/ek_token.hpp>
#include <bitcoin/bitcoin/wallet/encrypted_keys.hpp>
// #include <bitcoin/bitcoin/wallet/hd_private.hpp>
// #include <bitcoin/bitcoin/wallet/hd_public.hpp>
#include <bitcoin/bitcoin/wallet/message.hpp>
// #include <bitcoin/bitcoin/wallet/mini_keys.hpp>
// #include <bitcoin/bitcoin/wallet/mnemonic.hpp>
#include <bitcoin/bitcoin/wallet/payment_address.hpp>
// #include <bitcoin/bitcoin/wallet/qrcode.hpp>
#include <bitcoin/bitcoin/wallet/select_outputs.hpp>
#include <bitcoin/bitcoin/wallet/stealth_address.hpp>
#include <bitcoin/bitcoin/wallet/stealth_receiver.hpp>
#include <bitcoin/bitcoin/wallet/stealth_sender.hpp>
// #include <bitcoin/bitcoin/wallet/uri.hpp>
#include <bitcoin/bitcoin/wallet/uri_reader.hpp>

#include <bitcoin/infrastructure.hpp>

#endif

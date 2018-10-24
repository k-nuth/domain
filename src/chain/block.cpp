/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
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
#include <bitcoin/bitcoin/chain/block.hpp>

#include <algorithm>
#include <cfenv>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <numeric>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <boost/range/adaptor/reversed.hpp>

#include <bitcoin/bitcoin/chain/chain_state.hpp>
#include <bitcoin/bitcoin/chain/compact.hpp>
#include <bitcoin/bitcoin/chain/input_point.hpp>
#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/message/messages.hpp>
#include <bitcoin/bitcoin/multi_crypto_support.hpp>
#include <bitcoin/infrastructure/config/checkpoint.hpp>
#include <bitcoin/infrastructure/error.hpp>
#include <bitcoin/infrastructure/formats/base_16.hpp>
#include <bitcoin/infrastructure/machine/number.hpp>
#include <bitcoin/infrastructure/machine/opcode.hpp>
#include <bitcoin/infrastructure/machine/rule_fork.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/asio.hpp>
#include <bitcoin/infrastructure/utility/assert.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace chain {

using namespace bc::config;
using namespace bc::machine;
using namespace boost::adaptors;

#ifdef BITPRIM_CURRENCY_LTC
//Litecoin mainnet genesis block
static std::string const encoded_mainnet_genesis_block =
    "01000000"                                                                                                                                          //version
    "0000000000000000000000000000000000000000000000000000000000000000"                                                                                  //prev hash
    "d9ced4ed1130f7b7faad9be25323ffafa33232a17c3edf6cfd97bee6bafbdd97"                                                                                  //merkle root le *
    "b9aa8e4e"                                                                                                                                          //timestamp le *
    "f0ff0f1e"                                                                                                                                          //bits =
    "cd513f7c"                                                                                                                                          //nonce X
    "01"                                                                                                                                                //nro txs
    "01000000"                                                                                                                                          //version
    "01"                                                                                                                                                // inputs
    "0000000000000000000000000000000000000000000000000000000000000000ffffffff"                                                                          //prev output
    "48"                                                                                                                                                //script length
    "04ffff001d0104404e592054696d65732030352f4f63742f32303131205374657665204a6f62732c204170706c65e280997320566973696f6e6172792c2044696573206174203536"  //scriptsig
    "ffffffff"                                                                                                                                          //sequence
    "01"                                                                                                                                                //outputs
    "00f2052a01000000"                                                                                                                                  //50 btc
    "43"                                                                                                                                                //pk_script length
    "41040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9ac"            //pk_script
    "00000000";                                                                                                                                         //locktime

//Litecoin testnet genesis block
static std::string const encoded_testnet_genesis_block =
    "01000000"                                                                                                                                          //version
    "0000000000000000000000000000000000000000000000000000000000000000"                                                                                  //prev hash
    "d9ced4ed1130f7b7faad9be25323ffafa33232a17c3edf6cfd97bee6bafbdd97"                                                                                  //merkle root le
    "f60ba158"                                                                                                                                          //timestamp le
    "f0ff0f1e"                                                                                                                                          //bits
    "e1790400"                                                                                                                                          //nonce
    "01"                                                                                                                                                //nro txs
    "01000000"                                                                                                                                          //version
    "01"                                                                                                                                                // inputs
    "0000000000000000000000000000000000000000000000000000000000000000ffffffff"                                                                          //prev output
    "48"                                                                                                                                                //script length
    "04ffff001d0104404e592054696d65732030352f4f63742f32303131205374657665204a6f62732c204170706c65e280997320566973696f6e6172792c2044696573206174203536"  //scriptsig
    "ffffffff"                                                                                                                                          //sequence
    "01"                                                                                                                                                //outputs
    "00f2052a01000000"                                                                                                                                  //50 btc
    "43"                                                                                                                                                //pk_script length
    "41040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9ac"            //pk_script
    "00000000";                                                                                                                                         //locktime
#else                                                                                                                                                   //BITPRIM_CURRENCY_LTC

static std::string const encoded_mainnet_genesis_block =
    "01000000"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "3ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
    "29ab5f49"
    "ffff001d"
    "1dac2b7c"
    "01"
    "01000000"
    "01"
    "0000000000000000000000000000000000000000000000000000000000000000ffffffff"
    "4d"
    "04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73"
    "ffffffff"
    "01"
    "00f2052a01000000"
    "43"
    "4104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac"
    "00000000";

static std::string const encoded_testnet_genesis_block =
    "01000000"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "3ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
    "dae5494d"
    "ffff001d"
    "1aa4ae18"
    "01"
    "01000000"
    "01"
    "0000000000000000000000000000000000000000000000000000000000000000ffffffff"
    "4d"
    "04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73"
    "ffffffff"
    "01"
    "00f2052a01000000"
    "43"
    "4104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac"
    "00000000";
#endif                                                                                                                                                  //BITPRIM_CURRENCY_LTC

static std::string const encoded_regtest_genesis_block =
    "01000000"
    "0000000000000000000000000000000000000000000000000000000000000000"
    "3ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
    "dae5494d"
    "ffff7f20"
    "02000000"
    "01"
    "01000000"
    "01"
    "0000000000000000000000000000000000000000000000000000000000000000ffffffff"
    "4d"
    "04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73"
    "ffffffff"
    "01"
    "00f2052a01000000"
    "43"
    "4104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac"
    "00000000";

// Constructors.
//-----------------------------------------------------------------------------

block::block()
    : validation{} {
}

// TODO(libbitcoin): deal with possibility of inconsistent merkle root in relation to txs.
block::block(chain::header const& header, transaction::list const& transactions)
    : header_(header), transactions_(transactions), validation{} {
}

// TODO(libbitcoin): deal with possibility of inconsistent merkle root in relation to txs.
block::block(chain::header const& header, transaction::list&& transactions)
    : header_(header), transactions_(std::move(transactions)), validation{} {
}

block::block(block const& x)
    : block(x.header_, x.transactions_) {
    validation = x.validation;
}

block::block(block&& x) noexcept
    : block(x.header_, std::move(x.transactions_)) {
    validation = std::move(x.validation);
}

// Operators.
//-----------------------------------------------------------------------------

block& block::operator=(block&& x) noexcept {
    header_ = x.header_;
    transactions_ = std::move(x.transactions_);
    validation = std::move(x.validation);
    return *this;
}

bool block::operator==(block const& x) const {
    return (header_ == x.header_) && (transactions_ == x.transactions_);
}

bool block::operator!=(block const& x) const {
    return !(*this == x);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
block block::factory_from_data(data_chunk const& data, bool witness) {
    block instance;
    instance.from_data(data, witness_val(witness));
    return instance;
}

// static
block block::factory_from_data(data_source& stream, bool witness) {
    block instance;
    instance.from_data(stream, witness_val(witness));
    return instance;
}

// static
//block block::factory_from_data(reader& source, bool witness)
//{
//#ifdef BITPRIM_CURRENCY_BCH
//    witness = false;
//#endif
//    block instance;
//    instance.from_data(source, witness);
//    return instance;
//}

bool block::from_data(data_chunk const& data, bool witness) {
    data_source istream(data);
    return from_data(istream, witness_val(witness));
}

bool block::from_data(data_source& stream, bool witness) {
    istream_reader stream_r(stream);
    return from_data(stream_r, witness_val(witness));
}

// Full block deserialization is always canonical encoding.
//bool block::from_data(reader& source, bool witness)
//{
//#ifdef BITPRIM_CURRENCY_BCH
//    witness = false;
//#endif
//    validation.start_deserialize = asio::steady_clock::now();
//    reset();
//
//    if ( ! header_.from_data(source, true))
//        return false;
//
//    auto const count = source.read_size_little_endian();
//
//    // Guard against potential for arbitary memory allocation.
//    if (count > get_max_block_size())
//        source.invalidate();
//    else
//        transactions_.resize(count);
//
//    // Order is required, explicit loop allows early termination.
//    for (auto& tx: transactions_)
//        if ( ! tx.from_data(source, true, witness))
//            break;
//
//    // TODO(libbitcoin): optimize by having reader skip witness data.
//    if ( ! witness)
//        strip_witness();
//
//    if ( ! source)
//        reset();
//
//    validation.end_deserialize = asio::steady_clock::now();
//    return source;
//}

// private
void block::reset() {
    header_.reset();
    transactions_.clear();
    transactions_.shrink_to_fit();
}

bool block::is_valid() const {
    return !transactions_.empty() || header_.is_valid();
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk block::to_data(bool witness) const {
    data_chunk data;
    auto const size = serialized_size(witness_val(witness));
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void block::to_data(data_sink& stream, bool witness) const {
    ostream_writer sink_w(stream);
    to_data(sink_w, witness_val(witness));
}

// Full block serialization is always canonical encoding.
//void block::to_data(writer& sink, bool witness) const
//{
//#ifdef BITPRIM_CURRENCY_BCH
//    witness = false;
//#endif
//    header_.to_data(sink, true);
//    sink.write_size_little_endian(transactions_.size());
//    auto const to = [&sink, witness](transaction const& tx)
//    {
//        tx.to_data(sink, true, witness);
//    };
//
//    std::for_each(transactions_.begin(), transactions_.end(), to);
//}

hash_list block::to_hashes(bool witness) const {
    hash_list out;
    out.reserve(transactions_.size());
    auto const to_hash = [&out, witness](transaction const& tx) {
        out.push_back(tx.hash(witness_val(witness)));
    };

    // Hash ordering matters, don't use std::transform here.
    std::for_each(transactions_.begin(), transactions_.end(), to_hash);
    return out;
}

// Properties (size, accessors, cache).
//-----------------------------------------------------------------------------

// Full block serialization is always canonical encoding.
size_t block::serialized_size(bool witness) const {
    size_t value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (witness_val(witness) && total_size_ != boost::none) {
        value = total_size_.get();
        mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    if ( ! witness_val(witness) && base_size_ != boost::none) {
        value = base_size_.get();
        mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    mutex_.unlock_upgrade_and_lock();
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    auto const sum = [witness](size_t total, transaction const& tx) {
        return safe_add(total, tx.serialized_size(true, witness_val(witness)));
    };

    auto const& txs = transactions_;
    value = header_.serialized_size(true) +
            message::variable_uint_size(transactions_.size()) +
            std::accumulate(txs.begin(), txs.end(), size_t(0), sum);

    if (witness_val(witness)) {
        total_size_ = value;
    } else {
        base_size_ = value;
    }

    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

chain::header& block::header() {
    return header_;
}

chain::header const& block::header() const {
    return header_;
}

// TODO(libbitcoin): must call header.set_merkle(generate_merkle_root()) though this may
// be very suboptimal if the block is being constructed. First verify that all
// current uses will not be impacted and if so change them to use constructor.
void block::set_header(chain::header const& value) {
    header_ = value;
}

transaction::list& block::transactions() {
    return transactions_;
}

transaction::list const& block::transactions() const {
    return transactions_;
}

// TODO(libbitcoin): see set_header comments.
void block::set_transactions(transaction::list const& value) {
    transactions_ = value;
    segregated_ = boost::none;
    total_inputs_ = boost::none;
    base_size_ = boost::none;
    total_size_ = boost::none;
}

// TODO(libbitcoin): see set_header comments.
void block::set_transactions(transaction::list&& value) {
    transactions_ = std::move(value);
    segregated_ = boost::none;
    total_inputs_ = boost::none;
    base_size_ = boost::none;
    total_size_ = boost::none;
}

// Convenience property.
hash_digest block::hash() const {
    return header_.hash();
}

// Utilities.
//-----------------------------------------------------------------------------

chain::block block::genesis_mainnet() {
    data_chunk data;
    decode_base16(data, encoded_mainnet_genesis_block);
    auto const genesis = chain::block::factory_from_data(data);

    BITCOIN_ASSERT(genesis.is_valid());
    BITCOIN_ASSERT(genesis.transactions().size() == 1);
    BITCOIN_ASSERT(genesis.generate_merkle_root() == genesis.header().merkle());
    return genesis;
}

chain::block block::genesis_testnet() {
    data_chunk data;
    decode_base16(data, encoded_testnet_genesis_block);
    auto const genesis = chain::block::factory_from_data(data);

    BITCOIN_ASSERT(genesis.is_valid());
    BITCOIN_ASSERT(genesis.transactions().size() == 1);
    BITCOIN_ASSERT(genesis.generate_merkle_root() == genesis.header().merkle());
    return genesis;
}

chain::block block::genesis_regtest() {
    data_chunk data;
    decode_base16(data, encoded_regtest_genesis_block);
    auto const genesis = chain::block::factory_from_data(data);

    BITCOIN_ASSERT(genesis.is_valid());
    BITCOIN_ASSERT(genesis.transactions().size() == 1);
    BITCOIN_ASSERT(genesis.generate_merkle_root() == genesis.header().merkle());
    return genesis;
}

// With a 32 bit chain the size of the result should not exceed 43 and with a
// 64 bit chain should not exceed 75, using a limit of: 10 + log2(height) + 1.
size_t block::locator_size(size_t top) {
    // Set rounding behavior, not consensus-related, thread side effect :<.
    std::fesetround(FE_UPWARD);

    auto const first_ten_or_top = std::min(size_t(10), top);
    auto const remaining = top - first_ten_or_top;
    auto const back_off = remaining == 0 ? 0.0 : remaining == 1 ? 1.0 : std::log2(remaining);
    auto const rounded_up_log = static_cast<size_t>(std::nearbyint(back_off));
    return first_ten_or_top + rounded_up_log + size_t(1);
}

// This algorithm is a network best practice, not a consensus rule.
block::indexes block::locator_heights(size_t top) {
    size_t step = 1;
    block::indexes heights;
    auto const reservation = locator_size(top);
    heights.reserve(reservation);

    // Start at the top of the chain and work backwards to zero.
    for (auto height = top; height > 0; height = floor_subtract(height, step)) {
        // Push top 10 indexes first, then back off exponentially.
        if (heights.size() >= 10) {
            step <<= 1;
        }

        heights.push_back(height);
    }

    // Push the genesis block index.
    heights.push_back(0);

    // Validate the reservation computation.
    BITCOIN_ASSERT(heights.size() <= reservation);
    return heights;
}

// Utilities.
//-----------------------------------------------------------------------------

// Clear witness from all inputs (does not change default transaction hash).
void block::strip_witness() {
    auto const strip = [](transaction& transaction) {
        transaction.strip_witness();
    };

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    unique_lock lock(mutex_);

    segregated_ = false;
    total_size_ = boost::none;
    std::for_each(transactions_.begin(), transactions_.end(), strip);
    ///////////////////////////////////////////////////////////////////////////
}

// Validation helpers.
//-----------------------------------------------------------------------------

// [GetBlockProof]
uint256_t block::proof() const {
    return header_.proof();
}

uint64_t block::subsidy(size_t height, bool retarget) {
    static auto const overflow = sizeof(uint64_t) * byte_bits;
    auto subsidy = initial_block_subsidy_satoshi();
    auto const halvings = height / subsidy_interval(retarget);
    subsidy >>= (halvings >= overflow ? 0 : halvings);
    return subsidy;
}

// Returns max_size_t in case of overflow or unpopulated chain state.
size_t block::signature_operations() const {
    auto const state = validation.state;
    auto const bip16 = state->is_enabled(rule_fork::bip16_rule);
#ifdef BITPRIM_CURRENCY_BCH
    auto const bip141 = false;  // No segwit
#else
    auto const bip141 = state->is_enabled(rule_fork::bip141_rule);
#endif
    return state ? signature_operations(bip16, bip141) : max_size_t;
}

// Returns max_size_t in case of overflow.
size_t block::signature_operations(bool bip16, bool bip141) const {
#ifdef BITPRIM_CURRENCY_BCH
    bip141 = false;  // No segwit
#endif
    auto const value = [bip16, bip141](size_t total, transaction const& tx) {
        return ceiling_add(total, tx.signature_operations(bip16, bip141));
    };

    //*************************************************************************
    // CONSENSUS: Legacy sigops are counted in coinbase scripts despite the
    // fact that coinbase input scripts are never executed. There is no need
    // to exclude p2sh coinbase sigops since there is never a script to count.
    //*************************************************************************
    auto const& txs = transactions_;
    return std::accumulate(txs.begin(), txs.end(), size_t{0}, value);
}

size_t block::total_inputs(bool with_coinbase) const {
    size_t value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (total_inputs_ != boost::none) {
        value = total_inputs_.get();
        mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    mutex_.unlock_upgrade_and_lock();
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    auto const inputs = [](size_t total, transaction const& tx) {
        return safe_add(total, tx.inputs().size());
    };

    auto const& txs = transactions_;
    const size_t offset = with_coinbase ? 0 : 1;
    value = std::accumulate(txs.begin() + offset, txs.end(), size_t(0), inputs);
    total_inputs_ = value;
    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

size_t block::weight() const {
    // Block weight is 3 * Base size * + 1 * Total size (bip141).
    return base_size_contribution * serialized_size(false) +
           total_size_contribution * serialized_size(true);
}

// True if there is another coinbase other than the first tx.
// No txs or coinbases returns false.
bool block::is_extra_coinbases() const {
    if (transactions_.empty()) {
        return false;
    }

    auto const value = [](transaction const& tx) {
        return tx.is_coinbase();
    };

    auto const& txs = transactions_;
    return std::any_of(txs.begin() + 1, txs.end(), value);
}

bool block::is_final(size_t height, uint32_t block_time) const {
    auto const value = [=](transaction const& tx) {
        return tx.is_final(height, block_time);
    };

    auto const& txs = transactions_;
    return std::all_of(txs.begin(), txs.end(), value);
}

// Distinctness is defined by transaction hash.
bool block::is_distinct_transaction_set() const {
    auto const hasher = [](transaction const& tx) { return tx.hash(); };
    auto const& txs = transactions_;
    hash_list hashes(txs.size());
    std::transform(txs.begin(), txs.end(), hashes.begin(), hasher);
    std::sort(hashes.begin(), hashes.end());
    auto const distinct_end = std::unique(hashes.begin(), hashes.end());
    return distinct_end == hashes.end();
}

hash_digest block::generate_merkle_root(bool witness) const {
    if (transactions_.empty()) {
        return null_hash;
    }

    hash_list update;
    auto merkle = to_hashes(witness_val(witness));

    // Initial capacity is half of the original list (clear doesn't reset).
    update.reserve((merkle.size() + 1) / 2);

    while (merkle.size() > 1) {
        // If number of hashes is odd, duplicate last hash in the list.
        if (merkle.size() % 2 != 0) {
            merkle.push_back(merkle.back());
        }

        for (auto it = merkle.begin(); it != merkle.end(); it += 2) {
            update.push_back(bitcoin_hash(build_chunk({it[0], it[1]})));
        }

        std::swap(merkle, update);
        update.clear();
    }

    // There is now only one item in the list.
    return merkle.front();
}

size_t block::non_coinbase_input_count() const {
    if (transactions_.empty()) {
        return 0;
    }

    auto const counter = [](size_t sum, transaction const& tx) {
        return sum + tx.inputs().size();
    };

    auto const& txs = transactions_;
    return std::accumulate(txs.begin() + 1, txs.end(), size_t(0), counter);
}

//****************************************************************************
// CONSENSUS: This is only necessary because satoshi stores and queries as it
// validates, imposing an otherwise unnecessary partial transaction ordering.
//*****************************************************************************
bool block::is_forward_reference() const {
    std::unordered_map<hash_digest, bool> hashes(transactions_.size());
    auto const is_forward = [&hashes](input const& input) {
        return hashes.count(input.previous_output().hash()) != 0;
    };

    for (auto const& tx : reverse(transactions_)) {
        hashes.emplace(tx.hash(), true);

        if (std::any_of(tx.inputs().begin(), tx.inputs().end(), is_forward)) {
            return true;
        }
    }

    return false;
}

// This is an early check that is redundant with block pool accept checks.
bool block::is_internal_double_spend() const {
    if (transactions_.empty()) {
        return false;
    }

    point::list outs;
    outs.reserve(non_coinbase_input_count());
    auto const& txs = transactions_;

    // Merge the prevouts of all non-coinbase transactions into one set.
    for (auto tx = txs.begin() + 1; tx != txs.end(); ++tx) {
        auto out = tx->previous_outputs();
        std::move(out.begin(), out.end(), std::inserter(outs, outs.end()));
    }

    std::sort(outs.begin(), outs.end());
    auto const distinct_end = std::unique(outs.begin(), outs.end());
    auto const distinct = (distinct_end == outs.end());
    return !distinct;
}

bool block::is_valid_merkle_root() const {
    return generate_merkle_root() == header_.merkle();
}

// Overflow returns max_uint64.
uint64_t block::fees() const {
    ////static_assert(max_money() < max_uint64, "overflow sentinel invalid");
    auto const value = [](uint64_t total, transaction const& tx) {
        return ceiling_add(total, tx.fees());
    };

    auto const& txs = transactions_;
    return std::accumulate(txs.begin(), txs.end(), uint64_t{0}, value);
}

uint64_t block::claim() const {
    return transactions_.empty() ? 0 : transactions_.front().total_output_value();
}

// Overflow returns max_uint64.
uint64_t block::reward(size_t height) const {
    ////static_assert(max_money() < max_uint64, "overflow sentinel invalid");
    return ceiling_add(fees(), subsidy(height));
}

bool block::is_valid_coinbase_claim(size_t height) const {
    return claim() <= reward(height);
}

bool block::is_valid_coinbase_script(size_t height) const {
    if (transactions_.empty() || transactions_.front().inputs().empty()) {
        return false;
    }

    auto const& script = transactions_.front().inputs().front().script();
    return script::is_coinbase_pattern(script.operations(), height);
}

bool block::is_valid_witness_commitment() const {
#ifdef BITPRIM_CURRENCY_BCH
    return false;
#endif
    if (transactions_.empty() || transactions_.front().inputs().empty()) {
        return false;
    }

    hash_digest reserved, committed;
    auto const& coinbase = transactions_.front();

    // Last output of commitment pattern holds committed value (bip141).
    if (coinbase.inputs().front().extract_reserved_hash(reserved)) {
        for (auto const& output : reverse(coinbase.outputs())) {
            if (output.extract_committed_hash(committed)) {
                return committed == bitcoin_hash(build_chunk({generate_merkle_root(true), reserved}));
            }
        }
    }

    // If no txs in block are segregated the commitment is optional (bip141).
    return !is_segregated();
}

bool block::is_segregated() const {
#ifdef BITPRIM_CURRENCY_BCH
    return false;
#endif
    bool value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (segregated_ != boost::none) {
        value = segregated_.get();
        mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    mutex_.unlock_upgrade_and_lock();
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    auto const segregated = [](transaction const& tx) {
        return tx.is_segregated();
    };

    // If no block tx has witness data the commitment is optional (bip141).
    value = std::any_of(transactions_.begin(), transactions_.end(), segregated);

    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

code block::check_transactions() const {
    code ec;

    for (auto const& tx : transactions_) {
        if ((ec = tx.check(false))) {
            return ec;
        }
    }

    return error::success;
}

code block::accept_transactions(const chain_state& state) const {
    code ec;

    for (auto const& tx : transactions_) {
        if ((ec = tx.accept(state, false))) {
            return ec;
        }
    }

    return error::success;
}

code block::connect_transactions(const chain_state& state) const {
    code ec;

    for (auto const& tx : transactions_) {
        if ((ec = tx.connect(state))) {
            return ec;
        }
    }

    return error::success;
}

// Validation.
//-----------------------------------------------------------------------------

// These checks are self-contained; blockchain (and so version) independent.
code block::check() const {
    validation.start_check = asio::steady_clock::now();

    code ec;

    if ((ec = header_.check())) {
        return ec;

        // TODO(libbitcoin): relates to total of tx.size(false) (pool cache). -> no witness size
    } if (serialized_size(false) > get_max_block_size()) {
        return error::block_size_limit;

    } if (transactions_.empty()) {
        return error::empty_block;

    } if ( ! transactions_.front().is_coinbase()) {
        return error::first_not_coinbase;

    } if (is_extra_coinbases()) {
        return error::extra_coinbases;

        // TODO(libbitcoin): determinable from tx pool graph.
    } if (is_forward_reference()) {
        return error::forward_reference;

        // This is subset of is_internal_double_spend if collisions cannot happen.
        ////else if ( ! is_distinct_transaction_set())
        ////    return error::internal_duplicate;

        // TODO(libbitcoin): determinable from tx pool graph.
    } if (is_internal_double_spend()) {
        return error::block_internal_double_spend;

        // TODO(libbitcoin): relates height to tx.hash(false) (pool cache).
    } if ( ! is_valid_merkle_root()) {
        return error::merkle_mismatch;

        // We cannot know if bip16 is enabled at this point so we disable it.
        // This will not make a difference unless prevouts are populated, in which
        // case they are ignored. This means that p2sh sigops are not counted here.
        // This is a preliminary check, the final count must come from connect().
        // Reenable once sigop caching is implemented, otherwise is deoptimization.
        ////else if (signature_operations(false, false) > get_max_block_sigops())
        ////    return error::block_legacy_sigop_limit;

    } 
        return check_transactions();
    
}

code block::accept(bool transactions) const {
    auto const state = validation.state;
    return state ? accept(*state, transactions) : error::operation_failed;
}

// These checks assume that prevout caching is completed on all tx.inputs.
code block::accept(chain_state const& state, bool transactions) const {
    validation.start_accept = asio::steady_clock::now();

    code ec;
    auto const bip16 = state.is_enabled(rule_fork::bip16_rule);
    auto const bip34 = state.is_enabled(rule_fork::bip34_rule);
    auto const bip113 = state.is_enabled(rule_fork::bip113_rule);
#ifdef BITPRIM_CURRENCY_BCH
    auto const bip141 = false;  // No segwit
#else
    auto const bip141 = state.is_enabled(rule_fork::bip141_rule);
#endif

    auto const max_sigops = bip141 ? max_fast_sigops : get_max_block_sigops();
    auto const block_time = bip113 ? state.median_time_past() : header_.timestamp();

    size_t allowed_sigops = get_allowed_sigops(serialized_size());

    if ((ec = header_.accept(state))) {
        return ec;

        //In Bitcoin Cash, block size check is now dependent on the Blockchain state.
#if defined(BITPRIM_CURRENCY_BCH)
    } if ( ! state.is_monolith_enabled() && serialized_size() > max_block_size_old) {
        return error::block_size_limit;
#endif

    } if (state.is_under_checkpoint()) {
        return error::success;

        // TODO(libbitcoin): relates height to total of tx.size(true) (pool cache).
        // NOTE: for BCH bit141 is set as false
    } if (bip141 && weight() > max_block_weight) {
        return error::block_weight_limit;

    } if (bip34 && !is_valid_coinbase_script(state.height())) {
        return error::coinbase_height_mismatch;

        // TODO(libbitcoin): relates height to total of tx.fee (pool cach).
    } if ( ! is_valid_coinbase_claim(state.height())) {
        return error::coinbase_value_limit;

        // TODO(libbitcoin): relates median time past to tx.locktime (pool cache min tx.time).
    } if ( ! is_final(state.height(), block_time)) {
        return error::block_non_final;

        // TODO(libbitcoin): relates height to tx.hash(true) (pool cache).
        // NOTE: for BCH bit141 is set as false
    } if (bip141 && !is_valid_witness_commitment()) {
        return error::invalid_witness_commitment;

        // TODO(libbitcoin): determine if performance benefit is worth excluding sigops here.
        // TODO(libbitcoin): relates block limit to total of tx.sigops (pool cache tx.sigops).
        // This recomputes sigops to include p2sh from prevouts.
        // NOTE: for BCH bit141 is set as false
    } if (transactions && (signature_operations(bip16, bip141) > allowed_sigops)) {
        return error::block_embedded_sigop_limit;

    } if (transactions) {
        return accept_transactions(state);

    } 
        return ec;
    
}

code block::connect() const {
    auto const state = validation.state;
    return state ? connect(*state) : error::operation_failed;
}

code block::connect(const chain_state& state) const {
    validation.start_connect = asio::steady_clock::now();

    if (state.is_under_checkpoint()) {
        return error::success;
    } 
        return connect_transactions(state);
    
}

}  // namespace chain
}  // namespace libbitcoin

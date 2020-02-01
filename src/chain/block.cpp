// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

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
// #include <bitcoin/infrastructure/message/message_tools.hpp>
#include <bitcoin/bitcoin/machine/opcode.hpp>
#include <bitcoin/bitcoin/machine/rule_fork.hpp>
#include <bitcoin/bitcoin/multi_crypto_support.hpp>
#include <bitcoin/infrastructure/config/checkpoint.hpp>
#include <bitcoin/infrastructure/error.hpp>
#include <bitcoin/infrastructure/formats/base_16.hpp>
#include <bitcoin/infrastructure/machine/number.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/message/message_tools.hpp>
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

#ifdef KTH_CURRENCY_LTC
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
    "00000000";     //NOLINT                                                                                                                            //locktime

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
    "00000000";           //NOLINT                                                                                                                      //locktime
#else  //KTH_CURRENCY_LTC

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
    "00000000"; //NOLINT

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
    "00000000"; //NOLINT
#endif  //KTH_CURRENCY_LTC

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
    "00000000";  //NOLINT

// Constructors.
//-----------------------------------------------------------------------------

// TODO(libbitcoin): deal with possibility of inconsistent merkle root in relation to txs.
block::block(chain::header const& header, transaction::list const& transactions)
    : block_basis(header, transactions)
{}

// TODO(libbitcoin): deal with possibility of inconsistent merkle root in relation to txs.
block::block(chain::header const& header, transaction::list&& transactions)
    : block_basis(header, std::move(transactions))
{}

block::block(block const& x)
    : block_basis(x)
    , validation(x.validation)
{}

block::block(block&& x) noexcept
    : block_basis(std::move(x))
    , validation(std::move(x.validation))
{}

block& block::operator=(block const& x) {
    block_basis::operator=(x);
    validation = x.validation;
    return *this;
}

block& block::operator=(block&& x) noexcept {
    block_basis::operator=(std::move(static_cast<block_basis&&>(x)));
    validation = std::move(x.validation);
    return *this;
}

// Operators.
//-----------------------------------------------------------------------------

// bool block::operator==(block const& x) const {
//     return (header_ == x.header_) && (transactions_ == x.transactions_);
// }

// bool block::operator!=(block const& x) const {
//     return !(*this == x);
// }

// Deserialization.
//-----------------------------------------------------------------------------

// static
block block::factory_from_data(data_chunk const& data, bool witness) {
    block instance;
    instance.from_data(data, witness_val(witness));
    return instance;
}

// static
block block::factory_from_data(std::istream& stream, bool witness) {
    block instance;
    instance.from_data(stream, witness_val(witness));
    return instance;
}

// static
//block block::factory_from_data(reader& source, bool witness)
//{
//#ifdef KTH_CURRENCY_BCH
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

bool block::from_data(std::istream& stream, bool witness) {
    istream_reader stream_r(stream);
    return from_data(stream_r, witness_val(witness));
}

// // private
// void block::reset() {
//     block_basis::reset();
//     header_.reset();
//     transactions_.clear();
//     transactions_.shrink_to_fit();
// }

// bool block::is_valid() const {
//     return !transactions_.empty() || header_.is_valid();
// }

// Serialization.
//-----------------------------------------------------------------------------

data_chunk block::to_data(bool witness) const {
    return block_basis::to_data(serialized_size(witness_val(witness)), witness);

    // data_chunk data;
    // auto const size = serialized_size(witness_val(witness));
    // data.reserve(size);
    // data_sink ostream(data);
    // to_data(ostream);
    // ostream.flush();
    // BITCOIN_ASSERT(data.size() == size);
    // return data;
}

// void block::to_data(data_sink& stream, bool witness) const {
//     ostream_writer sink_w(stream);
//     to_data(sink_w, witness_val(witness));
// }



// hash_list block::to_hashes(bool witness) const {
//     hash_list out;
//     out.reserve(transactions_.size());
//     auto const to_hash = [&out, witness](transaction const& tx) {
//         out.push_back(tx.hash(witness_val(witness)));
//     };

//     // Hash ordering matters, don't use std::transform here.
//     std::for_each(transactions_.begin(), transactions_.end(), to_hash);
//     return out;
// }

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

    value = chain::serialized_size(*this, witness);

    if (witness_val(witness)) {
        total_size_ = value;
    } else {
        base_size_ = value;
    }

    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}


// TODO(libbitcoin): see set_header comments.
void block::set_transactions(transaction::list const& value) {
    block_basis::set_transactions(value);
    segregated_ = boost::none;
    total_inputs_ = boost::none;
    base_size_ = boost::none;
    total_size_ = boost::none;
}

// TODO(libbitcoin): see set_header comments.
void block::set_transactions(transaction::list&& value) {
    block_basis::set_transactions(std::move(value));
    segregated_ = boost::none;
    total_inputs_ = boost::none;
    base_size_ = boost::none;
    total_size_ = boost::none;
}

// // Convenience property.
// hash_digest block::hash() const {
//     return header_.hash();
// }

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
            step <<= 1u;
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

#ifndef KTH_CURRENCY_BCH
// Clear witness from all inputs (does not change default transaction hash).
void block::strip_witness() {
    auto const strip = [](transaction& transaction) {
        transaction.strip_witness();
    };

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    unique_lock lock(mutex_);

    // segregated_ = false;
    // total_size_ = boost::none;
    // std::for_each(transactions_.begin(), transactions_.end(), strip);

    block_basis::strip_witness();

    ///////////////////////////////////////////////////////////////////////////
}
#endif

// Validation helpers.
//-----------------------------------------------------------------------------

// // [GetBlockProof]
// uint256_t block::proof() const {
//     return header_.proof();
// }

// uint64_t block::subsidy(size_t height, bool retarget) {
//     static auto const overflow = sizeof(uint64_t) * byte_bits;
//     auto subsidy = initial_block_subsidy_satoshi();
//     auto const halvings = height / subsidy_interval(retarget);
//     subsidy >>= (halvings >= overflow ? 0 : halvings);
//     return subsidy;
// }

// Returns max_size_t in case of overflow or unpopulated chain state.
size_t block::signature_operations() const {
    auto const state = validation.state;
    auto const bip16 = state->is_enabled(rule_fork::bip16_rule);
#ifdef KTH_CURRENCY_BCH
    auto const bip141 = false;  // No segwit
#else
    auto const bip141 = state->is_enabled(rule_fork::bip141_rule);
#endif
    return state ? block_basis::signature_operations(bip16, bip141) : max_size_t;
}

// // Returns max_size_t in case of overflow.
// size_t block::signature_operations(bool bip16, bool bip141) const {
// #ifdef KTH_CURRENCY_BCH
//     bip141 = false;  // No segwit
// #endif
//     auto const value = [bip16, bip141](size_t total, transaction const& tx) {
//         return ceiling_add(total, tx.signature_operations(bip16, bip141));
//     };

//     //*************************************************************************
//     // CONSENSUS: Legacy sigops are counted in coinbase scripts despite the
//     // fact that coinbase input scripts are never executed. There is no need
//     // to exclude p2sh coinbase sigops since there is never a script to count.
//     //*************************************************************************
//     auto const& txs = transactions_;
//     return std::accumulate(txs.begin(), txs.end(), size_t{0}, value);
// }

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

    value = chain::total_inputs(*this, with_coinbase);
    total_inputs_ = value;
    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

size_t block::weight() const {
    return chain::weight(serialized_size(true), serialized_size(false));
    // // Block weight is 3 * Base size * + 1 * Total size (bip141).
    // return base_size_contribution * serialized_size(false) +
    //        total_size_contribution * serialized_size(true);
}

// // True if there is another coinbase other than the first tx.
// // No txs or coinbases returns false.
// bool block::is_extra_coinbases() const {
//     if (transactions_.empty()) {
//         return false;
//     }

//     auto const value = [](transaction const& tx) {
//         return tx.is_coinbase();
//     };

//     auto const& txs = transactions_;
//     return std::any_of(txs.begin() + 1, txs.end(), value);
// }

// bool block::is_final(size_t height, uint32_t block_time) const {
//     auto const value = [=](transaction const& tx) {
//         return tx.is_final(height, block_time);
//     };

//     auto const& txs = transactions_;
//     return std::all_of(txs.begin(), txs.end(), value);
// }

// // Distinctness is defined by transaction hash.
// bool block::is_distinct_transaction_set() const {
//     auto const hasher = [](transaction const& tx) { return tx.hash(); };
//     auto const& txs = transactions_;
//     hash_list hashes(txs.size());
//     std::transform(txs.begin(), txs.end(), hashes.begin(), hasher);
//     std::sort(hashes.begin(), hashes.end());
//     auto const distinct_end = std::unique(hashes.begin(), hashes.end());
//     return distinct_end == hashes.end();
// }

// hash_digest block::generate_merkle_root(bool witness) const {
//     if (transactions_.empty()) {
//         return null_hash;
//     }

//     hash_list update;
//     auto merkle = to_hashes(witness_val(witness));

//     // Initial capacity is half of the original list (clear doesn't reset).
//     update.reserve((merkle.size() + 1) / 2);

//     while (merkle.size() > 1) {
//         // If number of hashes is odd, duplicate last hash in the list.
//         if (merkle.size() % 2 != 0) {
//             merkle.push_back(merkle.back());
//         }

//         for (auto it = merkle.begin(); it != merkle.end(); it += 2) {
//             update.push_back(bitcoin_hash(build_chunk({it[0], it[1]})));
//         }

//         std::swap(merkle, update);
//         update.clear();
//     }

//     // There is now only one item in the list.
//     return merkle.front();
// }

// size_t block::non_coinbase_input_count() const {
//     if (transactions_.empty()) {
//         return 0;
//     }

//     auto const counter = [](size_t sum, transaction const& tx) {
//         return sum + tx.inputs().size();
//     };

//     auto const& txs = transactions_;
//     return std::accumulate(txs.begin() + 1, txs.end(), size_t(0), counter);
// }

// //****************************************************************************
// // CONSENSUS: This is only necessary because satoshi stores and queries as it
// // validates, imposing an otherwise unnecessary partial transaction ordering.
// //*****************************************************************************
// bool block::is_forward_reference() const {
//     std::unordered_map<hash_digest, bool> hashes(transactions_.size());
//     auto const is_forward = [&hashes](input const& input) {
//         return hashes.count(input.previous_output().hash()) != 0;
//     };

//     for (auto const& tx : reverse(transactions_)) {
//         hashes.emplace(tx.hash(), true);

//         if (std::any_of(tx.inputs().begin(), tx.inputs().end(), is_forward)) {
//             return true;
//         }
//     }

//     return false;
// }

// // This is an early check that is redundant with block pool accept checks.
// bool block::is_internal_double_spend() const {
//     if (transactions_.empty()) {
//         return false;
//     }

//     point::list outs;
//     outs.reserve(non_coinbase_input_count());
//     auto const& txs = transactions_;

//     // Merge the prevouts of all non-coinbase transactions into one set.
//     for (auto tx = txs.begin() + 1; tx != txs.end(); ++tx) {
//         auto out = tx->previous_outputs();
//         std::move(out.begin(), out.end(), std::inserter(outs, outs.end()));
//     }

//     std::sort(outs.begin(), outs.end());
//     auto const distinct_end = std::unique(outs.begin(), outs.end());
//     auto const distinct = (distinct_end == outs.end());
//     return !distinct;
// }

// bool block::is_valid_merkle_root() const {
//     return generate_merkle_root() == header_.merkle();
// }

// // Overflow returns max_uint64.
// uint64_t block::fees() const {
//     ////static_assert(max_money() < max_uint64, "overflow sentinel invalid");
//     auto const value = [](uint64_t total, transaction const& tx) {
//         return ceiling_add(total, tx.fees());
//     };

//     auto const& txs = transactions_;
//     return std::accumulate(txs.begin(), txs.end(), uint64_t{0}, value);
// }

// uint64_t block::claim() const {
//     return transactions_.empty() ? 0 : transactions_.front().total_output_value();
// }

// // Overflow returns max_uint64.
// uint64_t block::reward(size_t height) const {
//     ////static_assert(max_money() < max_uint64, "overflow sentinel invalid");
//     return ceiling_add(fees(), subsidy(height));
// }

// bool block::is_valid_coinbase_claim(size_t height) const {
//     return claim() <= reward(height);
// }

// bool block::is_valid_coinbase_script(size_t height) const {
//     if (transactions_.empty() || transactions_.front().inputs().empty()) {
//         return false;
//     }

//     auto const& script = transactions_.front().inputs().front().script();
//     return script::is_coinbase_pattern(script.operations(), height);
// }

// bool block::is_valid_witness_commitment() const {
// #ifdef KTH_CURRENCY_BCH
//     return false;
// #else
//     if (transactions_.empty() || transactions_.front().inputs().empty()) {
//         return false;
//     }

//     hash_digest reserved, committed;
//     auto const& coinbase = transactions_.front();

//     // Last output of commitment pattern holds committed value (bip141).
//     if (coinbase.inputs().front().extract_reserved_hash(reserved)) {
//         for (auto const& output : reverse(coinbase.outputs())) {
//             if (output.extract_committed_hash(committed)) {
//                 return committed == bitcoin_hash(build_chunk({generate_merkle_root(true), reserved}));
//             }
//         }
//     }

//     // If no txs in block are segregated the commitment is optional (bip141).
//     return !is_segregated();
// #endif // KTH_CURRENCY_BCH
// }

bool block::is_segregated() const {
#ifdef KTH_CURRENCY_BCH
    return false;
#else
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

    // If no block tx has witness data the commitment is optional (bip141).
    value = chain::is_segregated(*this);

    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
#endif // KTH_CURRENCY_BCH
}

// code block::check_transactions() const {
//     code ec;

//     for (auto const& tx : transactions_) {
//         if ((ec = tx.check(false))) {
//             return ec;
//         }
//     }

//     return error::success;
// }

// code block::accept_transactions(chain_state const& state) const {
//     code ec;

//     for (auto const& tx : transactions_) {
//         if ((ec = tx.accept(state, false))) {
//             return ec;
//         }
//     }

//     return error::success;
// }

// code block::connect_transactions(chain_state const& state) const {
//     code ec;

//     for (auto const& tx : transactions_) {
//         if ((ec = tx.connect(state))) {
//             return ec;
//         }
//     }

//     return error::success;
// }

// Validation.
//-----------------------------------------------------------------------------

// These checks are self-contained; blockchain (and so version) independent.
code block::check() const {
    validation.start_check = asio::steady_clock::now();
    return block_basis::check(serialized_size(false));
}

code block::accept(bool transactions) const {
    auto const state = validation.state;
    return state ? accept(*state, transactions) : error::operation_failed;
}

// These checks assume that prevout caching is completed on all tx.inputs.
code block::accept(chain_state const& state, bool transactions) const {
    validation.start_accept = asio::steady_clock::now();
    return block_basis::accept(state, serialized_size(), weight(), transactions);
}

code block::connect() const {
    auto const state = validation.state;
    return state ? block_basis::connect(*state) : error::operation_failed;
}

// code block::connect(chain_state const& state) const {
//     validation.start_connect = asio::steady_clock::now();

//     if (state.is_under_checkpoint()) {
//         return error::success;
//     }
//     return connect_transactions(state);
// }

}  // namespace chain
}  // namespace kth

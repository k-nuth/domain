// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/block_basis.hpp>

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

#include <kth/domain/chain/chain_state.hpp>
#include <kth/domain/chain/compact.hpp>
#include <kth/domain/chain/input_point.hpp>
#include <kth/domain/chain/script.hpp>
#include <kth/domain/constants.hpp>
#include <kth/domain/machine/opcode.hpp>
#include <kth/domain/machine/rule_fork.hpp>
#include <kth/domain/multi_crypto_support.hpp>
#include <kth/infrastructure/config/checkpoint.hpp>
#include <kth/infrastructure/error.hpp>
#include <kth/infrastructure/formats/base_16.hpp>
#include <kth/infrastructure/machine/number.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/asio.hpp>
#include <kth/infrastructure/utility/assert.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/limits.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::domain::chain {

using namespace kth::domain::machine;
using namespace boost::adaptors;

#ifdef KTH_CURRENCY_LTC
//Litecoin mainnet genesis block
static
std::string const encoded_mainnet_genesis_block =
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
static
std::string const encoded_testnet_genesis_block =
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

static
std::string const encoded_mainnet_genesis_block =
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

static
std::string const encoded_testnet_genesis_block =
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

static
std::string const encoded_regtest_genesis_block =
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

// TODO(legacy): deal with possibility of inconsistent merkle root in relation to txs.
block_basis::block_basis(chain::header const& header, transaction::list const& transactions)
    : header_(header)
    , transactions_(transactions)
{}

// TODO(legacy): deal with possibility of inconsistent merkle root in relation to txs.
block_basis::block_basis(chain::header const& header, transaction::list&& transactions)
    : header_(header)
    , transactions_(std::move(transactions))
{}

// block_basis::block_basis(block_basis const& x)
//     : header_(x.header_)
//     , transactions_(x.transactions_)
// {}

// block_basis::block_basis(block_basis&& x) noexcept
//     : header_(x.header_)
//     , transactions_(std::move(x.transactions_))
// {}

// block_basis& block_basis::operator=(block_basis const& x) {
//     header_ = x.header_;
//     transactions_ = x.transactions_;
//     return *this;
// }

// block_basis& block_basis::operator=(block_basis&& x) noexcept {
//     header_ = x.header_;
//     transactions_ = std::move(x.transactions_);
//     return *this;
// }

// Operators.
//-----------------------------------------------------------------------------

bool block_basis::operator==(block_basis const& x) const {
    return (header_ == x.header_) && (transactions_ == x.transactions_);
}

bool block_basis::operator!=(block_basis const& x) const {
    return !(*this == x);
}

// Deserialization.
//-----------------------------------------------------------------------------

// private
void block_basis::reset() {
    header_.reset();
    transactions_.clear();
    transactions_.shrink_to_fit();
}

bool block_basis::is_valid() const {
    return !transactions_.empty() || header_.is_valid();
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk block_basis::to_data(size_t serialized_size, bool /*witness*/) const {
    data_chunk data;
    
    // auto const size = serialized_size(witness_val(witness));
    auto const size = serialized_size;

    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void block_basis::to_data(data_sink& stream, bool witness) const {
    ostream_writer sink_w(stream);
    to_data(sink_w, witness_val(witness));
}

hash_list block_basis::to_hashes(bool witness) const {
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

chain::header& block_basis::header() {
    return header_;
}

chain::header const& block_basis::header() const {
    return header_;
}

// TODO(legacy): must call header.set_merkle(generate_merkle_root()) though this may
// be very suboptimal if the block is being constructed. First verify that all
// current uses will not be impacted and if so change them to use constructor.
void block_basis::set_header(chain::header const& value) {
    header_ = value;
}

transaction::list& block_basis::transactions() {
    return transactions_;
}

transaction::list const& block_basis::transactions() const {
    return transactions_;
}

// TODO(legacy): see set_header comments.
void block_basis::set_transactions(transaction::list const& value) {
    transactions_ = value;
}

// TODO(legacy): see set_header comments.
void block_basis::set_transactions(transaction::list&& value) {
    transactions_ = std::move(value);
}

// Convenience property.
hash_digest block_basis::hash() const {
    return header_.hash();
}


// Utilities.
//-----------------------------------------------------------------------------

#if defined(KTH_SEGWIT_ENABLED)
// Clear witness from all inputs (does not change default transaction hash).
void strip_witness(block_basis& blk) {
    std::for_each(blk.transactions().begin(), blk.transactions().end(), [](transaction& tx) {
        tx.strip_witness();
    });
}
#endif

// Validation helpers.
//-----------------------------------------------------------------------------

// [GetBlockProof]
uint256_t block_basis::proof() const {
    return header_.proof();
}

// static
uint64_t block_basis::subsidy(size_t height, bool retarget) {
    static auto const overflow = sizeof(uint64_t) * byte_bits;
    auto subsidy = initial_block_subsidy_satoshi();
    auto const halvings = height / subsidy_interval(retarget);
    subsidy >>= (halvings >= overflow ? 0 : halvings);
    return subsidy;
}

// Returns max_size_t in case of overflow.
size_t block_basis::signature_operations(bool bip16, bool bip141) const {
#if ! defined(KTH_SEGWIT_ENABLED)
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

// True if there is another coinbase other than the first tx.
// No txs or coinbases returns false.
bool block_basis::is_extra_coinbases() const {
    if (transactions_.empty()) {
        return false;
    }

    auto const value = [](transaction const& tx) {
        return tx.is_coinbase();
    };

    auto const& txs = transactions_;
    return std::any_of(txs.begin() + 1, txs.end(), value);
}

bool block_basis::is_final(size_t height, uint32_t block_time) const {
    auto const value = [=](transaction const& tx) {
        return tx.is_final(height, block_time);
    };

    auto const& txs = transactions_;
    return std::all_of(txs.begin(), txs.end(), value);
}

// Distinctness is defined by transaction hash.
bool block_basis::is_distinct_transaction_set() const {
    auto const hasher = [](transaction const& tx) { return tx.hash(); };
    auto const& txs = transactions_;
    hash_list hashes(txs.size());
    std::transform(txs.begin(), txs.end(), hashes.begin(), hasher);
    std::sort(hashes.begin(), hashes.end());
    auto const distinct_end = std::unique(hashes.begin(), hashes.end());
    return distinct_end == hashes.end();
}

hash_digest block_basis::generate_merkle_root(bool witness) const {
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

size_t block_basis::non_coinbase_input_count() const {
    if (transactions_.empty()) {
        return 0;
    }

    auto const counter = [](size_t sum, transaction const& tx) {
        return sum + tx.inputs().size();
    };

    auto const& txs = transactions_;
    return std::accumulate(txs.begin() + 1, txs.end(), size_t(0), counter);
}


//Note(kth): LTOR (Legacy Transaction ORdering) is a check just for Bitcoin (BTC) 
//               and for BitcoinCash (BCH) before 2018-Nov-15.
//****************************************************************************
// CONSENSUS: This is only necessary because satoshi stores and queries as it
// validates, imposing an otherwise unnecessary partial transaction ordering.
//*****************************************************************************
bool block_basis::is_forward_reference() const {
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

bool block_basis::is_canonical_ordered() const {
    //precondition: transactions_.size() > 1
    
    auto const hash_cmp = [](transaction const& a, transaction const& b){
        return std::lexicographical_compare(a.hash().rbegin(), a.hash().rend(), b.hash().rbegin(), b.hash().rend());
    };

    // Skip the coinbase
    return std::is_sorted(transactions_.begin() + 1, transactions_.end(), hash_cmp);
}

// This is an early check that is redundant with block pool accept checks.
bool block_basis::is_internal_double_spend() const {
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

bool block_basis::is_valid_merkle_root() const {
    return generate_merkle_root() == header_.merkle();
}

// Overflow returns max_uint64.
uint64_t block_basis::fees() const {
    ////static_assert(max_money() < max_uint64, "overflow sentinel invalid");
    auto const value = [](uint64_t total, transaction const& tx) {
        return ceiling_add(total, tx.fees());
    };

    auto const& txs = transactions_;
    return std::accumulate(txs.begin(), txs.end(), uint64_t{0}, value);
}

uint64_t block_basis::claim() const {
    return transactions_.empty() ? 0 : transactions_.front().total_output_value();
}

// Overflow returns max_uint64.
uint64_t block_basis::reward(size_t height) const {
    ////static_assert(max_money() < max_uint64, "overflow sentinel invalid");
    return ceiling_add(fees(), subsidy(height));
}

bool block_basis::is_valid_coinbase_claim(size_t height) const {
    return claim() <= reward(height);
}

bool block_basis::is_valid_coinbase_script(size_t height) const {
    if (transactions_.empty() || transactions_.front().inputs().empty()) {
        return false;
    }

    auto const& script = transactions_.front().inputs().front().script();
    return script::is_coinbase_pattern(script.operations(), height);
}

#if defined(KTH_SEGWIT_ENABLED)
bool block_basis::is_valid_witness_commitment() const {
#if ! defined(KTH_SEGWIT_ENABLED)
    return false;
#else
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
    return !is_segregated(*this);
#endif // KTH_SEGWIT_ENABLED
}
#endif // KTH_SEGWIT_ENABLED


code block_basis::check_transactions() const {
    code ec;
    for (auto const& tx : transactions_) {
        if ((ec = tx.check(false))) {
            return ec;
        }
    }
    return error::success;
}

code block_basis::accept_transactions(chain_state const& state) const {
    code ec;
    for (auto const& tx : transactions_) {
        if ( ! tx.validation.validated && (ec = tx.accept(state, false))) {
            return ec;
        }
    }
    return error::success;
}

code block_basis::connect_transactions(chain_state const& state) const {
    code ec;
    for (auto const& tx : transactions_) {
        if ( ! tx.validation.validated && (ec = tx.connect(state))) {
            return ec;
        }
    }
    return error::success;
}

// Validation.
//-----------------------------------------------------------------------------

// These checks are self-contained; blockchain (and so version) independent.
code block_basis::check(size_t serialized_size_false) const {
    code ec;

    if ((ec = header_.check())) {
        return ec;

        // TODO(legacy): relates to total of tx.size(false) (pool cache). -> no witness size
    }

    // if (serialized_size(false) > get_max_block_size()) {
    if (serialized_size_false > get_max_block_size()) {
        return error::block_size_limit;
    }

    if (transactions_.empty()) {
        return error::empty_block;
    }

    if ( ! transactions_.front().is_coinbase()) {
        return error::first_not_coinbase;
    }

    if (is_extra_coinbases()) {
        return error::extra_coinbases;
        // TODO(legacy): determinable from tx pool graph.
    }

#if ! defined(KTH_CURRENCY_BCH) // BTC and LTC
    //Note(kth): LTOR (Legacy Transaction ORdering) is a check just for Bitcoin (BTC) 
    //               and for BitcoinCash (BCH) before 2018-Nov-15.
    if (is_forward_reference()) {
        return error::forward_reference;
    }
#endif


    // This is subset of is_internal_double_spend if collisions cannot happen.
    ////else if ( ! is_distinct_transaction_set())
    ////    return error::internal_duplicate;

    // TODO(legacy): determinable from tx pool graph.


    if (is_internal_double_spend()) {
        return error::block_internal_double_spend;

        // TODO(legacy): relates height to tx.hash(false) (pool cache).
    }

    if ( ! is_valid_merkle_root()) {
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

// code block_basis::accept(bool transactions) const {
//     auto const state = validation.state;
//     return state ? accept(*state, transactions) : error::operation_failed;
// }

// These checks assume that prevout caching is completed on all tx.inputs.
code block_basis::accept(chain_state const& state, size_t serialized_size, size_t weight, bool transactions) const {
    // validation.start_accept = asio::steady_clock::now();

    code ec;
    auto const bip16 = state.is_enabled(rule_fork::bip16_rule);
    auto const bip34 = state.is_enabled(rule_fork::bip34_rule);
    auto const bip113 = state.is_enabled(rule_fork::bip113_rule);
#if ! defined(KTH_SEGWIT_ENABLED)
    auto const bip141 = false;  // No segwit
#else
    auto const bip141 = state.is_enabled(rule_fork::bip141_rule);
#endif

    auto const max_sigops = bip141 ? max_fast_sigops : get_max_block_sigops();
    auto const block_time = bip113 ? state.median_time_past() : header_.timestamp();

    // size_t allowed_sigops = get_allowed_sigops(serialized_size());
    size_t allowed_sigops = get_allowed_sigops(serialized_size);

    if ((ec = header_.accept(state))) {
        return ec;
    }

#if defined(KTH_CURRENCY_BCH)
    //Note(kth): In Bitcoin Cash, block size check is now dependent on the Blockchain state.
    // if ( ! state.is_monolith_enabled() && serialized_size() > max_block_size_old) {
    if ( ! state.is_monolith_enabled() && serialized_size > max_block_size_old) {
        return error::block_size_limit;
    }
#endif

    if (state.is_under_checkpoint()) {
        return error::success;
    }

#if defined(KTH_CURRENCY_BCH)
    //Note(kth): LTOR (Legacy Transaction ORdering) is a check just for Bitcoin (BTC) 
    //               and for BitcoinCash (BCH) before 2018-Nov-15.

    if (state.is_magnetic_anomaly_enabled()) {
        if ( ! is_canonical_ordered()) {
            return error::non_canonical_ordered;
        }
    } else {
        if (is_forward_reference()) {
            return error::forward_reference;
        }
    }
#endif

    // TODO(legacy): relates height to total of tx.size(true) (pool cache).
    // NOTE: for BCH bit141 is set as false
    // if (bip141 && weight() > max_block_weight) {
    if (bip141 && weight > max_block_weight) {
        return error::block_weight_limit;
    }

    if (bip34 && !is_valid_coinbase_script(state.height())) {
        return error::coinbase_height_mismatch;

    }
    // TODO(legacy): relates height to total of tx.fee (pool cach).
    if ( ! is_valid_coinbase_claim(state.height())) {
        return error::coinbase_value_limit;

    }

    // TODO(legacy): relates median time past to tx.locktime (pool cache min tx.time).
    if ( ! is_final(state.height(), block_time)) {
        return error::block_non_final;
    }

    // TODO(legacy): relates height to tx.hash(true) (pool cache).
    // NOTE: for BCH bit141 is set as false
    if (bip141 && !is_valid_witness_commitment()) {
        return error::invalid_witness_commitment;

        // TODO(legacy): determine if performance benefit is worth excluding sigops here.
        // TODO(legacy): relates block limit to total of tx.sigops (pool cache tx.sigops).
        // This recomputes sigops to include p2sh from prevouts.
        // NOTE: for BCH bit141 is set as false
    }

    if (transactions && (signature_operations(bip16, bip141) > allowed_sigops)) {
        return error::block_embedded_sigop_limit;
    }

    if (transactions) {
        return accept_transactions(state);
    }

    return ec;
}

// code block_basis::connect() const {
//     auto const state = validation.state;
//     return state ? connect(*state) : error::operation_failed;
// }

code block_basis::connect(chain_state const& state) const {
    // validation.start_connect = asio::steady_clock::now();

    if (state.is_under_checkpoint()) {
        return error::success;
    }
    return connect_transactions(state);
}

size_t total_inputs(block_basis const& blk, bool with_coinbase /*= true*/) {
    auto const inputs = [](size_t total, transaction const& tx) {
        return safe_add(total, tx.inputs().size());
    };

    auto const& txs = blk.transactions();
    const size_t offset = with_coinbase ? 0 : 1;
    return std::accumulate(txs.begin() + offset, txs.end(), size_t(0), inputs);
}

bool is_segregated(block_basis const& blk) {
#if ! defined(KTH_SEGWIT_ENABLED)
    (void)blk;
    return false;
#else
    auto const segregated = [](transaction const& tx) {
        return tx.is_segregated();
    };

    // If no block tx has witness data the commitment is optional (bip141).
    return std::any_of(blk.transactions().begin(), blk.transactions().end(), segregated);
#endif // KTH_CURRENCY_BCH
}

// Full block serialization is always canonical encoding.
size_t serialized_size(block_basis const& blk, bool witness /*= false*/) {
    auto const sum = [witness](size_t total, transaction const& tx) {
        return safe_add(total, tx.serialized_size(true, witness_val(witness)));
    };

    auto const& txs = blk.transactions();
    return blk.header().serialized_size(true) +
           message::variable_uint_size(txs.size()) +
           std::accumulate(txs.begin(), txs.end(), size_t(0), sum);
}

}  // namespace kth

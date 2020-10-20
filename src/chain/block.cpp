// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/block.hpp>

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
#include <kth/domain/common.hpp>
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

#if defined(KTH_CURRENCY_LTC)
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

#if defined(KTH_CURRENCY_BCH)
static
std::string const encoded_testnet4_genesis_block =
    "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4af1a93c5fffff001d01d3cd060101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"; //NOLINT

//TODO(fernando): scalenet
static
std::string const encoded_scalenet_genesis_block =
    "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4af1a93c5fffff001d01d3cd060101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"; //NOLINT
#endif

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
block::block(chain::header const& header, transaction::list const& transactions)
    : block_basis(header, transactions)
{}

// TODO(legacy): deal with possibility of inconsistent merkle root in relation to txs.
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
}

// Properties (size, accessors, cache).
//-----------------------------------------------------------------------------

// Full block serialization is always canonical encoding.
size_t block::serialized_size(bool witness) const {
    size_t value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (witness_val(witness) && total_size_ != std::nullopt) {
        value = total_size_.value();
        mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    if ( ! witness_val(witness) && base_size_ != std::nullopt) {
        value = base_size_.value();
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

// TODO(legacy): see set_header comments.
void block::set_transactions(transaction::list const& value) {
    block_basis::set_transactions(value);

#if defined(KTH_SEGWIT_ENABLED)
    segregated_ = std::nullopt;
#endif
    total_inputs_ = std::nullopt;
    base_size_ = std::nullopt;
    total_size_ = std::nullopt;
}

// TODO(legacy): see set_header comments.
void block::set_transactions(transaction::list&& value) {
    block_basis::set_transactions(std::move(value));

#if defined(KTH_SEGWIT_ENABLED)
    segregated_ = std::nullopt;
#endif

    total_inputs_ = std::nullopt;
    base_size_ = std::nullopt;
    total_size_ = std::nullopt;
}

// Utilities.
//-----------------------------------------------------------------------------

chain::block genesis_generic(std::string const& raw_data) {
    data_chunk data;
    decode_base16(data, raw_data);
    auto const genesis = create<chain::block>(data);

    KTH_ASSERT(genesis.is_valid());
    KTH_ASSERT(genesis.transactions().size() == 1);
    KTH_ASSERT(genesis.generate_merkle_root() == genesis.header().merkle());
    return genesis;
}

chain::block block::genesis_mainnet() {
    return genesis_generic(encoded_mainnet_genesis_block);
}

chain::block block::genesis_testnet() {
    return genesis_generic(encoded_testnet_genesis_block);
}

chain::block block::genesis_regtest() {
    return genesis_generic(encoded_regtest_genesis_block);
}

#if defined(KTH_CURRENCY_BCH)
chain::block block::genesis_testnet4() {
    return genesis_generic(encoded_testnet4_genesis_block);
}

chain::block block::genesis_scalenet() {
    return genesis_generic(encoded_scalenet_genesis_block);
}
#endif


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
            step <<= 1U;
        }

        heights.push_back(height);
    }

    // Push the genesis block index.
    heights.push_back(0);

    // Validate the reservation computation.
    KTH_ASSERT(heights.size() <= reservation);
    return heights;
}

// Utilities.
//-----------------------------------------------------------------------------

#if defined(KTH_SEGWIT_ENABLED)
// Clear witness from all inputs (does not change default transaction hash).
void block::strip_witness() {
    auto const strip = [](transaction& transaction) {
        transaction.strip_witness();
    };

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    unique_lock lock(mutex_);

    segregated_ = false;
    total_size_ = std::nullopt;
    chain::strip_witness(*this);

    ///////////////////////////////////////////////////////////////////////////
}
#endif

// Validation helpers.
//-----------------------------------------------------------------------------

// Returns max_size_t in case of overflow or unpopulated chain state.
size_t block::signature_operations() const {
    auto const state = validation.state;
    auto const bip16 = state ? state->is_enabled(rule_fork::bip16_rule) : true;
#if ! defined(KTH_SEGWIT_ENABLED)
    auto const bip141 = false;
#else
    auto const bip141 = state ? state->is_enabled(rule_fork::bip141_rule) : false;
#endif
    return block_basis::signature_operations(bip16, bip141);
}

size_t block::total_inputs(bool with_coinbase) const {
    size_t value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (total_inputs_ != std::nullopt) {
        value = total_inputs_.value();
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

#if defined(KTH_SEGWIT_ENABLED)
size_t block::weight() const {
    return chain::weight(serialized_size(true), serialized_size(false));
}

bool block::is_segregated() const {
    bool value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (segregated_ != std::nullopt) {
        value = segregated_.value();
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
}
#endif // defined(KTH_SEGWIT_ENABLED)


// Validation.
//-----------------------------------------------------------------------------

// These checks are self-contained; blockchain (and so version) independent.
code block::check(size_t max_block_size) const {
    validation.start_check = asio::steady_clock::now();
    return block_basis::check(serialized_size(false), max_block_size);
}

code block::accept(bool transactions) const {
    auto const state = validation.state;
    return state ? accept(*state, transactions) : error::operation_failed;
}

// These checks assume that prevout caching is completed on all tx.inputs.
code block::accept(chain_state const& state, bool transactions) const {
    validation.start_accept = asio::steady_clock::now();

#if defined(KTH_SEGWIT_ENABLED)
    return block_basis::accept(state, serialized_size(), weight(), transactions);
#else
    return block_basis::accept(state, serialized_size(), transactions);
#endif      
}

code block::connect() const {
    auto const state = validation.state;
    return state ? block_basis::connect(*state) : error::operation_failed;
}

} // namespace kth

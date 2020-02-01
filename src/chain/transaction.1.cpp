// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin/bitcoin/chain/transaction.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <sstream>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/optional.hpp>

#include <bitcoin/bitcoin/chain/chain_state.hpp>
#include <bitcoin/bitcoin/chain/input.hpp>
#include <bitcoin/bitcoin/chain/output.hpp>
#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/machine/operation.hpp>
// #include <bitcoin/infrastructure/message/message_tools.hpp>
#include <bitcoin/bitcoin/multi_crypto_support.hpp>
#include <bitcoin/infrastructure/error.hpp>
#include <bitcoin/bitcoin/machine/opcode.hpp>
#include <bitcoin/bitcoin/machine/rule_fork.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/message/message_tools.hpp>
#include <bitcoin/infrastructure/utility/collection.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/endian.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace chain {

// using namespace bc::machine;

// Constructors.
//-----------------------------------------------------------------------------

transaction::transaction()
    : validation{}
{}

transaction::transaction(uint32_t version, uint32_t locktime, input::list const& inputs, output::list const& outputs
#ifdef KTH_CACHED_RPC_DATA
                        , uint32_t cached_sigops, uint64_t cached_fees, bool cached_is_standard
#endif
                        )
    : version_(version)
    , locktime_(locktime)
    , inputs_(inputs)
    , outputs_(outputs)
#ifdef KTH_CACHED_RPC_DATA
    , cached_fees_(cached_fees)
    , cached_sigops_(cached_sigops)
    , cached_is_standard_(cached_is_standard)
#endif
    , validation{} 
{}

transaction::transaction(uint32_t version, uint32_t locktime, input::list&& inputs, output::list&& outputs
#ifdef KTH_CACHED_RPC_DATA
                       , uint32_t cached_sigops, uint64_t cached_fees, bool cached_is_standard
#endif
                        )
    : version_(version)
    , locktime_(locktime)
    , inputs_(std::move(inputs))
    , outputs_(std::move(outputs))
#ifdef KTH_CACHED_RPC_DATA
    , cached_fees_(cached_fees)
    , cached_sigops_(cached_sigops)
    , cached_is_standard_(cached_is_standard)
#endif
    , validation{} 
{}

transaction::transaction(transaction const& x, hash_digest const& hash)
    : version_(x.version_)
    , locktime_(x.locktime_)
    , inputs_(x.inputs_)
    , outputs_(x.outputs_)
#ifdef KTH_CACHED_RPC_DATA
    , cached_fees_(x.cached_fees_)
    , cached_sigops_(x.cached_sigops_)
    , cached_is_standard_(x.cached_is_standard_)
#endif
    , validation(x.validation) 
{
    hash_ = std::make_shared<hash_digest>(hash);
    // validation = x.validation;
}

transaction::transaction(transaction&& x, hash_digest const& hash)
    : version_(x.version_)
    , locktime_(x.locktime_)
    , inputs_(std::move(x.inputs_))
    , outputs_(std::move(x.outputs_))
#ifdef KTH_CACHED_RPC_DATA
    , cached_fees_(x.cached_fees_)
    , cached_sigops_(x.cached_sigops_)
    , cached_is_standard_(x.cached_is_standard_)
#endif
    , validation(std::move(x.validation)) 
{
    hash_ = std::make_shared<hash_digest>(hash);
    // validation = std::move(x.validation);
}


transaction::transaction(transaction const& x)
    : version_(x.version_)
    , locktime_(x.locktime_)
    , inputs_(x.inputs_)
    , outputs_(x.outputs_)
#ifdef KTH_CACHED_RPC_DATA
    , cached_fees_(0)
    , cached_sigops_(0)
    , cached_is_standard_(false)
#endif
    , validation(x.validation) 
{}

transaction::transaction(transaction&& x) noexcept
    : version_(x.version_)
    , locktime_(x.locktime_)
    , inputs_(std::move(x.inputs_))
    , outputs_(std::move(x.outputs_))

#ifdef KTH_CACHED_RPC_DATA
    , cached_fees_(0)
    , cached_sigops_(0)
    , cached_is_standard_(false)
#endif
    , validation(std::move(x.validation)) 
{}

transaction& transaction::operator=(transaction const& x) {
    version_ = x.version_;
    locktime_ = x.locktime_;
    inputs_ = x.inputs_;
    outputs_ = x.outputs_;
    validation = x.validation;

#ifdef KTH_CACHED_RPC_DATA
    cached_fees_ = x.cached_fees_;
    cached_sigops_ = x.cached_sigops_;
    cached_is_standard_ = x.cached_is_standard_;
#endif

    return *this;
}

transaction& transaction::operator=(transaction&& x) noexcept {
    version_ = x.version_;
    locktime_ = x.locktime_;
    inputs_ = std::move(x.inputs_);
    outputs_ = std::move(x.outputs_);
    validation = std::move(x.validation);
#ifdef KTH_CACHED_RPC_DATA
    cached_fees_ = x.cached_fees_;
    cached_sigops_ = x.cached_sigops_;
    cached_is_standard_ = x.cached_is_standard_;
#endif
    return *this;
}


// Operators.
//-----------------------------------------------------------------------------

bool transaction::operator==(transaction const& x) const {
    return (version_ == x.version_) && (locktime_ == x.locktime_) && (inputs_ == x.inputs_) && (outputs_ == x.outputs_);
}

bool transaction::operator!=(transaction const& x) const {
    return !(*this == x);
}

// Deserialization.
//-----------------------------------------------------------------------------

// static
transaction transaction::factory_from_data(data_chunk const& data, bool wire, bool witness) {
    transaction instance;
    instance.from_data(data, wire, witness_val(witness));
    return instance;
}

// static
transaction transaction::factory_from_data(std::istream& stream, bool wire, bool witness) {
    transaction instance;
    instance.from_data(stream, wire, witness_val(witness));
    return instance;
}

bool transaction::from_data(data_chunk const& data, bool wire, bool witness
#ifdef KTH_CACHED_RPC_DATA
    , bool unconfirmed
#endif
    ) {
    data_source istream(data);
    return from_data(istream, wire, witness_val(witness)
#ifdef KTH_CACHED_RPC_DATA    
                    , unconfirmed
#endif
                    );
}

bool transaction::from_data(std::istream& stream, bool wire, bool witness
#ifdef KTH_CACHED_RPC_DATA
    , bool unconfirmed
#endif
    ) {
    istream_reader stream_r(stream);
    return from_data(stream_r, wire, witness_val(witness)
#ifdef KTH_CACHED_RPC_DATA    
                    , unconfirmed
#endif

                    );
}

// protected
void transaction::reset() {
    version_ = 0;
    locktime_ = 0;
    inputs_.clear();
    inputs_.shrink_to_fit();
    outputs_.clear();
    outputs_.shrink_to_fit();
    invalidate_cache();
    outputs_hash_.reset();
    inpoints_hash_.reset();
    sequences_hash_.reset();
    segregated_ = boost::none;
    total_input_value_ = boost::none;
    total_output_value_ = boost::none;
}

bool transaction::is_valid() const {
    return (version_ != 0) || (locktime_ != 0) || !inputs_.empty() || !outputs_.empty();
}

// Serialization.
//-----------------------------------------------------------------------------

// Transactions with empty witnesses always use old serialization (bip144).
// If no inputs are witness programs then witness hash is tx hash (bip141).
data_chunk transaction::to_data(bool wire, bool witness
#ifdef KTH_CACHED_RPC_DATA
    , bool unconfirmed
#endif
    ) const {
    // Witness handling must be disabled for non-segregated txs.
    witness &= is_segregated();

    data_chunk data;
    auto const size = serialized_size(wire, witness_val(witness)
#ifdef KTH_CACHED_RPC_DATA
                                     , unconfirmed
#endif
                                     );

    // Reserve an extra byte to prevent full reallocation in the case of
    // generate_signature_hash extension by addition of the sighash_type.
    data.reserve(size + sizeof(uint8_t));

    data_sink ostream(data);
    to_data(ostream, wire, witness_val(witness)
#ifdef KTH_CACHED_RPC_DATA
           , unconfirmed
#endif
           );

    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void transaction::to_data(data_sink& stream, bool wire, bool witness
#ifdef KTH_CACHED_RPC_DATA
    , bool unconfirmed
#endif
    ) const {
    // Witness handling must be disabled for non-segregated txs.
    witness &= is_segregated();

    ostream_writer sink_w(stream);
    to_data(sink_w, wire, witness_val(witness)
#ifdef KTH_CACHED_RPC_DATA
           , unconfirmed
#endif
           );
}

// Size.
//-----------------------------------------------------------------------------

size_t transaction::serialized_size(bool wire, bool witness
#ifdef KTH_CACHED_RPC_DATA
    , bool unconfirmed
#endif
    ) const {
    // The witness parameter must be set to false for non-segregated txs.
    witness &= is_segregated();

    // Returns space for the witness although not serialized by input.
    // Returns witness space if specified even if input not segregated.
    auto const ins = [wire, witness](size_t size, input const& input) {
        return size + input.serialized_size(wire, witness_val(witness));
    };

    auto const outs = [wire](size_t size, output const& output) {
        return size + output.serialized_size(wire);
    };

    // Must be both witness and wire encoding for bip144 serialization.
    return (wire && witness_val(witness) ? sizeof(witness_marker) : 0) 
         + (wire && witness_val(witness) ? sizeof(witness_flag) : 0) 
         + (wire ? sizeof(version_) : message::variable_uint_size(version_)) 
         + (wire ? sizeof(locktime_) : message::variable_uint_size(locktime_)) 
         + message::variable_uint_size(inputs_.size()) + message::variable_uint_size(outputs_.size()) 
         + std::accumulate(inputs_.begin(), inputs_.end(), size_t{0}, ins) 
         + std::accumulate(outputs_.begin(), outputs_.end(), size_t{0}, outs) 
#ifdef KTH_CACHED_RPC_DATA         
         + ((!wire && unconfirmed) ? sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint8_t) : 0)
#endif
         ;
}

// Accessors.
//-----------------------------------------------------------------------------

uint32_t transaction::version() const {
    return version_;
}

void transaction::set_version(uint32_t value) {
    version_ = value;
    invalidate_cache();
}

uint32_t transaction::locktime() const {
    return locktime_;
}

void transaction::set_locktime(uint32_t value) {
    locktime_ = value;
    invalidate_cache();
}

input::list& transaction::inputs() {
    return inputs_;
}

input::list const& transaction::inputs() const {
    return inputs_;
}

void transaction::set_inputs(input::list const& value) {
    inputs_ = value;
    invalidate_cache();
    inpoints_hash_.reset();
    sequences_hash_.reset();
    segregated_ = boost::none;
    total_input_value_ = boost::none;
}

void transaction::set_inputs(input::list&& value) {
    inputs_ = std::move(value);
    invalidate_cache();
    segregated_ = boost::none;
    total_input_value_ = boost::none;
}

output::list& transaction::outputs() {
    return outputs_;
}

output::list const& transaction::outputs() const {
    return outputs_;
}

void transaction::set_outputs(output::list const& value) {
    outputs_ = value;
    invalidate_cache();
    outputs_hash_.reset();
    total_output_value_ = boost::none;
}

void transaction::set_outputs(output::list&& value) {
    outputs_ = std::move(value);
    invalidate_cache();
    total_output_value_ = boost::none;
}

#ifdef KTH_CACHED_RPC_DATA
uint64_t transaction::cached_fees() const {
    return cached_fees_;
}

uint32_t transaction::cached_sigops() const {
    return cached_sigops_;
}

bool transaction::cached_is_standard() const {
    return cached_is_standard_;
}
#endif // KTH_CACHED_RPC_DATA

// Cache.
//-----------------------------------------------------------------------------

// protected
void transaction::invalidate_cache() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    hash_mutex_.lock_upgrade();

    if (hash_ || witness_hash_) {
        hash_mutex_.unlock_upgrade_and_lock();
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        hash_.reset();
        witness_hash_.reset();
        //---------------------------------------------------------------------
        hash_mutex_.unlock_and_lock_upgrade();
    }

    hash_mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////
}

hash_digest transaction::hash(bool witness) const {
    // Witness hashing must be disabled for non-segregated txs.
    witness &= is_segregated();

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    hash_mutex_.lock_upgrade();

    if (witness_val(witness)) {
        if ( ! witness_hash_) {
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            hash_mutex_.unlock_upgrade_and_lock();

            // Witness coinbase tx hash is assumed to be null_hash (bip141).
            witness_hash_ = std::make_shared<hash_digest>(is_coinbase() ? null_hash : bitcoin_hash(to_data(true, true)));

            hash_mutex_.unlock_and_lock_upgrade();
            //-----------------------------------------------------------------
        }
    } else {
        if ( ! hash_) {
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            hash_mutex_.unlock_upgrade_and_lock();
            hash_ = std::make_shared<hash_digest>(bitcoin_hash(to_data(true)));
            hash_mutex_.unlock_and_lock_upgrade();
            //-----------------------------------------------------------------
        }
    }

    auto const hash = witness_val(witness) ? *witness_hash_ : *hash_;
    hash_mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return hash;
}

hash_digest transaction::outputs_hash() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    hash_mutex_.lock_upgrade();

    if ( ! outputs_hash_) {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        hash_mutex_.unlock_upgrade_and_lock();
        outputs_hash_ = std::make_shared<hash_digest>(to_outputs(*this));
        hash_mutex_.unlock_and_lock_upgrade();
        //-----------------------------------------------------------------
    }

    auto const hash = *outputs_hash_;
    hash_mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return hash;
}

hash_digest transaction::inpoints_hash() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    hash_mutex_.lock_upgrade();

    if ( ! inpoints_hash_) {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        hash_mutex_.unlock_upgrade_and_lock();
        inpoints_hash_ = std::make_shared<hash_digest>(to_inpoints(*this));
        hash_mutex_.unlock_and_lock_upgrade();
        //-----------------------------------------------------------------
    }

    auto const hash = *inpoints_hash_;
    hash_mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return hash;
}

hash_digest transaction::sequences_hash() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    hash_mutex_.lock_upgrade();

    if ( ! sequences_hash_) {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        hash_mutex_.unlock_upgrade_and_lock();
        sequences_hash_ = std::make_shared<hash_digest>(to_sequences(*this));
        hash_mutex_.unlock_and_lock_upgrade();
        //-----------------------------------------------------------------
    }

    auto const hash = *sequences_hash_;
    hash_mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return hash;
}

// Utilities.
//-----------------------------------------------------------------------------

#ifndef KTH_CURRENCY_BCH
// Clear witness from all inputs (does not change default transaction hash).
void transaction::strip_witness() {
    auto const strip = [](input& input) {
        input.strip_witness();
    };

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    unique_lock lock(mutex_);

    segregated_ = false;
    std::for_each(inputs_.begin(), inputs_.end(), strip);
    ///////////////////////////////////////////////////////////////////////////
}
#endif

void transaction::recompute_hash() {
    hash_ = nullptr;
    hash();
}

// Validation helpers.
//-----------------------------------------------------------------------------

bool transaction::is_coinbase() const {
    return inputs_.size() == 1 && inputs_.front().previous_output().is_null();
}

// True if coinbase and has invalid input[0] script size.
bool transaction::is_oversized_coinbase() const {
    if ( ! is_coinbase()) {
        return false;
    }

    auto const script_size = inputs_.front().script().serialized_size(false);
    return script_size < min_coinbase_size || script_size > max_coinbase_size;
}

// True if not coinbase but has null previous_output(s).
bool transaction::is_null_non_coinbase() const {
    if (is_coinbase()) {
        return false;
    }

    auto const invalid = [](input const& input) {
        return input.previous_output().is_null();
    };

    return std::any_of(inputs_.begin(), inputs_.end(), invalid);
}

// private
bool transaction::all_inputs_final() const {
    auto const finalized = [](input const& input) {
        return input.is_final();
    };

    return std::all_of(inputs_.begin(), inputs_.end(), finalized);
}

bool transaction::is_final(size_t block_height, uint32_t block_time) const {
    auto const max_locktime = [=]() {
        return locktime_ < locktime_threshold ? safe_unsigned<uint32_t>(block_height) : block_time;
    };

    return locktime_ == 0 || locktime_ < max_locktime() || all_inputs_final();
}

bool transaction::is_locked(size_t block_height,
                            uint32_t median_time_past) const {
    if (version_ < relative_locktime_min_version || is_coinbase()) {
        return false;
    }

    auto const locked = [block_height, median_time_past](input const& input) {
        return input.is_locked(block_height, median_time_past);
    };

    // If any input is relative time locked the transaction is as well.
    return std::any_of(inputs_.begin(), inputs_.end(), locked);
}

// This is not a consensus rule, just detection of an irrational use.
bool transaction::is_locktime_conflict() const {
    return locktime_ != 0 && all_inputs_final();
}

// Returns max_uint64 in case of overflow.
uint64_t transaction::total_input_value() const {
    uint64_t value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (total_input_value_ != boost::none) {
        value = total_input_value_.get();
        mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    mutex_.unlock_upgrade_and_lock();
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    ////static_assert(max_money() < max_uint64, "overflow sentinel invalid");
    auto const sum = [](uint64_t total, input const& input) {
        auto const& prevout = input.previous_output().validation.cache;
        auto const missing = !prevout.is_valid();

        // Treat missing previous outputs as zero-valued, no math on sentinel.
        return ceiling_add(total, missing ? 0 : prevout.value());
    };

    value = std::accumulate(inputs_.begin(), inputs_.end(), uint64_t(0), sum);
    total_input_value_ = value;
    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

// Returns max_uint64 in case of overflow.
uint64_t transaction::total_output_value() const {
    uint64_t value;

    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (total_output_value_ != boost::none) {
        value = total_output_value_.get();
        mutex_.unlock_upgrade();
        //---------------------------------------------------------------------
        return value;
    }

    mutex_.unlock_upgrade_and_lock();
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    ////static_assert(max_money() < max_uint64, "overflow sentinel invalid");
    auto const sum = [](uint64_t total, output const& output) {
        return ceiling_add(total, output.value());
    };

    value = std::accumulate(outputs_.begin(), outputs_.end(), uint64_t(0), sum);
    total_output_value_ = value;
    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

uint64_t transaction::fees() const {
    return floor_subtract(total_input_value(), total_output_value());
}

bool transaction::is_overspent() const {
    return !is_coinbase() && total_output_value() > total_input_value();
}

// Returns max_size_t in case of overflow.
size_t transaction::signature_operations() const {
    auto const state = validation.state;
    auto const bip16_enabled = state->is_enabled(bc::machine::rule_fork::bip16_rule);
#ifdef KTH_CURRENCY_BCH
    auto const bip141_enabled = false;
#else
    auto const bip141_enabled = state->is_enabled(bc::machine::rule_fork::bip141_rule);
#endif
    return state ? signature_operations(bip16_enabled, bip141_enabled) : max_size_t;
}

// Returns max_size_t in case of overflow.
size_t transaction::signature_operations(bool bip16, bool bip141) const {
#ifdef KTH_CURRENCY_BCH
    bip141 = false;  // No segwit
#endif
    auto const in = [bip16, bip141](size_t total, input const& input) {
        // This includes BIP16 p2sh additional sigops if prevout is cached.
        return ceiling_add(total, input.signature_operations(bip16, bip141));
    };

    auto const out = [bip141](size_t total, output const& output) {
        return ceiling_add(total, output.signature_operations(bip141));
    };

    return std::accumulate(inputs_.begin(), inputs_.end(), size_t{0}, in) +
           std::accumulate(outputs_.begin(), outputs_.end(), size_t{0}, out);
}

size_t transaction::weight() const {
    // Block weight is 3 * Base size * + 1 * Total size (bip141).
    return base_size_contribution * serialized_size(true, false) +
           total_size_contribution * serialized_size(true, true);
}

bool transaction::is_missing_previous_outputs() const {
    auto const missing = [](input const& input) {
        auto const& prevout = input.previous_output();
        auto const coinbase = prevout.is_null();
        auto const missing = !prevout.validation.cache.is_valid();
        return missing && !coinbase;
    };

    // This is an optimization of !missing_inputs().empty();
    return std::any_of(inputs_.begin(), inputs_.end(), missing);
}

point::list transaction::previous_outputs() const {
    point::list prevouts;
    prevouts.reserve(inputs_.size());
    auto const pointer = [&prevouts](input const& input) {
        prevouts.push_back(input.previous_output());
    };

    auto const& ins = inputs_;
    std::for_each(ins.begin(), ins.end(), pointer);
    return prevouts;
}

point::list transaction::missing_previous_outputs() const {
    point::list prevouts;
    prevouts.reserve(inputs_.size());
    auto const accumulator = [&prevouts](input const& input) {
        auto const& prevout = input.previous_output();
        auto const missing = !prevout.validation.cache.is_valid();

        if (missing && !prevout.is_null()) {
            prevouts.push_back(prevout);
        }
    };

    std::for_each(inputs_.begin(), inputs_.end(), accumulator);
    prevouts.shrink_to_fit();
    return prevouts;
}

hash_list transaction::missing_previous_transactions() const {
    auto const points = missing_previous_outputs();
    hash_list hashes;
    hashes.reserve(points.size());
    auto const hasher = [&hashes](output_point const& point) {
        return point.hash();
    };

    std::for_each(points.begin(), points.end(), hasher);
    return distinct(hashes);
}

bool transaction::is_internal_double_spend() const {
    auto prevouts = previous_outputs();
    std::sort(prevouts.begin(), prevouts.end());
    auto const distinct_end = std::unique(prevouts.begin(), prevouts.end());
    auto const distinct = (distinct_end == prevouts.end());
    return !distinct;
}

bool transaction::is_double_spend(bool include_unconfirmed) const {
    auto const spent = [include_unconfirmed](input const& input) {
        auto const& prevout = input.previous_output().validation;
        return prevout.spent && (include_unconfirmed || prevout.confirmed);
    };

    return std::any_of(inputs_.begin(), inputs_.end(), spent);
}

bool transaction::is_dusty(uint64_t minimum_output_value) const {
    auto const dust = [minimum_output_value](output const& output) {
        return output.is_dust(minimum_output_value);
    };

    return std::any_of(outputs_.begin(), outputs_.end(), dust);
}

bool transaction::is_mature(size_t height) const {
    auto const mature = [height](input const& input) {
        return input.previous_output().is_mature(height);
    };

    return std::all_of(inputs_.begin(), inputs_.end(), mature);
}

bool transaction::is_segregated() const {
#ifdef KTH_CURRENCY_BCH
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

    auto const segregated = [](input const& input) {
        return input.is_segregated();
    };

    // If no block tx is has witness data the commitment is optional (bip141).
    value = std::any_of(inputs_.begin(), inputs_.end(), segregated);
    mutex_.unlock();
    ///////////////////////////////////////////////////////////////////////////

    return value;
}

// Coinbase transactions return success, to simplify iteration.
code transaction::connect_input(chain_state const& state, size_t input_index) const {
    if (input_index >= inputs_.size()) {
        return error::operation_failed;
    }

    if (is_coinbase()) {
        return error::success;
    }

    auto const& prevout = inputs_[input_index].previous_output().validation;

    // Verify that the previous output cache has been populated.
    if ( ! prevout.cache.is_valid()) {
        return error::missing_previous_output;
    }

    auto const forks = state.enabled_forks();
    auto const index32 = static_cast<uint32_t>(input_index);

    // Verify the transaction input script against the previous output.
    return script::verify(*this, index32, forks);
}

// Validation.
//-----------------------------------------------------------------------------

// These checks are self-contained; blockchain (and so version) independent.
code transaction::check(bool transaction_pool, bool retarget) const {
    if (inputs_.empty() || outputs_.empty()) {
        return error::empty_transaction;
    }

    if (is_null_non_coinbase()) {
        return error::previous_output_null;
    }

    if (total_output_value() > max_money(retarget)) {
        return error::spend_overflow;
    }

    if ( ! transaction_pool && is_oversized_coinbase()) {
        return error::invalid_coinbase_script_size;
    }

    if (transaction_pool && is_coinbase()) {
        return error::coinbase_transaction;
    }

    if (transaction_pool && is_internal_double_spend()) {
        return error::transaction_internal_double_spend;
        // TODO(libbitcoin): reduce by header, txcount and smallest coinbase size for height.
    }

    if (transaction_pool && serialized_size(true, false) >= get_max_block_size()) {
        return error::transaction_size_limit;

        // We cannot know if bip16/bip141 is enabled here so we do not check it.
        // This will not make a difference unless prevouts are populated, in which
        // case they are ignored. This means that p2sh sigops are not counted here.
        // This is a preliminary check, the final count must come from accept().
        // Reenable once sigop caching is implemented, otherwise is deoptimization.
        ////else if (transaction_pool && signature_operations(false, false) > get_max_block_sigops()
        ////    return error::transaction_legacy_sigop_limit;
    }

    return error::success;
}

code transaction::accept(bool transaction_pool) const {
    auto const state = validation.state;
    return state ? accept(*state, transaction_pool) : error::operation_failed;
}

// These checks assume that prevout caching is completed on all tx.inputs.
code transaction::accept(chain_state const& state, bool transaction_pool) const {
    auto const bip16 = state.is_enabled(bc::machine::rule_fork::bip16_rule);
    auto const bip30 = state.is_enabled(bc::machine::rule_fork::bip30_rule);
    auto const bip68 = state.is_enabled(bc::machine::rule_fork::bip68_rule);
#ifdef KTH_CURRENCY_BCH
    auto const bip141 = false;  // No segwit
#else
    auto const bip141 = state.is_enabled(bc::machine::rule_fork::bip141_rule);
#endif
    auto const revert_bip30 = state.is_enabled(bc::machine::rule_fork::allow_collisions);

    // bip141 discounts segwit sigops by increasing limit and legacy weight.
    auto const max_sigops = bip141 ? max_fast_sigops : get_max_block_sigops();

    if (transaction_pool && state.is_under_checkpoint()) {
        return error::premature_validation;
        // A segregated tx should appear empty if bip141 is not enabled.
    }

    if ( ! bip141 && is_segregated()) {
        return error::empty_transaction;
    }

    if (transaction_pool && !is_final(state.height(), state.median_time_past())) {
        return error::transaction_non_final;

        //*************************************************************************
        // CONSENSUS:
        // A transaction hash that exists in the chain is not acceptable even if
        // the original is spent in the new block. This is not necessary nor is it
        // described by BIP30, but it is in the code referenced by BIP30.
        //*************************************************************************
    }

    if (bip30 && !revert_bip30 && validation.duplicate) {
        return error::unspent_duplicate;
    }

    if (is_missing_previous_outputs()) {
        return error::missing_previous_output;
    }

    if (is_double_spend(transaction_pool)) {
        return error::double_spend;
        // This relates height to maturity of spent coinbase. Since reorg is the
        // only way to decrease height and reorg invalidates, this is cache safe.
    }

    if ( ! is_mature(state.height())) {
        return error::coinbase_maturity;
    }

    if (is_overspent()) {
        return error::spend_exceeds_value;
    }

    if (bip68 && is_locked(state.height(), state.median_time_past())) {
        return error::sequence_locked;
        // This recomputes sigops to include p2sh from prevouts if bip16 is true.
    }

    if (transaction_pool && signature_operations(bip16, bip141) > max_sigops) {
        return error::transaction_embedded_sigop_limit;
        // This causes second serialized_size(true, false) computation (uncached).
        // TODO(libbitcoin): reduce by header, txcount and smallest coinbase size for height.
    }

    if (transaction_pool && bip141 && weight() > max_block_weight) {
        return error::transaction_weight_limit;
    }

    return error::success;
}

code transaction::connect() const {
    auto const state = validation.state;
    return state ? connect(*state) : error::operation_failed;
}

code transaction::connect(chain_state const& state) const {
    code ec;

    for (size_t input = 0; input < inputs_.size(); ++input) {
        if ((ec = connect_input(state, input))) {
            return ec;
        }
    }

    return error::success;
}

bool transaction::is_standard() const {
    for (auto const& in : inputs()) {
        if (in.script().pattern() == libbitcoin::machine::script_pattern::non_standard) {
            return false;
        }
    }

    for (auto const& out : outputs()) {
        if (out.script().pattern() == libbitcoin::machine::script_pattern::non_standard) {
            return false;
        }
    }

    return true;
}

}  // namespace chain
}  // namespace kth

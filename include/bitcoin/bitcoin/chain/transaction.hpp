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
#ifndef LIBBITCOIN_CHAIN_TRANSACTION_HPP
#define LIBBITCOIN_CHAIN_TRANSACTION_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <bitcoin/bitcoin/chain/chain_state.hpp>
#include <bitcoin/bitcoin/chain/input.hpp>
#include <bitcoin/bitcoin/chain/output.hpp>
#include <bitcoin/bitcoin/chain/point.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/error.hpp>
#include <bitcoin/infrastructure/machine/opcode.hpp>
#include <bitcoin/infrastructure/machine/rule_fork.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/thread.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace chain {

namespace detail {
// Read a length-prefixed collection of inputs or outputs from the source.
template <class Source, class Put>
bool read(Source& source, std::vector<Put>& puts, bool wire, bool witness) {
    auto result = true;
    auto const count = source.read_size_little_endian();

    // Guard against potential for arbitary memory allocation.
    if (count > get_max_block_size())
        source.invalidate();
    else
        puts.resize(count);

    auto const deserialize = [&](Put& put) {
        result = result && put.from_data(source, wire, witness_val(witness));
#ifndef NDBEUG
        put.script().operations();
#endif
    };

    std::for_each(puts.begin(), puts.end(), deserialize);
    return result;
}

// Write a length-prefixed collection of inputs or outputs to the sink.
template <class Sink, class Put>
void write(Sink& sink, const std::vector<Put>& puts, bool wire, bool witness) {
    sink.write_variable_little_endian(puts.size());

    auto const serialize = [&](const Put& put) {
        put.to_data(sink, wire, witness_val(witness));
    };

    std::for_each(puts.begin(), puts.end(), serialize);
}

#ifndef BITPRIM_CURRENCY_BCH
// Input list must be pre-populated as it determines witness count.
template <Reader R, BITPRIM_IS_READER(R)>
inline void read_witnesses(R& source, input::list& inputs) {
    auto const deserialize = [&](input& input) {
        input.witness().from_data(source, true);
    };

    std::for_each(inputs.begin(), inputs.end(), deserialize);
}

// Witness count is not written as it is inferred from input count.
template <typename W>
inline void write_witnesses(W& sink, input::list const& inputs) {
    auto const serialize = [&sink](input const& input) {
        input.witness().to_data(sink, true);
    };

    std::for_each(inputs.begin(), inputs.end(), serialize);
}
#endif // not defined BITPRIM_CURRENCY_BCH

}  // namespace detail

class BC_API transaction {
public:
    using ins = input::list;
    using outs = output::list;
    using list = std::vector<transaction>;
    using hash_ptr = std::shared_ptr<hash_digest>;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation {
        uint64_t originator = 0;
        code error = error::not_found;
        chain_state::ptr state = nullptr;

        // The transaction is an unspent duplicate.
        bool duplicate = false;

        // The unconfirmed tx exists in the store.
        bool pooled = false;

        // The unconfirmed tx is validated at the block's current fork state.
        bool current = false;

        // Similate organization and instead just validate the transaction.
        bool simulate = false;
    };

    // Constructors.
    //-----------------------------------------------------------------------------

    transaction();

    transaction(uint32_t version, uint32_t locktime, ins const& inputs, outs const& outputs
#ifdef BITPRIM_CACHED_RPC_DATA
                , uint32_t cached_sigops = 0, uint64_t cached_fees = 0, bool cached_is_standard = false
#endif
               );
    transaction(uint32_t version, uint32_t locktime, ins&& inputs, outs&& outputs
#ifdef BITPRIM_CACHED_RPC_DATA
               , uint32_t cached_sigops = 0, uint64_t cached_fees = 0, bool cached_is_standard = false
#endif
               );
    transaction(transaction const& x, hash_digest const& hash);
    transaction(transaction&& x, hash_digest const& hash);



    transaction(transaction const& x);
    transaction(transaction&& x) noexcept;

    /// This class is move assignable and copy assignable.
    transaction& operator=(transaction const& x);
    transaction& operator=(transaction&& x) noexcept;




    // Operators.
    //-----------------------------------------------------------------------------

    bool operator==(transaction const& x) const;
    bool operator!=(transaction const& x) const;

    // Deserialization.
    //-----------------------------------------------------------------------------

    static transaction factory_from_data(data_chunk const& data, bool wire = true, bool witness = false);
    static transaction factory_from_data(data_source& stream, bool wire = true, bool witness = false);

    template <Reader R, BITPRIM_IS_READER(R)>
    static transaction factory_from_data(R& source, bool wire = true, bool witness = false) {
        transaction instance;
        instance.from_data(source, wire, witness_val(witness));
        return instance;
    }

    //static transaction factory_from_data(reader& source, bool wire=true, bool witness=false);

    bool from_data(data_chunk const& data, bool wire = true, bool witness = false
#ifdef BITPRIM_CACHED_RPC_DATA
                    , bool unconfirmed = false
#endif
                    );

    bool from_data(data_source& stream, bool wire = true, bool witness = false
#ifdef BITPRIM_CACHED_RPC_DATA
                    , bool unconfirmed = false
#endif
                    );

    // Witness is not used by outputs, just for template normalization.
    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(R& source, bool wire = true, bool witness = false
#ifdef BITPRIM_CACHED_RPC_DATA
                    , bool unconfirmed = false
#endif
                ) {
        reset();

        if (wire) {
            // Wire (satoshi protocol) deserialization.
            version_ = source.read_4_bytes_little_endian();
            detail::read(source, inputs_, wire, witness_val(witness));
#ifdef BITPRIM_CURRENCY_BCH
            auto const marker = false;
#else
            // Detect witness as no inputs (marker) and expected flag (bip144).
            auto const marker = inputs_.size() == witness_marker && source.peek_byte() == witness_flag;
#endif

            // This is always enabled so caller should validate with is_segregated.
            if (marker) {
                // Skip over the peeked witness flag.
                source.skip(1);
                detail::read(source, inputs_, wire, witness_val(witness));
                detail::read(source, outputs_, wire, witness_val(witness));
#ifndef BITPRIM_CURRENCY_BCH
                detail::read_witnesses(source, inputs_);
#endif
            } else {
                detail::read(source, outputs_, wire, witness_val(witness));
            }

            locktime_ = source.read_4_bytes_little_endian();
        } else {
            // Database (outputs forward) serialization.
            // Witness data is managed internal to inputs.
            detail::read(source, outputs_, wire, witness_val(witness));
            detail::read(source, inputs_, wire, witness_val(witness));
            auto const locktime = source.read_variable_little_endian();
            auto const version = source.read_variable_little_endian();

            if (locktime > max_uint32 || version > max_uint32)
                source.invalidate();

            locktime_ = static_cast<uint32_t>(locktime);
            version_ = static_cast<uint32_t>(version);

#ifdef BITPRIM_CACHED_RPC_DATA
            if (unconfirmed) {
                auto const sigops = source.read_4_bytes_little_endian();
                cached_sigops_ = static_cast<uint32_t>(sigops);
                auto const fees = source.read_8_bytes_little_endian();
                cached_fees_ = static_cast<uint64_t>(fees);
                auto const is_standard = source.read_byte();
                cached_is_standard_ = static_cast<bool>(is_standard);
            }
#endif
        }

        // TODO(libbitcoin): optimize by having reader skip witness data.
        if ( ! witness_val(witness)) {
            strip_witness();
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    bool is_valid() const;

    // Serialization.
    //-----------------------------------------------------------------------------

    data_chunk to_data(bool wire = true, bool witness = false
#ifdef BITPRIM_CACHED_RPC_DATA
                        , bool unconfirmed = false
#endif
                    ) const;

    void to_data(data_sink& stream, bool wire = true, bool witness = false
#ifdef BITPRIM_CACHED_RPC_DATA
                    , bool unconfirmed = false
#endif
                ) const;

    // Witness is not used by outputs, just for template normalization.
    template <Writer W>
    void to_data(W& sink, bool wire = true, bool witness = false
#ifdef BITPRIM_CACHED_RPC_DATA
                , bool unconfirmed = false
#endif
                ) const {
        if (wire) {
            // Witness handling must be disabled for non-segregated txs.
            witness &= is_segregated();

            // Wire (satoshi protocol) serialization.
            sink.write_4_bytes_little_endian(version_);

            if (witness_val(witness)) {
                sink.write_byte(witness_marker);
                sink.write_byte(witness_flag);
                detail::write(sink, inputs_, wire, witness_val(witness));
                detail::write(sink, outputs_, wire, witness_val(witness));
#ifndef BITPRIM_CURRENCY_BCH
                detail::write_witnesses(sink, inputs_);
#endif
            } else {
                detail::write(sink, inputs_, wire, witness_val(witness));
                detail::write(sink, outputs_, wire, witness_val(witness));
            }

            sink.write_4_bytes_little_endian(locktime_);
        } else {
            // Database (outputs forward) serialization.
            // Witness data is managed internal to inputs.
            detail::write(sink, outputs_, wire, witness_val(witness));
            detail::write(sink, inputs_, wire, witness_val(witness));
            sink.write_variable_little_endian(locktime_);
            sink.write_variable_little_endian(version_);

#ifdef BITPRIM_CACHED_RPC_DATA            
            if (unconfirmed) {
                sink.write_4_bytes_little_endian(signature_operations());
                sink.write_8_bytes_little_endian(fees());
                sink.write_byte(is_standard());
            }
#endif
        }
    }

    // Properties (size, accessors, cache).
    //-----------------------------------------------------------------------------

    size_t serialized_size(bool wire = true, bool witness = false
#ifdef BITPRIM_CACHED_RPC_DATA
                            , bool unconfirmed = false
#endif
                        ) const;

    uint32_t version() const;
    void set_version(uint32_t value);

    uint32_t locktime() const;
    void set_locktime(uint32_t value);

    // Deprecated (unsafe).
    ins& inputs();

    const ins& inputs() const;
    void set_inputs(const ins& value);
    void set_inputs(ins&& value);

    // Deprecated (unsafe).
    outs& outputs();

    const outs& outputs() const;
    void set_outputs(const outs& value);
    void set_outputs(outs&& value);

#ifdef BITPRIM_CACHED_RPC_DATA
    uint64_t cached_fees() const;
    uint32_t cached_sigops() const;
    bool cached_is_standard() const;
#endif

    hash_digest outputs_hash() const;
    hash_digest inpoints_hash() const;
    hash_digest sequences_hash() const;
    hash_digest hash(bool witness = false) const;

    // Utilities.
    //-------------------------------------------------------------------------

    /// Clear witness from all inputs (does not change default hash).
    void strip_witness();

    void recompute_hash();

    // Validation.
    //-----------------------------------------------------------------------------

    uint64_t fees() const;
    point::list previous_outputs() const;
    point::list missing_previous_outputs() const;
    hash_list missing_previous_transactions() const;
    uint64_t total_input_value() const;
    uint64_t total_output_value() const;
    size_t signature_operations() const;
    size_t signature_operations(bool bip16, bool bip141) const;
    size_t weight() const;

    bool is_coinbase() const;
    bool is_null_non_coinbase() const;
    bool is_oversized_coinbase() const;
    bool is_mature(size_t height) const;
    bool is_overspent() const;
    bool is_internal_double_spend() const;
    bool is_double_spend(bool include_unconfirmed) const;
    bool is_dusty(uint64_t minimum_output_value) const;
    bool is_missing_previous_outputs() const;
    bool is_final(size_t block_height, uint32_t block_time) const;
    bool is_locked(size_t block_height, uint32_t median_time_past) const;
    bool is_locktime_conflict() const;
    bool is_segregated() const;

    code check(bool transaction_pool = true, bool retarget = true) const;
    code accept(bool transaction_pool = true) const;
    code accept(const chain_state& state, bool transaction_pool = true) const;
    code connect() const;
    code connect(const chain_state& state) const;
    code connect_input(const chain_state& state, size_t input_index) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation validation;

    bool is_standard() const;

protected:
    void reset();
    void invalidate_cache() const;
    bool all_inputs_final() const;

private:
    uint32_t version_;
    uint32_t locktime_;
    input::list inputs_;
    output::list outputs_;

    // TODO(bitprim): (refactor to transaction_result)
    // this 3 variables should be stored in transaction_unconfired database when the store
    // function is called. This values will be in the transaction_result object before
    // creating the transaction object

    //Note(bitprim): Only accesible for unconfirmed txs
#ifdef BITPRIM_CACHED_RPC_DATA
    uint64_t cached_fees_;
    uint32_t cached_sigops_;
    bool cached_is_standard_;
#endif

    // These share a mutex as they are not expected to contend.
    mutable hash_ptr hash_;
    mutable hash_ptr witness_hash_;
    mutable hash_ptr outputs_hash_;
    mutable hash_ptr inpoints_hash_;
    mutable hash_ptr sequences_hash_;
    mutable upgrade_mutex hash_mutex_;

    // These share a mutex as they are not expected to contend.
    mutable boost::optional<uint64_t> total_input_value_;
    mutable boost::optional<uint64_t> total_output_value_;
    mutable boost::optional<bool> segregated_;
    mutable upgrade_mutex mutex_;
};

}  // namespace chain
}  // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif

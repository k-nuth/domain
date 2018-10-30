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
#include <bitcoin/bitcoin/chain/transaction_basis.hpp>
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

// namespace detail {
// // Read a length-prefixed collection of inputs or outputs from the source.
// template <class Source, class Put>
// bool read(Source& source, std::vector<Put>& puts, bool wire, bool witness) {
//     auto result = true;
//     auto const count = source.read_size_little_endian();

//     // Guard against potential for arbitary memory allocation.
//     if (count > get_max_block_size()) {
//         source.invalidate();
//     } else {
//         puts.resize(count);
//     }

//     auto const deserialize = [&](Put& put) {
//         result = result && put.from_data(source, wire, witness_val(witness));
// #ifndef NDBEUG
//         put.script().operations();
// #endif
//     };

//     std::for_each(puts.begin(), puts.end(), deserialize);
//     return result;
// }

// // Write a length-prefixed collection of inputs or outputs to the sink.
// template <class Sink, class Put>
// void write(Sink& sink, const std::vector<Put>& puts, bool wire, bool witness) {
//     sink.write_variable_little_endian(puts.size());

//     auto const serialize = [&](const Put& put) {
//         put.to_data(sink, wire, witness_val(witness));
//     };

//     std::for_each(puts.begin(), puts.end(), serialize);
// }

// #ifndef BITPRIM_CURRENCY_BCH
// // Input list must be pre-populated as it determines witness count.
// template <Reader R, BITPRIM_IS_READER(R)>
// inline void read_witnesses(R& source, input::list& inputs) {
//     auto const deserialize = [&](input& input) {
//         input.witness().from_data(source, true);
//     };

//     std::for_each(inputs.begin(), inputs.end(), deserialize);
// }

// // Witness count is not written as it is inferred from input count.
// template <typename W>
// inline void write_witnesses(W& sink, input::list const& inputs) {
//     auto const serialize = [&sink](input const& input) {
//         input.witness().to_data(sink, true);
//     };

//     std::for_each(inputs.begin(), inputs.end(), serialize);
// }
// #endif // not defined BITPRIM_CURRENCY_BCH

// }  // namespace detail

class BC_API transaction : public transaction_basis {
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


    //Note(bitprim): cannot be defaulted because of the mutex data member.
    transaction(transaction const& x);
    transaction(transaction&& x) noexcept;
    transaction& operator=(transaction const& x);
    transaction& operator=(transaction&& x) noexcept;


    // Operators.
    //-----------------------------------------------------------------------------

    // bool operator==(transaction const& x) const;
    // bool operator!=(transaction const& x) const;

    // Deserialization.
    //-----------------------------------------------------------------------------

    static transaction factory_from_data(data_chunk const& data, bool wire = true, bool witness = false);
    static transaction factory_from_data(std::istream& stream, bool wire = true, bool witness = false);

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

    bool from_data(std::istream& stream, bool wire = true, bool witness = false
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

        transaction_basis::from_data(source, wire, witness);

#ifdef BITPRIM_CACHED_RPC_DATA
        if (! wire && unconfirmed) {
            auto const sigops = source.read_4_bytes_little_endian();
            cached_sigops_ = static_cast<uint32_t>(sigops);
            auto const fees = source.read_8_bytes_little_endian();
            cached_fees_ = static_cast<uint64_t>(fees);
            auto const is_standard = source.read_byte();
            cached_is_standard_ = static_cast<bool>(is_standard);
        }
#endif

        return source;
    }

    // bool is_valid() const;

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

        transaction_basis::to_data(sink, wire, witness);

#ifdef BITPRIM_CACHED_RPC_DATA            
        if ( ! wire && unconfirmed) {
            sink.write_4_bytes_little_endian(signature_operations());
            sink.write_8_bytes_little_endian(fees());
            sink.write_byte(is_standard());
        }
#endif
    }

    // Properties (size, accessors, cache).
    //-----------------------------------------------------------------------------

    size_t serialized_size(bool wire = true, bool witness = false
#ifdef BITPRIM_CACHED_RPC_DATA
                            , bool unconfirmed = false
#endif
                        ) const;

    void set_version(uint32_t value);
    void set_locktime(uint32_t value);
    void set_inputs(const ins& value);
    void set_inputs(ins&& value);
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

#ifndef BITPRIM_CURRENCY_BCH
    /// Clear witness from all inputs (does not change default hash).
    void strip_witness();
#endif

    void recompute_hash();

    // Validation.
    //-----------------------------------------------------------------------------

    using transaction_basis::signature_operations;

    uint64_t fees() const;
    // point::list previous_outputs() const;
    point::list missing_previous_outputs() const;
    // hash_list missing_previous_transactions() const;
    uint64_t total_input_value() const;
    uint64_t total_output_value() const;
    size_t signature_operations() const;
    // size_t signature_operations(bool bip16, bool bip141) const;
    size_t weight() const;

    bool is_overspent() const;
    bool is_segregated() const;

    using transaction_basis::accept;

    code check(bool transaction_pool, bool retarget = true) const;
    code accept(bool transaction_pool = true) const;
    code accept(chain_state const& state, bool transaction_pool = true) const;
    code connect() const;
    code connect(chain_state const& state) const;
    code connect_input(chain_state const& state, size_t input_index) const;


    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation validation{};

    bool is_standard() const;

// protected:
    void reset();

protected:
    void invalidate_cache() const;
    bool all_inputs_final() const;

private:

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

#ifdef BITPRIM_CURRENCY_BCH
code verify(transaction const& tx, uint32_t input_index, uint32_t forks, script const& input_script, script const& prevout_script, uint64_t /*value*/);
#else
code verify(transaction const& tx, uint32_t input_index, uint32_t forks, script const& input_script, witness const& input_witness, script const& prevout_script, uint64_t value);
#endif

code verify(transaction const& tx, uint32_t input, uint32_t forks);



}  // namespace chain
}  // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif

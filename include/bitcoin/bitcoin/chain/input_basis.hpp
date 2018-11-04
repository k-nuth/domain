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
#ifndef LIBBITCOIN_CHAIN_INPUT_BASIS_HPP_
#define LIBBITCOIN_CHAIN_INPUT_BASIS_HPP_

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <vector>

#include <bitcoin/bitcoin/chain/output_point.hpp>
#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/chain/witness.hpp>
#include <bitcoin/bitcoin/define.hpp>
// #include <bitcoin/bitcoin/wallet/payment_address.hpp>
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

class BC_API input_basis {
public:
    using list = std::vector<input_basis>;

    // Constructors.
    //-------------------------------------------------------------------------

    input_basis() = default;
    input_basis(output_point const& previous_output, chain::script const& script, uint32_t sequence);
    input_basis(output_point&& previous_output, chain::script&& script, uint32_t sequence);

#ifndef BITPRIM_CURRENCY_BCH
    input_basis(output_point const& previous_output, chain::script const& script, chain::witness const& witness, uint32_t sequence);
    input_basis(output_point&& previous_output, chain::script&& script, chain::witness&& witness, uint32_t sequence);
#endif

    input_basis(input_basis const& x) = default;
    input_basis(input_basis&& x)  = default;
    input_basis& operator=(input_basis const& x) = default;
    input_basis& operator=(input_basis&& x) = default;

    // Operators.
    //-------------------------------------------------------------------------

    bool operator==(input_basis const& x) const;
    bool operator!=(input_basis const& x) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static input_basis factory_from_data(data_chunk const& data, bool wire = true, bool witness = false);
    static input_basis factory_from_data(std::istream& stream, bool wire = true, bool witness = false);

    template <Reader R, BITPRIM_IS_READER(R)>
    static input_basis factory_from_data(R& source, bool wire = true, bool witness = false) {
        input_basis instance;
        instance.from_data(source, wire, witness_val(witness));
        return instance;
    }

    bool from_data(data_chunk const& data, bool wire = true, bool witness = false);
    bool from_data(std::istream& stream, bool wire = true, bool witness = false);

    template <Reader R, BITPRIM_IS_READER(R)>

    bool from_data(R& source, bool wire = true, BITPRIM_DECL_WITN_ARG) {
#ifndef BITPRIM_CURRENCY_BCH
        // Always write witness to store so that we know how to read it.
        witness |= !wire;
#endif

        reset();

        if ( ! previous_output_.from_data(source, wire)) {
            return false;
        }

        script_.from_data(source, true);

#ifndef BITPRIM_CURRENCY_BCH
        // Transaction from_data handles the discontiguous wire witness decoding.
        if (witness_val(witness) && !wire) {
            witness_.from_data(source, true);
        }
#endif
        sequence_ = source.read_4_bytes_little_endian();

        if ( ! source) {
            reset();
        }

        return source;
    }

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool wire = true, bool witness = false) const;
    void to_data(data_sink& stream, bool wire = true, bool witness = false) const;

    template <Writer W>
    void to_data(W& sink, bool wire = true, BITPRIM_DECL_WITN_ARG) const {
#ifndef BITPRIM_CURRENCY_BCH
        // Always write witness to store so that we know how to read it.
        witness |= !wire;
#endif

        previous_output_.to_data(sink, wire);
        script_.to_data(sink, true);

#ifndef BITPRIM_CURRENCY_BCH
        // Transaction to_data handles the discontiguous wire witness encoding.
        if (witness_val(witness) && !wire) {
            witness_.to_data(sink, true);
        }
#endif
        sink.write_4_bytes_little_endian(sequence_);
    }

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size_non_witness(bool wire) const;

    /// This accounts for wire witness, but does not read or write it.
    size_t serialized_size(bool wire = true, bool witness = false) const;


    output_point& previous_output();
    output_point const& previous_output() const;
    void set_previous_output(output_point const& value);
    void set_previous_output(output_point&& value);

    // Deprecated (unsafe).
    chain::script& script();
    chain::script const& script() const;
    void set_script(chain::script const& value);
    void set_script(chain::script&& value);


#ifndef BITPRIM_CURRENCY_BCH
    // Deprecated (unsafe).
    chain::witness& witness();
    chain::witness const& witness() const;
    void set_witness(chain::witness const& value);
    void set_witness(chain::witness&& value);
#endif // BITPRIM_CURRENCY_BCH

    uint32_t sequence() const;
    void set_sequence(uint32_t value);

    // Utilities.
    //-------------------------------------------------------------------------

#ifndef BITPRIM_CURRENCY_BCH
    /// Clear witness.
    void strip_witness();
#endif

    // Validation.
    //-------------------------------------------------------------------------

    bool is_final() const;
    bool is_segregated() const;
    bool is_locked(size_t block_height, uint32_t median_time_past) const;
    size_t signature_operations(bool bip16, bool bip141) const;
    bool extract_reserved_hash(hash_digest& out) const;
    bool extract_embedded_script(chain::script& out) const;

#ifndef BITPRIM_CURRENCY_BCH
    bool extract_witness_script(chain::script& out, chain::script const& prevout) const;
#endif

// protected:
    void reset();

private:
    output_point previous_output_;
    chain::script script_;
#ifndef BITPRIM_CURRENCY_BCH
    chain::witness witness_;
#endif
    uint32_t sequence_{0};
};

}  // namespace chain
}  // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif // LIBBITCOIN_CHAIN_INPUT_BASIS_HPP_
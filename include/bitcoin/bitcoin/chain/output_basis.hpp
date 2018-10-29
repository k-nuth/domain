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
#ifndef LIBBITCOIN_CHAIN_OUTPUT_BASIS_HPP_
#define LIBBITCOIN_CHAIN_OUTPUT_BASIS_HPP_

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/define.hpp>
// #include <bitcoin/bitcoin/wallet/payment_address.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/thread.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace chain {

class BC_API output_basis {
public:
    using list = std::vector<output_basis>;

    /// This is a sentinel used in .value to indicate not found in store.
    /// This is a sentinel used in cache.value to indicate not populated.
    /// This is a consensus value used in script::generate_signature_hash.
    static uint64_t const not_found;

    // Constructors.
    //-------------------------------------------------------------------------

    output_basis();

    output_basis(uint64_t value, chain::script const& script);
    output_basis(uint64_t value, chain::script&& script);


    output_basis(output_basis const& x);
    output_basis(output_basis&& x) noexcept;
    output_basis& operator=(output_basis const& x);
    output_basis& operator=(output_basis&& x) noexcept;

    // Operators.
    //-------------------------------------------------------------------------
    bool operator==(output_basis const& x) const;
    bool operator!=(output_basis const& x) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static output_basis factory_from_data(data_chunk const& data, bool wire = true);
    static output_basis factory_from_data(std::istream& stream, bool wire = true);

    template <Reader R, BITPRIM_IS_READER(R)>
    static output_basis factory_from_data(R& source, bool wire = true) {
        output_basis instance;
        instance.from_data(source, wire);
        return instance;
    }

    bool from_data(data_chunk const& data, bool wire = true);
    bool from_data(std::istream& stream, bool wire = true);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(R& source, bool wire = true, bool  /*unused*/ = false) {
        reset();

        value_ = source.read_8_bytes_little_endian();
        script_.from_data(source, true);

        if ( ! source) {
            reset();
        }

        return source;
    }

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool wire = true) const;
    void to_data(data_sink& stream, bool wire = true) const;

    template <Writer W>
    void to_data(W& sink, bool wire = true, bool  /*unused*/ = false) const {
        sink.write_8_bytes_little_endian(value_);
        script_.to_data(sink, true);
    }

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size(bool wire = true) const;

    uint64_t value() const;
    void set_value(uint64_t value);

    // Deprecated (unsafe).
    chain::script& script();
    chain::script const& script() const;
    void set_script(chain::script const& value);
    void set_script(chain::script&& value);

    // /// The payment address extracted from this output as a standard script.
    // wallet::payment_address address(bool testnet = false) const;

    // /// The first payment address extracted (may be invalid).
    // wallet::payment_address address(uint8_t p2kh_version, uint8_t p2sh_version) const;

    // /// The payment addresses extracted from this output as a standard script.
    // wallet::payment_address::list addresses(
    //     uint8_t p2kh_version = wallet::payment_address::mainnet_p2kh,
    //     uint8_t p2sh_version = wallet::payment_address::mainnet_p2sh) const;

    // Validation.
    //-------------------------------------------------------------------------

    size_t signature_operations(bool bip141) const;
    bool is_dust(uint64_t minimum_output_value) const;
    bool extract_committed_hash(hash_digest& out) const;

// protected:
    void reset();

private:
    uint64_t value_{not_found};
    chain::script script_;
};

}  // namespace chain
}  // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif // LIBBITCOIN_CHAIN_OUTPUT_BASIS_HPP_
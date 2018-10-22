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
#ifndef LIBBITCOIN_CHAIN_OUTPUT_HPP
#define LIBBITCOIN_CHAIN_OUTPUT_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/thread.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>
#include <bitcoin/bitcoin/wallet/payment_address.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace chain {

class BC_API output
{
public:
    typedef std::vector<output> list;

    /// This is a sentinel used in .value to indicate not found in store.
    /// This is a sentinel used in cache.value to indicate not populated.
    /// This is a consensus value used in script::generate_signature_hash.
    static const uint64_t not_found;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation
    {
        /// This is a non-consensus sentinel indicating output is unspent.
        static const uint32_t not_spent;

        size_t spender_height = validation::not_spent;
    };

    // Constructors.
    //-------------------------------------------------------------------------

    output();

    output(output&& other);
    output(const output& other);

    output(uint64_t value, chain::script&& script);
    output(uint64_t value, const chain::script& script);

    // Operators.
    //-------------------------------------------------------------------------

    output& operator=(output&& other);
    output& operator=(const output& other);

    bool operator==(const output& other) const;
    bool operator!=(const output& other) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static output factory_from_data(const data_chunk& data, bool wire=true);
    static output factory_from_data(data_source& stream, bool wire=true);
    
    template <Reader R, BITPRIM_IS_READER(R)>
    static output factory_from_data(R& source, bool wire=true)
    {
        output instance;
        instance.from_data(source, wire);
        return instance;
    }

    //static output factory_from_data(reader& source, bool wire=true);

    bool from_data(const data_chunk& data, bool wire=true);
    bool from_data(data_source& stream, bool wire=true);
    
    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(R& source, bool wire=true, bool unused=false)
    {
        reset();
    
        if (!wire)
            validation.spender_height = source.read_4_bytes_little_endian();
    
        value_ = source.read_8_bytes_little_endian();
        script_.from_data(source, true);
    
        if (!source)
            reset();
    
        return source;
    }

    //bool from_data(reader& source, bool wire=true, bool unused=false);

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool wire=true) const;
    void to_data(data_sink& stream, bool wire=true) const;
    
    template <Writer W>
    void to_data(W& sink, bool wire=true, bool unused=false) const
    {
        if (!wire)
        {
            auto height32 = safe_unsigned<uint32_t>(validation.spender_height);
            sink.write_4_bytes_little_endian(height32);
        }
    
        sink.write_8_bytes_little_endian(value_);
        script_.to_data(sink, true);
    }

    //void to_data(writer& sink, bool wire=true, bool unused=false) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size(bool wire=true) const;

    uint64_t value() const;
    void set_value(uint64_t value);

    // Deprecated (unsafe).
    chain::script& script();
    const chain::script& script() const;
    void set_script(const chain::script& value);
    void set_script(chain::script&& value);

    /// The payment address extracted from this output as a standard script.
    wallet::payment_address address(bool testnet = false) const;

    /// The first payment address extracted (may be invalid).
    wallet::payment_address address(uint8_t p2kh_version, uint8_t p2sh_version) const;

    /// The payment addresses extracted from this output as a standard script.
    wallet::payment_address::list addresses(
        uint8_t p2kh_version=wallet::payment_address::mainnet_p2kh,
        uint8_t p2sh_version=wallet::payment_address::mainnet_p2sh) const;

    // Validation.
    //-------------------------------------------------------------------------

    size_t signature_operations(bool bip141) const;
    bool is_dust(uint64_t minimum_output_value) const;
    bool extract_committed_hash(hash_digest& out) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation validation;

protected:
    void reset();
    void invalidate_cache() const;

private:
    typedef std::shared_ptr<wallet::payment_address::list> addresses_ptr;

    addresses_ptr addresses_cache() const;

    mutable upgrade_mutex mutex_;
    mutable addresses_ptr addresses_;

    uint64_t value_;
    chain::script script_;
};

} // namespace chain
} // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif
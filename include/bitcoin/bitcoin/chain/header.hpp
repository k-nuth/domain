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
#ifndef LIBBITCOIN_CHAIN_HEADER_HPP
#define LIBBITCOIN_CHAIN_HEADER_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <bitcoin/bitcoin/chain/chain_state.hpp>
#include <bitcoin/bitcoin/chain/header_basis.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/error.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/thread.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace chain {

class BC_API header : public header_basis {
public:
    using list = std::vector<header>;
    using ptr = std::shared_ptr<header>;
    using const_ptr = std::shared_ptr<header const>;
    using ptr_list = std::vector<header>;
    using const_ptr_list = std::vector<const_ptr>;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation {
        size_t height = 0;
        uint32_t median_time_past = 0;
    };

    // Constructors.
    //-----------------------------------------------------------------------------

    header();
    header(header const& x, hash_digest const& hash);
    header(uint32_t version, hash_digest const& previous_block_hash, hash_digest const& merkle, uint32_t timestamp, uint32_t bits, uint32_t nonce);

    header(header const& x);
    /// This class is copy assignable.
    header& operator=(header const& x);

    // Operators.
    //-----------------------------------------------------------------------------

    bool operator==(header const& x) const;
    bool operator!=(header const& x) const;

    // Deserialization.
    //-----------------------------------------------------------------------------

    static header factory_from_data(data_chunk const& data, bool wire = true);
    // static header factory_from_data(data_source& stream, bool wire=true);
    static header factory_from_data(data_source& stream, bool wire = true);

    template <Reader R, BITPRIM_IS_READER(R)>
    static header factory_from_data(R& source, bool wire = true) {
        header instance;
        instance.from_data(source, wire);
        return instance;
    }

    //static header factory_from_data(reader& source, bool wire=true);

    bool from_data(data_chunk const& data, bool wire = true);

    //TODO(fernando): check what happend when replacing std::istream to data_source
    // bool from_data(data_source& stream, bool wire=true);
    bool from_data(data_source& stream, bool wire = true);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(R& source, bool wire = true) {
        header_basis::from_data(source, wire);

        if ( ! wire) {
            validation.median_time_past = source.read_4_bytes_little_endian();
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    //bool from_data(reader& source, bool wire=true);

    bool is_valid() const;

    // Serialization.
    //-----------------------------------------------------------------------------

    data_chunk to_data(bool wire = true) const;
    // void to_data(data_sink& stream, bool wire=true) const;
    void to_data(data_sink& stream, bool wire = true) const;

    template <Writer W>
    void to_data(W& sink, bool wire = true) const {
        header_basis::to_data(sink, wire);

        if ( ! wire) {
            sink.write_4_bytes_little_endian(validation.median_time_past);
        }
    }

    // Properties (size, accessors, cache).
    //-----------------------------------------------------------------------------
    static uint256_t proof(uint32_t bits);
    uint256_t proof() const;

    static size_t satoshi_fixed_size();
    size_t serialized_size(bool wire = true) const;

    uint32_t version() const;
    void set_version(uint32_t value);

    // Deprecated (unsafe).
    hash_digest& previous_block_hash();

    hash_digest const& previous_block_hash() const;
    void set_previous_block_hash(hash_digest const& value);

    // Deprecated (unsafe).
    hash_digest& merkle();

    hash_digest const& merkle() const;
    void set_merkle(hash_digest const& value);

    uint32_t timestamp() const;
    void set_timestamp(uint32_t value);

    uint32_t bits() const;
    void set_bits(uint32_t value);

    uint32_t nonce() const;
    void set_nonce(uint32_t value);

    hash_digest hash() const;

#ifdef BITPRIM_CURRENCY_LTC
    hash_digest litecoin_proof_of_work_hash() const;
#endif  //BITPRIM_CURRENCY_LTC

    // Validation.
    //-----------------------------------------------------------------------------

    bool is_valid_timestamp() const;
    bool is_valid_proof_of_work(bool retarget = true) const;

    code check(bool retarget = false) const;
    code accept(const chain_state& state) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation validation;

protected:
    // So that block may call reset from its own.
    friend class block;

    void reset();
    void invalidate_cache() const;

private:
    mutable upgrade_mutex mutex_;
    mutable std::shared_ptr<hash_digest> hash_;
};

}  // namespace chain
}  // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif

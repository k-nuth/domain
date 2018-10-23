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

class BC_API header {
   public:
    typedef std::vector<header> list;
    typedef std::shared_ptr<header> ptr;
    typedef std::shared_ptr<const header> const_ptr;
    typedef std::vector<header> ptr_list;
    typedef std::vector<const_ptr> const_ptr_list;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation {
        size_t height = 0;
        uint32_t median_time_past = 0;
    };

    // Constructors.
    //-----------------------------------------------------------------------------

    header();

    header(header&& other);
    header(const header& other);

    header(header&& other, hash_digest&& hash);
    header(const header& other, const hash_digest& hash);

    header(uint32_t version, const hash_digest& previous_block_hash, const hash_digest& merkle, uint32_t timestamp, uint32_t bits, uint32_t nonce);
    header(uint32_t version, hash_digest&& previous_block_hash, hash_digest&& merkle, uint32_t timestamp, uint32_t bits, uint32_t nonce);

    // Operators.
    //-----------------------------------------------------------------------------

    /// This class is move and copy assignable.
    header& operator=(header&& other);
    header& operator=(const header& other);

    bool operator==(const header& other) const;
    bool operator!=(const header& other) const;

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
        ////reset();

        version_ = source.read_4_bytes_little_endian();
        previous_block_hash_ = source.read_hash();
        merkle_ = source.read_hash();
        timestamp_ = source.read_4_bytes_little_endian();
        bits_ = source.read_4_bytes_little_endian();
        nonce_ = source.read_4_bytes_little_endian();

        if (!wire)
            validation.median_time_past = source.read_4_bytes_little_endian();

        if (!source)
            reset();

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
        sink.write_4_bytes_little_endian(version_);
        sink.write_hash(previous_block_hash_);
        sink.write_hash(merkle_);
        sink.write_4_bytes_little_endian(timestamp_);
        sink.write_4_bytes_little_endian(bits_);
        sink.write_4_bytes_little_endian(nonce_);

        if (!wire)
            sink.write_4_bytes_little_endian(validation.median_time_past);
    }

    //void to_data(writer& sink, bool wire=true) const;

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

    const hash_digest& previous_block_hash() const;
    void set_previous_block_hash(const hash_digest& value);
    void set_previous_block_hash(hash_digest&& value);

    // Deprecated (unsafe).
    hash_digest& merkle();

    const hash_digest& merkle() const;
    void set_merkle(const hash_digest& value);
    void set_merkle(hash_digest&& value);

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

    uint32_t version_;
    hash_digest previous_block_hash_;
    hash_digest merkle_;
    uint32_t timestamp_;
    uint32_t bits_;
    uint32_t nonce_;
};

}  // namespace chain
}  // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif

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
    struct validation_t {
        size_t height = 0;
        uint32_t median_time_past = 0;
    };

    // Constructors.
    //-----------------------------------------------------------------------------
    using header_basis::header_basis; // inherit constructors from header_basis

    header() = default;
    header(header const& x, hash_digest const& hash);

    /// This class is copy constructible and copy assignable.
    // Note(bitprim): Cannot be defaulted because the std::mutex data member.
    header(header const& x);
    header& operator=(header const& x);

    // Operators.
    //-----------------------------------------------------------------------------

    // bool operator==(header const& x) const;
    // bool operator!=(header const& x) const;

    // Deserialization.
    //-----------------------------------------------------------------------------

    static header factory_from_data(data_chunk const& data, bool wire = true);
    // static header factory_from_data(std::istream& stream, bool wire=true);
    static header factory_from_data(std::istream& stream, bool wire = true);

    template <Reader R, BITPRIM_IS_READER(R)>
    static header factory_from_data(R& source, bool wire = true) {
        header instance;
        instance.from_data(source, wire);
        return instance;
    }

    bool from_data(data_chunk const& data, bool wire = true);

    //TODO(fernando): check what happend when replacing std::istream to data_source
    bool from_data(std::istream& stream, bool wire = true);

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

    size_t serialized_size(bool wire = true) const;

    void set_version(uint32_t value);
    void set_previous_block_hash(hash_digest const& value);
    void set_merkle(hash_digest const& value);
    void set_timestamp(uint32_t value);
    void set_bits(uint32_t value);
    void set_nonce(uint32_t value);

    hash_digest hash() const;
    hash_digest hash_pow() const;

#ifdef BITPRIM_CURRENCY_LTC
    hash_digest litecoin_proof_of_work_hash() const;
#endif  //BITPRIM_CURRENCY_LTC

    // Validation.
    //-----------------------------------------------------------------------------

    bool is_valid_proof_of_work(bool retarget = true) const;

    code check(bool retarget = false) const;
    code accept(chain_state const& state) const;


    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation_t validation{};

// protected:
    // So that block may call reset from its own.
    // friend class block;

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

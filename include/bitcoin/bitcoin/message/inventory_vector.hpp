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
#ifndef LIBBITCOIN_MESSAGE_INVENTORY_VECTOR_HPP
#define LIBBITCOIN_MESSAGE_INVENTORY_VECTOR_HPP

#include <cstdint>
#include <istream>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API inventory_vector {
public:
    typedef std::vector<inventory_vector> list;

    enum class type_id : uint32_t {
        error = 0,
        transaction = 1,
        block = 2,
        filtered_block = 3,
        compact_block = 4,
        witness = (1u << 30),
        witness_transaction = witness | transaction,
        witness_block = witness | block,
        reserved = witness | filtered_block
    };

    static type_id to_type(uint32_t value);
    static uint32_t to_number(type_id type);
    static std::string to_string(type_id type);

    static inventory_vector factory_from_data(uint32_t version, data_chunk const& data);
    static inventory_vector factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static inventory_vector factory_from_data(uint32_t version, R& source) {
        inventory_vector instance;
        instance.from_data(version, source);
        return instance;
    }

    //static inventory_vector factory_from_data(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    inventory_vector();
    inventory_vector(type_id type, hash_digest const& hash);
    inventory_vector(type_id type, hash_digest const& hash);
    inventory_vector(const inventory_vector& x);
    inventory_vector(inventory_vector&& x);

    type_id type() const;
    void set_type(type_id value);

    hash_digest& hash();
    hash_digest const& hash() const;
    void set_hash(hash_digest const& value);
    void set_hash(hash_digest const& value);

    bool is_block_type() const;
    bool is_transaction_type() const;

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        auto const raw_type = source.read_4_bytes_little_endian();
        type_ = inventory_vector::to_type(raw_type);
        hash_ = source.read_hash();

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        auto const raw_type = inventory_vector::to_number(type_);
        sink.write_4_bytes_little_endian(raw_type);
        sink.write_hash(hash_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    void to_witness();
    size_t serialized_size(uint32_t version) const;

    // This class is move assignable but not copy assignable.
    inventory_vector& operator=(inventory_vector&& x);
    void operator=(const inventory_vector& x);

    bool operator==(const inventory_vector& x) const;
    bool operator!=(const inventory_vector& x) const;

private:
    type_id type_;
    hash_digest hash_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

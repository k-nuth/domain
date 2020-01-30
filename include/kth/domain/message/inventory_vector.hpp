// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_INVENTORY_VECTOR_HPP
#define KTH_MESSAGE_INVENTORY_VECTOR_HPP

#include <cstdint>
#include <istream>
#include <string>

#include <kth/domain/define.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace kth {
namespace message {

class BC_API inventory_vector {
public:
    using list = std::vector<inventory_vector>;

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
    static inventory_vector factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static inventory_vector factory_from_data(uint32_t version, R& source) {
        inventory_vector instance;
        instance.from_data(version, source);
        return instance;
    }

    //static inventory_vector factory_from_data(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    inventory_vector();
    inventory_vector(type_id type, hash_digest const& hash);
    inventory_vector(inventory_vector const& x) = default;
    inventory_vector(inventory_vector&& x) = default;

    // This class is move assignable but not copy assignable.
    inventory_vector& operator=(inventory_vector&& x) = default;
    inventory_vector& operator=(inventory_vector const& x) = default;

    bool operator==(inventory_vector const& x) const;
    bool operator!=(inventory_vector const& x) const;


    type_id type() const;
    void set_type(type_id value);

    hash_digest& hash();
    hash_digest const& hash() const;
    void set_hash(hash_digest const& value);

    bool is_block_type() const;
    bool is_transaction_type() const;

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(uint32_t  /*version*/, R& source) {
        reset();

        auto const raw_type = source.read_4_bytes_little_endian();
        type_ = inventory_vector::to_type(raw_type);
        hash_ = source.read_hash();

        if ( ! source) {
            reset();
}

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t  /*version*/, W& sink) const {
        auto const raw_type = inventory_vector::to_number(type_);
        sink.write_4_bytes_little_endian(raw_type);
        sink.write_hash(hash_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();

#ifndef KTH_CURRENCY_BCH    
    void to_witness();
#endif
    
    size_t serialized_size(uint32_t version) const;


private:
    type_id type_{type_id::error};
    hash_digest hash_{null_hash};
};

}  // namespace message
}  // namespace kth

#endif

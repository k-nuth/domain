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
#ifndef LIBBITCOIN_MESSAGE_INVENTORY_HPP
#define LIBBITCOIN_MESSAGE_INVENTORY_HPP

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/inventory_vector.hpp>
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

class BC_API inventory {
public:
    using ptr = std::shared_ptr<inventory>;
    using const_ptr = std::shared_ptr<const inventory>;
    using type_id = inventory_vector::type_id;

    static inventory factory_from_data(uint32_t version, data_chunk const& data);
    static inventory factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static inventory factory_from_data(uint32_t version, R& source) {
        inventory instance;
        instance.from_data(version, source);
        return instance;
    }

    inventory() = default;
    inventory(inventory_vector::list const& values);
    inventory(inventory_vector::list&& values);
    inventory(hash_list const& hashes, type_id type);
    inventory(std::initializer_list<inventory_vector> const& values);

    // inventory(inventory const& x) = default;
    // inventory(inventory&& x) = default;
    // // This class is move assignable but not copy assignable.
    // inventory& operator=(inventory&& x) = default;
    // inventory& operator=(inventory const&) = default;

    bool operator==(inventory const& x) const;
    bool operator!=(inventory const& x) const;


    inventory_vector::list& inventories();
    inventory_vector::list const& inventories() const;
    void set_inventories(inventory_vector::list const& value);
    void set_inventories(inventory_vector::list&& value);

    /*virtual*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, data_chunk const& data);

    /*virtual*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    /*virtual*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, R& source) {
        reset();

        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > max_inventory) {
            source.invalidate();
        } else {
            inventories_.resize(count);
        }

        // Order is required.
        for (auto& inventory : inventories_) {
            if ( ! inventory.from_data(version, source)) {
                break;
            }
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_variable_little_endian(inventories_.size());

        for (auto const& inventory : inventories_) {
            inventory.to_data(version, sink);
}
    }

    //void to_data(uint32_t version, writer& sink) const;
    void to_hashes(hash_list& out, type_id type) const;
    void reduce(inventory_vector::list& out, type_id type) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;
    size_t count(type_id type) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    inventory_vector::list inventories_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

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
#ifndef LIBBITCOIN_MESSAGE_GET_DATA_HPP
#define LIBBITCOIN_MESSAGE_GET_DATA_HPP

#include <initializer_list>
#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/inventory.hpp>
#include <bitcoin/bitcoin/message/inventory_vector.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API get_data
    : public inventory {
public:
    typedef std::shared_ptr<get_data> ptr;
    typedef std::shared_ptr<const get_data> const_ptr;

    static get_data factory_from_data(uint32_t version, data_chunk const& data);
    static get_data factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static get_data factory_from_data(uint32_t version, R& source) {
        get_data instance;
        instance.from_data(version, source);
        return instance;
    }

    //static get_data factory_from_data(uint32_t version, reader& source);

    get_data();
    get_data(const inventory_vector::list& list);
    get_data(inventory_vector::list&& list);
    get_data(const hash_list& hashes, type_id type);
    get_data(std::initializer_list<inventory_vector> const& elements);
    get_data(const get_data& other);
    get_data(get_data&& other);

    bool from_data(uint32_t version, data_chunk const& data); /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, data_source& stream); /*override*/     //TODO(fernando): check if this function is used in a run-time-polymorphic way

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    {
        if ( ! inventory::from_data(version, source))
            return false;

        if (version < get_data::version_minimum)
            source.invalidate();

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source) override;

    /// Convert message types to witness types.
    void to_witness();

    // This class is move assignable but not copy assignable.
    get_data& operator=(get_data&& other);
    void operator=(const get_data&) = delete;

    bool operator==(const get_data& other) const;
    bool operator!=(const get_data& other) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

}  // namespace message
}  // namespace libbitcoin

#endif

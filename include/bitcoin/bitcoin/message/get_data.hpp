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

class BC_API get_data : public inventory {
public:
    using ptr = std::shared_ptr<get_data>;
    using const_ptr = std::shared_ptr<const get_data>;

    static get_data factory_from_data(uint32_t version, data_chunk const& data);
    static get_data factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static get_data factory_from_data(uint32_t version, R& source) {
        get_data instance;
        instance.from_data(version, source);
        return instance;
    }

    get_data() = default;
    get_data(inventory_vector::list const& values);
    get_data(inventory_vector::list&& values);
    get_data(hash_list const& hashes, type_id type);
    get_data(std::initializer_list<inventory_vector> const& values);

    // get_data(get_data const& x) = default;
    // get_data(get_data&& x) = default;
    // // This class is move assignable but not copy assignable.
    // get_data& operator=(get_data&& x) = default;
    // get_data& operator=(get_data const&) = default;

    bool operator==(get_data const& x) const;
    bool operator!=(get_data const& x) const;


    bool from_data(uint32_t version, data_chunk const& data); /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, std::istream& stream); /*override*/     //TODO(fernando): check if this function is used in a run-time-polymorphic way

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    {
        if ( ! inventory::from_data(version, source)) {
            return false;
}

        if (version < get_data::version_minimum) {
            source.invalidate();
}

        if ( ! source) {
            reset();
}

        return source;
    }

#ifndef BITPRIM_CURRENCY_BCH
    /// Convert message types to witness types.
    void to_witness();
#endif

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

}  // namespace message
}  // namespace libbitcoin

#endif

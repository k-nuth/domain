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
#ifndef LIBBITCOIN_MESSAGE_NOT_FOUND_HPP
#define LIBBITCOIN_MESSAGE_NOT_FOUND_HPP

#include <initializer_list>
#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/constants.hpp>
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

class BC_API not_found
    : public inventory {
public:
    typedef std::shared_ptr<not_found> ptr;
    typedef std::shared_ptr<const not_found> const_ptr;

    static not_found factory_from_data(uint32_t version, data_chunk const& data);
    static not_found factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static not_found factory_from_data(uint32_t version, R& source) {
        not_found instance;
        instance.from_data(version, source);
        return instance;
    }

    //static not_found factory_from_data(uint32_t version, reader& source);

    not_found();
    not_found(inventory_vector::list const& values);
    not_found(inventory_vector::list&& values);
    not_found(hash_list const& hashes, type_id type);
    not_found(std::initializer_list<inventory_vector> const& values);
    not_found(const not_found& x);
    not_found(not_found&& x);

    bool from_data(uint32_t version, data_chunk const& data); /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, data_source& stream); /*override*/     //TODO(fernando): check if this function is used in a run-time-polymorphic way

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    {
        if ( ! inventory::from_data(version, source))
            return false;

        if (version < not_found::version_minimum)
            source.invalidate();

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source) override;

    // This class is move assignable but not copy assignable.
    not_found& operator=(not_found&& x);
    void operator=(const not_found&) = delete;

    bool operator==(const not_found& x) const;
    bool operator!=(const not_found& x) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

}  // namespace message
}  // namespace libbitcoin

#endif

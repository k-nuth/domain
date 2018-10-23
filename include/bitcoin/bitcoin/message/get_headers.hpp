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
#ifndef LIBBITCOIN_MESSAGE_GET_HEADERS_HPP
#define LIBBITCOIN_MESSAGE_GET_HEADERS_HPP

#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/message/get_blocks.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API get_headers
    : public get_blocks {
public:
    typedef std::shared_ptr<get_headers> ptr;
    typedef std::shared_ptr<const get_headers> const_ptr;

    static get_headers factory_from_data(uint32_t version, data_chunk const& data);
    static get_headers factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static get_headers factory_from_data(uint32_t version, R& source) {
        get_headers instance;
        instance.from_data(version, source);
        return instance;
    }

    //static get_headers factory_from_data(uint32_t version, reader& source);

    get_headers();
    get_headers(const hash_list& start, hash_digest const& stop);
    get_headers(hash_list&& start, hash_digest const& stop);
    get_headers(const get_headers& x);
    get_headers(get_headers&& x);

    bool from_data(uint32_t version, data_chunk const& data); /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, data_source& stream); /*override*/     //TODO(fernando): check if this function is used in a run-time-polymorphic way

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    {
        if ( ! get_blocks::from_data(version, source))
            return false;

        if (version < get_headers::version_minimum)
            source.invalidate();

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source) override;

    // This class is move assignable but not copy assignable.
    get_headers& operator=(get_headers&& x);
    void operator=(const get_headers&) = delete;

    bool operator==(const get_headers& x) const;
    bool operator!=(const get_headers& x) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

}  // namespace message
}  // namespace libbitcoin

#endif

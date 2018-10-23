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
#ifndef LIBBITCOIN_MESSAGE_HEADERS_HPP
#define LIBBITCOIN_MESSAGE_HEADERS_HPP

#include <cstdint>
#include <initializer_list>
#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/header.hpp>
#include <bitcoin/bitcoin/message/inventory.hpp>
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

class BC_API headers {
public:
    typedef std::shared_ptr<headers> ptr;
    typedef std::shared_ptr<headers const> const_ptr;

    static headers factory_from_data(uint32_t version, data_chunk const& data);
    static headers factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static headers factory_from_data(uint32_t version, R& source) {
        headers instance;
        instance.from_data(version, source);
        return instance;
    }

    //static headers factory_from_data(uint32_t version, reader& source);

    headers();
    headers(header::list const& values);
    headers(header::list&& values);
    headers(std::initializer_list<header> const& values);
    headers(headers const& x);
    headers(headers&& x);

    header::list& elements();
    header::list const& elements() const;
    void set_elements(header::list const& values);
    void set_elements(header::list&& values);

    bool is_sequential() const;
    void to_hashes(hash_list& out) const;
    void to_inventory(inventory_vector::list& out,
                      inventory::type_id type) const;

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > max_get_headers)
            source.invalidate();
        else
            elements_.resize(count);

        // Order is required.
        for (auto& element : elements_)
            if ( ! element.from_data(version, source))
                break;

        if (version < headers::version_minimum)
            source.invalidate();

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_variable_little_endian(elements_.size());

        for (auto const& element : elements_)
            element.to_data(version, sink);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    // This class is move assignable but not copy assignable.
    headers& operator=(headers&& x);
    void operator=(headers const&) = delete;

    bool operator==(headers const& x) const;
    bool operator!=(headers const& x) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    header::list elements_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

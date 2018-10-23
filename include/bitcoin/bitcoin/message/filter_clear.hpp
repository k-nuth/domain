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
#ifndef LIBBITCOIN_MESSAGE_FILTER_CLEAR_HPP
#define LIBBITCOIN_MESSAGE_FILTER_CLEAR_HPP

#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API filter_clear {
   public:
    typedef std::shared_ptr<filter_clear> ptr;
    typedef std::shared_ptr<const filter_clear> const_ptr;

    static filter_clear factory_from_data(uint32_t version, data_chunk const& data);
    static filter_clear factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static filter_clear factory_from_data(uint32_t version, R& source) {
        filter_clear instance;
        instance.from_data(version, source);
        return instance;
    }

    //static filter_clear factory_from_data(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    filter_clear();
    filter_clear(const filter_clear& other);
    filter_clear(filter_clear&& other);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        // Initialize as valid from deserialization.
        insufficient_version_ = false;

        if (version < filter_clear::version_minimum)
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
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    static const std::string command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

   protected:
    filter_clear(bool insufficient_version);

   private:
    bool insufficient_version_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

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
#ifndef LIBBITCOIN_MESSAGE_MEMORY_POOL_HPP
#define LIBBITCOIN_MESSAGE_MEMORY_POOL_HPP

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

class BC_API memory_pool {
   public:
    typedef std::shared_ptr<memory_pool> ptr;
    typedef std::shared_ptr<const memory_pool> const_ptr;

    static memory_pool factory_from_data(uint32_t version, data_chunk const& data);
    static memory_pool factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static memory_pool factory_from_data(uint32_t version, R& source) {
        memory_pool instance;
        instance.from_data(version, source);
        return instance;
    }

    //static memory_pool factory_from_data(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    memory_pool();
    memory_pool(const memory_pool& x);
    memory_pool(memory_pool&& x);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        // Initialize as valid from deserialization.
        insufficient_version_ = false;

        if (version < memory_pool::version_minimum)
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

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

   protected:
    memory_pool(bool insufficient_version);

   private:
    bool insufficient_version_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

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
#ifndef LIBBITCOIN_MESSAGE_FILTER_ADD_HPP
#define LIBBITCOIN_MESSAGE_FILTER_ADD_HPP

#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/constants.hpp>
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

class BC_API filter_add {
public:
    typedef std::shared_ptr<filter_add> ptr;
    typedef std::shared_ptr<const filter_add> const_ptr;

    static filter_add factory_from_data(uint32_t version, data_chunk const& data);
    static filter_add factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static filter_add factory_from_data(uint32_t version, R& source) {
        filter_add instance;
        instance.from_data(version, source);
        return instance;
    }

    //static filter_add factory_from_data(uint32_t version, reader& source);

    filter_add() = default;
    filter_add(data_chunk const& data);
    filter_add(data_chunk&& data);

    filter_add(filter_add const& x) = default;
    filter_add(filter_add&& x) = default;

    // This class is move assignable but not copy assignable.
    filter_add& operator=(filter_add&& x) = default;
    void operator=(filter_add const&) = delete;

    bool operator==(filter_add const& x) const;
    bool operator!=(filter_add const& x) const;

    data_chunk& data();
    data_chunk const& data() const;
    void set_data(data_chunk const& value);
    void set_data(data_chunk&& value);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        auto const size = source.read_size_little_endian();

        if (size > max_filter_add)
            source.invalidate();
        else
            data_ = source.read_bytes(size);

        if (version < filter_add::version_minimum)
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
        sink.write_variable_little_endian(data_.size());
        sink.write_bytes(data_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    data_chunk data_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

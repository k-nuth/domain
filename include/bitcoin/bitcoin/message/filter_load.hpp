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
#ifndef LIBBITCOIN_MESSAGE_FILTER_LOAD_HPP
#define LIBBITCOIN_MESSAGE_FILTER_LOAD_HPP

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

class BC_API filter_load {
public:
    using ptr = std::shared_ptr<filter_load>;
    using const_ptr = std::shared_ptr<const filter_load>;

    static filter_load factory_from_data(uint32_t version, data_chunk const& data);
    static filter_load factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static filter_load factory_from_data(uint32_t version, R& source) {
        filter_load instance;
        instance.from_data(version, source);
        return instance;
    }

    //static filter_load factory_from_data(uint32_t version, reader& source);

    filter_load() = default;
    filter_load(data_chunk const& filter, uint32_t hash_functions, uint32_t tweak, uint8_t flags);
    filter_load(data_chunk&& filter, uint32_t hash_functions, uint32_t tweak, uint8_t flags);
    filter_load(filter_load const& x) = default;
    filter_load(filter_load&& x) = default;

    // This class is move assignable but not copy assignable.
    filter_load& operator=(filter_load&& x) = default;
    filter_load& operator=(filter_load const&) = default;

    bool operator==(filter_load const& x) const;
    bool operator!=(filter_load const& x) const;


    data_chunk& filter();
    data_chunk const& filter() const;
    void set_filter(data_chunk const& value);
    void set_filter(data_chunk&& value);

    uint32_t hash_functions() const;
    void set_hash_functions(uint32_t value);

    uint32_t tweak() const;
    void set_tweak(uint32_t value);

    uint8_t flags() const;
    void set_flags(uint8_t value);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        auto const size = source.read_size_little_endian();

        if (size > max_filter_load) {
            source.invalidate();
        } else {
            filter_ = source.read_bytes(size);
}

        hash_functions_ = source.read_4_bytes_little_endian();

        if (hash_functions_ > max_filter_functions) {
            source.invalidate();
}

        tweak_ = source.read_4_bytes_little_endian();
        flags_ = source.read_byte();

        if (version < filter_load::version_minimum) {
            source.invalidate();
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
    void to_data(uint32_t  /*version*/, W& sink) const {
        sink.write_variable_little_endian(filter_.size());
        sink.write_bytes(filter_);
        sink.write_4_bytes_little_endian(hash_functions_);
        sink.write_4_bytes_little_endian(tweak_);
        sink.write_byte(flags_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;


    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    data_chunk filter_;
    uint32_t hash_functions_{0};
    uint32_t tweak_{0};
    uint8_t flags_{0x00};
};

}  // namespace message
}  // namespace libbitcoin

#endif

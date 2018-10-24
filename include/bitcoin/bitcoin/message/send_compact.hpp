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
#ifndef LIBBITCOIN_MESSAGE_SEND_COMPACT_BLOCKS_HPP
#define LIBBITCOIN_MESSAGE_SEND_COMPACT_BLOCKS_HPP

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

class BC_API send_compact {
   public:
    typedef std::shared_ptr<send_compact> ptr;
    typedef std::shared_ptr<const send_compact> const_ptr;

    static send_compact factory_from_data(uint32_t version, data_chunk const& data);
    static send_compact factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static send_compact factory_from_data(uint32_t version, R& source) {
        send_compact instance;
        instance.from_data(version, source);
        return instance;
    }

    //static send_compact factory_from_data(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    send_compact();
    send_compact(bool high_bandwidth_mode, uint64_t version);
    send_compact(const send_compact& x);
    send_compact(send_compact&& x);

    bool high_bandwidth_mode() const;
    void set_high_bandwidth_mode(bool mode);

    uint64_t version() const;
    void set_version(uint64_t version);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        auto const mode = source.read_byte();

        if (mode > 1)
            source.invalidate();

        high_bandwidth_mode_ = (mode == 1);
        this->version_ = source.read_8_bytes_little_endian();

        if (version < send_compact::version_minimum)
            source.invalidate();

        if (!source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_byte(static_cast<uint8_t>(high_bandwidth_mode_));
        sink.write_8_bytes_little_endian(this->version_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    /// This class is move assignable but not copy assignable.
    send_compact& operator=(send_compact&& x);
    void operator=(const send_compact&) = delete;

    bool operator==(const send_compact& x) const;
    bool operator!=(const send_compact& x) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

   private:
    bool high_bandwidth_mode_;
    uint64_t version_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

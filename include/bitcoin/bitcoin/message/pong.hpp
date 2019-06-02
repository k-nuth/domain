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
#ifndef LIBBITCOIN_MESSAGE_PONG_HPP
#define LIBBITCOIN_MESSAGE_PONG_HPP

#include <cstdint>
#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API pong {
public:
    using ptr = std::shared_ptr<pong>;
    using const_ptr = std::shared_ptr<const pong>;

    static pong factory_from_data(uint32_t version, data_chunk const& data);
    static pong factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KNUTH_IS_READER(R)>
    static pong factory_from_data(uint32_t version, R& source) {
        pong instance;
        instance.from_data(version, source);
        return instance;
    }

    //static pong factory_from_data(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    pong() = default;
    pong(uint64_t nonce);

    // pong(pong const& x) = default;
    // // This class is move assignable but not copy assignable.
    // pong& operator=(pong&& x) = default;
    // pong& operator=(pong const&) = default;

    bool operator==(pong const& x) const;
    bool operator!=(pong const& x) const;


    uint64_t nonce() const;
    void set_nonce(uint64_t value);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KNUTH_IS_READER(R)>
    bool from_data(uint32_t  /*version*/, R& source) {
        reset();

        valid_ = true;
        nonce_ = source.read_8_bytes_little_endian();

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
        sink.write_8_bytes_little_endian(nonce_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;


    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    uint64_t nonce_{0};
    bool valid_{false};
};

}  // namespace message
}  // namespace libbitcoin

#endif

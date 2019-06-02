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
#ifndef LIBBITCOIN_MESSAGE_HEADER_MESSAGE_HPP
#define LIBBITCOIN_MESSAGE_HEADER_MESSAGE_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>

#include <bitcoin/bitcoin/chain/header.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API header : public chain::header {
public:
    using list = std::vector<header>;
    using ptr = std::shared_ptr<header>;
    using const_ptr = std::shared_ptr<const header>;
    using ptr_list = std::vector<ptr>;
    using const_ptr_list = std::vector<const_ptr>;

    static header factory_from_data(uint32_t version, data_chunk const& data);
    static header factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KNUTH_IS_READER(R)>
    static header factory_from_data(uint32_t version, R& source) {
        header instance;
        instance.from_data(version, source);
        return instance;
    }

    //static header factory_from_data(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    header() = default;
    header(uint32_t version, hash_digest const& previous_block_hash, hash_digest const& merkle, uint32_t timestamp, uint32_t bits, uint32_t nonce);
    header(chain::header const& x);
    header(header const& x) = default;
    header(header&& x) = default;

    header& operator=(chain::header const& x);

    /// This class is move assignable but not copy assignable.
    header& operator=(header&& x) = default;
    header& operator=(header const& /*x*/) /*= delete*/;

    bool operator==(chain::header const& x) const;
    bool operator!=(chain::header const& x) const;
    bool operator==(header const& x) const;
    bool operator!=(header const& x) const;


    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KNUTH_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        if ( ! chain::header::from_data(source)) {
            return false;
}

        // The header message must trail a zero byte (yes, it's stoopid).
        // bitcoin.org/en/developer-reference#headers
        if (version != version::level::canonical && source.read_byte() != 0x00) {
            source.invalidate();
}

        if ( ! source) {
            reset();
}

        return source;
    }

    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        chain::header::to_data(sink);

        if (version != version::level::canonical) {
            sink.write_variable_little_endian(0);
}
    }

    void reset();
    size_t serialized_size(uint32_t version) const;


    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

}  // namespace message
}  // namespace libbitcoin

#endif

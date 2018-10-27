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
#ifndef LIBBITCOIN_MESSAGE_GET_BLOCKS_HPP
#define LIBBITCOIN_MESSAGE_GET_BLOCKS_HPP

#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
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

class BC_API get_blocks {
public:
    using ptr = std::shared_ptr<get_blocks>;
    using const_ptr = std::shared_ptr<const get_blocks>;

    static get_blocks factory_from_data(uint32_t version, data_chunk const& data);
    static get_blocks factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static get_blocks factory_from_data(uint32_t version, R& source) {
        get_blocks instance;
        instance.from_data(version, source);
        return instance;
    }

    //static get_blocks factory_from_data(uint32_t version, reader& source);

    get_blocks();
    get_blocks(hash_list const& start, hash_digest const& stop);
    get_blocks(hash_list&& start, hash_digest const& stop);
    get_blocks(const get_blocks& x) = default;
    get_blocks(get_blocks&& x) = default;

    // This class is move assignable but not copy assignable.
    get_blocks& operator=(get_blocks&& x) = default;
    void operator=(const get_blocks&) = delete;

    bool operator==(const get_blocks& x) const;
    bool operator!=(const get_blocks& x) const;


    hash_list& start_hashes();
    hash_list const& start_hashes() const;
    void set_start_hashes(hash_list const& value);
    void set_start_hashes(hash_list&& value);

    hash_digest& stop_hash();
    hash_digest const& stop_hash() const;
    void set_stop_hash(hash_digest const& value);

    virtual bool from_data(uint32_t version, data_chunk const& data);
    virtual bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    /*virtual*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t  /*version*/, R& source) {
        reset();

        // Discard protocol version because it is stupid.
        source.read_4_bytes_little_endian();
        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > max_get_blocks) {
            source.invalidate();
        } else {
            start_hashes_.reserve(count);
}

        for (size_t hash = 0; hash < count && source; ++hash) {
            start_hashes_.push_back(source.read_hash());
}

        stop_hash_ = source.read_hash();

        if ( ! source) {
            reset();
}

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_4_bytes_little_endian(version);
        sink.write_variable_little_endian(start_hashes_.size());

        for (auto const& start_hash : start_hashes_) {
            sink.write_hash(start_hash);
}

        sink.write_hash(stop_hash_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;


    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    // 10 sequential hashes, then exponential samples until reaching genesis.
    hash_list start_hashes_;
    hash_digest stop_hash_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

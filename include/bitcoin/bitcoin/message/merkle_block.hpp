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
#ifndef LIBBITCOIN_MESSAGE_MERKLE_BLOCK_HPP
#define LIBBITCOIN_MESSAGE_MERKLE_BLOCK_HPP

#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/chain/block.hpp>
#include <bitcoin/bitcoin/chain/header.hpp>
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

class BC_API merkle_block {
   public:
    typedef std::vector<merkle_block> list;
    typedef std::shared_ptr<merkle_block> ptr;
    typedef std::shared_ptr<const merkle_block> const_ptr;

    static merkle_block factory_from_data(uint32_t version, data_chunk const& data);
    static merkle_block factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static merkle_block factory_from_data(uint32_t version, R& source) {
        merkle_block instance;
        instance.from_data(version, source);
        return instance;
    }

    //static merkle_block factory_from_data(uint32_t version, reader& source);

    merkle_block();
    merkle_block(chain::header const& header, size_t total_transactions, const hash_list& hashes, data_chunk const& flags);
    merkle_block(chain::header&& header, size_t total_transactions, hash_list&& hashes, data_chunk&& flags);
    merkle_block(const chain::block& block);
    merkle_block(const merkle_block& other);
    merkle_block(merkle_block&& other);

    chain::header& header();
    chain::header const& header() const;
    void set_header(chain::header const& value);
    void set_header(chain::header&& value);

    size_t total_transactions() const;
    void set_total_transactions(size_t value);

    hash_list& hashes();
    const hash_list& hashes() const;
    void set_hashes(const hash_list& value);
    void set_hashes(hash_list&& value);

    data_chunk& flags();
    data_chunk const& flags() const;
    void set_flags(data_chunk const& value);
    void set_flags(data_chunk&& value);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        if ( ! header_.from_data(source))
            return false;

        total_transactions_ = source.read_4_bytes_little_endian();
        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > get_max_block_size())
            source.invalidate();
        else
            hashes_.reserve(count);

        for (size_t hash = 0; hash < hashes_.capacity() && source; ++hash)
            hashes_.push_back(source.read_hash());

        flags_ = source.read_bytes(source.read_size_little_endian());

        if (version < merkle_block::version_minimum)
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
        header_.to_data(sink);

        auto const total32 = safe_unsigned<uint32_t>(total_transactions_);
        sink.write_4_bytes_little_endian(total32);
        sink.write_variable_little_endian(hashes_.size());

        for (auto const& hash : hashes_)
            sink.write_hash(hash);

        sink.write_variable_little_endian(flags_.size());
        sink.write_bytes(flags_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    // This class is move assignable but not copy assignable.
    merkle_block& operator=(merkle_block&& other);
    void operator=(const merkle_block&) = delete;

    bool operator==(const merkle_block& other) const;
    bool operator!=(const merkle_block& other) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

   private:
    chain::header header_;
    size_t total_transactions_;
    hash_list hashes_;
    data_chunk flags_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

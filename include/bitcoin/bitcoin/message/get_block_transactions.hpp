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
#ifndef LIBBITCOIN_MESSAGE_GET_BLOCK_TRANSACTIONS_HPP
#define LIBBITCOIN_MESSAGE_GET_BLOCK_TRANSACTIONS_HPP

#include <istream>
#include <memory>

#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API get_block_transactions {
public:
    using ptr = std::shared_ptr<get_block_transactions>;
    using const_ptr = std::shared_ptr<const get_block_transactions>;

    static get_block_transactions factory_from_data(uint32_t version, data_chunk const& data);
    static get_block_transactions factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KNUTH_IS_READER(R)>
    static get_block_transactions factory_from_data(uint32_t version, R& source) {
        get_block_transactions instance;
        instance.from_data(version, source);
        return instance;
    }

    get_block_transactions();
    get_block_transactions(hash_digest const& block_hash, const std::vector<uint64_t>& indexes);
    get_block_transactions(hash_digest const& block_hash, std::vector<uint64_t>&& indexes);

    // get_block_transactions(get_block_transactions const& x) = default;
    // get_block_transactions(get_block_transactions&& x) = default;
    // // This class is move assignable but not copy assignable.
    // get_block_transactions& operator=(get_block_transactions&& x) = default;
    // get_block_transactions& operator=(get_block_transactions const&) = default;

    bool operator==(get_block_transactions const& x) const;
    bool operator!=(get_block_transactions const& x) const;


    hash_digest& block_hash();
    hash_digest const& block_hash() const;
    void set_block_hash(hash_digest const& value);

    std::vector<uint64_t>& indexes();
    const std::vector<uint64_t>& indexes() const;
    void set_indexes(const std::vector<uint64_t>& values);
    void set_indexes(std::vector<uint64_t>&& values);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KNUTH_IS_READER(R)>
    bool from_data(uint32_t  /*version*/, R& source) {
        reset();

        block_hash_ = source.read_hash();
        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > get_max_block_size()) {
            source.invalidate();
        } else {
            indexes_.reserve(count);
}

        for (size_t position = 0; position < count && source; ++position) {
            indexes_.push_back(source.read_size_little_endian());
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
        sink.write_hash(block_hash_);
        sink.write_variable_little_endian(indexes_.size());
        for (auto const& element : indexes_) {
            sink.write_variable_little_endian(element);
}
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    hash_digest block_hash_;
    std::vector<uint64_t> indexes_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

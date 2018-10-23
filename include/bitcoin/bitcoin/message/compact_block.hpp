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
#ifndef LIBBITCOIN_MESSAGE_COMPACT_BLOCK_HPP
#define LIBBITCOIN_MESSAGE_COMPACT_BLOCK_HPP

#include <istream>

#include <bitcoin/bitcoin/chain/header.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/block.hpp>
#include <bitcoin/bitcoin/message/prefilled_transaction.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API compact_block {
   public:
    typedef std::shared_ptr<compact_block> ptr;
    typedef std::shared_ptr<const compact_block> const_ptr;

    //typedef mini_hash short_id;
    //typedef mini_hash_list short_id_list;
    using short_id = uint64_t;
    using short_id_list = std::vector<short_id>;

    static compact_block factory_from_data(uint32_t version, const data_chunk& data);
    static compact_block factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static compact_block factory_from_data(uint32_t version, R& source) {
        //std::cout << "compact_block::factory_from_data 3\n";

        compact_block instance;
        instance.from_data(version, source);
        return instance;
    }

    //static compact_block factory_from_data(uint32_t version, reader& source);

    static compact_block factory_from_block(message::block const& block);

    compact_block();
    compact_block(const chain::header& header, uint64_t nonce, const short_id_list& short_ids, const prefilled_transaction::list& transactions);
    compact_block(chain::header&& header, uint64_t nonce, short_id_list&& short_ids, prefilled_transaction::list&& transactions);
    compact_block(const compact_block& other);
    compact_block(compact_block&& other);

    chain::header& header();
    const chain::header& header() const;
    void set_header(const chain::header& value);
    void set_header(chain::header&& value);

    uint64_t nonce() const;
    void set_nonce(uint64_t value);

    short_id_list& short_ids();
    const short_id_list& short_ids() const;
    void set_short_ids(const short_id_list& value);
    void set_short_ids(short_id_list&& value);

    prefilled_transaction::list& transactions();
    const prefilled_transaction::list& transactions() const;
    void set_transactions(const prefilled_transaction::list& value);
    void set_transactions(prefilled_transaction::list&& value);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        //std::cout << "compact_block::from_data 3\n";

        reset();

        if (!header_.from_data(source))
            return false;

        nonce_ = source.read_8_bytes_little_endian();
        auto count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > get_max_block_size())
            source.invalidate();
        else
            short_ids_.reserve(count);

        //todo:move to function
        // Order is required.
        for (size_t id = 0; id < count && source; ++id) {
            uint32_t lsb = source.read_4_bytes_little_endian();
            uint16_t msb = source.read_2_bytes_little_endian();
            short_ids_.push_back((uint64_t(msb) << 32) | uint64_t(lsb));
            //short_ids_.push_back(source.read_mini_hash());
        }

        count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > get_max_block_size())
            source.invalidate();
        else
            transactions_.resize(count);

        // NOTE: Witness flag is controlled by prefilled tx
        // Order is required.
        for (auto& tx : transactions_)
            if (!tx.from_data(version, source))
                break;

        if (version < compact_block::version_minimum)
            source.invalidate();

        if (!source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source);

    bool from_block(message::block const& block);

    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        //std::cout << "compact_block::to_data 3\n";

        header_.to_data(sink);
        sink.write_8_bytes_little_endian(nonce_);
        sink.write_variable_little_endian(short_ids_.size());

        for (auto const& element : short_ids_) {
            //sink.write_mini_hash(element);
            uint32_t lsb = element & 0xffffffff;
            uint16_t msb = (element >> 32) & 0xffff;
            sink.write_4_bytes_little_endian(lsb);
            sink.write_2_bytes_little_endian(msb);
        }

        sink.write_variable_little_endian(transactions_.size());

        // NOTE: Witness flag is controlled by prefilled tx
        for (auto const& element : transactions_)
            element.to_data(version, sink);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    // This class is move assignable but not copy assignable.
    compact_block& operator=(compact_block&& other);
    void operator=(const compact_block&) = delete;

    bool operator==(const compact_block& other) const;
    bool operator!=(const compact_block& other) const;

    static const std::string command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

   private:
    chain::header header_;
    uint64_t nonce_;
    short_id_list short_ids_;
    prefilled_transaction::list transactions_;
};

template <typename W>
void to_data_header_nonce(compact_block const& block, W& sink) {
    block.header().to_data(sink);
    sink.write_8_bytes_little_endian(block.nonce());
}
// void to_data_header_nonce(compact_block const& block, writer& sink);

// void to_data_header_nonce(compact_block const& block, std::ostream& stream);
void to_data_header_nonce(compact_block const& block, data_sink& stream);

data_chunk to_data_header_nonce(compact_block const& block);

hash_digest hash(compact_block const& block);

}  // namespace message
}  // namespace libbitcoin

#endif

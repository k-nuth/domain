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
#ifndef LIBBITCOIN_MESSAGE_TRANSACTION_HPP
#define LIBBITCOIN_MESSAGE_TRANSACTION_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>

#include <bitcoin/bitcoin/chain/input.hpp>
#include <bitcoin/bitcoin/chain/output.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API transaction
    : public chain::transaction {
   public:
    typedef std::shared_ptr<transaction> ptr;
    typedef std::shared_ptr<transaction const> const_ptr;
    typedef std::vector<ptr> ptr_list;
    typedef std::vector<const_ptr> const_ptr_list;
    typedef std::shared_ptr<const_ptr_list> const_ptr_list_ptr;
    typedef std::shared_ptr<const const_ptr_list> const_ptr_list_const_ptr;

    static transaction factory_from_data(uint32_t version, data_chunk const& data);
    static transaction factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static transaction factory_from_data(uint32_t version, R& source) {
        transaction instance;
        instance.from_data(version, source);
        return instance;
    }

    //static transaction factory_from_data(uint32_t version, reader& source);

    transaction();

    transaction(transaction&& other);
    transaction(chain::transaction&& other);

    transaction(transaction const& other);
    transaction(const chain::transaction& other);

    transaction(uint32_t version, uint32_t locktime, chain::input::list&& inputs, chain::output::list&& outputs);
    transaction(uint32_t version, uint32_t locktime, const chain::input::list& inputs, const chain::output::list& outputs);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        return chain::transaction::from_data(source, true, true);
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version, bool witness = true) const;
    void to_data(uint32_t version, data_sink& stream, bool witness = true) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink, bool witness = true) const {
        chain::transaction::to_data(sink, true, witness);
    }

    //void to_data(uint32_t version, writer& sink, bool witness = true) const;
    size_t serialized_size(uint32_t version) const;

    transaction& operator=(chain::transaction&& other);

    /// This class is move assignable but not copy assignable.
    transaction& operator=(transaction&& other);
    void operator=(transaction const&) = delete;

    bool operator==(const chain::transaction& other) const;
    bool operator!=(const chain::transaction& other) const;

    bool operator==(transaction const& other) const;
    bool operator!=(transaction const& other) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

}  // namespace message
}  // namespace libbitcoin

#endif

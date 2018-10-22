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
#ifndef LIBBITCOIN_MESSAGE_PREFILLED_TRANSACTION_HPP
#define LIBBITCOIN_MESSAGE_PREFILLED_TRANSACTION_HPP

#include <istream>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API prefilled_transaction
{
public:
    typedef std::vector<prefilled_transaction> list;
    typedef std::shared_ptr<const prefilled_transaction> const_ptr;

    static prefilled_transaction factory_from_data(uint32_t version, const data_chunk& data);
    static prefilled_transaction factory_from_data(uint32_t version, data_source& stream);
    
    template <Reader R, BITPRIM_IS_READER(R)>
    static prefilled_transaction factory_from_data(uint32_t version, R& source)
    {
        prefilled_transaction instance;
        instance.from_data(version, source);
        return instance;
    }

    //static prefilled_transaction factory_from_data(uint32_t version, reader& source);

    prefilled_transaction();
    prefilled_transaction(uint64_t index, const chain::transaction& tx);
    prefilled_transaction(uint64_t index, chain::transaction&& tx);
    prefilled_transaction(const prefilled_transaction& other);
    prefilled_transaction(prefilled_transaction&& other);

    uint64_t index() const;
    void set_index(uint64_t value);

    chain::transaction& transaction();
    const chain::transaction& transaction() const;
    void set_transaction(const chain::transaction& tx);
    void set_transaction(chain::transaction&& tx);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, data_source& stream);
    
    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source)
    {
    #ifdef BITPRIM_CURRENCY_BCH
        bool witness = false;
    #else
        bool witness = true;
    #endif
        reset();
    
        index_ = source.read_variable_little_endian();
        transaction_.from_data(source, true, witness);
    
        if (!source)
            reset();
    
        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;
    
    template <Writer W>
    void to_data(uint32_t version, W& sink) const
    {
        sink.write_variable_little_endian(index_);
    #ifdef BITPRIM_CURRENCY_BCH
        bool witness = false;
    #else
        bool witness = true;
    #endif
        transaction_.to_data(sink, /*wire*/ true, witness, /*unconfirmed*/ false);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    prefilled_transaction& operator=(prefilled_transaction&& other);
    prefilled_transaction& operator=(const prefilled_transaction& other);

    bool operator==(const prefilled_transaction& other) const;
    bool operator!=(const prefilled_transaction& other) const;

private:
    uint64_t index_;
    chain::transaction transaction_;
};

} // namespace message
} // namespace libbitcoin

#endif

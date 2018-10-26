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

#include <bitcoin/bitcoin/chain/transaction.hpp>
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

class BC_API prefilled_transaction {
public:
    using list = std::vector<prefilled_transaction>;
    using const_ptr = std::shared_ptr<const prefilled_transaction>;

    static prefilled_transaction factory_from_data(uint32_t version, data_chunk const& data);
    static prefilled_transaction factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static prefilled_transaction factory_from_data(uint32_t version, R& source) {
        prefilled_transaction instance;
        instance.from_data(version, source);
        return instance;
    }

    //static prefilled_transaction factory_from_data(uint32_t version, reader& source);

    prefilled_transaction();
    prefilled_transaction(uint64_t index, chain::transaction const& tx);
    prefilled_transaction(uint64_t index, chain::transaction&& tx);
    prefilled_transaction(prefilled_transaction const& x) = default;
    prefilled_transaction(prefilled_transaction&& x) = default;

    prefilled_transaction& operator=(prefilled_transaction&& x) = default;
    prefilled_transaction& operator=(prefilled_transaction const& x) = default;

    bool operator==(prefilled_transaction const& x) const;
    bool operator!=(prefilled_transaction const& x) const;


    uint64_t index() const;
    void set_index(uint64_t value);

    chain::transaction& transaction();
    chain::transaction const& transaction() const;
    void set_transaction(chain::transaction const& tx);
    void set_transaction(chain::transaction&& tx);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        index_ = source.read_variable_little_endian();
        transaction_.from_data(source, true, witness_default());

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_variable_little_endian(index_);
        transaction_.to_data(sink, /*wire*/ true, witness_default()
#ifdef BITPRIM_CACHED_RPC_DATA        
                             , /*unconfirmed*/ false
#endif
                             );
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;


private:
    uint64_t index_;
    chain::transaction transaction_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

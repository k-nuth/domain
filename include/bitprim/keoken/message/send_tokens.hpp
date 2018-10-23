/**
 * Copyright (c) 2016-2018 Bitprim Inc.
 *
 * This file is part of Bitprim.
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
#ifndef BITPRIM_KEOKEN_MESSAGE_SEND_TOKENS_HPP_
#define BITPRIM_KEOKEN_MESSAGE_SEND_TOKENS_HPP_

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/keoken/message/base.hpp>
#include <bitprim/keoken/primitives.hpp>

namespace bitprim {
namespace keoken {
namespace message {

class BC_API send_tokens {
public:
    static const uint16_t version = 0;
    static const uint16_t type = 1;
    
    // Constructors.
    //-------------------------------------------------------------------------

    send_tokens(asset_id_t asset_id, amount_t amount);

    // Semiregular.
    //-------------------------------------------------------------------------

    send_tokens() = default;
    // send_tokens(send_tokens const& x) = default;
    // send_tokens(send_tokens&& x) = default;
    // send_tokens& operator=(send_tokens const& x) = default;
    // send_tokens& operator=(send_tokens&& x) = default;

    // Regular.
    //-------------------------------------------------------------------------

    friend
    bool operator==(send_tokens const& a, send_tokens const& b);
    
    friend
    bool operator!=(send_tokens const& a, send_tokens const& b);

    // Deserialization.
    //-------------------------------------------------------------------------

    static send_tokens factory_from_data(bc::data_chunk const& data);
    static send_tokens factory_from_data(data_source& stream);
    static send_tokens factory_from_data(bc::reader& source);

    bool from_data(bc::data_chunk const& data);
    bool from_data(data_source& stream);
    bool from_data(bc::reader& source);

    // Serialization.
    //-------------------------------------------------------------------------

    bc::data_chunk to_data() const;
    void to_data(data_sink& stream) const;
    void to_data(bc::writer& sink) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size() const;

    asset_id_t asset_id() const;
    void set_asset_id(asset_id_t x);

    amount_t amount() const;
    void set_amount(amount_t x);

private:
    asset_id_t asset_id_;
    amount_t amount_;
};

} // namespace message
} // namespace keoken
} // namespace bitprim

#endif //BITPRIM_KEOKEN_MESSAGE_SEND_TOKENS_HPP_

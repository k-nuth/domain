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
#ifndef BITPRIM_KEOKEN_MESSAGE_CREATE_ASSET_HPP_
#define BITPRIM_KEOKEN_MESSAGE_CREATE_ASSET_HPP_

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/keoken/message/base.hpp>
#include <bitprim/keoken/primitives.hpp>

namespace bitprim {
namespace keoken {
namespace message {

class BC_API create_asset {
public:
    static const uint16_t version = 0;
    static const uint16_t type = 0;
    static const size_t max_name_size = 32u + 1u;  //32 real chars plus null-terminated

    // Constructors.
    //-------------------------------------------------------------------------

    create_asset(std::string name, amount_t amount);

    // Semiregular.
    //-------------------------------------------------------------------------

    create_asset() = default;
    // create_asset(create_asset const& x) = default;
    // create_asset(create_asset&& x) = default;
    // create_asset& operator=(create_asset const& x) = default;
    // create_asset& operator=(create_asset&& x) = default;

    // Regular.
    //-------------------------------------------------------------------------

    friend bool operator==(create_asset const& a, create_asset const& b);

    friend bool operator!=(create_asset const& a, create_asset const& b);

    // Deserialization.
    //-------------------------------------------------------------------------

    static create_asset factory_from_data(bc::data_chunk const& data);
    static create_asset factory_from_data(bc::data_source& stream);
    static create_asset factory_from_data(bc::reader& source);

    bool from_data(bc::data_chunk const& data);
    bool from_data(bc::data_source& stream);
    bool from_data(bc::reader& source);

    // Serialization.
    //-------------------------------------------------------------------------

    bc::data_chunk to_data() const;
    void to_data(libbitcoin::data_sink& stream) const;
    void to_data(bc::writer& sink) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size() const;

    std::string const& name() const;
    void set_name(std::string const& x);
    void set_name(std::string&& x);

    amount_t amount() const;
    void set_amount(amount_t x);

private:
    // char name[17];      // 16 + 1. minus the \0 termination
    std::string name_;
    amount_t amount_;  //NOLINT
};

}  // namespace message
}  // namespace keoken
}  // namespace bitprim

#endif  //BITPRIM_KEOKEN_MESSAGE_CREATE_ASSET_HPP_

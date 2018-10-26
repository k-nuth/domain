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
#ifndef LIBBITCOIN_MESSAGE_ADDRESS_HPP
#define LIBBITCOIN_MESSAGE_ADDRESS_HPP

#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/message/network_address.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API address {
public:
    using ptr = std::shared_ptr<address>;
    using const_ptr = std::shared_ptr<const address>;

    static address factory_from_data(uint32_t version, data_chunk const& data);
    static address factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static address factory_from_data(uint32_t version, R& source) {
        address instance;
        instance.from_data(version, source);
        return instance;
    }

    //static address factory_from_data(uint32_t version, reader& source);

    address() = default;
    address(network_address::list const& addresses);
    address(network_address::list&& addresses);


    /// This class is move assignable but not copy assignable.
    // address(address const& x);
    // address(address&& x) noexcept;
    // address& operator=(address&& x) noexcept;
    address(address const& x) = default;
    address(address&& x) = default;
    address& operator=(address&& x) = default;
    void operator=(address const&) = delete;

    bool operator==(address const& x) const;
    bool operator!=(address const& x) const;


    network_address::list& addresses();
    network_address::list const& addresses() const;
    void set_addresses(network_address::list const& value);
    void set_addresses(network_address::list&& value);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > max_address)
            source.invalidate();
        else
            addresses_.resize(count);

        for (auto& address : addresses_)
            if ( ! address.from_data(version, source, true))
                break;

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source);

    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_variable_little_endian(addresses_.size());

        for (auto const& net_address : addresses_)
            net_address.to_data(version, sink, true);
    }

    //void to_data(uint32_t version, writer& sink) const;

    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;


    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    network_address::list addresses_;
};

}  // namespace message
}  // namespace libbitcoin

#endif

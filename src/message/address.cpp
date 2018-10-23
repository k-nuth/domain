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
#include <bitcoin/bitcoin/message/address.hpp>

#include <bitcoin/bitcoin/message/messages.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const address::command = "addr";
uint32_t const address::version_minimum = version::level::minimum;
uint32_t const address::version_maximum = version::level::maximum;

address address::factory_from_data(uint32_t version, data_chunk const& data) {
    address instance;
    instance.from_data(version, data);
    return instance;
}

address address::factory_from_data(uint32_t version, data_source& stream) {
    address instance;
    instance.from_data(version, stream);
    return instance;
}

//address address::factory_from_data(uint32_t version, reader& source)
//{
//    address instance;
//    instance.from_data(version, source);
//    return instance;
//}

address::address()
    : addresses_() {
}

address::address(const network_address::list& addresses)
    : addresses_(addresses) {
}

address::address(network_address::list&& addresses)
    : addresses_(std::move(addresses)) {
}

address::address(const address& x)
    : address(x.addresses_) {
}

address::address(address&& x)
    : address(std::move(x.addresses_)) {
}

bool address::is_valid() const {
    return !addresses_.empty();
}

void address::reset() {
    addresses_.clear();
    addresses_.shrink_to_fit();
}

bool address::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool address::from_data(uint32_t version, data_source& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

//bool address::from_data(uint32_t version, reader& source)
//{
//    reset();
//
//    auto const count = source.read_size_little_endian();
//
//    // Guard against potential for arbitary memory allocation.
//    if (count > max_address)
//        source.invalidate();
//    else
//        addresses_.resize(count);
//
//    for (auto& address: addresses_)
//        if ( ! address.from_data(version, source, true))
//            break;
//
//    if ( ! source)
//        reset();
//
//    return source;
//}

data_chunk address::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void address::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

//void address::to_data(uint32_t version, writer& sink) const
//{
//    sink.write_variable_little_endian(addresses_.size());
//
//    for (auto const& net_address: addresses_)
//        net_address.to_data(version, sink, true);
//}

size_t address::serialized_size(uint32_t version) const {
    return message::variable_uint_size(addresses_.size()) +
           (addresses_.size() * network_address::satoshi_fixed_size(version, true));
}

network_address::list& address::addresses() {
    return addresses_;
}

const network_address::list& address::addresses() const {
    return addresses_;
}

void address::set_addresses(const network_address::list& value) {
    addresses_ = value;
}

void address::set_addresses(network_address::list&& value) {
    addresses_ = std::move(value);
}

address& address::operator=(address&& x) {
    addresses_ = std::move(x.addresses_);
    return *this;
}

bool address::operator==(const address& x) const {
    return (addresses_ == x.addresses_);
}

bool address::operator!=(const address& x) const {
    return !(*this == other);
}

}  // namespace message
}  // namespace libbitcoin

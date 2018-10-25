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
#include <bitcoin/bitcoin/message/get_address.hpp>

#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const get_address::command = "getaddr";
uint32_t const get_address::version_minimum = version::level::minimum;
uint32_t const get_address::version_maximum = version::level::maximum;

get_address get_address::factory_from_data(uint32_t version, data_chunk const& data) {
    get_address instance;
    instance.from_data(version, data);
    return instance;
}

get_address get_address::factory_from_data(uint32_t version, data_source& stream) {
    get_address instance;
    instance.from_data(version, stream);
    return instance;
}

bool get_address::is_valid() const {
    return true;
}

void get_address::reset() {
}

bool get_address::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool get_address::from_data(uint32_t version, data_source& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk get_address::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void get_address::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t get_address::serialized_size(uint32_t version) const {
    return get_address::satoshi_fixed_size(version);
}

size_t get_address::satoshi_fixed_size(uint32_t /*version*/) {
    return 0;
}

}  // namespace message
}  // namespace libbitcoin

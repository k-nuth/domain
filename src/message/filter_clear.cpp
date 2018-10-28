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
#include <bitcoin/bitcoin/message/filter_clear.hpp>

#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const filter_clear::command = "filterclear";
uint32_t const filter_clear::version_minimum = version::level::bip37;
uint32_t const filter_clear::version_maximum = version::level::maximum;

filter_clear filter_clear::factory_from_data(uint32_t version, data_chunk const& data) {
    filter_clear instance;
    instance.from_data(version, data);
    return instance;
}

filter_clear filter_clear::factory_from_data(uint32_t version, std::istream& stream) {
    filter_clear instance;
    instance.from_data(version, stream);
    return instance;
}

//filter_clear filter_clear::factory_from_data(uint32_t version, reader& source)
//{
//    filter_clear instance;
//    instance.from_data(version, source);
//    return instance;
//}

// This is a default instance so is invalid.
// The only way to make this valid is to deserialize it :/.
filter_clear::filter_clear()
{}

// protected
filter_clear::filter_clear(bool insufficient_version)
    : insufficient_version_(insufficient_version) {
}

// filter_clear::filter_clear(const filter_clear& x)
//     : filter_clear(x.insufficient_version_) {
// }

// filter_clear::filter_clear(filter_clear&& x) noexcept
//     : filter_clear(x.insufficient_version_) {
// }

bool filter_clear::is_valid() const {
    return !insufficient_version_;
}

// This is again a default instance so is invalid.
void filter_clear::reset() {
    insufficient_version_ = true;
}

bool filter_clear::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool filter_clear::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk filter_clear::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void filter_clear::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t filter_clear::serialized_size(uint32_t version) const {
    return filter_clear::satoshi_fixed_size(version);
}

size_t filter_clear::satoshi_fixed_size(uint32_t /*version*/) {
    return 0;
}

}  // namespace message
}  // namespace libbitcoin

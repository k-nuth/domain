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
#include <bitcoin/bitcoin/message/filter_add.hpp>

#include <bitcoin/bitcoin/message/messages.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/assert.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const filter_add::command = "filteradd";
uint32_t const filter_add::version_minimum = version::level::bip37;
uint32_t const filter_add::version_maximum = version::level::maximum;

filter_add filter_add::factory_from_data(uint32_t version, data_chunk const& data) {
    filter_add instance;
    instance.from_data(version, data);
    return instance;
}

filter_add filter_add::factory_from_data(uint32_t version, std::istream& stream) {
    filter_add instance;
    instance.from_data(version, stream);
    return instance;
}

filter_add::filter_add(data_chunk const& data)
    : data_(data) {
}

filter_add::filter_add(data_chunk&& data)
    : data_(std::move(data)) {
}

// filter_add::filter_add(filter_add const& x)
//     : filter_add(x.data_) {
// }

// filter_add::filter_add(filter_add&& x) noexcept
//     : filter_add(std::move(x.data_)) 
// {}

// filter_add& filter_add::operator=(filter_add&& x) noexcept {
//     data_ = std::move(x.data_);
//     return *this;
// }

bool filter_add::operator==(filter_add const& x) const {
    return (data_ == x.data_);
}

bool filter_add::operator!=(filter_add const& x) const {
    return !(*this == x);
}

bool filter_add::is_valid() const {
    return !data_.empty();
}

void filter_add::reset() {
    data_.clear();
    data_.shrink_to_fit();
}

bool filter_add::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool filter_add::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk filter_add::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void filter_add::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t filter_add::serialized_size(uint32_t /*version*/) const {
    return message::variable_uint_size(data_.size()) + data_.size();
}

data_chunk& filter_add::data() {
    return data_;
}

data_chunk const& filter_add::data() const {
    return data_;
}

void filter_add::set_data(data_chunk const& value) {
    data_ = value;
}

void filter_add::set_data(data_chunk&& value) {
    data_ = std::move(value);
}

}  // namespace message
}  // namespace libbitcoin

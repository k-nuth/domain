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
#include <bitcoin/bitcoin/message/headers.hpp>

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <istream>
#include <utility>

#include <bitcoin/bitcoin/message/inventory.hpp>
#include <bitcoin/bitcoin/message/inventory_vector.hpp>
#include <bitcoin/bitcoin/message/messages.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const headers::command = "headers";
uint32_t const headers::version_minimum = version::level::headers;
uint32_t const headers::version_maximum = version::level::maximum;

headers headers::factory_from_data(uint32_t version, data_chunk const& data) {
    headers instance;
    instance.from_data(version, data);
    return instance;
}

headers headers::factory_from_data(uint32_t version, data_source& stream) {
    headers instance;
    instance.from_data(version, stream);
    return instance;
}

//headers headers::factory_from_data(uint32_t version, reader& source)
//{
//    headers instance;
//    instance.from_data(version, source);
//    return instance;
//}

headers::headers()
    : elements_() {
}

// Uses headers copy assignment.
headers::headers(header::list const& values)
    : elements_(values) {
}

headers::headers(header::list&& values)
    : elements_(std::move(values)) {
}

headers::headers(std::initializer_list<header> const& values)
    : elements_(values) {
}

// headers::headers(headers const& x)
//     : headers(x.elements_) {
// }

// headers::headers(headers&& x) noexcept
//     : headers(std::move(x.elements_)) 
// {}

// headers& headers::operator=(headers&& x) noexcept {
//     elements_ = std::move(x.elements_);
//     return *this;
// }

bool headers::operator==(headers const& x) const {
    return elements_ == x.elements_;
}

bool headers::operator!=(headers const& x) const {
    return !(*this == x);
}


bool headers::is_valid() const {
    return !elements_.empty();
}

void headers::reset() {
    elements_.clear();
    elements_.shrink_to_fit();
}

bool headers::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool headers::from_data(uint32_t version, data_source& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk headers::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void headers::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

bool headers::is_sequential() const {
    if (elements_.empty()) {
        return true;
    }

    auto previous = elements_.front().hash();

    for (auto it = elements_.begin() + 1; it != elements_.end(); ++it) {
        if (it->previous_block_hash() != previous) {
            return false;
        }

        previous = it->hash();
    }

    return true;
}

void headers::to_hashes(hash_list& out) const {
    out.clear();
    out.reserve(elements_.size());
    auto const map = [&out](header const& header) {
        out.push_back(header.hash());
    };

    std::for_each(elements_.begin(), elements_.end(), map);
}

void headers::to_inventory(inventory_vector::list& out,
                           inventory::type_id type) const {
    out.clear();
    out.reserve(elements_.size());
    auto const map = [&out, type](header const& header) {
        out.emplace_back(type, header.hash());
    };

    std::for_each(elements_.begin(), elements_.end(), map);
}

size_t headers::serialized_size(uint32_t version) const {
    return message::variable_uint_size(elements_.size()) +
           (elements_.size() * header::satoshi_fixed_size(version));
}

header::list& headers::elements() {
    return elements_;
}

header::list const& headers::elements() const {
    return elements_;
}

void headers::set_elements(header::list const& values) {
    elements_ = values;
}

void headers::set_elements(header::list&& values) {
    elements_ = std::move(values);
}

}  // namespace message
}  // namespace libbitcoin

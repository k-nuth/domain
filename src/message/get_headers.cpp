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
#include <bitcoin/bitcoin/message/get_headers.hpp>

#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>

namespace libbitcoin {
namespace message {

std::string const get_headers::command = "getheaders";
uint32_t const get_headers::version_minimum = version::level::headers;
uint32_t const get_headers::version_maximum = version::level::maximum;

get_headers get_headers::factory_from_data(uint32_t version, data_chunk const& data) {
    get_headers instance;
    instance.from_data(version, data);
    return instance;
}

get_headers get_headers::factory_from_data(uint32_t version, std::istream& stream) {
    get_headers instance;
    instance.from_data(version, stream);
    return instance;
}

get_headers::get_headers(hash_list const& start, hash_digest const& stop)
    : get_blocks(start, stop) {
}

//TODO(fernando): move semantics lost
get_headers::get_headers(hash_list&& start, hash_digest const& stop)
    : get_headers(start, stop) 
{}

// get_headers::get_headers(get_headers&& x) noexcept
//     : get_blocks(x) 
// {}

// get_headers& get_headers::operator=(get_headers&& x) noexcept {
//     set_start_hashes(x.start_hashes());
//     set_stop_hash(x.stop_hash());
//     return *this;
// }

bool get_headers::operator==(get_headers const& x) const {
    return (static_cast<get_blocks const&>(*this) == static_cast<get_blocks const&>(x));
}

bool get_headers::operator!=(get_headers const& x) const {
    return !(*this == x);
}

// bool get_headers::from_data(uint32_t version, data_chunk const& data) {
//     return get_blocks::from_data(version, data);
// }

// bool get_headers::from_data(uint32_t version, std::istream& stream) {
//     return get_blocks::from_data(version, stream);
// }

bool get_headers::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool get_headers::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

}  // namespace message
}  // namespace libbitcoin

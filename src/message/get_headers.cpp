// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/get_headers.hpp>

#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>

namespace kth::domain::message {

std::string const get_headers::command = "getheaders";
uint32_t const get_headers::version_minimum = version::level::headers;
uint32_t const get_headers::version_maximum = version::level::maximum;

get_headers::get_headers(hash_list const& start, hash_digest const& stop)
    : get_blocks(start, stop) {
}

//TODO(fernando): move semantics lost
get_headers::get_headers(hash_list&& start, hash_digest const& stop)
    : get_headers(start, stop) 
{}

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

}  // namespace kth

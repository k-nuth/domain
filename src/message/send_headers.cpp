// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/send_headers.hpp>

#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth {
namespace message {

std::string const send_headers::command = "sendheaders";
uint32_t const send_headers::version_minimum = version::level::bip130;
uint32_t const send_headers::version_maximum = version::level::maximum;

send_headers send_headers::factory_from_data(uint32_t version, data_chunk const& data) {
    send_headers instance;
    instance.from_data(version, data);
    return instance;
}

send_headers send_headers::factory_from_data(uint32_t version, std::istream& stream) {
    send_headers instance;
    instance.from_data(version, stream);
    return instance;
}

size_t send_headers::satoshi_fixed_size(uint32_t /*version*/) {
    return 0;
}

// protected
send_headers::send_headers(bool insufficient_version)
    : insufficient_version_(insufficient_version) {
}

// send_headers::send_headers(send_headers const& x)
//     : send_headers(x.insufficient_version_) {
// }

// send_headers::send_headers(send_headers&& x) noexcept
//     : send_headers(x.insufficient_version_) 
// {}

bool send_headers::is_valid() const {
    return !insufficient_version_;
}

// This is again a default instance so is invalid.
void send_headers::reset() {
    insufficient_version_ = true;
}

bool send_headers::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool send_headers::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk send_headers::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

//TODO(fernando): empty?
void send_headers::to_data(uint32_t /*version*/, data_sink& /*stream*/) const {
}

size_t send_headers::serialized_size(uint32_t version) const {
    return send_headers::satoshi_fixed_size(version);
}

}  // namespace message
}  // namespace kth

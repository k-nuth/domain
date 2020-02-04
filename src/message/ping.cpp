// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/ping.hpp>

// #include <kth/domain/message/version.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::message {

std::string const ping::command = "ping";
uint32_t const ping::version_minimum = version::level::minimum;
uint32_t const ping::version_maximum = version::level::maximum;

ping ping::factory_from_data(uint32_t version, data_chunk const& data) {
    ping instance;
    instance.from_data(version, data);
    return instance;
}

ping ping::factory_from_data(uint32_t version, std::istream& stream) {
    ping instance;
    instance.from_data(version, stream);
    return instance;
}

size_t ping::satoshi_fixed_size(uint32_t version) {
    return version < version::level::bip31 ? 0 : sizeof(nonce_);
}

//TODO(fernando): nonceless_ is never used! Check it!
ping::ping(uint64_t nonce)
    : nonce_(nonce), valid_(true) {
}

// ping::ping(ping const& x)
//     : nonce_(x.nonce_), nonceless_(x.nonceless_), valid_(x.valid_) {
// }

// ping& ping::operator=(ping&& x) noexcept {
//     nonce_ = x.nonce_;
//     return *this;
// }

bool ping::operator==(ping const& x) const {
    // Nonce should be zero if not used.
    return (nonce_ == x.nonce_);
}

bool ping::operator!=(ping const& x) const {
    // Nonce should be zero if not used.
    return !(*this == x);
}

bool ping::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool ping::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk ping::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void ping::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

bool ping::is_valid() const {
    return valid_ || nonceless_ || nonce_ != 0;
}

void ping::reset() {
    nonce_ = 0;
    nonceless_ = false;
    valid_ = false;
}

size_t ping::serialized_size(uint32_t version) const {
    return satoshi_fixed_size(version);
}

uint64_t ping::nonce() const {
    return nonce_;
}

void ping::set_nonce(uint64_t value) {
    nonce_ = value;
}

}  // namespace kth

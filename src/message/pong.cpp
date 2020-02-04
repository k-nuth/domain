// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/pong.hpp>

#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::message {

std::string const pong::command = "pong";
uint32_t const pong::version_minimum = version::level::minimum;
uint32_t const pong::version_maximum = version::level::maximum;

pong pong::factory_from_data(uint32_t version, data_chunk const& data) {
    pong instance;
    instance.from_data(version, data);
    return instance;
}

pong pong::factory_from_data(uint32_t version, std::istream& stream) {
    pong instance;
    instance.from_data(version, stream);
    return instance;
}

size_t pong::satoshi_fixed_size(uint32_t /*version*/) {
    return sizeof(nonce_);
}

pong::pong(uint64_t nonce)
    : nonce_(nonce), valid_(true) {
}

// pong::pong(pong const& x)
//     : nonce_(x.nonce_), valid_(x.valid_) {
// }

// pong& pong::operator=(pong&& x) noexcept {
//     nonce_ = x.nonce_;
//     return *this;
// }

bool pong::operator==(pong const& x) const {
    return (nonce_ == x.nonce_);
}

bool pong::operator!=(pong const& x) const {
    return !(*this == x);
}
bool pong::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool pong::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk pong::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void pong::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

bool pong::is_valid() const {
    return valid_ || (nonce_ != 0);
}

void pong::reset() {
    nonce_ = 0;
    valid_ = false;
}

size_t pong::serialized_size(uint32_t version) const {
    return satoshi_fixed_size(version);
}

uint64_t pong::nonce() const {
    return nonce_;
}

void pong::set_nonce(uint64_t value) {
    nonce_ = value;
}

}  // namespace kth

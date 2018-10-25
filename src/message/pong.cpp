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
#include <bitcoin/bitcoin/message/pong.hpp>

#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const pong::command = "pong";
uint32_t const pong::version_minimum = version::level::minimum;
uint32_t const pong::version_maximum = version::level::maximum;

pong pong::factory_from_data(uint32_t version, data_chunk const& data) {
    pong instance;
    instance.from_data(version, data);
    return instance;
}

pong pong::factory_from_data(uint32_t version, data_source& stream) {
    pong instance;
    instance.from_data(version, stream);
    return instance;
}

size_t pong::satoshi_fixed_size(uint32_t /*version*/) {
    return sizeof(nonce_);
}

pong::pong()
    : nonce_(0), valid_(false) {
}

pong::pong(uint64_t nonce)
    : nonce_(nonce), valid_(true) {
}

// pong::pong(const pong& x)
//     : nonce_(x.nonce_), valid_(x.valid_) {
// }

// pong& pong::operator=(pong&& x) noexcept {
//     nonce_ = x.nonce_;
//     return *this;
// }

bool pong::operator==(const pong& x) const {
    return (nonce_ == x.nonce_);
}

bool pong::operator!=(const pong& x) const {
    return !(*this == x);
}
bool pong::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool pong::from_data(uint32_t version, data_source& stream) {
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
    BITCOIN_ASSERT(data.size() == size);
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

}  // namespace message
}  // namespace libbitcoin

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
#include <bitcoin/bitcoin/message/header.hpp>

#include <cstddef>
#include <cstdint>
#include <istream>
#include <utility>

#include <bitcoin/bitcoin/chain/header.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/message/message_tools.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const header::command = "headers";
uint32_t const header::version_minimum = version::level::minimum;
uint32_t const header::version_maximum = version::level::maximum;

header header::factory_from_data(uint32_t version, data_chunk const& data) {
    header instance;
    instance.from_data(version, data);
    return instance;
}

header header::factory_from_data(uint32_t version, std::istream& stream) {
    header instance;
    instance.from_data(version, stream);
    return instance;
}

size_t header::satoshi_fixed_size(uint32_t version) {
    auto const canonical = (version == version::level::canonical);
    return chain::header::satoshi_fixed_size() + (canonical ? 0 : message::variable_uint_size(0));
}

header::header(uint32_t version,
               hash_digest const& previous_block_hash,
               hash_digest const& merkle,
               uint32_t timestamp,
               uint32_t bits,
               uint32_t nonce)
    : chain::header(version, previous_block_hash, merkle, timestamp, bits, nonce) {}

header::header(chain::header const& x)
    : chain::header(x) {
}

// header::header(header const& x)
//     : chain::header(x) {
// }

// header::header(header&& x) noexcept
//     : chain::header(std::move(x)) 
// {}

header& header::operator=(chain::header const& x) {
    chain::header::operator=(x);
    return *this;
}

// header& header::operator=(header&& x) noexcept {
//     chain::header::operator=(std::move(x));
//     return *this;
// }

header& header::operator=(header const& x) {
    chain::header::operator=(x);
    return *this;
}

bool header::operator==(chain::header const& x) const {
    return chain::header::operator==(x);
}

bool header::operator!=(chain::header const& x) const {
    return chain::header::operator!=(x);
}

bool header::operator==(header const& x) const {
    return chain::header::operator==(x);
}

bool header::operator!=(header const& x) const {
    return chain::header::operator!=(x);
}

bool header::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool header::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk header::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void header::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

void header::reset() {
    chain::header::reset();
}

size_t header::serialized_size(uint32_t version) const {
    return satoshi_fixed_size(version);
}

}  // namespace message
}  // namespace libbitcoin

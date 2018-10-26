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
#include <bitcoin/bitcoin/message/block.hpp>

#include <cstddef>
#include <cstdint>
#include <istream>
#include <utility>

#include <bitcoin/bitcoin/chain/header.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>

namespace libbitcoin {
namespace message {

std::string const block::command = "block";
uint32_t const block::version_minimum = version::level::minimum;
uint32_t const block::version_maximum = version::level::maximum;

block block::factory_from_data(uint32_t version, data_chunk const& data) {
    block instance;
    instance.from_data(version, data);
    return instance;
}

block block::factory_from_data(uint32_t version, std::istream& stream) {
    block instance;
    instance.from_data(version, stream);
    return instance;
}

block::block(chain::block&& x)
    : chain::block(std::move(x)) 
{}

block::block(chain::block const& x)
    : chain::block(x) 
{}

block::block(chain::header const& header, chain::transaction::list&& transactions)
    : chain::block(header, std::move(transactions)) 
{}

block::block(chain::header const& header, chain::transaction::list const& transactions)
    : chain::block(header, transactions) 
{}

// block::block(block&& x) noexcept
//     : chain::block(std::move(x)) 
// {}

block& block::operator=(chain::block&& x) {
    reset();
    chain::block::operator=(std::move(x));
    return *this;
}

// block& block::operator=(block&& x) noexcept {
//     chain::block::operator=(std::move(x));
//     return *this;
// }

bool block::operator==(chain::block const& x) const {
    return chain::block::operator==(x);
}

bool block::operator!=(chain::block const& x) const {
    return chain::block::operator!=(x);
}

bool block::operator==(block const& x) const {
    return chain::block::operator==(x);
}

bool block::operator!=(block const& x) const {
    return !(*this == x);
}

// Witness is always deserialized if present.
// NOTE: Witness on bch is dissabled on the chain::block class

bool block::from_data(uint32_t /*version*/, data_chunk const& data) {
    return chain::block::from_data(data, true);
}

bool block::from_data(uint32_t /*version*/, std::istream& stream) {
    return chain::block::from_data(stream, true);
}


// Witness is always serialized if present.
// NOTE: Witness on bch is dissabled on the chain::block class

data_chunk block::to_data(uint32_t /*unused*/) const {
    return chain::block::to_data(true);
}

void block::to_data(uint32_t /*version*/, data_sink& stream) const {
    chain::block::to_data(stream, true);
}

// Witness size is always counted if present.
// NOTE: Witness on bch is dissabled on the chain::block class

size_t block::serialized_size(uint32_t /*unused*/) const {
    return chain::block::serialized_size(true);
}



// //TODO(fernando): check this family of functions: to_data_header_nonce
// void to_data_header_nonce(block const& block, uint64_t nonce, writer& sink) {
//     block.header().to_data(sink);
//     sink.write_8_bytes_little_endian(nonce);
// }

// void to_data_header_nonce(block const& block, uint64_t nonce, std::ostream& stream) {
void to_data_header_nonce(block const& block, uint64_t nonce, data_sink& stream) {
    ostream_writer sink_w(stream);
    to_data_header_nonce(block, nonce, sink_w);
}

data_chunk to_data_header_nonce(block const& block, uint64_t nonce) {
    data_chunk data;
    auto size = chain::header::satoshi_fixed_size() + sizeof(nonce);

    data.reserve(size);
    data_sink ostream(data);
    to_data_header_nonce(block, nonce, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

hash_digest hash(block const& block, uint64_t nonce) {
    return sha256_hash(to_data_header_nonce(block, nonce));
}

}  // namespace message
}  // namespace libbitcoin

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
#include <bitcoin/bitcoin/message/merkle_block.hpp>

#include <bitcoin/bitcoin/chain/block.hpp>
#include <bitcoin/bitcoin/chain/header.hpp>
#include <bitcoin/bitcoin/message/messages.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/bitcoin/multi_crypto_support.hpp>
#include <bitcoin/infrastructure/utility/assert.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const merkle_block::command = "merkleblock";
uint32_t const merkle_block::version_minimum = version::level::bip37;
uint32_t const merkle_block::version_maximum = version::level::maximum;

merkle_block merkle_block::factory_from_data(uint32_t version, data_chunk const& data) {
    merkle_block instance;
    instance.from_data(version, data);
    return instance;
}

merkle_block merkle_block::factory_from_data(uint32_t version, data_source& stream) {
    merkle_block instance;
    instance.from_data(version, stream);
    return instance;
}

//merkle_block merkle_block::factory_from_data(uint32_t version, reader& source)
//{
//    merkle_block instance;
//    instance.from_data(version, source);
//    return instance;
//}

merkle_block::merkle_block()
    : header_(), total_transactions_(0), hashes_(), flags_() {
}

merkle_block::merkle_block(chain::header const& header,
                           size_t total_transactions,
                           const hash_list& hashes,
                           data_chunk const& flags)
    : header_(header), total_transactions_(total_transactions), hashes_(hashes), flags_(flags) {
}

merkle_block::merkle_block(chain::header const& header, size_t total_transactions, hash_list&& hashes, data_chunk&& flags)
    : header_(std::move(header)), total_transactions_(total_transactions), hashes_(std::move(hashes)), flags_(std::move(flags)) {
}

// Hack: use of safe_unsigned here isn't great. We should consider using size_t
// for the transaction count and invalidating on deserialization and construct.
merkle_block::merkle_block(const chain::block& block)
    : merkle_block(block.header(),
                   safe_unsigned<uint32_t>(block.transactions().size()),
                   block.to_hashes(),
                   {}) {
}

merkle_block::merkle_block(const merkle_block& x)
    : merkle_block(x.header_, x.total_transactions_, x.hashes_, x.flags_) {
}

merkle_block::merkle_block(merkle_block&& x)
    : merkle_block(std::move(x.header_), x.total_transactions_, std::move(x.hashes_), std::move(x.flags_)) {
}

bool merkle_block::is_valid() const {
    return !hashes_.empty() || !flags_.empty() || header_.is_valid();
}

void merkle_block::reset() {
    header_ = chain::header{};
    total_transactions_ = 0;
    hashes_.clear();
    hashes_.shrink_to_fit();
    flags_.clear();
    flags_.shrink_to_fit();
}

bool merkle_block::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool merkle_block::from_data(uint32_t version, data_source& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

//bool merkle_block::from_data(uint32_t version, reader& source)
//{
//    reset();
//
//    if ( ! header_.from_data(source))
//        return false;
//
//    total_transactions_ = source.read_4_bytes_little_endian();
//    auto const count = source.read_size_little_endian();
//
//    // Guard against potential for arbitary memory allocation.
//    if (count > get_max_block_size())
//        source.invalidate();
//    else
//        hashes_.reserve(count);
//
//    for (size_t hash = 0; hash < hashes_.capacity() && source; ++hash)
//        hashes_.push_back(source.read_hash());
//
//    flags_ = source.read_bytes(source.read_size_little_endian());
//
//    if (version < merkle_block::version_minimum)
//        source.invalidate();
//
//    if ( ! source)
//        reset();
//
//    return source;
//}

data_chunk merkle_block::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void merkle_block::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

//void merkle_block::to_data(uint32_t version, writer& sink) const
//{
//    header_.to_data(sink);
//
//    auto const total32 = safe_unsigned<uint32_t>(total_transactions_);
//    sink.write_4_bytes_little_endian(total32);
//    sink.write_variable_little_endian(hashes_.size());
//
//    for (auto const& hash : hashes_)
//        sink.write_hash(hash);
//
//    sink.write_variable_little_endian(flags_.size());
//    sink.write_bytes(flags_);
//}

size_t merkle_block::serialized_size(uint32_t version) const {
    return header_.serialized_size() + 4u +
           message::variable_uint_size(hashes_.size()) + (hash_size * hashes_.size()) +
           message::variable_uint_size(flags_.size()) + flags_.size();
}

chain::header& merkle_block::header() {
    return header_;
}

chain::header const& merkle_block::header() const {
    return header_;
}

void merkle_block::set_header(chain::header const& value) {
    header_ = value;
}

size_t merkle_block::total_transactions() const {
    return total_transactions_;
}

void merkle_block::set_total_transactions(size_t value) {
    total_transactions_ = value;
}

hash_list& merkle_block::hashes() {
    return hashes_;
}

const hash_list& merkle_block::hashes() const {
    return hashes_;
}

void merkle_block::set_hashes(const hash_list& value) {
    hashes_ = value;
}

void merkle_block::set_hashes(hash_list&& value) {
    hashes_ = std::move(value);
}

data_chunk& merkle_block::flags() {
    return flags_;
}

data_chunk const& merkle_block::flags() const {
    return flags_;
}

void merkle_block::set_flags(data_chunk const& value) {
    flags_ = value;
}

void merkle_block::set_flags(data_chunk&& value) {
    flags_ = std::move(value);
}

merkle_block& merkle_block::operator=(merkle_block&& x) {
    header_ = std::move(x.header_);
    hashes_ = std::move(x.hashes_);
    flags_ = std::move(x.flags_);
    return *this;
}

bool merkle_block::operator==(const merkle_block& x) const {
    auto result = (header_ == x.header_) &&
                  (hashes_.size() == x.hashes_.size()) &&
                  (flags_.size() == x.flags_.size());

    for (size_t i = 0; i < hashes_.size() && result; i++)
        result = (hashes_[i] == x.hashes_[i]);

    for (size_t i = 0; i < flags_.size() && result; i++)
        result = (flags_[i] == x.flags_[i]);

    return result;
}

bool merkle_block::operator!=(const merkle_block& x) const {
    return !(*this == other);
}

}  // namespace message
}  // namespace libbitcoin

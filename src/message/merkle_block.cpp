// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/merkle_block.hpp>

#include <kth/domain/chain/block.hpp>
#include <kth/domain/chain/header.hpp>
#include <kth/domain/message/version.hpp>
#include <kth/domain/multi_crypto_support.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/assert.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/limits.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::message {

std::string const merkle_block::command = "merkleblock";
uint32_t const merkle_block::version_minimum = version::level::bip37;
uint32_t const merkle_block::version_maximum = version::level::maximum;

merkle_block merkle_block::factory_from_data(uint32_t version, data_chunk const& data) {
    merkle_block instance;
    instance.from_data(version, data);
    return instance;
}

merkle_block merkle_block::factory_from_data(uint32_t version, std::istream& stream) {
    merkle_block instance;
    instance.from_data(version, stream);
    return instance;
}

merkle_block::merkle_block(chain::header const& header, size_t total_transactions, hash_list const& hashes, data_chunk const& flags)
    : header_(header), total_transactions_(total_transactions), hashes_(hashes), flags_(flags) {
}

merkle_block::merkle_block(chain::header const& header, size_t total_transactions, hash_list&& hashes, data_chunk&& flags)
    : header_(header), total_transactions_(total_transactions), hashes_(std::move(hashes)), flags_(std::move(flags)) {
}

// Hack: use of safe_unsigned here isn't great. We should consider using size_t
// for the transaction count and invalidating on deserialization and construct.
merkle_block::merkle_block(chain::block const& block)
    : merkle_block(block.header(),
                   safe_unsigned<uint32_t>(block.transactions().size()),
                   block.to_hashes(),
                   {}) {
}

// merkle_block::merkle_block(merkle_block const& x)
//     : merkle_block(x.header_, x.total_transactions_, x.hashes_, x.flags_) {
// }

// merkle_block::merkle_block(merkle_block&& x) noexcept
//     : merkle_block(x.header_, x.total_transactions_, std::move(x.hashes_), std::move(x.flags_)) 
// {}

// merkle_block& merkle_block::operator=(merkle_block&& x) noexcept {
//     header_ = x.header_;
//     hashes_ = std::move(x.hashes_);
//     flags_ = std::move(x.flags_);
//     return *this;
// }

bool merkle_block::operator==(merkle_block const& x) const {
    auto result = (header_ == x.header_) &&
                  (hashes_.size() == x.hashes_.size()) &&
                  (flags_.size() == x.flags_.size());

    for (size_t i = 0; i < hashes_.size() && result; i++) {
        result = (hashes_[i] == x.hashes_[i]);
    }

    for (size_t i = 0; i < flags_.size() && result; i++) {
        result = (flags_[i] == x.flags_[i]);
    }

    return result;
}

bool merkle_block::operator!=(merkle_block const& x) const {
    return !(*this == x);
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

bool merkle_block::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk merkle_block::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void merkle_block::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t merkle_block::serialized_size(uint32_t /*version*/) const {
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

hash_list const& merkle_block::hashes() const {
    return hashes_;
}

void merkle_block::set_hashes(hash_list const& value) {
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

}  // namespace kth

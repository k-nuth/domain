// Copyright (c) 2016-2021 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/double_spend_proofs.hpp>

#include <initializer_list>

// #include <kth/infrastructure/message/message_tools.hpp>
#include <kth/domain/message/version.hpp>
#include <kth/domain/multi_crypto_support.hpp>
#include <kth/infrastructure/math/sip_hash.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/limits.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>
#include <kth/infrastructure/utility/pseudo_random.hpp>

namespace kth::domain::message {

std::string const double_spend_proofs::command = "cmpctblock";
uint32_t const double_spend_proofs::version_minimum = version::level::bip152;
uint32_t const double_spend_proofs::version_maximum = version::level::bip152;

double_spend_proofs double_spend_proofs::factory_from_block(message::block const& blk) {
    double_spend_proofs instance;
    instance.from_block(blk);
    return instance;
}

double_spend_proofs::double_spend_proofs(chain::header const& header, uint64_t nonce, const short_id_list& short_ids, prefilled_transaction::list const& transactions)
    : header_(header)
    , nonce_(nonce)
    , short_ids_(short_ids)
    , transactions_(transactions) 
{}

double_spend_proofs::double_spend_proofs(chain::header const& header, uint64_t nonce, short_id_list&& short_ids, prefilled_transaction::list&& transactions)
    : header_(header)
    , nonce_(nonce)
    , short_ids_(std::move(short_ids))
    , transactions_(std::move(transactions)) 
{}

bool double_spend_proofs::operator==(double_spend_proofs const& x) const {
    return (header_ == x.header_) && (nonce_ == x.nonce_) && (short_ids_ == x.short_ids_) && (transactions_ == x.transactions_);
}

bool double_spend_proofs::operator!=(double_spend_proofs const& x) const {
    return !(*this == x);
}

bool double_spend_proofs::is_valid() const {
    return header_.is_valid() 
        && ! short_ids_.empty() 
        && ! transactions_.empty();
}

void double_spend_proofs::reset() {
    header_ = chain::header{};
    nonce_ = 0;
    short_ids_.clear();
    short_ids_.shrink_to_fit();
    transactions_.clear();
    transactions_.shrink_to_fit();
}

bool double_spend_proofs::from_block(message::block const& block) {
    reset();

    header_ = block.header();
    nonce_ = pseudo_random(1, max_uint64);

    prefilled_transaction::list prefilled_list{prefilled_transaction{0, block.transactions()[0]}};

    auto header_hash = hash(block, nonce_);

    auto k0 = from_little_endian_unsafe<uint64_t>(header_hash.begin());
    auto k1 = from_little_endian_unsafe<uint64_t>(header_hash.begin() + sizeof(uint64_t));

    double_spend_proofs::short_id_list short_ids_list;
    short_ids_list.reserve(block.transactions().size() - 1);
    for (size_t i = 1; i < block.transactions().size(); ++i) {
        uint64_t shortid = sip_hash_uint256(k0, k1, block.transactions()[i].hash(witness_default())) & uint64_t(0xffffffffffff);
        short_ids_list.push_back(shortid);
    }

    short_ids_ = std::move(short_ids_list);
    transactions_ = std::move(prefilled_list);

    return true;
}

data_chunk double_spend_proofs::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void double_spend_proofs::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t double_spend_proofs::serialized_size(uint32_t version) const {
    auto size = chain::header::satoshi_fixed_size() +
                infrastructure::message::variable_uint_size(short_ids_.size()) +
                (short_ids_.size() * 6u) +
                infrastructure::message::variable_uint_size(transactions_.size()) + 8u;

    // NOTE: Witness flag is controlled by prefilled tx
    for (auto const& tx : transactions_) {
        size += tx.serialized_size(version);
    }

    return size;
}

chain::header& double_spend_proofs::header() {
    return header_;
}

chain::header const& double_spend_proofs::header() const {
    return header_;
}

void double_spend_proofs::set_header(chain::header const& value) {
    header_ = value;
}

uint64_t double_spend_proofs::nonce() const {
    return nonce_;
}

void double_spend_proofs::set_nonce(uint64_t value) {
    nonce_ = value;
}

double_spend_proofs::short_id_list& double_spend_proofs::short_ids() {
    return short_ids_;
}

double_spend_proofs::short_id_list const& double_spend_proofs::short_ids() const {
    return short_ids_;
}

void double_spend_proofs::set_short_ids(const short_id_list& value) {
    short_ids_ = value;
}

void double_spend_proofs::set_short_ids(short_id_list&& value) {
    short_ids_ = std::move(value);
}

prefilled_transaction::list& double_spend_proofs::transactions() {
    return transactions_;
}

prefilled_transaction::list const& double_spend_proofs::transactions() const {
    return transactions_;
}

void double_spend_proofs::set_transactions(prefilled_transaction::list const& value) {
    transactions_ = value;
}

void double_spend_proofs::set_transactions(prefilled_transaction::list&& value) {
    transactions_ = std::move(value);
}

void to_data_header_nonce(double_spend_proofs const& block, data_sink& stream) {
    ostream_writer sink_w(stream);
    to_data_header_nonce(block, sink_w);
}

data_chunk to_data_header_nonce(double_spend_proofs const& block) {
    data_chunk data;
    auto size = chain::header::satoshi_fixed_size() + sizeof(block.nonce());

    data.reserve(size);
    data_sink ostream(data);
    to_data_header_nonce(block, ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

hash_digest hash(double_spend_proofs const& block) {
    return sha256_hash(to_data_header_nonce(block));
}

} // namespace kth::domain::message

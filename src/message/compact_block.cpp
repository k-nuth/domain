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
#include <bitcoin/bitcoin/message/compact_block.hpp>

#include <initializer_list>
#include <bitcoin/bitcoin/math/limits.hpp>
#include <bitcoin/bitcoin/message/messages.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/bitcoin/multi_crypto_support.hpp>
#include <bitcoin/bitcoin/utility/container_sink.hpp>
#include <bitcoin/bitcoin/utility/container_source.hpp>
#include <bitcoin/bitcoin/utility/istream_reader.hpp>
#include <bitcoin/bitcoin/utility/ostream_writer.hpp>
#include <bitcoin/bitcoin/utility/pseudo_random.hpp>
#include <bitcoin/bitcoin/math/sip_hash.hpp>


namespace libbitcoin {
namespace message {

const std::string compact_block::command = "cmpctblock";
const uint32_t compact_block::version_minimum = version::level::bip152;
const uint32_t compact_block::version_maximum = version::level::bip152;

compact_block compact_block::factory_from_data(uint32_t version,
    const data_chunk& data)
{
    //std::cout << "compact_block::factory_from_data\n";
    compact_block instance;
    instance.from_data(version, data);
    return instance;
}

compact_block compact_block::factory_from_data(uint32_t version,
    std::istream& stream)
{
    //std::cout << "compact_block::factory_from_data 2\n";
    compact_block instance;
    instance.from_data(version, stream);
    return instance;
}

compact_block compact_block::factory_from_data(uint32_t version,
    reader& source)
{
    //std::cout << "compact_block::factory_from_data 3\n";

    compact_block instance;
    instance.from_data(version, source);
    return instance;
}

compact_block compact_block::factory_from_block(message::block const& blk) {
    compact_block instance;
    instance.from_block(blk);
    return instance;
}

compact_block::compact_block()
  : header_(), nonce_(0), short_ids_(), transactions_()
{
    //std::cout << "compact_block::compact_block\n";

}

compact_block::compact_block(const chain::header& header, uint64_t nonce,
    const short_id_list& short_ids,
    const prefilled_transaction::list& transactions)
  : header_(header),
    nonce_(nonce),
    short_ids_(short_ids),
    transactions_(transactions)
{
    //std::cout << "compact_block::compact_block 2\n";

}

compact_block::compact_block(chain::header&& header, uint64_t nonce,
    short_id_list&& short_ids, prefilled_transaction::list&& transactions)
  : header_(std::move(header)),
    nonce_(nonce),
    short_ids_(std::move(short_ids)),
    transactions_(std::move(transactions))
{
    //std::cout << "compact_block::compact_block 3\n";

}

compact_block::compact_block(const compact_block& other)
  : compact_block(other.header_, other.nonce_, other.short_ids_,
      other.transactions_)
{
    //std::cout << "compact_block::compact_block 4\n";

}

compact_block::compact_block(compact_block&& other)
  : compact_block(std::move(other.header_), other.nonce_,
      std::move(other.short_ids_), std::move(other.transactions_))
{
    //std::cout << "compact_block::compact_block 5\n";

}

bool compact_block::is_valid() const
{
    //std::cout << "compact_block::is_valid\n";

    return header_.is_valid() && !short_ids_.empty() && !transactions_.empty();
}

void compact_block::reset()
{
    //std::cout << "compact_block::reset\n";

    header_ = chain::header{};
    nonce_ = 0;
    short_ids_.clear();
    short_ids_.shrink_to_fit();
    transactions_.clear();
    transactions_.shrink_to_fit();
}


 bool compact_block::from_block(message::block const& block) {
#ifdef BITPRIM_CURRENCY_BCH
     bool witness = false;
#else
     bool witness = true;
#endif
 
    reset();

    header_ = std::move(block.header());
    nonce_ = pseudo_random(1, max_uint64);

    prefilled_transaction::list prefilled_list {
        prefilled_transaction{0, block.transactions()[0]}
    };

    auto header_hash = hash(block, nonce_);
            
    auto k0 = from_little_endian_unsafe<uint64_t>(header_hash.begin());
    auto k1 = from_little_endian_unsafe<uint64_t>(header_hash.begin() + sizeof(uint64_t));

    compact_block::short_id_list short_ids_list;
    short_ids_list.reserve(block.transactions().size() - 1);
    for (size_t i = 1; i < block.transactions().size(); ++i) {
        uint64_t shortid = sip_hash_uint256(k0, k1, block.transactions()[i].hash(witness)) & uint64_t(0xffffffffffff);
        short_ids_list.push_back(shortid);
    }
            
    short_ids_ = std::move(short_ids_list);
    transactions_ = std::move(prefilled_list);

    return true;
 }

bool compact_block::from_data(uint32_t version, const data_chunk& data)
{
    //std::cout << "compact_block::from_data\n";

    data_source istream(data);
    return from_data(version, istream);
}

bool compact_block::from_data(uint32_t version, std::istream& stream)
{
    //std::cout << "compact_block::from_data 2\n";

    istream_reader source(stream);
    return from_data(version, source);
}

bool compact_block::from_data(uint32_t version, reader& source)
{
    //std::cout << "compact_block::from_data 3\n";

    reset();

    if (!header_.from_data(source))
        return false;

    nonce_ = source.read_8_bytes_little_endian();
    auto count = source.read_size_little_endian();

    // Guard against potential for arbitary memory allocation.
    if (count > get_max_block_size())
        source.invalidate();
    else
        short_ids_.reserve(count);

    //todo:move to function
    // Order is required.
    for (size_t id = 0; id < count && source; ++id) {
        uint32_t lsb = source.read_4_bytes_little_endian();
        uint16_t msb = source.read_2_bytes_little_endian();
        short_ids_.push_back((uint64_t(msb) << 32) | uint64_t(lsb));
        //short_ids_.push_back(source.read_mini_hash());
    }

    count = source.read_size_little_endian();

    // Guard against potential for arbitary memory allocation.
    if (count > get_max_block_size())
        source.invalidate();
    else
        transactions_.resize(count);

    // NOTE: Witness flag is controlled by prefilled tx
    // Order is required.
    for (auto& tx : transactions_)
        if (!tx.from_data(version, source))
            break;

    if (version < compact_block::version_minimum)
        source.invalidate();

    if (!source)
        reset();

    return source;
}

data_chunk compact_block::to_data(uint32_t version) const
{
    //std::cout << "compact_block::to_data\n";

    data_chunk data;
    const auto size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void compact_block::to_data(uint32_t version, std::ostream& stream) const
{
    //std::cout << "compact_block::to_data 2\n";

    ostream_writer sink(stream);
    to_data(version, sink);
}

void compact_block::to_data(uint32_t version, writer& sink) const
{
    //std::cout << "compact_block::to_data 3\n";

    header_.to_data(sink);
    sink.write_8_bytes_little_endian(nonce_);
    sink.write_variable_little_endian(short_ids_.size());

    for (const auto& element: short_ids_) {
        //sink.write_mini_hash(element);
        uint32_t lsb = element & 0xffffffff;
        uint16_t msb = (element >> 32) & 0xffff;
        sink.write_4_bytes_little_endian(lsb);
        sink.write_2_bytes_little_endian(msb);
    }

    sink.write_variable_little_endian(transactions_.size());

    // NOTE: Witness flag is controlled by prefilled tx
    for (const auto& element: transactions_)
        element.to_data(version, sink);
}

size_t compact_block::serialized_size(uint32_t version) const
{
    //std::cout << "compact_block::serialized_size\n";

    auto size = chain::header::satoshi_fixed_size() +
        message::variable_uint_size(short_ids_.size()) +
        (short_ids_.size() * 6u) +
        message::variable_uint_size(transactions_.size()) + 8u;

    // NOTE: Witness flag is controlled by prefilled tx
    for (const auto& tx: transactions_)
        size += tx.serialized_size(version);

    return size;
}

chain::header& compact_block::header()
{
    //std::cout << "compact_block::header\n";

    return header_;
}

const chain::header& compact_block::header() const
{
    //std::cout << "compact_block::header 2\n";

    return header_;
}

void compact_block::set_header(const chain::header& value)
{
    //std::cout << "compact_block::set_header\n";

    header_ = value;
}

void compact_block::set_header(chain::header&& value)
{
    //std::cout << "compact_block::set_header 2\n";

    header_ = std::move(value);
}

uint64_t compact_block::nonce() const
{
    //std::cout << "compact_block::nonce\n";

    return nonce_;
}

void compact_block::set_nonce(uint64_t value)
{
    //std::cout << "compact_block::set_nonce\n";
    nonce_ = value;
}

compact_block::short_id_list& compact_block::short_ids()
{
    //std::cout << "compact_block::short_ids\n";
    return short_ids_;
}

const compact_block::short_id_list& compact_block::short_ids() const
{
    //std::cout << "compact_block::short_ids 2\n";
    return short_ids_;
}

void compact_block::set_short_ids(const short_id_list& value)
{
    short_ids_ = value;
}

void compact_block::set_short_ids(short_id_list&& value)
{
    short_ids_ = std::move(value);
}

prefilled_transaction::list& compact_block::transactions()
{
    return transactions_;
}

const prefilled_transaction::list& compact_block::transactions() const
{
    return transactions_;
}

void compact_block::set_transactions(const prefilled_transaction::list& value)
{
    transactions_ = value;
}

void compact_block::set_transactions(prefilled_transaction::list&& value)
{
    transactions_ = std::move(value);
}

compact_block& compact_block::operator=(compact_block&& other)
{
    header_ = std::move(other.header_);
    nonce_ = other.nonce_;
    short_ids_ = std::move(other.short_ids_);
    transactions_ = std::move(other.transactions_);
    return *this;
}

bool compact_block::operator==(const compact_block& other) const
{
    return (header_ == other.header_) && (nonce_ == other.nonce_)
        && (short_ids_ == other.short_ids_)
        && (transactions_ == other.transactions_);
}

bool compact_block::operator!=(const compact_block& other) const
{
    return !(*this == other);
}

void to_data_header_nonce(compact_block const& block, writer& sink) {
    block.header().to_data(sink);
    sink.write_8_bytes_little_endian(block.nonce());
}

void to_data_header_nonce(compact_block const& block, std::ostream& stream) {
    ostream_writer sink(stream);
    to_data_header_nonce(block, sink);
}

data_chunk to_data_header_nonce(compact_block const& block) {
    //std::cout << "compact_block::to_data\n";

    data_chunk data;
    auto size = chain::header::satoshi_fixed_size() + sizeof(block.nonce());

    data.reserve(size);
    data_sink ostream(data);
    to_data_header_nonce(block, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

hash_digest hash(compact_block const& block) {
    return sha256_hash(to_data_header_nonce(block));
}


} // namespace message
} // namespace libbitcoin

// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_COMPACT_BLOCK_HPP
#define KTH_DOMAIN_MESSAGE_COMPACT_BLOCK_HPP

#include <istream>

#include <kth/domain/chain/header.hpp>
#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/message/block.hpp>
#include <kth/domain/message/prefilled_transaction.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

class KD_API compact_block {
public:
    using ptr = std::shared_ptr<compact_block>;
    using const_ptr = std::shared_ptr<const compact_block>;

    //using short_id = mini_hash;
    //using short_id_list = mini_hash_list;
    using short_id = uint64_t;
    using short_id_list = std::vector<short_id>;

    static
    compact_block factory_from_block(message::block const& blk);

    compact_block() = default;
    compact_block(chain::header const& header, uint64_t nonce, short_id_list const& short_ids, prefilled_transaction::list const& transactions);
    compact_block(chain::header const& header, uint64_t nonce, short_id_list&& short_ids, prefilled_transaction::list&& transactions);

    bool operator==(compact_block const& x) const;
    bool operator!=(compact_block const& x) const;

    chain::header& header();

    [[nodiscard]]
    chain::header const& header() const;

    void set_header(chain::header const& value);

    [[nodiscard]]
    uint64_t nonce() const;

    void set_nonce(uint64_t value);

    short_id_list& short_ids();

    [[nodiscard]]
    short_id_list const& short_ids() const;

    void set_short_ids(short_id_list const& value);
    void set_short_ids(short_id_list&& value);

    prefilled_transaction::list& transactions();

    [[nodiscard]]
    prefilled_transaction::list const& transactions() const;

    void set_transactions(prefilled_transaction::list const& value);
    void set_transactions(prefilled_transaction::list&& value);

    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, uint32_t version) {
        reset();

        if ( ! header_.from_data(source)) {
            return false;
        }

        nonce_ = source.read_8_bytes_little_endian();
        auto count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > static_absolute_max_block_size()) {
            source.invalidate();
        } else {
            short_ids_.reserve(count);
        }

        //TODO: move to function
        // Order is required.
        for (size_t id = 0; id < count && source; ++id) {
            uint32_t lsb = source.read_4_bytes_little_endian();
            uint16_t msb = source.read_2_bytes_little_endian();
            short_ids_.push_back((uint64_t(msb) << 32) | uint64_t(lsb));
            //short_ids_.push_back(source.read_mini_hash());
        }

        count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > static_absolute_max_block_size()) {
            source.invalidate();
        } else {
            transactions_.resize(count);
        }

        // NOTE: Witness flag is controlled by prefilled tx
        // Order is required.
        for (auto& tx : transactions_) {
            if ( ! entity_from_data(tx, version, source)) {
                break;
            }
        }

        if (version < compact_block::version_minimum) {
            source.invalidate();
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    bool from_block(message::block const& block);

    [[nodiscard]]
    data_chunk to_data(uint32_t version) const;

    void to_data(uint32_t version, data_sink& stream) const;

    template <typename W>
    void to_data(uint32_t version, W& sink) const {
        header_.to_data(sink);
        sink.write_8_bytes_little_endian(nonce_);
        sink.write_variable_little_endian(short_ids_.size());

        for (auto const& element : short_ids_) {
            //sink.write_mini_hash(element);
            uint32_t lsb = element & 0xffffffff;
            uint16_t msb = (element >> 32) & 0xffff;
            sink.write_4_bytes_little_endian(lsb);
            sink.write_2_bytes_little_endian(msb);
        }

        sink.write_variable_little_endian(transactions_.size());

        // NOTE: Witness flag is controlled by prefilled tx
        for (auto const& element : transactions_) {
            element.to_data(version, sink);
        }
    }

    //void to_data(uint32_t version, writer& sink) const;
    [[nodiscard]]
    bool is_valid() const;

    void reset();

    [[nodiscard]]
    size_t serialized_size(uint32_t version) const;

    static
    std::string const command;

    static
    uint32_t const version_minimum;

    static
    uint32_t const version_maximum;

private:
    chain::header header_;
    uint64_t nonce_{0};
    short_id_list short_ids_;
    prefilled_transaction::list transactions_;
};

template <typename W>
void to_data_header_nonce(compact_block const& block, W& sink) {
    block.header().to_data(sink);
    sink.write_8_bytes_little_endian(block.nonce());
}

void to_data_header_nonce(compact_block const& block, data_sink& stream);

data_chunk to_data_header_nonce(compact_block const& block);

hash_digest hash(compact_block const& block);

} // namespace kth::domain::message

#endif

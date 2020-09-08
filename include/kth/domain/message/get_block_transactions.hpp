// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_GET_BLOCK_TRANSACTIONS_HPP
#define KTH_DOMAIN_MESSAGE_GET_BLOCK_TRANSACTIONS_HPP

#include <istream>
#include <memory>

#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

class KD_API get_block_transactions {
public:
    using ptr = std::shared_ptr<get_block_transactions>;
    using const_ptr = std::shared_ptr<const get_block_transactions>;

    get_block_transactions();
    get_block_transactions(hash_digest const& block_hash, const std::vector<uint64_t>& indexes);
    get_block_transactions(hash_digest const& block_hash, std::vector<uint64_t>&& indexes);

    bool operator==(get_block_transactions const& x) const;
    bool operator!=(get_block_transactions const& x) const;

    hash_digest& block_hash();
    
    [[nodiscard]]
    hash_digest const& block_hash() const;
    
    void set_block_hash(hash_digest const& value);

    std::vector<uint64_t>& indexes();
    
    [[nodiscard]]
    const std::vector<uint64_t>& indexes() const;
    
    void set_indexes(const std::vector<uint64_t>& values);
    void set_indexes(std::vector<uint64_t>&& values);

    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, uint32_t /*version*/) {
        reset();

        block_hash_ = source.read_hash();
        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > get_max_block_size_network_independent()) {
            source.invalidate();
        } else {
            indexes_.reserve(count);
        }

        for (size_t position = 0; position < count && source; ++position) {
            indexes_.push_back(source.read_size_little_endian());
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    [[nodiscard]]
    data_chunk to_data(uint32_t version) const;

    void to_data(uint32_t version, data_sink& stream) const;

    template <typename W>
    void to_data(uint32_t /*version*/, W& sink) const {
        sink.write_hash(block_hash_);
        sink.write_variable_little_endian(indexes_.size());
        for (auto const& element : indexes_) {
            sink.write_variable_little_endian(element);
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
    hash_digest block_hash_;
    std::vector<uint64_t> indexes_;
};

} // namespace kth::domain::message

#endif

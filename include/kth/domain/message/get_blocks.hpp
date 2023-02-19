// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_GET_BLOCKS_HPP_
#define KTH_DOMAIN_MESSAGE_GET_BLOCKS_HPP_

// #ifndef KTH_PROTOCOL_DISABLE_GET_BLOCKS

#include <istream>
#include <memory>
#include <string>
#include <vector>

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

class KD_API get_blocks {
public:
    using ptr = std::shared_ptr<get_blocks>;
    using const_ptr = std::shared_ptr<const get_blocks>;

    get_blocks();
    get_blocks(hash_list const& start, hash_digest const& stop);
    get_blocks(hash_list&& start, hash_digest const& stop);

    bool operator==(get_blocks const& x) const;
    bool operator!=(get_blocks const& x) const;

    hash_list& start_hashes();

    [[nodiscard]]
    hash_list const& start_hashes() const;

    void set_start_hashes(hash_list const& value);
    void set_start_hashes(hash_list&& value);

    hash_digest& stop_hash();

    [[nodiscard]]
    hash_digest const& stop_hash() const;

    void set_stop_hash(hash_digest const& value);

    template <typename R, KTH_IS_READER(R)>
    /*virtual*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(R& source, uint32_t /*version*/) {
        reset();

        // Discard protocol version because it is stupid.
        source.read_4_bytes_little_endian();
        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > max_get_blocks) {
            source.invalidate();
        } else {
            start_hashes_.reserve(count);
        }

        for (size_t hash = 0; hash < count && source; ++hash) {
            start_hashes_.push_back(source.read_hash());
        }

        stop_hash_ = source.read_hash();

        if ( ! source) {
            reset();
        }

        return source;
    }

    [[nodiscard]]
    data_chunk to_data(uint32_t version) const;

    void to_data(uint32_t version, data_sink& stream) const;

    template <typename W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_4_bytes_little_endian(version);
        sink.write_variable_little_endian(start_hashes_.size());

        for (auto const& start_hash : start_hashes_) {
            sink.write_hash(start_hash);
        }

        sink.write_hash(stop_hash_);
    }

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
    // 10 sequential hashes, then exponential samples until reaching genesis.
    hash_list start_hashes_;
    hash_digest stop_hash_;
};

} // namespace kth::domain::message

// #endif // KTH_PROTOCOL_DISABLE_GET_BLOCKS

#endif // KTH_DOMAIN_MESSAGE_GET_BLOCKS_HPP_

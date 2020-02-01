// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_BLOCK_HPP
#define KTH_MESSAGE_BLOCK_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>

#include <bitcoin/bitcoin/chain/block.hpp>
#include <bitcoin/bitcoin/chain/header.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API block : public chain::block {
public:
    using ptr = std::shared_ptr<block>;
    using const_ptr = std::shared_ptr<const block>;
    using ptr_list = std::vector<ptr>;
    using const_ptr_list = std::vector<const_ptr>;
    using const_ptr_list_ptr = std::shared_ptr<const_ptr_list>;
    using const_ptr_list_const_ptr = std::shared_ptr<const const_ptr_list>;

    static block factory_from_data(uint32_t version, data_chunk const& data);
    static block factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static block factory_from_data(uint32_t version, R& source) {
        block instance;
        instance.from_data(version, source);
        return instance;
    }

    //static block factory_from_data(uint32_t version, reader& source);

    block() = default;

    block(chain::block const& x);
    block(chain::block&& x);

    block(chain::header const& header, chain::transaction::list&& transactions);
    block(chain::header const& header, chain::transaction::list const& transactions);

    block& operator=(chain::block&& x);

    // block(block const& x) = default;
    // block(block&& x) = default;
    // // This class is move assignable but not copy assignable.
    // block& operator=(block&& x) = default;
    // block& operator=(block const&) = default;

    bool operator==(chain::block const& x) const;
    bool operator!=(chain::block const& x) const;
    bool operator==(block const& x) const;
    bool operator!=(block const& x) const;



    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(uint32_t  /*version*/, R& source) {
        return chain::block::from_data(source, true);
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t  /*version*/, W& sink) const {
        chain::block::to_data(sink, true);
    }

    //void to_data(uint32_t version, writer& sink) const;
    size_t serialized_size(uint32_t version) const;


    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

//TODO(fernando): check this family of functions: to_data_header_nonce
template <typename W>
void to_data_header_nonce(block const& block, uint64_t nonce, W& sink) {
    block.header().to_data(sink);
    sink.write_8_bytes_little_endian(nonce);
}
// void to_data_header_nonce(block const& block, uint64_t nonce, writer& sink);

// void to_data_header_nonce(block const& block, uint64_t nonce, std::ostream& stream);
void to_data_header_nonce(block const& block, uint64_t nonce, data_sink& stream);

data_chunk to_data_header_nonce(block const& block, uint64_t nonce);

hash_digest hash(block const& block, uint64_t nonce);

}  // namespace message
}  // namespace kth

#endif

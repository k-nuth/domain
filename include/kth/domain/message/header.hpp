// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_HEADER_MESSAGE_HPP
#define KTH_DOMAIN_MESSAGE_HEADER_MESSAGE_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>

#include <kth/domain/chain/header.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

class KD_API header : public chain::header {
public:
    using list = std::vector<header>;
    using ptr = std::shared_ptr<header>;
    using const_ptr = std::shared_ptr<const header>;
    using ptr_list = std::vector<ptr>;
    using const_ptr_list = std::vector<const_ptr>;

    static
    size_t satoshi_fixed_size(uint32_t version);

    header() = default;
    header(uint32_t version, hash_digest const& previous_block_hash, hash_digest const& merkle, uint32_t timestamp, uint32_t bits, uint32_t nonce);
    header(chain::header const& x);
    header(header const& x) = default;
    header(header&& x) = default;

    header& operator=(chain::header const& x);

    /// This class is move assignable but not copy assignable. //TODO(fernando): why?
    header& operator=(header&& x) = default;
    header& operator=(header const& /*x*/) /*= delete*/;

    friend
    bool operator==(header const& x, header const& y) {
        return chain::header(x) == chain::header(y);
    }

    friend
    bool operator!=(header const& x, header const& y) {
        return !(x == y);    
    }

    friend
    bool operator==(header const& x, chain::header const& y) {
        return chain::header(x) == y;
    }

    friend
    bool operator!=(header const& x, chain::header const& y) {
        return !(x == y);    
    }

    friend
    bool operator==(chain::header const& x, header const& y) {
        return x == chain::header(y);
    }

    friend
    bool operator!=(chain::header const& x, header const& y) {
        return !(x == y);    
    }
    
    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, uint32_t version) {
        if ( ! chain::header::from_data(source)) {
            return false;
        }

        // The header message must trail a zero byte (yes, it's stoopid).
        // bitcoin.org/en/developer-reference#headers
        if (version != version::level::canonical && source.read_byte() != 0x00) {
            source.invalidate();
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <typename W>
    void to_data(uint32_t version, W& sink) const {
        chain::header::to_data(sink);

        if (version != version::level::canonical) {
            sink.write_variable_little_endian(0);
}
    }

    void reset();
    size_t serialized_size(uint32_t version) const;


    static
    std::string const command;
    
    static
    uint32_t const version_minimum;
    
    static
    uint32_t const version_maximum;
};

} // namespace kth::domain::message

#endif

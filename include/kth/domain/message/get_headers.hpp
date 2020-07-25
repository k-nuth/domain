// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_GET_HEADERS_HPP
#define KTH_DOMAIN_MESSAGE_GET_HEADERS_HPP

#include <istream>
#include <memory>
#include <string>

#include <kth/domain/message/get_blocks.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

class KD_API get_headers : public get_blocks {
public:
    using ptr = std::shared_ptr<get_headers>;
    using const_ptr = std::shared_ptr<const get_headers>;

    get_headers() = default;
    get_headers(hash_list const& start, hash_digest const& stop);
    get_headers(hash_list&& start, hash_digest const& stop);

    // get_headers(get_headers const& x) = default;
    // get_headers(get_headers&& x) = default;
    // // This class is move assignable but not copy assignable.
    // get_headers& operator=(get_headers&& x) = default;
    // get_headers& operator=(get_headers const&) = default;

    bool operator==(get_headers const& x) const;
    bool operator!=(get_headers const& x) const;

    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, uint32_t version) { /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
        if ( ! get_blocks::from_data(version, source)) {
            return false;
        }

        if (version < get_headers::version_minimum) {
            source.invalidate();
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    static
    std::string const command;

    static
    uint32_t const version_minimum;
    
    static
    uint32_t const version_maximum;
};

} // namespace kth::domain::message

#endif

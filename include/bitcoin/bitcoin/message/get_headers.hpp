// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_GET_HEADERS_HPP
#define KTH_MESSAGE_GET_HEADERS_HPP

#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/message/get_blocks.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API get_headers : public get_blocks {
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

    static get_headers factory_from_data(uint32_t version, data_chunk const& data);
    static get_headers factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static get_headers factory_from_data(uint32_t version, R& source) {
        get_headers instance;
        instance.from_data(version, source);
        return instance;
    }

    bool from_data(uint32_t version, data_chunk const& data); /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, std::istream& stream); /*override*/     //TODO(fernando): check if this function is used in a run-time-polymorphic way

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(uint32_t version, R& source) { /*override*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
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


    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

}  // namespace message
}  // namespace kth

#endif

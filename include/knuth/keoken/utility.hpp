// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_UTILITY_HPP_
#define KTH_KEOKEN_UTILITY_HPP_

#include <boost/optional.hpp>

#include <kth/domain/chain/transaction.hpp>
#include <kth/infrastructure/utility/data.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace knuth {
namespace keoken {

//C++11
template <typename E>
constexpr
    typename std::underlying_type<E>::type
    to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

// //C++14
// template <typename E>
// constexpr auto to_underlying(E e) noexcept {
//     return static_cast<std::underlying_type_t<E>>(e);
// }

template <Reader R, KTH_IS_READER(R)>
inline std::string read_null_terminated_string_unlimited(R& source) {
    // precondition: there is almost 1 `\0` byte in source
    std::string res;

    auto b = source.read_byte();
    while (source && b != 0) {
        res.push_back(b);
        b = source.read_byte();
    }

    return res;
}

template <Reader R, KTH_IS_READER(R)>
inline boost::optional<std::string> read_null_terminated_string(R& source, size_t max) {
    if (max == 0) {
        return boost::none;
    }

    std::string res;

    auto b = source.read_byte();
    while (source && b != 0) {
        res.push_back(b);
        if (res.size() >= max) {
            return boost::none;
        }
        b = source.read_byte();
    }

    return source ? boost::make_optional(res) : boost::none;
}

}  // namespace keoken
}  // namespace knuth

#endif  //KTH_KEOKEN_UTILITY_HPP_

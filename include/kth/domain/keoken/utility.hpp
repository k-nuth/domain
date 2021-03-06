// Copyright (c) 2016-2021 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_UTILITY_HPP_
#define KTH_KEOKEN_UTILITY_HPP_

#include <optional>

#include <kth/domain/chain/transaction.hpp>
#include <kth/infrastructure/utility/data.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::keoken {

//C++14
template <typename E>
constexpr auto to_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

template <typename R, KTH_IS_READER(R)>
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

template <typename R, KTH_IS_READER(R)>
inline std::optional<std::string> read_null_terminated_string(R& source, size_t max) {
    if (max == 0) {
        return std::nullopt;
    }

    std::string res;

    auto b = source.read_byte();
    while (source && b != 0) {
        res.push_back(b);
        if (res.size() >= max) {
            return std::nullopt;
        }
        b = source.read_byte();
    }

    return source ? boost::make_optional(res) : std::nullopt;
}

} // namespace keoken
} // namespace kth

#endif  //KTH_KEOKEN_UTILITY_HPP_

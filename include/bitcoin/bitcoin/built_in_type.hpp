// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_BUILT_IN_TYPE_HPP_
#define KTH_BUILT_IN_TYPE_HPP_

// #include <cstddef>
// #include <cstdint>

namespace libbitcoin {

template <typename T>
struct built_in_t {
    constexpr
    built_in_t() = default;

    constexpr
    built_in_t(T x)
        : val_(x)
    {}

    constexpr
    // explicit
    operator T() const {
        return val_;
    }

private:
    T val_;
};

}  // namespace kth

#endif // KTH_BUILT_IN_TYPE_HPP_

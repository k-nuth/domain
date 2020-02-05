// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_COMMON_HPP_
#define KTH_DOMAIN_COMMON_HPP_

namespace kth {

constexpr inline bool witness_default() {
#ifdef KTH_CURRENCY_BCH
    return false;
#else
    return true;
#endif
}

constexpr inline
#ifdef KTH_CURRENCY_BCH
bool witness_val(bool /*x*/) {
    return false;
#else
bool witness_val(bool x) {
    return x;
#endif
}

#ifdef KTH_CURRENCY_BCH
#define KTH_DECL_WITN_ARG bool /*witness*/ = false      //NOLINT
#define KTH_DEF_WITN_ARG bool /*witness = false*/       //NOLINT
#else
#define KTH_DECL_WITN_ARG bool witness = false          //NOLINT
#define KTH_DEF_WITN_ARG bool witness /*= false*/       //NOLINT
#endif


//TODO(fernando): move to infrastructure
//C++14
template <typename E>
constexpr auto to_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

}  // namespace kth

#endif  //KTH_DOMAIN_COMMON_HPP_

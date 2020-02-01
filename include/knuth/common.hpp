// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_COMMON_HPP_
#define KTH_COMMON_HPP_

<<<<<<< HEAD
namespace libbitcoin {
=======
namespace kth {
>>>>>>> dev

constexpr inline bool witness_default() {
#ifdef KTH_CURRENCY_BCH
    return false;
#else
    return true;
#endif
}

constexpr inline
#ifdef KTH_CURRENCY_BCH
<<<<<<< HEAD
    bool
    witness_val(bool /*x*/) {
=======
bool witness_val(bool /*x*/) {
>>>>>>> dev
    return false;
#else
bool witness_val(bool x) {
    return x;
#endif
}

#ifdef KTH_CURRENCY_BCH
<<<<<<< HEAD
#define KTH_DECL_WITN_ARG bool /*witness*/ = false
#define KTH_DEF_WITN_ARG bool /*witness = false*/
#else
#define KTH_DECL_WITN_ARG bool witness = false
#define KTH_DEF_WITN_ARG bool witness /*= false*/
=======
#define KTH_DECL_WITN_ARG bool /*witness*/ = false      //NOLINT
#define KTH_DEF_WITN_ARG bool /*witness = false*/       //NOLINT
#else
#define KTH_DECL_WITN_ARG bool witness = false          //NOLINT
#define KTH_DEF_WITN_ARG bool witness /*= false*/       //NOLINT
>>>>>>> dev
#endif



}  // namespace kth

#endif  //KTH_COMMON_HPP_

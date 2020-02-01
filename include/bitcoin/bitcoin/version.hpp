///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2015 libbitcoin developers (see COPYING).
//
//        GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
//
///////////////////////////////////////////////////////////////////////////////
#ifndef KTH_VERSION_HPP_
#define KTH_VERSION_HPP_

/**
 * The semantic version of this repository as: [major].[minor].[patch]
 * For interpretation of the versioning scheme see: http://semver.org
 */

#ifdef KTH_PROJECT_VERSION
#define KTH_DOMAIN_VERSION KTH_PROJECT_VERSION
#else
#define KTH_DOMAIN_VERSION "0.0.0"
#endif

// #define KTH_VERSION KTH_DOMAIN_VERSION

namespace libbitcoin {
namespace domain {

char const* version();

}  // namespace domain
}  // namespace kth

#endif  // KTH_VERSION_HPP_

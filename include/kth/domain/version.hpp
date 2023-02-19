// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

///////////////////////////////////////////////////////////////////////////////
//
//        GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
//
///////////////////////////////////////////////////////////////////////////////
#ifndef KTH_DOMAIN_VERSION_HPP
#define KTH_DOMAIN_VERSION_HPP

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

namespace kth {
namespace domain {

char const* version();

} // namespace domain
} // namespace kth

#endif // KTH_DOMAIN_VERSION_HPP

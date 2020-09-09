// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CONFIG_SETTINGS_HPP_
#define KTH_DOMAIN_CONFIG_SETTINGS_HPP_

namespace kth::domain::config {

// For configuration settings initialization in other libraries.
enum class settings {
     mainnet
    ,testnet
    ,regtest
#if defined(KTH_CURRENCY_BCH)    
    ,testnet4
#endif    
};

} // namespace kth::domain::config

#endif // KTH_DOMAIN_CONFIG_SETTINGS_HPP_

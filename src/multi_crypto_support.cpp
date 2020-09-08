// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


// #include <iostream>

#include <string>

#include <kth/domain/multi_crypto_support.hpp>

namespace kth {

namespace {

// static config::currency currency_ = config::currency::none;
// static infrastructure::config::settings network_ = kth::infrastructure::config::settings::none;

#if defined(KTH_CURRENCY_BCH)
// static   //Note(kth): static is redundant in a anonymous namespace
std::string cashaddr_prefix_ = "bitcoincash";
#endif  // KTH_CURRENCY_BCH

} // namespace

config::currency get_currency() {
#ifdef KTH_CURRENCY_LTC
    return config::currency::litecoin;
#elif defined(KTH_CURRENCY_BCH)
    return config::currency::bitcoin_cash;
#else
    return config::currency::bitcoin;
#endif  //KTH_CURRENCY_LTC
}

// void set_network(infrastructure::config::settings x) {
//     network_ = x;
// }

// void set_network(uint32_t identifier) {
// #ifdef KTH_CURRENCY_LTC
//     switch(identifier) {
//         case netmagic::ltc_mainnet: set_network(kth::infrastructure::config::settings::mainnet); break;
//         case netmagic::ltc_testnet: set_network(kth::infrastructure::config::settings::testnet); break;
//         case netmagic::ltc_regtest: set_network(kth::infrastructure::config::settings::regtest); break;
//         default: set_network(kth::infrastructure::config::settings::none);
//     }
// #elif defined(KTH_CURRENCY_BCH)
//     switch(identifier) {
//         case netmagic::bch_mainnet: set_network(kth::infrastructure::config::settings::mainnet); break;
//         case netmagic::bch_testnet: set_network(kth::infrastructure::config::settings::testnet); break;
//         case netmagic::bch_regtest: set_network(kth::infrastructure::config::settings::regtest); break;
//         default: set_network(kth::infrastructure::config::settings::none);
//     }
// #else
//     switch(identifier) {
//         case netmagic::btc_mainnet: set_network(kth::infrastructure::config::settings::mainnet); break;
//         case netmagic::btc_testnet: set_network(kth::infrastructure::config::settings::testnet); break;
//         case netmagic::btc_regtest: set_network(kth::infrastructure::config::settings::regtest); break;
//         default: set_network(kth::infrastructure::config::settings::none);
//     }
// #endif //KTH_CURRENCY_LTC
// }

// infrastructure::config::settings get_network() {
//     return network_;
// }

infrastructure::config::settings get_network(uint32_t identifier) {
#ifdef KTH_CURRENCY_LTC
    switch (identifier) {
        case netmagic::ltc_testnet:
            return kth::infrastructure::config::settings::testnet;
        case netmagic::ltc_regtest:
            return kth::infrastructure::config::settings::regtest;
        default:
        case netmagic::ltc_mainnet:
            return kth::infrastructure::config::settings::mainnet;
    }
#elif defined(KTH_CURRENCY_BCH)
    switch (identifier) {
        case netmagic::bch_testnet:
            return kth::infrastructure::config::settings::testnet;
        case netmagic::bch_regtest:
            return kth::infrastructure::config::settings::regtest;
        default:
        case netmagic::bch_mainnet:
            return kth::infrastructure::config::settings::mainnet;
    }
#else
    switch (identifier) {
        case netmagic::btc_testnet:
            return kth::infrastructure::config::settings::testnet;
        case netmagic::btc_regtest:
            return kth::infrastructure::config::settings::regtest;
        default:
        case netmagic::btc_mainnet:
            return kth::infrastructure::config::settings::mainnet;
    }
#endif  //KTH_CURRENCY_LTC
}

infrastructure::config::settings get_network(bool easy_blocks, bool retarget) {
    if ( ! retarget && ! easy_blocks ) {
        return kth::infrastructure::config::settings::regtest;
    }
    return easy_blocks ? kth::infrastructure::config::settings::testnet : kth::infrastructure::config::settings::mainnet;
}

#if defined(KTH_CURRENCY_BCH)
std::string cashaddr_prefix() {
    return cashaddr_prefix_;
}

void set_cashaddr_prefix(std::string const& x) {
    cashaddr_prefix_ = x;
}
#endif  //KTH_CURRENCY_BCH

// bool is_testnet(uint32_t identifier, bool bitcoin_cash) {
// #ifdef KTH_CURRENCY_LTC
//     return identifier == 0xf1c8d2fdu //4056470269u; //Litecoin Testnet Netmagic
// #else
//     if (bitcoin_cash) {
//         return identifier == 0xf4f3e5f4u;  //Bitcoin Cash Testnet Netmagic
//     } else {
//         return identifier == 0x0709110bu;  //Bitcoin Testnet Netmagic
//     }
// #endif //KTH_CURRENCY_LTC
// }

} // namespace kth

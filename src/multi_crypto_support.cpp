// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


// #include <iostream>

#include <string>

#include <bitcoin/bitcoin/multi_crypto_support.hpp>

namespace libbitcoin {

namespace {

// static config::currency currency_ = config::currency::none;
// static config::settings network_ = config::settings::none;

#ifdef KTH_CURRENCY_BCH
// static   //Note(kth): static is redundant in a anonymous namespace
std::string cashaddr_prefix_ = "bitcoincash";
#endif  // KTH_CURRENCY_BCH

}  // namespace

config::currency get_currency() {
#ifdef KTH_CURRENCY_LTC
    return config::currency::litecoin;
#elif defined(KTH_CURRENCY_BCH)
    return config::currency::bitcoin_cash;
#else
    return config::currency::bitcoin;
#endif  //KTH_CURRENCY_LTC
}

// void set_network(config::settings x) {
//     network_ = x;
// }

// void set_network(uint32_t identifier) {
// #ifdef KTH_CURRENCY_LTC
//     switch(identifier) {
//         case netmagic::ltc_mainnet: set_network(config::settings::mainnet); break;
//         case netmagic::ltc_testnet: set_network(config::settings::testnet); break;
//         case netmagic::ltc_regtest: set_network(config::settings::regtest); break;
//         default: set_network(config::settings::none);
//     }
// #elif defined(KTH_CURRENCY_BCH)
//     switch(identifier) {
//         case netmagic::bch_mainnet: set_network(config::settings::mainnet); break;
//         case netmagic::bch_testnet: set_network(config::settings::testnet); break;
//         case netmagic::bch_regtest: set_network(config::settings::regtest); break;
//         default: set_network(config::settings::none);
//     }
// #else
//     switch(identifier) {
//         case netmagic::btc_mainnet: set_network(config::settings::mainnet); break;
//         case netmagic::btc_testnet: set_network(config::settings::testnet); break;
//         case netmagic::btc_regtest: set_network(config::settings::regtest); break;
//         default: set_network(config::settings::none);
//     }
// #endif //KTH_CURRENCY_LTC
// }

// config::settings get_network() {
//     return network_;
// }

config::settings get_network(uint32_t identifier) {
#ifdef KTH_CURRENCY_LTC
    switch (identifier) {
        case netmagic::ltc_mainnet:
            return config::settings::mainnet;
        case netmagic::ltc_testnet:
            return config::settings::testnet;
        case netmagic::ltc_regtest:
            return config::settings::regtest;
        default:
            return config::settings::none;
    }
#elif defined(KTH_CURRENCY_BCH)
    switch (identifier) {
        case netmagic::bch_mainnet:
            return config::settings::mainnet;
        case netmagic::bch_testnet:
            return config::settings::testnet;
        case netmagic::bch_regtest:
            return config::settings::regtest;
        default:
            return config::settings::none;
    }
#else
    switch (identifier) {
        case netmagic::btc_mainnet:
            return config::settings::mainnet;
        case netmagic::btc_testnet:
            return config::settings::testnet;
        case netmagic::btc_regtest:
            return config::settings::regtest;
        default:
            return config::settings::none;
    }
#endif  //KTH_CURRENCY_LTC
}

config::settings get_network(bool easy_blocks, bool retarget) {
    if ( ! retarget && ! easy_blocks ) {
        return config::settings::regtest;
    }
    return easy_blocks ? config::settings::testnet : config::settings::mainnet;
}

#ifdef KTH_CURRENCY_BCH
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

} /*namespace libbitcoin*/

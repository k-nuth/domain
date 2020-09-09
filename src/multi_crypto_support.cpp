// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


// #include <iostream>

#include <string>

#include <kth/domain/multi_crypto_support.hpp>

namespace kth {

namespace {

#if defined(KTH_CURRENCY_BCH)
// static   //Note(kth): static is redundant in a anonymous namespace
std::string cashaddr_prefix_ = "bitcoincash";
#endif  // KTH_CURRENCY_BCH

} // namespace

config::currency get_currency() {
#if defined(KTH_CURRENCY_LTC)
    return config::currency::litecoin;
#elif defined(KTH_CURRENCY_BCH)
    return config::currency::bitcoin_cash;
#else
    return config::currency::bitcoin;
#endif
}

domain::config::settings get_network(uint32_t identifier) {
#if defined(KTH_CURRENCY_LTC)
    switch (identifier) {
        case netmagic::ltc_testnet:
            return domain::config::settings::testnet;
        case netmagic::ltc_regtest:
            return domain::config::settings::regtest;
        default:
        case netmagic::ltc_mainnet:
            return domain::config::settings::mainnet;
    }
#elif defined(KTH_CURRENCY_BCH)
    switch (identifier) {
        case netmagic::bch_testnet:
            return domain::config::settings::testnet;
        case netmagic::bch_regtest:
            return domain::config::settings::regtest;
        case netmagic::bch_testnet4:
            return domain::config::settings::testnet4;
        default:
        case netmagic::bch_mainnet:
            return domain::config::settings::mainnet;
    }
#else
    switch (identifier) {
        case netmagic::btc_testnet:
            return domain::config::settings::testnet;
        case netmagic::btc_regtest:
            return domain::config::settings::regtest;
        default:
        case netmagic::btc_mainnet:
            return domain::config::settings::mainnet;
    }
#endif
}

#if defined(KTH_CURRENCY_BCH)
std::string cashaddr_prefix() {
    return cashaddr_prefix_;
}

void set_cashaddr_prefix(std::string const& x) {
    cashaddr_prefix_ = x;
}
#endif  //KTH_CURRENCY_BCH

} // namespace kth

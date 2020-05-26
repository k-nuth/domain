// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MULTI_CRYPTO_SUPPORT_HPP
#define KTH_DOMAIN_MULTI_CRYPTO_SUPPORT_HPP

#include <string>

#include <kth/infrastructure/config/settings.hpp>

namespace kth {

namespace netmagic {
constexpr uint32_t btc_mainnet = 0xd9b4bef9U;
constexpr uint32_t btc_testnet = 0x0709110bU;
constexpr uint32_t btc_regtest = 0xdab5bffaU;

constexpr uint32_t ltc_mainnet = 0xdbb6c0fbU;
constexpr uint32_t ltc_testnet = 0xf1c8d2fdU;
constexpr uint32_t ltc_regtest = 0xdab5bffaU;

constexpr uint32_t bch_mainnet = 0xe8f3e1e3U;
constexpr uint32_t bch_testnet = 0xf4f3e5f4U;
constexpr uint32_t bch_regtest = 0xfabfb5daU;
} // namespace netmagic

namespace config {

enum class currency {
    none,
    bitcoin,
    bitcoin_cash,
    litecoin
};

}  // namespace config

config::currency get_currency();

// void set_network(config::settings x);
// void set_network(uint32_t identifier);
// config::settings get_network();
config::settings get_network(uint32_t identifier);
config::settings get_network(bool easy_blocks, bool retarget);

#ifdef KTH_CURRENCY_BCH
std::string cashaddr_prefix();
void set_cashaddr_prefix(std::string const& x);
#endif  //KTH_CURRENCY_BCH

} // namespace kth

#endif /*KTH_MULTI_CRYPTO_SUPPORT_HPP_*/

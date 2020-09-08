// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CONSTANTS_HPP_
#define KTH_DOMAIN_CONSTANTS_HPP_

#include <kth/infrastructure/assumptions.hpp>
#include <kth/infrastructure/constants.hpp>

#if defined(KTH_CURRENCY_BCH)
#include <kth/domain/constants/bch.hpp>
#elif defined(KTH_CURRENCY_BTC)
#include <kth/domain/constants/btc.hpp>
#elif defined(KTH_CURRENCY_LTC)
#include <kth/domain/constants/ltc.hpp>
#else
#error Unsupported currency
#endif

#include <kth/domain/constants/functions.hpp>

namespace kth {

#if defined(KTH_CURRENCY_BCH)
constexpr size_t max_block_sigchecks = get_max_block_size(kth::infrastructure::config::settings::mainnet) / block_maxbytes_maxsigchecks_ratio;
constexpr size_t max_block_sigchecks_testnet4 = get_max_block_size(kth::infrastructure::config::settings::testnet4) / block_maxbytes_maxsigchecks_ratio;
#endif

} // namespace kth

#endif // KTH_DOMAIN_CONSTANTS_HPP_

// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CONSTANTS_FUNCTIONS_HPP_
#define KTH_DOMAIN_CONSTANTS_FUNCTIONS_HPP_

// #include <cstddef>
// #include <cstdint>

// #include <kth/domain/define.hpp>
// #include <kth/domain/multi_crypto_settings.hpp>
// #include <kth/domain/version.hpp>

// #include <kth/infrastructure/compat.hpp>
// #include <kth/infrastructure/config/checkpoint.hpp>
// #include <kth/infrastructure/constants.hpp>
// #include <kth/infrastructure/math/hash.hpp>
// #include <kth/infrastructure/message/network_address.hpp>

namespace kth {

constexpr uint32_t work_limit(bool retarget = true) noexcept {
    return retarget ? retarget_proof_of_work_limit : no_retarget_proof_of_work_limit;
}

constexpr inline 
size_t get_max_block_size(domain::config::settings network) noexcept {
#if defined(KTH_CURRENCY_BCH)
    if (network == domain::config::settings::testnet4) return max_block_size_testnet4;
    return max_block_size_new;
#else
    return max_block_size;
#endif  //KTH_CURRENCY_BCH
}

constexpr inline 
size_t get_max_block_size_network_independent() noexcept {
#if defined(KTH_CURRENCY_BCH)
    return max_block_size_new;
#else
    return max_block_size;
#endif  //KTH_CURRENCY_BCH
}

constexpr inline 
size_t get_max_block_sigops(domain::config::settings network) noexcept {
#if defined(KTH_CURRENCY_BCH)
    if (network == domain::config::settings::testnet4) return max_block_sigops_testnet4;
    return max_block_sigops_new;
#else
    return max_block_sigops;
#endif  //KTH_CURRENCY_BCH
}

constexpr inline 
size_t get_allowed_sigops(size_t block_size) noexcept {
    return (1 + ((block_size - 1) / one_million_bytes_block)) * sigops_per_million_bytes;
}

constexpr inline 
uint64_t max_money_recursive(uint64_t money) noexcept {
    return money > 0 ? money + max_money_recursive(money >> 1) : 0;
}

constexpr inline 
uint64_t bitcoin_to_satoshi(uint64_t bitcoin_uints = 1) noexcept {
    return bitcoin_uints * satoshi_per_bitcoin;
}

constexpr inline 
uint64_t initial_block_subsidy_satoshi() noexcept {
    return bitcoin_to_satoshi(initial_block_subsidy_bitcoin);
}

constexpr 
uint64_t subsidy_interval(bool retarget = true) noexcept {
    return retarget ? retarget_subsidy_interval : no_retarget_subsidy_interval;
}

constexpr 
uint64_t max_money(bool retarget = true) noexcept {
    ////// Optimize out the derivation of recursive_money.
    ////KTH_ASSERT(recursive_money == max_money_recursive(
    ////    initial_block_subsidy_satoshi()));

    return recursive_money * subsidy_interval(retarget);
}


template <typename... Ts>
constexpr inline
auto&& network_map(domain::config::settings network, Ts&&... args) noexcept {
#if defined(KTH_CURRENCY_BCH)
    static_assert(sizeof...(Ts) == 4, "this function requires 4 values");
#else
    static_assert(sizeof...(Ts) == 3, "this function requires 3 values");
#endif

    auto const values = std::make_tuple(args...);
    switch (network) {
        case domain::config::settings::testnet:
            return std::forward<std::tuple_element_t<1, decltype(values)>>(std::get<1>(values));
        case domain::config::settings::regtest:
            return std::forward<std::tuple_element_t<2, decltype(values)>>(std::get<2>(values));

#if defined(KTH_CURRENCY_BCH)
        case domain::config::settings::testnet4:
            return std::forward<std::tuple_element_t<3, decltype(values)>>(std::get<3>(values));
#endif

        default:
        case domain::config::settings::mainnet:
            return std::forward<std::tuple_element_t<0, decltype(values)>>(std::get<0>(values));
    }
}

} // namespace kth

#endif // KTH_DOMAIN_CONSTANTS_FUNCTIONS_HPP_

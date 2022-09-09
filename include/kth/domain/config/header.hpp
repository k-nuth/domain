// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_HEADER_HPP
#define KTH_HEADER_HPP

#ifdef KTH_INFRASTUCTURE_CONFIG_ENABLED

#include <iostream>
#include <string>

#include <kth/domain/chain/header.hpp>
#include <kth/domain/define.hpp>

namespace kth::domain::config {

/**
 * Serialization helper to convert between serialized and deserialized satoshi header.
 */
class KD_API header {
public:
    header() = default;

    header(std::string const& hexcode);

    header(chain::header const& value);

    // header(header const& x);

    operator chain::header const&() const;

    // friend
    // std::istream& operator>>(std::istream& input, header& argument);

    // friend
    // std::ostream& operator<<(std::ostream& output, header const& argument);

private:
    chain::header value_;
};

} // namespace kth::domain::config

#endif

#endif // KTH_INFRASTUCTURE_CONFIG_ENABLED

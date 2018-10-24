/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/config/output.hpp>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

#include <bitcoin/bitcoin/config/point.hpp>
#include <bitcoin/bitcoin/config/script.hpp>
#include <bitcoin/bitcoin/math/stealth.hpp>
#include <bitcoin/bitcoin/wallet/stealth_address.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/string.hpp>

namespace libbitcoin {
namespace config {

using namespace boost::program_options;

output::output()
    : is_stealth_(false), amount_(0), version_(0), script_(), pay_to_hash_(null_short_hash) {
}

output::output(std::string const& tuple)
    : output() {
    std::stringstream(tuple) >> *this;
}

bool output::is_stealth() const {
    return is_stealth_;
}

uint64_t output::amount() const {
    return amount_;
}

uint8_t output::version() const {
    return version_;
}

chain::script const& output::script() const {
    return script_;
}

short_hash const& output::pay_to_hash() const {
    return pay_to_hash_;
}

std::istream& operator>>(std::istream& input, output& argument) {
    std::string tuple;
    input >> tuple;

    auto const tokens = split(tuple, point::delimeter);
    if (tokens.size() < 2 || tokens.size() > 3) {
        BOOST_THROW_EXCEPTION(invalid_option_value(tuple));
    }

    uint64_t amount;
    deserialize(amount, tokens[1], true);
    if (amount > max_money()) {
        BOOST_THROW_EXCEPTION(invalid_option_value(tuple));
    }

    argument.amount_ = amount;
    auto const& target = tokens.front();

    // Is the target a payment address?
    const wallet::payment_address payment(target);
    if (payment) {
        argument.version_ = payment.version();
        argument.pay_to_hash_ = payment.hash();
        return input;
    }

    // Is the target a stealth address?
    const wallet::stealth_address stealth(target);
    if (stealth) {
        // TODO(libbitcoin): finish stealth multisig implemetation (p2sh and !p2sh).

        if (stealth.spend_keys().size() != 1 || tokens.size() != 3) {
            BOOST_THROW_EXCEPTION(invalid_option_value(tuple));
        }

        data_chunk seed;
        if ( ! decode_base16(seed, tokens[2]) || seed.size() < minimum_seed_size) {
            BOOST_THROW_EXCEPTION(invalid_option_value(tuple));
        }

        ec_secret ephemeral_secret;
        if ( ! create_stealth_data(argument.script_, ephemeral_secret,
                                 stealth.filter(), seed)) {
            BOOST_THROW_EXCEPTION(invalid_option_value(tuple));
        }

        ec_compressed stealth_key;
        if ( ! uncover_stealth(stealth_key, stealth.scan_key(), ephemeral_secret,
                             stealth.spend_keys().front())) {
            BOOST_THROW_EXCEPTION(invalid_option_value(tuple));
        }

        argument.is_stealth_ = true;
        argument.pay_to_hash_ = bitcoin_short_hash(stealth_key);
        argument.version_ = stealth.version();
        return input;
    }

    // The target must be a serialized script.
    // Note that it is possible for a base16 encoded script to be interpreted
    // as an address above. That is unlikely but considered intended behavior.
    data_chunk decoded;
    if ( ! decode_base16(decoded, target)) {
        BOOST_THROW_EXCEPTION(invalid_option_value(target));
    }

    argument.script_ = script(decoded);
    return input;
}

}  // namespace config
}  // namespace libbitcoin

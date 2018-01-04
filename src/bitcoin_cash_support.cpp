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

// #include <iostream>

#include <string>

#include <bitcoin/bitcoin/bitcoin_cash_support.hpp>

namespace libbitcoin {

namespace {

static bool is_bitcoin_cash_= false;
static std::string cashaddr_prefix_ = "bitcoincash";

} // namespace anonymous

bool is_bitcoin_cash() {
    return is_bitcoin_cash_;
}

void set_bitcoin_cash(bool value) {
    is_bitcoin_cash_ = value;
}

std::string cashaddr_prefix() {
    return cashaddr_prefix_;
}

void set_cashaddr_prefix(std::string const& x) {
    cashaddr_prefix_ = x;
}

bool is_testnet(uint32_t identifier, bool bitcoin_cash) {
#ifdef LITECOIN
    return identifier == 0xf1c8d2fdu //4056470269u; //Litecoin Testnet Netmagic
#else
    if (bitcoin_cash) {
        return identifier == 0xf4f3e5f4u;  //Bitcoin Cash Testnet Netmagic
    } else {
        return identifier == 0x0709110bu;  //Bitcoin Testnet Netmagic
    }
#endif //LITECOIN
}

} /*namespace libbitcoin*/


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

#include <iostream>
#include <bitcoin/bitcoin/bitcoin_cash_support.hpp>

namespace libbitcoin {

static bool is_bitcoin_cash_= false;

bool is_bitcoin_cash() {
    std::cout << "is_bitcoin_cash(): " << is_bitcoin_cash_ << std::endl;
    return is_bitcoin_cash_;
}

bool set_bitcoin_cash(bool value) {
    std::cout << "set_bitcoin_cash(): " << is_bitcoin_cash_ << std::endl;

    is_bitcoin_cash_ = value;
}

} /*namespace libbitcoin*/


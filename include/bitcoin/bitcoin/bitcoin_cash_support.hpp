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
#ifndef LIBBITCOIN_CASH_SUPPORT_HPP
#define LIBBITCOIN_CASH_SUPPORT_HPP

namespace libbitcoin {

//bool is_bitcoin_cash_; /*= false;*/

bool is_bitcoin_cash();
void set_bitcoin_cash(bool value);

std::string cashaddr_prefix();
void set_cashaddr_prefix(std::string const& x);
bool is_testnet(uint32_t identifier, bool bitcoin_cash);

} /*namespace libbitcoin*/

#endif /*LIBBITCOIN_CASH_SUPPORT_HPP*/

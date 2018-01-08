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
#include <cstdlib>
#include <string>
#include <bitcoin/bitcoin.hpp>

BC_USE_LIBBITCOIN_MAIN

int bc::main(int argc, char* argv[])
{
    bc::wallet::payment_address const address1("1BpEi6DfDAUFd7GtittLSdBeYJvcoaVggu"); // Legacy Address Format
    //bc::wallet::payment_address const address2("bitcoincash:qpzry9x8gf2tvdw0s3jn54khce6mua7lcw20ayyn"); // CashAddrFormat Invalid
    //bc::wallet::payment_address const address2("bitcoincash:qpm2qsznhks23z7629mms6s4cwef74vcwvy22gdx6a"); // CashAddrFormat
    bc::wallet::payment_address const address2("bitcoincash:ppm2qsznhks23z7629mms6s4cwef74vcwvn0h829pq"); // CashAddrFormat



    auto enccash1 = address1.encoded_cashaddr();
    auto enccash2 = address2.encoded_cashaddr();

    auto encleg1 = address1.encoded();
    auto encleg2 = address2.encoded();


    return 0;
}

//// Testing out our http://utf8everywhere.org implementation.
//int bc::main(int argc, char* argv[])
//{
//    // Windows utf8 everywhere demonstration.
//    set_utf8_stdio();
//
//    bc::cout << "output : acción.кошка.日本国" << std::endl;
//    bc::cerr << "error : acción.кошка.日本国" << std::endl;
//
//    bc::cout << "Enter text to input..." << std::endl;
//    std::string console;
//    bc::cin >> console;
//    bc::cout << "input[0]  : " << console << std::endl;
//
//    if (argc > 1)
//        bc::cout << "argv[1] : " << argv[1] << std::endl;
//
//#ifdef _MSC_VER
//    if (environ[0] != nullptr)
//        bc::cout << "environ[0] : " << environ[0] << std::endl;
//#endif
//
//    // Extracting Satoshi's words.
//    const auto block = bc::chain::block::genesis_mainnet();
//    const auto& tx = block.transactions().front();
//    const auto& input = tx.inputs().front();
//    const auto script = input.script().to_data(false);
//    std::string message(script.begin() + sizeof(uint64_t), script.end());
//    bc::cout << message << std::endl;
//
//    return EXIT_SUCCESS;
//}

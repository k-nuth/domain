// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin/bitcoin.hpp>
#include <boost/test/unit_test.hpp>

using namespace bc;

// TODO(libbitcoin): move this into the tests by source file organization.
BOOST_AUTO_TEST_SUITE(satoshi_words)

BOOST_AUTO_TEST_CASE(satoshi_words_mainnet) {
    // Create mainnet genesis block (contains a single coinbase transaction).
    auto const block = chain::block::genesis_mainnet();
    auto const& transactions = block.transactions();
    BOOST_REQUIRE_EQUAL(transactions.size(), 1u);

    // Coinbase tx (first in block) has a single input.
    auto const& coinbase_tx = transactions[0];
    auto const& coinbase_inputs = coinbase_tx.inputs();
    BOOST_REQUIRE_EQUAL(coinbase_inputs.size(), 1u);

    // Convert the input script to its raw format.
    auto const& coinbase_input = coinbase_inputs[0];
    auto const raw_message = coinbase_input.script().to_data(false);
    BOOST_REQUIRE_GT(raw_message.size(), 8u);

    // Convert to a string after removing the 8 byte checksum.
    std::string message;
    message.resize(raw_message.size() - 8);
    std::copy(raw_message.begin() + 8, raw_message.end(), message.begin());

    BOOST_REQUIRE_EQUAL(message, "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks");
}

BOOST_AUTO_TEST_SUITE_END()

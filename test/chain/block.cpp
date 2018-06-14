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
#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;

// Test helper.
static bool all_valid(const chain::transaction::list& transactions)
{
    auto valid = true;

    for (const auto& tx: transactions)
    {
        valid = valid && tx.is_valid();

        for (const auto& input: tx.inputs())
        {
            valid = valid && input.is_valid();
            valid = valid && input.script().is_valid();
        }

        for (const auto& output: tx.outputs())
        {
            valid = valid && output.is_valid();
            valid = valid && output.script().is_valid();
        }
    }

    return valid;
}

BOOST_AUTO_TEST_SUITE(chain_block_tests)

BOOST_AUTO_TEST_CASE(block__proof2__genesis_mainnet__expected)
{
    const auto block = chain::block::genesis_mainnet();
    BOOST_REQUIRE_EQUAL(block.proof(), 0x0000000100010001);
}

BOOST_AUTO_TEST_CASE(block__locator_size__zero_backoff__returns_top_plus_one)
{
    size_t top = 7u;
    BOOST_REQUIRE_EQUAL(top + 1, chain::block::locator_size(top));
}

BOOST_AUTO_TEST_CASE(block__locator_size__positive_backoff__returns_log_plus_eleven)
{
    size_t top = 138u;
    BOOST_REQUIRE_EQUAL(18u, chain::block::locator_size(top));
}

BOOST_AUTO_TEST_CASE(block__locator_heights__zero_backoff__returns_top_to_zero)
{
    const chain::block::indexes expected{ 7u, 6u, 5u, 4u, 3u, 2u, 1u, 0u };
    size_t top = 7u;
    auto result = chain::block::locator_heights(top);
    BOOST_REQUIRE_EQUAL(expected.size(), result.size());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(block__locator_heights__positive_backoff__returns_top_plus_log_offset_to_zero)
{
    const chain::block::indexes expected
    {
        138u, 137u, 136u, 135u, 134u, 133u, 132u, 131u, 130u,
        129u, 128u, 126u, 122u, 114u,  98u,  66u,   2u,   0u
    };

    size_t top = 138u;
    auto result = chain::block::locator_heights(top);
    BOOST_REQUIRE_EQUAL(expected.size(), result.size());
    BOOST_REQUIRE(expected == result);
}

BOOST_AUTO_TEST_CASE(block__constructor_1__always__invalid)
{
    chain::block instance;
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(block__constructor_2__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    chain::block instance(header, transactions);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__constructor_3__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    // These must be non-const.
    chain::header dup_header(header);
    chain::transaction::list dup_transactions(transactions);

    chain::block instance(std::move(dup_header), std::move(dup_transactions));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());

}

BOOST_AUTO_TEST_CASE(block__constructor_4__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    chain::block value(header, transactions);
    chain::block instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__constructor_5__always__equals_params)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    // This must be non-const.
    chain::block value(header, transactions);

    chain::block instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__hash__always__returns_header_hash)
{
    chain::block instance;
    BOOST_REQUIRE(instance.header().hash() == instance.hash());
}

BOOST_AUTO_TEST_CASE(block__is_valid_merkle_root__uninitialized__returns_true)
{
    chain::block instance;
    BOOST_REQUIRE(instance.is_valid_merkle_root());
}

BOOST_AUTO_TEST_CASE(block__is_valid_merkle_root__non_empty_tx_invalid_block__returns_false)
{
    chain::block instance;
    instance.set_transactions(chain::transaction::list{ chain::transaction{} });
    BOOST_REQUIRE(!instance.is_valid_merkle_root());
}

BOOST_AUTO_TEST_CASE(block__is_valid_merkle_root__valid__returns_true)
{
    const auto raw_block = to_chunk(base16_literal(
        "010000007f110631052deeee06f0754a3629ad7663e56359fd5f3aa7b3e30a0000000"
        "0005f55996827d9712147a8eb6d7bae44175fe0bcfa967e424a25bfe9f4dc118244d6"
        "7fb74c9d8e2f1bea5ee82a03010000000100000000000000000000000000000000000"
        "00000000000000000000000000000ffffffff07049d8e2f1b0114ffffffff0100f205"
        "2a0100000043410437b36a7221bc977dce712728a954e3b5d88643ed5aef46660ddcf"
        "eeec132724cd950c1fdd008ad4a2dfd354d6af0ff155fc17c1ee9ef802062feb07ef1"
        "d065f0ac000000000100000001260fd102fab456d6b169f6af4595965c03c2296ecf2"
        "5bfd8790e7aa29b404eff010000008c493046022100c56ad717e07229eb93ecef2a32"
        "a42ad041832ffe66bd2e1485dc6758073e40af022100e4ba0559a4cebbc7ccb5d14d1"
        "312634664bac46f36ddd35761edaae20cefb16f01410417e418ba79380f462a60d8dd"
        "12dcef8ebfd7ab1741c5c907525a69a8743465f063c1d9182eea27746aeb9f1f52583"
        "040b1bc341b31ca0388139f2f323fd59f8effffffff0200ffb2081d0000001976a914"
        "fc7b44566256621affb1541cc9d59f08336d276b88ac80f0fa02000000001976a9146"
        "17f0609c9fabb545105f7898f36b84ec583350d88ac00000000010000000122cd6da2"
        "6eef232381b1a670aa08f4513e9f91a9fd129d912081a3dd138cb013010000008c493"
        "0460221009339c11b83f234b6c03ebbc4729c2633cbc8cbd0d15774594bfedc45c4f9"
        "9e2f022100ae0135094a7d651801539df110a028d65459d24bc752d7512bc8a9f78b4"
        "ab368014104a2e06c38dc72c4414564f190478e3b0d01260f09b8520b196c2f6ec3d0"
        "6239861e49507f09b7568189efe8d327c3384a4e488f8c534484835f8020b3669e5ae"
        "bffffffff0200ac23fc060000001976a914b9a2c9700ff9519516b21af338d28d53dd"
        "f5349388ac00743ba40b0000001976a914eb675c349c474bec8dea2d79d12cff6f330"
        "ab48788ac00000000"));

    chain::block instance;
    BOOST_REQUIRE(instance.from_data(raw_block));
    BOOST_REQUIRE(instance.is_valid_merkle_root());
}

BOOST_AUTO_TEST_SUITE(block_serialization_tests)

BOOST_AUTO_TEST_CASE(block__from_data__insufficient_bytes__failure)
{
    data_chunk data(10);
    chain::block instance;
    BOOST_REQUIRE(!instance.from_data(data));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(block__from_data__insufficient_transaction_bytes__failure)
{
    const data_chunk data = to_chunk(base16_literal(
        "010000007f110631052deeee06f0754a3629ad7663e56359fd5f3aa7b3e30a00"
        "000000005f55996827d9712147a8eb6d7bae44175fe0bcfa967e424a25bfe9f4"
        "dc118244d67fb74c9d8e2f1bea5ee82a03010000000100000000000000000000"
        "00000000000000000000000000000000000000000000ffffffff07049d8e2f1b"
        "0114ffffffff0100f2052a0100000043410437b36a7221bc977dce712728a954"));

    chain::block instance;
    BOOST_REQUIRE(!instance.from_data(data));
    BOOST_REQUIRE(!instance.is_valid());
}

BOOST_AUTO_TEST_CASE(block__genesis__mainnet__valid_structure)
{
    const auto genesis = bc::chain::block::genesis_mainnet();
    BOOST_REQUIRE(genesis.is_valid());
    BOOST_REQUIRE_EQUAL(genesis.transactions().size(), 1u);
    BOOST_REQUIRE(genesis.header().merkle() == genesis.generate_merkle_root());
}

BOOST_AUTO_TEST_CASE(block__genesis__testnet__valid_structure)
{
    const auto genesis = bc::chain::block::genesis_testnet();
    BOOST_REQUIRE(genesis.is_valid());
    BOOST_REQUIRE_EQUAL(genesis.transactions().size(), 1u);
    BOOST_REQUIRE(genesis.header().merkle() == genesis.generate_merkle_root());
}

BOOST_AUTO_TEST_CASE(block__genesis__regtest__valid_structure)
{
    const auto genesis = bc::chain::block::genesis_regtest();
    BOOST_REQUIRE(genesis.is_valid());
    BOOST_REQUIRE_EQUAL(genesis.transactions().size(), 1u);
    BOOST_REQUIRE(genesis.header().merkle() == genesis.generate_merkle_root());
}


BOOST_AUTO_TEST_CASE(block__factory_from_data_1__genesis_mainnet__success)
{
    const auto genesis = bc::chain::block::genesis_mainnet();
    BOOST_REQUIRE_EQUAL(genesis.serialized_size(), 285u);
    BOOST_REQUIRE_EQUAL(genesis.header().serialized_size(), 80u);

    // Save genesis block.
    auto raw_block = genesis.to_data();
    BOOST_REQUIRE_EQUAL(raw_block.size(), 285u);

    // Reload genesis block.
    const auto block = chain::block::factory_from_data(raw_block);

    BOOST_REQUIRE(block.is_valid());
    BOOST_REQUIRE(genesis.header() == block.header());

    // Verify merkle root from transactions.
    BOOST_REQUIRE(genesis.header().merkle() == block.generate_merkle_root());
}

BOOST_AUTO_TEST_CASE(block__factory_from_data_2__genesis_mainnet__success)
{
    const auto genesis = bc::chain::block::genesis_mainnet();
    BOOST_REQUIRE_EQUAL(genesis.serialized_size(), 285u);
    BOOST_REQUIRE_EQUAL(genesis.header().serialized_size(), 80u);

    // Save genesis block.
    auto raw_block = genesis.to_data();
    BOOST_REQUIRE_EQUAL(raw_block.size(), 285u);

    // Reload genesis block.
    data_source stream(raw_block);
    const auto block = chain::block::factory_from_data(stream);

    BOOST_REQUIRE(block.is_valid());
    BOOST_REQUIRE(genesis.header() == block.header());

    // Verify merkle root from transactions.
    BOOST_REQUIRE(genesis.header().merkle() == block.generate_merkle_root());
}

BOOST_AUTO_TEST_CASE(block__factory_from_data_3__genesis_mainnet__success)
{
    const auto genesis = bc::chain::block::genesis_mainnet();
    BOOST_REQUIRE_EQUAL(genesis.serialized_size(), 285u);
    BOOST_REQUIRE_EQUAL(genesis.header().serialized_size(), 80u);

    // Save genesis block.
    data_chunk raw_block = genesis.to_data();
    BOOST_REQUIRE_EQUAL(raw_block.size(), 285u);

    // Reload genesis block.
    data_source stream(raw_block);
    istream_reader reader(stream);
    const auto block = chain::block::factory_from_data(reader);

    BOOST_REQUIRE(block.is_valid());
    BOOST_REQUIRE(genesis.header() == block.header());

    // Verify merkle root from transactions.
    BOOST_REQUIRE(genesis.header().merkle() == block.generate_merkle_root());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(block_generate_merkle_root_tests)

BOOST_AUTO_TEST_CASE(block__generate_merkle_root__block_with_zero_transactions__matches_null_hash)
{
    chain::block empty;
    BOOST_REQUIRE(empty.generate_merkle_root() == null_hash);
}

BOOST_AUTO_TEST_CASE(block__generate_merkle_root__block_with_multiple_transactions__matches_historic_data)
{
    // encodes the 100,000 block data.
    const data_chunk raw = to_chunk(base16_literal(
        "010000007f110631052deeee06f0754a3629ad7663e56359fd5f3aa7b3e30a00"
        "000000005f55996827d9712147a8eb6d7bae44175fe0bcfa967e424a25bfe9f4"
        "dc118244d67fb74c9d8e2f1bea5ee82a03010000000100000000000000000000"
        "00000000000000000000000000000000000000000000ffffffff07049d8e2f1b"
        "0114ffffffff0100f2052a0100000043410437b36a7221bc977dce712728a954"
        "e3b5d88643ed5aef46660ddcfeeec132724cd950c1fdd008ad4a2dfd354d6af0"
        "ff155fc17c1ee9ef802062feb07ef1d065f0ac000000000100000001260fd102"
        "fab456d6b169f6af4595965c03c2296ecf25bfd8790e7aa29b404eff01000000"
        "8c493046022100c56ad717e07229eb93ecef2a32a42ad041832ffe66bd2e1485"
        "dc6758073e40af022100e4ba0559a4cebbc7ccb5d14d1312634664bac46f36dd"
        "d35761edaae20cefb16f01410417e418ba79380f462a60d8dd12dcef8ebfd7ab"
        "1741c5c907525a69a8743465f063c1d9182eea27746aeb9f1f52583040b1bc34"
        "1b31ca0388139f2f323fd59f8effffffff0200ffb2081d0000001976a914fc7b"
        "44566256621affb1541cc9d59f08336d276b88ac80f0fa02000000001976a914"
        "617f0609c9fabb545105f7898f36b84ec583350d88ac00000000010000000122"
        "cd6da26eef232381b1a670aa08f4513e9f91a9fd129d912081a3dd138cb01301"
        "0000008c4930460221009339c11b83f234b6c03ebbc4729c2633cbc8cbd0d157"
        "74594bfedc45c4f99e2f022100ae0135094a7d651801539df110a028d65459d2"
        "4bc752d7512bc8a9f78b4ab368014104a2e06c38dc72c4414564f190478e3b0d"
        "01260f09b8520b196c2f6ec3d06239861e49507f09b7568189efe8d327c3384a"
        "4e488f8c534484835f8020b3669e5aebffffffff0200ac23fc060000001976a9"
        "14b9a2c9700ff9519516b21af338d28d53ddf5349388ac00743ba40b00000019"
        "76a914eb675c349c474bec8dea2d79d12cff6f330ab48788ac00000000"));

    chain::block block100k;
    BOOST_REQUIRE(block100k.from_data(raw));
    BOOST_REQUIRE(block100k.is_valid());

    const auto serial = block100k.to_data();
    BOOST_REQUIRE(raw == serial);

    const auto header = block100k.header();
    const auto transactions = block100k.transactions();
    BOOST_REQUIRE(all_valid(transactions));
    BOOST_REQUIRE(header.merkle() == block100k.generate_merkle_root());
}

BOOST_AUTO_TEST_CASE(block__header_accessor__always__returns_initialized_value)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    chain::block instance(header, transactions);
    BOOST_REQUIRE(header == instance.header());
}

BOOST_AUTO_TEST_CASE(block__header_setter_1__roundtrip__success)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    chain::block instance;
    BOOST_REQUIRE(header != instance.header());
    instance.set_header(header);
    BOOST_REQUIRE(header == instance.header());
}

BOOST_AUTO_TEST_CASE(block__header_setter_2__roundtrip__success)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    // This must be non-const.
    chain::header dup_header(header);

    chain::block instance;
    BOOST_REQUIRE(header != instance.header());
    instance.set_header(std::move(dup_header));
    BOOST_REQUIRE(header == instance.header());
}

BOOST_AUTO_TEST_CASE(block__transactions_accessor__always__returns_initialized_value)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    chain::block instance(header, transactions);
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__transactions_setter_1__roundtrip__success)
{
    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    chain::block instance;
    BOOST_REQUIRE(transactions != instance.transactions());
    instance.set_transactions(transactions);
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__transactions_setter_2__roundtrip__success)
{
    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    // This must be non-const.
    chain::transaction::list dup_transactions(transactions);

    chain::block instance;
    BOOST_REQUIRE(transactions != instance.transactions());
    instance.set_transactions(std::move(dup_transactions));
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__operator_assign_equals__always__matches_equivalent)
{
    const chain::header header(10u,
        hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
        hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
        531234u,
        6523454u,
        68644u);

    const chain::transaction::list transactions
    {
        chain::transaction(1, 48, {}, {}),
        chain::transaction(2, 32, {}, {}),
        chain::transaction(4, 16, {}, {})
    };

    // This must be non-const.
    chain::block value(header, transactions);

    BOOST_REQUIRE(value.is_valid());
    chain::block instance;
    BOOST_REQUIRE(!instance.is_valid());
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_equals__duplicates__returns_true)
{
    const chain::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    chain::block instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_equals__differs__returns_false)
{
    const chain::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });


    chain::block instance;
    BOOST_REQUIRE(!(instance == expected));
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_not_equals__duplicates__returns_false)
{
    const chain::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    chain::block instance(expected);
    BOOST_REQUIRE(!(instance != expected));
}

BOOST_AUTO_TEST_CASE(block__operator_boolean_not_equals__differs__returns_true)
{
    const chain::block expected(
        chain::header(10u,
            hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
            hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
            531234u,
            6523454u,
            68644u),
        {
            chain::transaction(1, 48, {}, {}),
            chain::transaction(2, 32, {}, {}),
            chain::transaction(4, 16, {}, {})
        });

    chain::block instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(block_is_distinct_transaction_set_tests)

BOOST_AUTO_TEST_CASE(block__distinct_transactions__empty__true)
{
    chain::block value;
    BOOST_REQUIRE(value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__single__true)
{
    chain::block value;
    value.set_transactions({ { 1, 0, {}, {} } });
    BOOST_REQUIRE(value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__duplicate__false)
{
    chain::block value;
    value.set_transactions({ { 1, 0, {}, {} }, { 1, 0, {}, {} } });
    BOOST_REQUIRE(!value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__distinct_by_version__true)
{
    chain::block value;
    value.set_transactions({ { 1, 0, {}, {} }, { 2, 0, {}, {} }, { 3, 0, {}, {} } });
    BOOST_REQUIRE(value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__partialy_distinct_by_version__false)
{
    chain::block value;
    value.set_transactions({ { 1, 0, {}, {} }, { 2, 0, {}, {} }, { 2, 0, {}, {} } });
    BOOST_REQUIRE(!value.is_distinct_transaction_set());
}

BOOST_AUTO_TEST_CASE(validate_block__is_distinct_tx_set__partialy_distinct_not_adjacent_by_version__false)
{
    chain::block value;
    value.set_transactions({ { 1, 0, {}, {} }, { 2, 0, {}, {} }, { 1, 0, {}, {} } });
    BOOST_REQUIRE(!value.is_distinct_transaction_set());
}

#ifdef BITPRIM_CURRENCY_BCH
BOOST_AUTO_TEST_CASE(validate_block__is_cash_pow_valid__true){
    uint32_t old_bits = 402736949;
    const chain::compact bits (old_bits);
    uint256_t target (bits);
    BOOST_REQUIRE_EQUAL(chain::compact(chain::chain_state::difficulty_adjustment_cash(target)).normal(), 402757890);
}
#endif //BITPRIM_CURRENCY_BCH


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(block_is_forward_reference_tests)

BOOST_AUTO_TEST_CASE(block__is_forward_reference__no_transactions__false)
{
    chain::block value;
    BOOST_REQUIRE(!value.is_forward_reference());
}

BOOST_AUTO_TEST_CASE(block__is_forward_reference__multiple_empty_transactions__false)
{
    chain::block value;
    value.set_transactions({ { 1, 0, {}, {} }, { 2, 0, {}, {} } });
    BOOST_REQUIRE(!value.is_forward_reference());
}

BOOST_AUTO_TEST_CASE(block__is_forward_reference__backward_reference__false)
{
    chain::block value;
    chain::transaction before{ 2, 0, {}, {} };
    chain::transaction after{ 1, 0, { { { before.hash(), 0 }, {}, 0 } }, {} };
    value.set_transactions({ before, after });
    BOOST_REQUIRE(!value.is_forward_reference());
}

BOOST_AUTO_TEST_CASE(block__is_forward_reference__duplicate_transactions__false)
{
    chain::block value;
    value.set_transactions({ { 1, 0, {}, {} }, { 1, 0, {}, {} } });
    BOOST_REQUIRE(!value.is_forward_reference());
}

BOOST_AUTO_TEST_CASE(block__is_forward_reference__coinbase_and_multiple_empty_transactions__false)
{
    chain::block value;
    chain::transaction coinbase{ 1, 0, { { { null_hash, chain::point::null_index }, {}, 0 } }, {} };
    value.set_transactions({ coinbase, { 2, 0, {}, {} }, { 3, 0, {}, {} } });
    BOOST_REQUIRE(!value.is_forward_reference());
}

BOOST_AUTO_TEST_CASE(block__is_forward_reference__forward_reference__true)
{
    chain::block value;
    chain::transaction after{ 2, 0, {}, {} };
    chain::transaction before{ 1, 0, { { { after.hash(), 0 }, {}, 0 } }, {} };
    value.set_transactions({ before, after });
    BOOST_REQUIRE(value.is_forward_reference());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

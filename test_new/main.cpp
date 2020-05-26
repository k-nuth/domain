// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <kth/domain/chain/transaction.hpp>
// #include <kth/domain/utility/container_source.hpp>
#include <kth/infrastructure/formats/base_16.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>

#include <kth/domain/keoken/message/base.hpp>
#include <kth/domain/keoken/message/create_asset.hpp>
#include <kth/domain/keoken/message/send_tokens.hpp>
#include <kth/domain/keoken/transaction_extractor.hpp>
#include <kth/domain/keoken/utility.hpp>

using namespace kth::keoken;

using kth::base16_literal;
using kth::data_chunk;
using kth::data_source;
using kth::istream_reader;
using kth::to_chunk;

TEST_CASE("[test_read_null_terminated_string_unlimited] 1") {
    data_chunk data = {'A', 'B', 'C', 0};
    kth::data_source ds(data);
    kth::istream_reader source(ds);

    auto ret = read_null_terminated_string_unlimited(source);
    REQUIRE(ret == "ABC");
}

TEST_CASE("[test_read_null_terminated_string_unlimited] 2") {
    data_chunk data = {0};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string_unlimited(source);
    REQUIRE(ret == "");
}

TEST_CASE("[test_read_null_terminated_string_unlimited] 3") {
    data_chunk data = {};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string_unlimited(source);
    REQUIRE(ret == "");
}

TEST_CASE("[test_read_null_terminated_string] 1") {
    data_chunk data = {'A', 'B', 'C', 0};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string(source, 4);
    REQUIRE(bool(ret));
    REQUIRE(*ret == "ABC");
}

TEST_CASE("[test_read_null_terminated_string] 2") {
    data_chunk data = {'A', 'B', 'C', 0};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string(source, 5);
    REQUIRE(bool(ret));
    REQUIRE(*ret == "ABC");
}

TEST_CASE("[test_read_null_terminated_string] 3") {
    data_chunk data = {'A', 'B', 'C', 0};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string(source, 3);
    REQUIRE(!bool(ret));
}

TEST_CASE("[test_read_null_terminated_string] 4") {
    data_chunk data = {'A', 'B', 'C', 0};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string(source, 2);
    REQUIRE(!bool(ret));
}

TEST_CASE("[test_read_null_terminated_string] 5") {
    data_chunk data = {'A', 'B', 'C', 0};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string(source, 1);
    REQUIRE(!bool(ret));
}

TEST_CASE("[test_read_null_terminated_string] 6") {
    data_chunk data = {'A', 'B', 'C', 0};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string(source, 0);
    REQUIRE(!bool(ret));
}

TEST_CASE("[test_read_null_terminated_string] 7") {
    data_chunk data = {0};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string(source, 0);
    REQUIRE(!bool(ret));
}

TEST_CASE("[test_read_null_terminated_string] 8") {
    data_chunk data = {0};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string(source, 1);
    REQUIRE(bool(ret));
    REQUIRE(*ret == "");
}

TEST_CASE("[test_read_null_terminated_string] 9") {
    data_chunk data = {};
    bc::data_source ds(data);
    bc::istream_reader source(ds);

    auto ret = read_null_terminated_string(source, 1);
    REQUIRE(!bool(ret));
}

TEST_CASE("[test_get_keoken_output_empty]") {
    data_chunk raw_tx = to_chunk(base16_literal(
        "0100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff63294"
        "789eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e"
        "2830b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b"
        "12c517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c467000000"
        "001976a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f00"
        "0000001976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000"
        "00"));

    kth::chain::transaction tx;
    tx.from_data(raw_tx);

    REQUIRE(tx.is_valid());
    auto ret = first_keoken_output(tx);
    REQUIRE(ret.empty());
}

TEST_CASE("[test_get_keoken_output_non_empty]") {
    data_chunk raw_tx = to_chunk(base16_literal("0100000001bd56eab5f51d3d888f72c3e88187dc6cbd0b1abeefbe2348912619301a9e489f000000006b4830450221009a89bf0c34b87154fc4eb3e99a6e044ae21e76e244264645e8de4a747f6989dc02205d350d3113af2ce3cb013f4931c9f4c34d5925d9ffc76e56272befd9f47b521a412102bbfc0ef6f18b7594a930e2dd4e05bb90fbe7be60f58fbc8829f4fda9580af72dffffffff02606b042a010000001976a91456233da90fa320a56359161d02a9eed76b6157c088ac00000000000000001b6a0400004b5014000000014269747072696d0000000000000f424000000000"));

    kth::chain::transaction tx;
    tx.from_data(raw_tx);

    REQUIRE(tx.is_valid());
    auto ret = first_keoken_output(tx);
    REQUIRE(!ret.empty());
}

TEST_CASE("[test_get_keoken_output_create_asset_valid]") {
    data_chunk raw_tx = to_chunk(base16_literal("01000000016ef955ef813fd167438ef35d862d9dcb299672b22ccbc20da598f5ddc59d69aa000000006a473044022056f0511deaaf7485d7f17ec953ad7f6ede03a73c957f98629d290f890aee165602207f1f1a4c04eadeafcd3f4eacd0bb85a45803ef715bfc9a3375fed472212b67fb4121036735a1fe1b39fbe39e629a6dd680bf00b13aefe40d9f3bb6f863d2c4094ddd0effffffff02a007052a010000001976a9140ef6dfde07323619edd2440ca0a54d311df1ee8b88ac00000000000000001b6a0400004b5014000000004269747072696d0000000000000f424000000000"));

    kth::chain::transaction tx;
    tx.from_data(raw_tx);

    REQUIRE(tx.is_valid());
    auto ret = first_keoken_output(tx);
    REQUIRE(!ret.empty());

    data_source ds(ret);
    istream_reader source(ds);

    auto version = source.read_2_bytes_big_endian();
    REQUIRE(version == 0);

    auto type = source.read_2_bytes_big_endian();
    REQUIRE(type == 0);

    auto msg = message::create_asset::factory_from_data(source);
    auto name = msg.name();
    REQUIRE(name == "Knuth");

    auto amount = msg.amount();
    REQUIRE(amount == 1000000);
}

TEST_CASE("[test_get_keoken_output_send_tokens_valid]") {
    data_chunk raw_tx = to_chunk(base16_literal("01000000011e572671f2cff67190785b52e72dc221b1c3a092159b70ec14bc2f433c4dcb2f000000006b48304502210084c05aa0d2a60f69045b46179cff207fde8003ea07a90a75d934ec35d6a46a3a02205b328724e736d9400b3f13ac6e0e49462048dfc2c9a7bd1be9944aa9baa455144121036735a1fe1b39fbe39e629a6dd680bf00b13aefe40d9f3bb6f863d2c4094ddd0effffffff03204e0000000000001976a914071ed73aa65c19f86c88a29a789210fafc8d675188ac606b042a010000001976a9140ef6dfde07323619edd2440ca0a54d311df1ee8b88ac0000000000000000176a0400004b50100000000100000002000000000000006400000000"));

    kth::chain::transaction tx;
    tx.from_data(raw_tx);

    REQUIRE(tx.is_valid());
    auto ret = first_keoken_output(tx);
    REQUIRE(!ret.empty());

    data_source ds(ret);
    istream_reader source(ds);

    auto version = source.read_2_bytes_big_endian();
    REQUIRE(version == 0);

    auto type = source.read_2_bytes_big_endian();
    REQUIRE(type == 1);

    auto msg = message::send_tokens::factory_from_data(source);
    auto name = msg.asset_id();
    REQUIRE(name == 2);

    auto amount = msg.amount();
    REQUIRE(amount == 100);
}

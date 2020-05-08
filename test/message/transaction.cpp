// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

#include <kth/infrastructure/utility/ostream_writer.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::message;

// Start Boost Suite: message transaction tests

TEST_CASE("message transaction  constructor 1  always  initialized invalid", "[message transaction]") {
    transaction instance;
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("message transaction  constructor 2  always  equals transaction", "[message transaction]") {
    data_chunk raw_tx = to_chunk(base16_literal(
        "0100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff63294"
        "789eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e"
        "2830b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b"
        "12c517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c467000000"
        "001976a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f00"
        "0000001976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000"
        "00"));

    chain::transaction tx;
    REQUIRE(entity_from_data(tx, raw_tx));
    transaction instance(tx);
    REQUIRE(instance.is_valid());
    REQUIRE(instance == tx);
}

TEST_CASE("message transaction  constructor 3  always  equals param", "[message transaction]") {
    data_chunk raw_tx = to_chunk(base16_literal(
        "0100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff63294"
        "789eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e"
        "2830b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b"
        "12c517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c467000000"
        "001976a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f00"
        "0000001976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000"
        "00"));

    chain::transaction tx;
    REQUIRE(entity_from_data(tx, raw_tx));
    transaction alpha(tx);
    REQUIRE(alpha.is_valid());
    REQUIRE(alpha == tx);

    transaction beta(alpha);
    REQUIRE(beta.is_valid());
    REQUIRE(beta == alpha);
}

TEST_CASE("message transaction  constructor 4  always  equals equivalent tx", "[message transaction]") {
    data_chunk raw_tx = to_chunk(base16_literal(
        "0100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff63294"
        "789eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e"
        "2830b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b"
        "12c517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c467000000"
        "001976a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f00"
        "0000001976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000"
        "00"));

    chain::transaction tx;
    REQUIRE(entity_from_data(tx, raw_tx));
    auto const inputs = tx.inputs();
    auto const outputs = tx.outputs();
    transaction instance(tx.version(), tx.locktime(), inputs, outputs);
    REQUIRE(instance.is_valid());
    REQUIRE(instance == tx);
}

TEST_CASE("message transaction  constructor 5  always  equals equivalent tx", "[message transaction]") {
    data_chunk raw_tx = to_chunk(base16_literal(
        "0100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff63294"
        "789eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e"
        "2830b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b"
        "12c517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c467000000"
        "001976a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f00"
        "0000001976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000"
        "00"));

    chain::transaction tx;
    REQUIRE(entity_from_data(tx, raw_tx));
    transaction instance(create<chain::transaction>(raw_tx));
    REQUIRE(instance.is_valid());
    REQUIRE(instance == tx);
}

TEST_CASE("message transaction  constructor 6  always  equals equivalent tx", "[message transaction]") {
    data_chunk raw_tx = to_chunk(base16_literal(
        "0100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff63294"
        "789eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e"
        "2830b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b"
        "12c517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c467000000"
        "001976a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f00"
        "0000001976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000"
        "00"));

    transaction value = create<transaction>(
        transaction::version_minimum, raw_tx);

    chain::transaction tx;
    REQUIRE(entity_from_data(tx, raw_tx));
    transaction instance(std::move(value));
    REQUIRE(instance.is_valid());
    REQUIRE(instance == tx);
}

TEST_CASE("message transaction  constructor 7  always  equals equivalent tx", "[message transaction]") {
    transaction instance(15u, 1234u, chain::input::list{{}, {}}, chain::output::list{{}, {}, {}});
    REQUIRE(instance.is_valid());
    REQUIRE(instance.version() == 15u);
    REQUIRE(instance.locktime() == 1234u);
    REQUIRE(instance.inputs().size() == 2);
    REQUIRE(instance.outputs().size() == 3);
}

TEST_CASE("message transaction  from data  insufficient data  failure", "[message transaction]") {
    data_chunk data(2);
    transaction instance;
    REQUIRE( ! entity_from_data(instance, version::level::minimum, data));
    REQUIRE( ! instance.is_valid());
}

TEST_CASE("message transaction  from data  valid junk  success", "[message transaction]") {
    auto junk = base16_literal(
        "000000000000005739943a9c29a1955dfae2b3f37de547005bfb9535192e5fb0"
        "000000000000005739943a9c29a1955dfae2b3f37de547005bfb9535192e5fb0");

    // data_chunk_stream_host host(junk);
    byte_source<std::array<uint8_t, 64>> source(junk);
    boost::iostreams::stream<byte_source<std::array<uint8_t, 64>>> stream(source);

    transaction tx;
    REQUIRE(entity_from_data(tx, version::level::minimum, stream));
}

TEST_CASE("message transaction  factory from data 1  case 1 valid data  success", "[message transaction]") {
    hash_digest tx_hash = hash_literal(
        "bf7c3f5a69a78edd81f3eff7e93a37fb2d7da394d48db4d85e7e5353b9b8e270");
    data_chunk raw_tx = to_chunk(base16_literal(
        "0100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff63294"
        "789eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e"
        "2830b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b"
        "12c517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c467000000"
        "001976a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f00"
        "0000001976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000"
        "00"));
    REQUIRE(raw_tx.size() == 225u);

    auto const tx = create<transaction>(version::level::minimum, raw_tx);
    REQUIRE(tx.is_valid());
    REQUIRE(tx.serialized_size(version::level::minimum) == 225u);
    REQUIRE(tx.hash() == tx_hash);

    // Re-save tx and compare against original.
    REQUIRE(tx.serialized_size(version::level::minimum) == raw_tx.size());
    data_chunk resave = tx.to_data(version::level::minimum);
    REQUIRE(resave == raw_tx);
}

TEST_CASE("message transaction  factory from data 1  case 2 valid data  success", "[message transaction]") {
    hash_digest tx_hash = hash_literal(
        "8a6d9302fbe24f0ec756a94ecfc837eaffe16c43d1e68c62dfe980d99eea556f");
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));
    REQUIRE(raw_tx.size() == 523u);

    auto const tx = create<transaction>(version::level::minimum, raw_tx);
    REQUIRE(tx.is_valid());
    REQUIRE(tx.hash() == tx_hash);

    // Re-save tx and compare against original.
    REQUIRE(tx.serialized_size(version::level::minimum) == raw_tx.size());
    data_chunk resave = tx.to_data(version::level::minimum);
    REQUIRE(resave == raw_tx);
}

TEST_CASE("message transaction  factory from data 2  case 1 valid data  success", "[message transaction]") {
    hash_digest tx_hash = hash_literal(
        "bf7c3f5a69a78edd81f3eff7e93a37fb2d7da394d48db4d85e7e5353b9b8e270");
    data_chunk raw_tx = to_chunk(base16_literal(
        "0100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff63294"
        "789eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e"
        "2830b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b"
        "12c517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c467000000"
        "001976a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f00"
        "0000001976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000"
        "00"));
    REQUIRE(raw_tx.size() == 225u);

    data_source stream(raw_tx);
    auto const tx = create<transaction>(version::level::minimum, stream);
    REQUIRE(tx.is_valid());
    REQUIRE(tx.serialized_size(version::level::minimum) == 225u);
    REQUIRE(tx.hash() == tx_hash);

    // Re-save tx and compare against original.
    REQUIRE(tx.serialized_size(version::level::minimum) == raw_tx.size());
    data_chunk resave;
    data_sink ostream(resave);
    tx.to_data(version::level::minimum, ostream);
    ostream.flush();
    REQUIRE(resave == raw_tx);
}

TEST_CASE("message transaction  factory from data 2  case 2 valid data  success", "[message transaction]") {
    hash_digest tx_hash = hash_literal(
        "8a6d9302fbe24f0ec756a94ecfc837eaffe16c43d1e68c62dfe980d99eea556f");
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));
    BOOST_REQUIRE_EQUAL(raw_tx.size(), 523u);

    data_source stream(raw_tx);
    auto const tx = create<transaction>(version::level::minimum, stream);
    BOOST_REQUIRE(tx.is_valid());
    BOOST_REQUIRE(tx.hash() == tx_hash);

    // Re-save tx and compare against original.
    BOOST_REQUIRE(tx.serialized_size(version::level::minimum) == raw_tx.size());
    data_chunk resave;
    data_sink ostream(resave);
    tx.to_data(version::level::minimum, ostream);
    ostream.flush();
    BOOST_REQUIRE(resave == raw_tx);
}

BOOST_AUTO_TEST_CASE(transaction__factory_from_data_3__case_1_valid_data__success) {
    hash_digest tx_hash = hash_literal(
        "bf7c3f5a69a78edd81f3eff7e93a37fb2d7da394d48db4d85e7e5353b9b8e270");
    data_chunk raw_tx = to_chunk(base16_literal(
        "0100000001f08e44a96bfb5ae63eda1a6620adae37ee37ee4777fb0336e1bbbc"
        "4de65310fc010000006a473044022050d8368cacf9bf1b8fb1f7cfd9aff63294"
        "789eb1760139e7ef41f083726dadc4022067796354aba8f2e02363c5e510aa7e"
        "2830b115472fb31de67d16972867f13945012103e589480b2f746381fca01a9b"
        "12c517b7a482a203c8b2742985da0ac72cc078f2ffffffff02f0c9c467000000"
        "001976a914d9d78e26df4e4601cf9b26d09c7b280ee764469f88ac80c4600f00"
        "0000001976a9141ee32412020a324b93b1a1acfdfff6ab9ca8fac288ac000000"
        "00"));
    BOOST_REQUIRE_EQUAL(raw_tx.size(), 225u);

    data_source stream(raw_tx);
    istream_reader source(stream);
    auto const tx = create<transaction>(version::level::minimum, source);
    BOOST_REQUIRE(tx.is_valid());
    BOOST_REQUIRE_EQUAL(tx.serialized_size(version::level::minimum), 225u);
    BOOST_REQUIRE(tx.hash() == tx_hash);

    // Re-save tx and compare against original.
    BOOST_REQUIRE_EQUAL(tx.serialized_size(version::level::minimum), raw_tx.size());
    data_chunk resave;
    data_sink ostream(resave);
    ostream_writer sink(ostream);
    tx.to_data(version::level::minimum, sink);
    ostream.flush();
    BOOST_REQUIRE(resave == raw_tx);
}

BOOST_AUTO_TEST_CASE(transaction__factory_from_data_3__case_2_valid_data__success) {
    hash_digest tx_hash = hash_literal(
        "8a6d9302fbe24f0ec756a94ecfc837eaffe16c43d1e68c62dfe980d99eea556f");
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));
    BOOST_REQUIRE_EQUAL(raw_tx.size(), 523u);

    data_source stream(raw_tx);
    istream_reader source(stream);
    auto const tx = create<transaction>(version::level::minimum, source);
    BOOST_REQUIRE(tx.is_valid());
    BOOST_REQUIRE(tx.hash() == tx_hash);

    // Re-save tx and compare against original.
    BOOST_REQUIRE(tx.serialized_size(version::level::minimum) == raw_tx.size());
    data_chunk resave;
    data_sink ostream(resave);
    ostream_writer sink(ostream);
    tx.to_data(version::level::minimum, sink);
    ostream.flush();
    BOOST_REQUIRE(resave == raw_tx);
}

BOOST_AUTO_TEST_CASE(transaction__operator_assign_equals_1__always__matches_equivalent) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    chain::transaction expected;
    BOOST_REQUIRE(entity_from_data(expected, raw_tx));
    transaction instance;
    instance = create<chain::transaction>(raw_tx);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(transaction__operator_assign_equals_2__always__matches_equivalent) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    transaction expected;
    BOOST_REQUIRE(entity_from_data(expected, transaction::version_minimum, raw_tx));
    transaction instance;
    instance = create<transaction>(transaction::version_minimum, raw_tx);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(transaction__operator_boolean_equals_1__duplicates__returns_true) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    transaction alpha;
    chain::transaction beta;
    BOOST_REQUIRE(entity_from_data(alpha, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE(entity_from_data(beta, raw_tx));
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(transaction__operator_boolean_equals_1__differs__returns_false) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    transaction alpha;
    chain::transaction beta;
    BOOST_REQUIRE(entity_from_data(alpha, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE_EQUAL(false, alpha == beta);
}

BOOST_AUTO_TEST_CASE(transaction__operator_boolean_not_equals_1__duplicates__returns_false) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    transaction alpha;
    chain::transaction beta;
    BOOST_REQUIRE(entity_from_data(alpha, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE(entity_from_data(beta, raw_tx));
    BOOST_REQUIRE_EQUAL(false, alpha != beta);
}

BOOST_AUTO_TEST_CASE(transaction__operator_boolean_not_equals_1__differs__returns_true) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    transaction alpha;
    chain::transaction beta;
    BOOST_REQUIRE(entity_from_data(alpha, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE(alpha != beta);
}

BOOST_AUTO_TEST_CASE(transaction__operator_boolean_equals_2__duplicates__returns_true) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    transaction alpha;
    transaction beta;
    BOOST_REQUIRE(entity_from_data(alpha, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE(entity_from_data(beta, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(transaction__operator_boolean_equals_2__differs__returns_false) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    transaction alpha;
    transaction beta;
    BOOST_REQUIRE(entity_from_data(alpha, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE_EQUAL(false, alpha == beta);
}

BOOST_AUTO_TEST_CASE(transaction__operator_boolean_not_equals_2__duplicates__returns_false) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    transaction alpha;
    transaction beta;
    BOOST_REQUIRE(entity_from_data(alpha, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE(entity_from_data(beta, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE_EQUAL(false, alpha != beta);
}

BOOST_AUTO_TEST_CASE(transaction__operator_boolean_not_equals_2__differs__returns_true) {
    data_chunk raw_tx = to_chunk(base16_literal(
        "010000000364e62ad837f29617bafeae951776e7a6b3019b2da37827921548d1"
        "a5efcf9e5c010000006b48304502204df0dc9b7f61fbb2e4c8b0e09f3426d625"
        "a0191e56c48c338df3214555180eaf022100f21ac1f632201154f3c69e1eadb5"
        "9901a34c40f1127e96adc31fac6ae6b11fb4012103893d5a06201d5cf61400e9"
        "6fa4a7514fc12ab45166ace618d68b8066c9c585f9ffffffff54b755c39207d4"
        "43fd96a8d12c94446a1c6f66e39c95e894c23418d7501f681b010000006b4830"
        "4502203267910f55f2297360198fff57a3631be850965344370f732950b47795"
        "737875022100f7da90b82d24e6e957264b17d3e5042bab8946ee5fc676d15d91"
        "5da450151d36012103893d5a06201d5cf61400e96fa4a7514fc12ab45166ace6"
        "18d68b8066c9c585f9ffffffff0aa14d394a1f0eaf0c4496537f8ab9246d9663"
        "e26acb5f308fccc734b748cc9c010000006c493046022100d64ace8ec2d5feeb"
        "3e868e82b894202db8cb683c414d806b343d02b7ac679de7022100a2dcd39940"
        "dd28d4e22cce417a0829c1b516c471a3d64d11f2c5d754108bdc0b012103893d"
        "5a06201d5cf61400e96fa4a7514fc12ab45166ace618d68b8066c9c585f9ffff"
        "ffff02c0e1e400000000001976a914884c09d7e1f6420976c40e040c30b2b622"
        "10c3d488ac20300500000000001976a914905f933de850988603aafeeb2fd7fc"
        "e61e66fe5d88ac00000000"));

    transaction alpha;
    transaction beta;
    BOOST_REQUIRE(entity_from_data(alpha, transaction::version_minimum, raw_tx));
    BOOST_REQUIRE(alpha != beta);
}

BOOST_AUTO_TEST_SUITE_END()

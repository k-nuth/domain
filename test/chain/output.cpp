// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;

data_chunk valid_raw_output = to_chunk(base16_literal("20300500000000001976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac"));

// Start Test Suite: output tests

TEST_CASE("output deserialization with just FT amount 1", "[output]") {
    auto const data = to_chunk(base16_literal("2030050000000000" "efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb1001" "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac"));
    // (16 + 70 + 52) / 2 = 69

    chain::output instance;
    REQUIRE(entity_from_data(instance, data));

    REQUIRE(instance.is_valid());

    REQUIRE(instance.value() == 0x53020);

    REQUIRE(instance.token_data().has_value());
    REQUIRE(encode_base16(instance.token_data().value().id) == "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    REQUIRE(std::holds_alternative<chain::fungible>(instance.token_data().value().data));
    auto const& ft = std::get<chain::fungible>(instance.token_data().value().data);
    REQUIRE(ft.amount == chain::amount_t{0x01});

    REQUIRE(encode_base16(instance.script().to_data(true)) == "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac");
}

// peak_byte: 3d
// peak_byte: e8
// peak_byte: 50
// peak_byte: 98
// peak_byte: 86
// peak_byte: 3
// peak_byte: aa
// peak_byte: fe
// peak_byte: eb
// peak_byte: 2f
// peak_byte: d7
// peak_byte: fc
// peak_byte: e6
// peak_byte: 1e
// peak_byte: 66
// peak_byte: fe
// peak_byte: 5d
// peak_byte: 88
// peak_byte: ac
// peak_byte: ff
// peak_byte: ff
// peak_byte: ff
// peak_byte: ff
// peak_byte: ff
// peak_byte: ff
// peak_byte: ff


TEST_CASE("output deserialization with just FT amount 252", "[output]") {
    auto const data = to_chunk(base16_literal("2030050000000000" "efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb10fc" "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac"));

    chain::output instance;
    REQUIRE(entity_from_data(instance, data));

    REQUIRE(instance.is_valid());

    REQUIRE(instance.value() == 0x53020);

    REQUIRE(instance.token_data().has_value());
    REQUIRE(encode_base16(instance.token_data().value().id) == "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    REQUIRE(std::holds_alternative<chain::fungible>(instance.token_data().value().data));
    auto const& ft = std::get<chain::fungible>(instance.token_data().value().data);
    REQUIRE(ft.amount == chain::amount_t{252});

    REQUIRE(encode_base16(instance.script().to_data(true)) == "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac");
}

TEST_CASE("output deserialization with just FT amount 253", "[output]") {
    auto const data = to_chunk(base16_literal("2030050000000000" "efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb10fdfd00" "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac"));

    chain::output instance;
    REQUIRE(entity_from_data(instance, data));

    REQUIRE(instance.is_valid());

    REQUIRE(instance.value() == 0x53020);

    REQUIRE(instance.token_data().has_value());
    REQUIRE(encode_base16(instance.token_data().value().id) == "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    REQUIRE(std::holds_alternative<chain::fungible>(instance.token_data().value().data));
    auto const& ft = std::get<chain::fungible>(instance.token_data().value().data);
    REQUIRE(ft.amount == chain::amount_t{253});

    REQUIRE(encode_base16(instance.script().to_data(true)) == "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac");
}

TEST_CASE("output deserialization with just FT amount 9223372036854775807", "[output]") {
    auto const data = to_chunk(base16_literal("2030050000000000" "efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb10ffffffffffffffff7f" "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac"));

    chain::output instance;
    REQUIRE(entity_from_data(instance, data));

    REQUIRE(instance.is_valid());

    REQUIRE(instance.value() == 0x53020);

    REQUIRE(instance.token_data().has_value());
    REQUIRE(encode_base16(instance.token_data().value().id) == "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    REQUIRE(std::holds_alternative<chain::fungible>(instance.token_data().value().data));
    auto const& ft = std::get<chain::fungible>(instance.token_data().value().data);
    REQUIRE(ft.amount == chain::amount_t{9223372036854775807});

    REQUIRE(encode_base16(instance.script().to_data(true)) == "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac");
}

TEST_CASE("output deserialization with just immutable NFT 0-byte commitment", "[output]") {
    auto const data = to_chunk(base16_literal("2030050000000000" "efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb20" "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac"));

    chain::output instance;
    REQUIRE(entity_from_data(instance, data));

    REQUIRE(instance.is_valid());

    REQUIRE(instance.value() == 0x53020);

    REQUIRE(instance.token_data().has_value());
    REQUIRE(encode_base16(instance.token_data().value().id) == "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    REQUIRE(std::holds_alternative<chain::non_fungible>(instance.token_data().value().data));
    auto const& nft = std::get<chain::non_fungible>(instance.token_data().value().data);
    REQUIRE(nft.commitment.size() == 0);
    REQUIRE(nft.capability == chain::capability_t::none); // immutable

    REQUIRE(encode_base16(instance.script().to_data(true)) == "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac");
}

TEST_CASE("output deserialization with both - immutable NFT 0-byte commitment - FT amount 1", "[output]") {
    auto const data = to_chunk(base16_literal("2030050000000000" "efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb3001" "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac"));

    chain::output instance;
    REQUIRE(entity_from_data(instance, data));

    REQUIRE(instance.is_valid());

    REQUIRE(instance.value() == 0x53020);

    REQUIRE(instance.token_data().has_value());
    REQUIRE(encode_base16(instance.token_data().value().id) == "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    REQUIRE(std::holds_alternative<chain::both_kinds>(instance.token_data().value().data));
    auto const& ft = std::get<chain::both_kinds>(instance.token_data().value().data).first;
    auto const& nft = std::get<chain::both_kinds>(instance.token_data().value().data).second;
    REQUIRE(ft.amount == chain::amount_t{1});
    REQUIRE(nft.commitment.size() == 0);
    REQUIRE(nft.capability == chain::capability_t::none); // immutable

    REQUIRE(encode_base16(instance.script().to_data(true)) == "1976a914905f933de850988603aafeeb2fd7fce61e66fe5d88ac");
}

// 0-byte immutable NFT; 1 fungible
// 0-byte immutable NFT; 253 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb30fdfd00
// 0-byte immutable NFT; 9223372036854775807 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb30ffffffffffffffff7f
// 1-byte immutable NFT; 0 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb6001cc
// 1-byte immutable NFT; 252 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb7001ccfc
// 2-byte immutable NFT; 253 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb7002ccccfdfd00
// 10-byte immutable NFT; 65535 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb700accccccccccccccccccccfdffff
// 40-byte immutable NFT; 65536 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb7028ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccfe00000100
// 0-byte, mutable NFT; 0 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb21
// 0-byte, mutable NFT; 4294967295 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb31feffffffff
// 1-byte, mutable NFT; 0 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb6101cc
// 1-byte, mutable NFT; 4294967296 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb7101ccff0000000001000000
// 2-byte, mutable NFT; 9223372036854775807 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb7102ccccffffffffffffffff7f
// 10-byte, mutable NFT; 1 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb710acccccccccccccccccccc01
// 40-byte, mutable NFT; 252 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb7128ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccfc
// 0-byte, minting NFT; 0 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb22
// 0-byte, minting NFT; 253 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb32fdfd00
// 1-byte, minting NFT; 0 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb6201cc
// 1-byte, minting NFT; 65535 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb7201ccfdffff
// 2-byte, minting NFT; 65536 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb7202ccccfe00000100
// 10-byte, minting NFT; 4294967297 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb720accccccccccccccccccccff0100000001000000
// 40-byte, minting NFT; 9223372036854775807 fungible	efbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb7228ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccffffffffffffffff7f


// TEST_CASE("output constructor 1 always returns default initialized", "[output]") {
//     chain::output instance;
//     REQUIRE( ! instance.is_valid());
// }

// TEST_CASE("output constructor 2  valid input  returns input initialized", "[output]") {
//     uint64_t value = 643u;
//     chain::script script;
//     auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
//     REQUIRE(entity_from_data(script, data, false));

//     chain::output instance(value, script, chain::token_data_opt{});
//     REQUIRE(instance.is_valid());
//     REQUIRE(script == instance.script());
//     REQUIRE(value == instance.value());
//     REQUIRE( ! instance.token_data().has_value());
// }

// TEST_CASE("output constructor 3  valid input  returns input initialized", "[output]") {
//     uint64_t value = 643u;
//     chain::script script;
//     auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
//     REQUIRE(entity_from_data(script, data, false));

//     // This must be non-const.
//     auto dup_script = script;

//     chain::output instance(value, std::move(dup_script), chain::token_data_opt{});

//     REQUIRE(instance.is_valid());
//     REQUIRE(script == instance.script());
//     REQUIRE(value == instance.value());
//     REQUIRE( ! instance.token_data().has_value());
// }

// TEST_CASE("output constructor 4  valid input  returns input initialized", "[output]") {
//     chain::output expected;
//     REQUIRE(entity_from_data(expected, valid_raw_output));

//     chain::output instance(expected);
//     REQUIRE(instance.is_valid());
//     REQUIRE(expected == instance);
//     REQUIRE(instance.value() == 0x53020);
//     REQUIRE( ! instance.token_data().has_value());
// }

// TEST_CASE("output constructor 5  valid input  returns input initialized", "[output]") {
//     // This must be non-const.
//     chain::output expected;
//     REQUIRE(entity_from_data(expected, valid_raw_output));

//     chain::output instance(std::move(expected));
//     REQUIRE(instance.is_valid());
//     REQUIRE( ! instance.token_data().has_value());
// }

// TEST_CASE("output from data  insufficient bytes  failure", "[output]") {
//     data_chunk data(2);

//     chain::output instance;

//     REQUIRE( ! entity_from_data(instance, data));
//     REQUIRE( ! instance.is_valid());
// }

// TEST_CASE("output factory from data 1  valid input success", "[output]") {
//     auto instance = create<chain::output>(valid_raw_output);
//     REQUIRE(instance.is_valid());
//     REQUIRE(instance.serialized_size() == valid_raw_output.size());
//     REQUIRE( ! instance.token_data().has_value());

//     // Re-save and compare against original.
//     data_chunk resave = instance.to_data();
//     REQUIRE(resave.size() == valid_raw_output.size());
//     REQUIRE(resave == valid_raw_output);
// }

// TEST_CASE("output factory from data 2  valid input success", "[output]") {
//     data_source stream(valid_raw_output);
//     auto instance = create<chain::output>(stream);
//     REQUIRE(instance.is_valid());
//     REQUIRE(instance.serialized_size() == valid_raw_output.size());
//     REQUIRE( ! instance.token_data().has_value());

//     // Re-save and compare against original.
//     data_chunk resave = instance.to_data();
//     REQUIRE(resave.size() == valid_raw_output.size());
//     REQUIRE(resave == valid_raw_output);
// }

// TEST_CASE("output factory from data 3  valid input success", "[output]") {
//     data_source stream(valid_raw_output);
//     istream_reader source(stream);
//     auto instance = create<chain::output>(source);
//     REQUIRE(instance.is_valid());
//     REQUIRE(instance.serialized_size() == valid_raw_output.size());
//     REQUIRE( ! instance.token_data().has_value());

//     // Re-save and compare against original.
//     data_chunk resave = instance.to_data();
//     REQUIRE(resave.size() == valid_raw_output.size());
//     REQUIRE(resave == valid_raw_output);
// }

// TEST_CASE("output signature operations  always  returns script sigops false", "[output]") {
//     chain::output instance;
//     REQUIRE(instance.script().sigops(false) == instance.signature_operations(false));
//     REQUIRE( ! instance.token_data().has_value());
// }

// TEST_CASE("output value  roundtrip  success", "[output]") {
//     uint64_t expected = 523542u;
//     chain::output instance;
//     REQUIRE(expected != instance.value());
//     REQUIRE( ! instance.token_data().has_value());
//     instance.set_value(expected);
//     REQUIRE(expected == instance.value());
//     REQUIRE( ! instance.token_data().has_value());
// }

// TEST_CASE("output script setter 1  roundtrip  success", "[output]") {
//     chain::script value;
//     auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
//     REQUIRE(entity_from_data(value, data, false));

//     chain::output instance;
//     REQUIRE(value != instance.script());
//     REQUIRE( ! instance.token_data().has_value());
//     instance.set_script(value);
//     REQUIRE(value == instance.script());
//     REQUIRE( ! instance.token_data().has_value());
//     auto const& restricted = instance;
//     REQUIRE(value == instance.script());
// }

// TEST_CASE("output script setter 2  roundtrip  success", "[output]") {
//     chain::script value;
//     auto const data = to_chunk(base16_literal("ece424a6bb6ddf4db592c0faed60685047a361b1"));
//     REQUIRE(entity_from_data(value, data, false));

//     // This must be non-const.
//     auto dup_value = value;

//     chain::output instance;
//     REQUIRE(value != instance.script());
//     REQUIRE( ! instance.token_data().has_value());
//     instance.set_script(std::move(dup_value));
//     REQUIRE(value == instance.script());
//     REQUIRE( ! instance.token_data().has_value());
//     auto const& restricted = instance;
//     REQUIRE(value == instance.script());
// }

// TEST_CASE("output operator assign equals 1  always  matches equivalent", "[output]") {
//     chain::output expected;
//     REQUIRE(entity_from_data(expected, valid_raw_output));
//     chain::output instance;
//     instance = create<chain::output>(valid_raw_output);
//     REQUIRE(instance == expected);
// }

// TEST_CASE("output operator assign equals 2  always  matches equivalent", "[output]") {
//     chain::output expected;
//     REQUIRE(entity_from_data(expected, valid_raw_output));
//     chain::output instance;
//     instance = expected;
//     REQUIRE(instance == expected);
// }

// TEST_CASE("output operator boolean equals  duplicates  returns true", "[output]") {
//     chain::output alpha;
//     chain::output beta;
//     REQUIRE(entity_from_data(alpha, valid_raw_output));
//     REQUIRE(entity_from_data(beta, valid_raw_output));
//     REQUIRE(alpha == beta);
// }

// TEST_CASE("output operator boolean equals  differs  returns false", "[output]") {
//     chain::output alpha;
//     chain::output beta;
//     REQUIRE(entity_from_data(alpha, valid_raw_output));
//     REQUIRE(alpha != beta);
// }

// TEST_CASE("output operator boolean not equals  duplicates  returns false", "[output]") {
//     chain::output alpha;
//     chain::output beta;
//     REQUIRE(entity_from_data(alpha, valid_raw_output));
//     REQUIRE(entity_from_data(beta, valid_raw_output));
//     REQUIRE(alpha == beta);
// }

// TEST_CASE("output operator boolean not equals  differs  returns true", "[output]") {
//     chain::output alpha;
//     chain::output beta;
//     REQUIRE(entity_from_data(alpha, valid_raw_output));
//     REQUIRE(alpha != beta);
// }

// End Test Suite

// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// #include <kth/domain/multi_crypto_support.hpp>
#include <test_helpers.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::chain;
using namespace kth::domain::wallet;

// Start Boost Suite: payment address tests

// $ bx base16-encode "Satoshi" | bx sha256
#define SECRET "002688cc350a5333a87fa622eacec626c3d1c0ebf9f3793de3885fa254d7e393"
#define SCRIPT "dup hash160 [18c0bd8d1818f1bf99cb1df2269c645318ef7b73] equalverify checksig"

// $ bx base16-encode "Satoshi" | bx sha256 | bx ec-to-public
#define COMPRESSED "03d24123978d696a6c964f2dcb1d1e000d4150102fbbcc37f020401e35fb4cb745"
// $ bx base16-encode "Satoshi" | bx sha256 | bx ec-to-public -u
#define UNCOMPRESSED "04d24123978d696a6c964f2dcb1d1e000d4150102fbbcc37f020401e35fb4cb74561a3362716303b0469f04c3d0e3cbc4b5b62a2da7add6ecc3b254404b12d2f83"

// $ bx base16-encode "Satoshi" | bx sha256 | bx ec-to-public | bx bitcoin160
#define COMPRESSED_HASH "f85beb6356d0813ddb0dbb14230a249fe931a135"
// $ bx base16-encode "Satoshi" | bx sha256 | bx ec-to-public -u | bx bitcoin160
#define UNCOMPRESSED_HASH "96ec4e06c665b7bd62cbe3d232f7c2d34016e136"

// $ bx base16-encode "Satoshi" | bx sha256 | bx ec-to-public | bx ec-to-address
#define ADDRESS_COMPRESSED "1PeChFbhxDD9NLbU21DfD55aQBC4ZTR3tE"
// $ bx base16-encode "Satoshi" | bx sha256 | bx ec-to-public -u | bx ec-to-address
#define ADDRESS_UNCOMPRESSED "1Em1SX7qQq1pTmByqLRafhL1ypx2V786tP"

// $ bx base16-encode "Satoshi" | bx sha256 | bx ec-to-public | bx ec-to-address -v 111
#define ADDRESS_COMPRESSED_TESTNET "n4A9zJggmEeQ9T55jaC32zHuGAnmSzPU2L"
// $ bx script-encode "dup hash160 [18c0bd8d1818f1bf99cb1df2269c645318ef7b73] equalverify checksig"
#define ADDRESS_UNCOMPRESSED_TESTNET "muGxjaCpDrT5EsfbYuPxVcYLqpYjNQnbkR"

// $ bx script-to-address "dup hash160 [18c0bd8d1818f1bf99cb1df2269c645318ef7b73] equalverify checksig"
#define ADDRESS_SCRIPT "3CPSWnCGjkePffNyVptkv45Bx35SaAwm7d"
// $ bx script-to-address "dup hash160 [18c0bd8d1818f1bf99cb1df2269c645318ef7b73] equalverify checksig" -v 196
#define ADDRESS_SCRIPT_TESTNET "2N3weaX8JMD9jsT1XAxWdY14TAPHcKYKHCT"

// $ bx script-to-address "dup hash160 [18c0bd8d1818f1bf99cb1df2269c645318ef7b73] equalverify checksig" | bx base58-decode
#define PAYMENT "0575566c599452b7bcb7f8cd4087bde9686fa9c52d8c2a7d90"
// $ bx script-to-address "dup hash160 [18c0bd8d1818f1bf99cb1df2269c645318ef7b73] equalverify checksig" -v 196 | bx base58-decode
#define PAYMENT_TESTNET "c475566c599452b7bcb7f8cd4087bde9686fa9c52d2fba2898"

// $ bx base58-decode 1111111111111111111114oLvT2 | bx wrap-decode
// wrapper
// {
//     checksum 285843604
//     payload 0000000000000000000000000000000000000000
//     version 0
// }
#define UNINITIALIZED_ADDRESS "1111111111111111111114oLvT2"

// negative tests:

TEST_CASE("payment address  construct  default invalid", "[payment address]") {
    const payment_address address;
    REQUIRE( ! address);
    REQUIRE(address.encoded() == UNINITIALIZED_ADDRESS);
}

TEST_CASE("payment address  construct  string invalid invalid", "[payment address]") {
    const payment_address address("bogus");
    REQUIRE( ! address);
    REQUIRE(address.encoded() == UNINITIALIZED_ADDRESS);
}

// construct secret:

TEST_CASE("payment address  construct  secret  valid expected", "[payment address]") {
    ec_secret secret;
    REQUIRE(decode_base16(secret, SECRET));
    const payment_address address(secret);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_COMPRESSED);
}

TEST_CASE("payment address  construct  secret testnet  valid expected", "[payment address]") {
    ec_secret secret;
    REQUIRE(decode_base16(secret, SECRET));

    // MSVC CTP loses the MSB (WIF prefix) byte of the literal version when
    // using this initializer, but the MSB isn't used by payment_address.
    const payment_address address({secret, 0x806f});
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_COMPRESSED_TESTNET);
}

TEST_CASE("payment address  construct  secret mainnet uncompressed  valid expected", "[payment address]") {
    ec_secret secret;
    REQUIRE(decode_base16(secret, SECRET));
    const payment_address address({secret, payment_address::mainnet_p2kh, false});
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_UNCOMPRESSED);
}

TEST_CASE("payment address  construct  secret testnet uncompressed  valid expected", "[payment address]") {
    ec_secret secret;
    REQUIRE(decode_base16(secret, SECRET));

    // MSVC CTP loses the MSB (WIF prefix) byte of the literal version when
    // using this initializer, but the MSB isn't used by payment_address.
    const payment_address address({secret, 0x806f, false});
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_UNCOMPRESSED_TESTNET);
}

// construct public:

TEST_CASE("payment address  construct  public  valid expected", "[payment address]") {
    const payment_address address(ec_public(COMPRESSED));
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_COMPRESSED);
}

TEST_CASE("payment address  construct  public testnet  valid expected", "[payment address]") {
    const payment_address address(ec_public(COMPRESSED), 0x6f);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_COMPRESSED_TESTNET);
}

TEST_CASE("payment address  construct  public uncompressed  valid expected", "[payment address]") {
    const payment_address address(ec_public(UNCOMPRESSED));
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_UNCOMPRESSED);
}

TEST_CASE("payment address  construct  public testnet uncompressed  valid expected", "[payment address]") {
    const payment_address address(ec_public(UNCOMPRESSED), 0x6f);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_UNCOMPRESSED_TESTNET);
}

TEST_CASE("payment address  construct  public compressed from uncompressed testnet  valid expected", "[payment address]") {
    ec_uncompressed point;
    REQUIRE(decode_base16(point, UNCOMPRESSED));
    const payment_address address({point, true}, 0x6f);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_COMPRESSED_TESTNET);
}

TEST_CASE("payment address  construct  public uncompressed from compressed testnet  valid expected", "[payment address]") {
    ec_compressed point;
    REQUIRE(decode_base16(point, COMPRESSED));
    const payment_address address({point, false}, 0x6f);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_UNCOMPRESSED_TESTNET);
}

// construct hash:

TEST_CASE("payment address  construct  hash  valid expected", "[payment address]") {
    short_hash hash;
    REQUIRE(decode_base16(hash, COMPRESSED_HASH));
    const payment_address address(hash);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_COMPRESSED);
}

TEST_CASE("payment address  construct  uncompressed hash testnet  valid expected", "[payment address]") {
    short_hash hash;
    REQUIRE(decode_base16(hash, UNCOMPRESSED_HASH));
    const payment_address address(hash, 0x6f);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_UNCOMPRESSED_TESTNET);
}

// construct script:

TEST_CASE("payment address  construct  script  valid expected", "[payment address]") {
    script ops;
    REQUIRE(ops.from_string(SCRIPT));
    const payment_address address(ops);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_SCRIPT);
}

TEST_CASE("payment address  construct  script testnet  valid expected", "[payment address]") {
    script ops;
    REQUIRE(ops.from_string(SCRIPT));
    const payment_address address(ops, 0xc4);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_SCRIPT_TESTNET);
}

// construct payment:

TEST_CASE("payment address  construct  payment  valid expected", "[payment address]") {
    payment pay;
    REQUIRE(decode_base16(pay, PAYMENT));
    const payment_address address(pay);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_SCRIPT);
}

TEST_CASE("payment address  construct  payment testnet  valid expected", "[payment address]") {
    payment pay;
    REQUIRE(decode_base16(pay, PAYMENT_TESTNET));
    const payment_address address(pay);
    REQUIRE(address);
    REQUIRE(address.encoded() == ADDRESS_SCRIPT_TESTNET);
}

// construct copy:

TEST_CASE("payment address  construct  copy  valid expected", "[payment address]") {
    payment pay;
    REQUIRE(decode_base16(pay, PAYMENT));
    const payment_address address(pay);
    const payment_address copy(address);
    REQUIRE(copy);
    REQUIRE(copy.encoded() == ADDRESS_SCRIPT);
}

// version property:

TEST_CASE("payment address  version  default  mainnet", "[payment address]") {
    const payment_address address(ec_public(COMPRESSED));
    REQUIRE(address.version() == payment_address::mainnet_p2kh);
}

TEST_CASE("payment address  version  testnet  testnet", "[payment address]") {
    uint8_t const testnet = 0x6f;
    const payment_address address(ec_public(COMPRESSED), testnet);
    REQUIRE(address);
    REQUIRE(address.version() == testnet);
}

TEST_CASE("payment address  version  script  valid mainnet p2sh", "[payment address]") {
    script ops;
    REQUIRE(ops.from_string(SCRIPT));
    const payment_address address(ops);
    REQUIRE(address);
    REQUIRE(address.version() == payment_address::mainnet_p2sh);
}

// hash property:

TEST_CASE("payment address  hash  compressed point  expected", "[payment address]") {
    const payment_address address(ec_public(COMPRESSED));
    REQUIRE(address);
    REQUIRE(encode_base16(address.hash()) == COMPRESSED_HASH);
}

#if defined(KTH_CURRENCY_BCH)
//cashAddr payment_address
TEST_CASE("payment address  cashAddr  mainnet  encode", "[payment address]") {
    const payment_address address(ec_public("04278f7bfee4ef625f85279c3a01d57c22e2877a902128b2df85071f9d6c95b290f094f5bd1bff5880d09cc231c774d71ac22d3ab9bdd9dda2e75017b52d893367"),
                                  kth::domain::wallet::payment_address::mainnet_p2kh);
    REQUIRE(address);
    REQUIRE(address.encoded_cashaddr() == "bitcoincash:qpzz8n7jp6847yyx8t33matrgcsdx6c0cvmtevrfgz");
}

TEST_CASE("payment address  cashAddr  testnet  encode", "[payment address]") {
    const payment_address address(ec_public("04278f7bfee4ef625f85279c3a01d57c22e2877a902128b2df85071f9d6c95b290f094f5bd1bff5880d09cc231c774d71ac22d3ab9bdd9dda2e75017b52d893367"),
                                  kth::domain::wallet::payment_address::testnet_p2kh);
    REQUIRE(address);
    REQUIRE(address.encoded_cashaddr() == "bchtest:qpzz8n7jp6847yyx8t33matrgcsdx6c0cvleatp707");
}

TEST_CASE("payment address  cashAddr  mainnet  from string", "[payment address]") {
    const payment_address address("bitcoincash:qpzz8n7jp6847yyx8t33matrgcsdx6c0cvmtevrfgz");
    REQUIRE(address);
    REQUIRE(address.encoded() == "17DHrHvtmMRs9ciersFCPNhvJtryd5NWbT");
}

TEST_CASE("payment address  cashAddr  testnet  from string", "[payment address]") {
    set_cashaddr_prefix("bchtest");
    const payment_address address("bchtest:qpzz8n7jp6847yyx8t33matrgcsdx6c0cvleatp707");
    REQUIRE(address);
    REQUIRE(address.encoded() == "mmjF9M1saNs7vjCGaSDaDHvFAtTgUNtfrJ");
}
#endif

// End Boost Suite

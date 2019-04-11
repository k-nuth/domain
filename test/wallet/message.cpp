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
#include <bitcoin/bitcoin.hpp>
#include <boost/test/unit_test.hpp>

using namespace bc;
using namespace bc::wallet;

// $ bx base16-encode "Satoshi" | bx sha256
#define SECRET "002688cc350a5333a87fa622eacec626c3d1c0ebf9f3793de3885fa254d7e393"
// #define SIGNATURE_COMPRESSED "20c0ae26619db18abd1e8a84d005bafd336512eda7207cf7f4f6c36c9614ed6bcf531a954929ddc0a86578f4d28a26e19b676c890a49881d6f25e393befd6d1682"
// #define SIGNATURE_UNCOMPRESSED "1c3484d71301fbdd9eec713894add25867663d9a91d637682f09179a211d16a1f26068178de890a0117df61c436e9062f87ae1790579829caae2911833ba9e35b0"
#define SIGNATURE_COMPRESSED "1fa7b234dc5168e6ee3cab90c9cb5517dc851d18d5ac6e0b183daa9ed4aec8a1e752982ff04d0dce0f80d1d20ef68d31aeb000aca1313b9ca72c0acb1cea555ea4"
#define SIGNATURE_UNCOMPRESSED "1c4f4780ab587f0a9011e2f5543acfb8628710a5c1edc6937f62a524bdbea65a9462e8f8ea2767b917382182d5fa498c2790b18bc4dbd2579dcc41703c880f6c3b"

// WIF keys also used in WIF test vectors.
#define WIF_COMPRESSED "L1WepftUBemj6H4XQovkiW1ARVjxMqaw4oj2kmkYqdG1xTnBcHfC"
#define WIF_UNCOMPRESSED "5JngqQmHagNTknnCshzVUysLMWAjT23FWs1TgNU5wyFH5SB3hrP"
// #define SIGNATURE_WIF_COMPRESSED "20813288c5d9e3a56a297758df28bec5ffe4ceb107ac66f1d215c156ecb7845ca65efb7a84a5267edc77538479ccb01efdb006837d35e246b2cacae22acb4c6e46"
// #define SIGNATURE_WIF_UNCOMPRESSED "1b25c35d61aa2ff5353efc36d747ed3ef179bc0f5b3d1c60f0617006c9015a0d8271da05103b987d1c26a2ecb053a56ce885805cbacefa230e69bfe18727ea4a04"
#define SIGNATURE_WIF_COMPRESSED "1f667668be93a0be1f3cbb086a39326f3c82b2e6ef3fb98e370fe499e3a822111d23d82f01a4a6b1b9c6cfb9b2f01c4dde5e8a16b19b0be7d7452b6f8b29bc095d"
#define SIGNATURE_WIF_UNCOMPRESSED "1b1b6c97843dcea3f7b5557492c4da6bad50917be4d8e2cbde47e580e95abaff653f71ac8b796705811fc0e7f74492c3614f6ec0797146607fda4ce4252627c738"

// Generated using Electrum and above SECRET (compressed):
#define ELECTRUM_SIGNATURE "1f1429ddc5e03888411065e4b36eec7de4901d580d51e6209798b9c06fdd39461a4884679f35d1e8d7321fe01f3401ed916732383f6b5f8a688ea9ae4321fbf4ae"

BOOST_AUTO_TEST_SUITE(message_tests)

BOOST_AUTO_TEST_SUITE(message__recovery_magic)

BOOST_AUTO_TEST_CASE(message__recovery_id_to_magic__uncompressed_valid__expected) {
    uint8_t out_magic;
    BOOST_REQUIRE(recovery_id_to_magic(out_magic, 0, false));
    BOOST_REQUIRE_EQUAL(out_magic, 0x1b);
    BOOST_REQUIRE(recovery_id_to_magic(out_magic, 1, false));
    BOOST_REQUIRE_EQUAL(out_magic, 0x1c);
    BOOST_REQUIRE(recovery_id_to_magic(out_magic, 2, false));
    BOOST_REQUIRE_EQUAL(out_magic, 0x1d);
    BOOST_REQUIRE(recovery_id_to_magic(out_magic, 3, false));
    BOOST_REQUIRE_EQUAL(out_magic, 0x1e);
}

BOOST_AUTO_TEST_CASE(message__recovery_id_to_magic__compressed_valid__expected) {
    uint8_t out_magic;
    BOOST_REQUIRE(recovery_id_to_magic(out_magic, 0, true));
    BOOST_REQUIRE_EQUAL(out_magic, 0x1f);
    BOOST_REQUIRE(recovery_id_to_magic(out_magic, 1, true));
    BOOST_REQUIRE_EQUAL(out_magic, 0x20);
    BOOST_REQUIRE(recovery_id_to_magic(out_magic, 2, true));
    BOOST_REQUIRE_EQUAL(out_magic, 0x21);
    BOOST_REQUIRE(recovery_id_to_magic(out_magic, 3, true));
    BOOST_REQUIRE_EQUAL(out_magic, 0x22);
}

BOOST_AUTO_TEST_CASE(message__magic_to_recovery_id__uncompressed__expected) {
    bool out_compressed = true;
    uint8_t out_recovery_id = 0xff;
    BOOST_REQUIRE(magic_to_recovery_id(out_recovery_id, out_compressed, 0x1b));
    BOOST_REQUIRE(!out_compressed);
    BOOST_REQUIRE_EQUAL(out_recovery_id, 0u);
    BOOST_REQUIRE(magic_to_recovery_id(out_recovery_id, out_compressed, 0x1c));
    BOOST_REQUIRE(!out_compressed);
    BOOST_REQUIRE_EQUAL(out_recovery_id, 1u);
    BOOST_REQUIRE(magic_to_recovery_id(out_recovery_id, out_compressed, 0x1d));
    BOOST_REQUIRE(!out_compressed);
    BOOST_REQUIRE_EQUAL(out_recovery_id, 2u);
    BOOST_REQUIRE(magic_to_recovery_id(out_recovery_id, out_compressed, 0x1e));
    BOOST_REQUIRE(!out_compressed);
    BOOST_REQUIRE_EQUAL(out_recovery_id, 3u);
}

BOOST_AUTO_TEST_CASE(message__magic_to_recovery_id__compressed__expected) {
    bool out_compressed = false;
    uint8_t out_recovery_id = 0xff;
    BOOST_REQUIRE(magic_to_recovery_id(out_recovery_id, out_compressed, 0x1f));
    BOOST_REQUIRE(out_compressed);
    BOOST_REQUIRE_EQUAL(out_recovery_id, 0u);
    BOOST_REQUIRE(magic_to_recovery_id(out_recovery_id, out_compressed, 0x20));
    BOOST_REQUIRE(out_compressed);
    BOOST_REQUIRE_EQUAL(out_recovery_id, 1u);
    BOOST_REQUIRE(magic_to_recovery_id(out_recovery_id, out_compressed, 0x21));
    BOOST_REQUIRE(out_compressed);
    BOOST_REQUIRE_EQUAL(out_recovery_id, 2u);
    BOOST_REQUIRE(magic_to_recovery_id(out_recovery_id, out_compressed, 0x22));
    BOOST_REQUIRE(out_compressed);
    BOOST_REQUIRE_EQUAL(out_recovery_id, 3u);
}

BOOST_AUTO_TEST_CASE(message__recovery_id_to_magic__uncompressed_invalid__false) {
    uint8_t out_magic;
    BOOST_REQUIRE(!recovery_id_to_magic(out_magic, 4, false));
    BOOST_REQUIRE(!recovery_id_to_magic(out_magic, max_uint8, false));
}

BOOST_AUTO_TEST_CASE(message__recovery_id_to_magic__compressed_invalid__false) {
    uint8_t out_magic;
    BOOST_REQUIRE(!recovery_id_to_magic(out_magic, 4, true));
    BOOST_REQUIRE(!recovery_id_to_magic(out_magic, max_uint8, true));
}

BOOST_AUTO_TEST_CASE(message__magic_to_recovery_id__invalid__false) {
    bool out_compressed;
    uint8_t out_recovery_id;
    BOOST_REQUIRE(!magic_to_recovery_id(out_recovery_id, out_compressed, 0));
    BOOST_REQUIRE(!magic_to_recovery_id(out_recovery_id, out_compressed, max_uint8));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(message__sign_message)

BOOST_AUTO_TEST_CASE(message__sign_message__compressed__expected) {
    auto const compressed = true;
    auto const secret = base16_literal(SECRET);
    const payment_address address({secret, 0x00, compressed});
    auto const message = to_chunk(std::string("Compressed"));
    message_signature out_signature;
    BOOST_REQUIRE(sign_message(out_signature, message, secret, compressed));
    BOOST_REQUIRE_EQUAL(encode_base16(out_signature), SIGNATURE_COMPRESSED);
}

BOOST_AUTO_TEST_CASE(message__sign_message__uncompressed__expected) {
    auto const compressed = false;
    auto const secret = base16_literal(SECRET);
    const payment_address address({secret, 0x00, compressed});
    auto const message = to_chunk(std::string("Uncompressed"));
    message_signature out_signature;
    BOOST_REQUIRE(sign_message(out_signature, message, secret, compressed));
    BOOST_REQUIRE_EQUAL(encode_base16(out_signature), SIGNATURE_UNCOMPRESSED);
}

BOOST_AUTO_TEST_CASE(message__sign_message_secret__compressed__expected) {
    ec_private secret(WIF_COMPRESSED);
    const payment_address address(secret);
    auto const message = to_chunk(std::string("Compressed"));
    message_signature out_signature;
    BOOST_REQUIRE(sign_message(out_signature, message, secret));
    BOOST_REQUIRE_EQUAL(encode_base16(out_signature), SIGNATURE_WIF_COMPRESSED);
}

BOOST_AUTO_TEST_CASE(message__sign_message_wif__compressed__expected) {
    ec_private secret(WIF_COMPRESSED);
    const payment_address address(secret);
    auto const message = to_chunk(std::string("Compressed"));
    message_signature out_signature;
    BOOST_REQUIRE(sign_message(out_signature, message, secret, secret.compressed()));
    BOOST_REQUIRE_EQUAL(encode_base16(out_signature), SIGNATURE_WIF_COMPRESSED);
}

BOOST_AUTO_TEST_CASE(message__sign_message_wif__uncompressed__expected) {
    ec_private secret(WIF_UNCOMPRESSED);
    const payment_address address(secret);
    auto const message = to_chunk(std::string("Uncompressed"));
    message_signature out_signature;
    BOOST_REQUIRE(sign_message(out_signature, message, secret, secret.compressed()));
    BOOST_REQUIRE_EQUAL(encode_base16(out_signature), SIGNATURE_WIF_UNCOMPRESSED);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(message__verify_message)

BOOST_AUTO_TEST_CASE(message__verify_message__compressed__expected) {
    const payment_address address(base16_literal(SECRET));
    auto const message = to_chunk(std::string("Compressed"));
    auto const signature = base16_literal(SIGNATURE_COMPRESSED);
    BOOST_REQUIRE(verify_message(message, address, signature));
}

BOOST_AUTO_TEST_CASE(message__verify_message__uncompressed__expected) {
    const payment_address address({base16_literal(SECRET), 0x00, false});
    auto const message = to_chunk(std::string("Uncompressed"));
    auto const signature = base16_literal(SIGNATURE_UNCOMPRESSED);
    BOOST_REQUIRE(verify_message(message, address, signature));
}

BOOST_AUTO_TEST_CASE(message__verify_message_wif__compressed__round_trip) {
    ec_private secret(WIF_COMPRESSED);
    const payment_address address(secret);
    auto const message = to_chunk(std::string("Compressed"));
    auto const signature = base16_literal(SIGNATURE_WIF_COMPRESSED);
    BOOST_REQUIRE(verify_message(message, address, signature));
}

BOOST_AUTO_TEST_CASE(message__verify_message_wif__uncompressed__round_trip) {
    ec_private secret(WIF_UNCOMPRESSED);
    const payment_address address(secret);
    auto const message = to_chunk(std::string("Uncompressed"));
    auto const signature = base16_literal(SIGNATURE_WIF_UNCOMPRESSED);
    BOOST_REQUIRE(verify_message(message, address, signature));
}

BOOST_AUTO_TEST_CASE(message__verify_message__electrum_compressed__okay) {
    message_signature signature;
    BOOST_REQUIRE(decode_base16(signature, ELECTRUM_SIGNATURE));

    // Address of the compressed public key of the message signer.
    const payment_address address("1PeChFbhxDD9NLbU21DfD55aQBC4ZTR3tE");
    auto const message = to_chunk(std::string("Nakomoto"));
    BOOST_REQUIRE(verify_message(message, address, signature));
}

BOOST_AUTO_TEST_CASE(message__verify_message__electrum_incorrect_address__false) {
    message_signature signature;
    BOOST_REQUIRE(decode_base16(signature, ELECTRUM_SIGNATURE));

    // Address of the uncompressed public key of the message signer (incorrect).
    const payment_address address("1Em1SX7qQq1pTmByqLRafhL1ypx2V786tP");
    auto const message = to_chunk(std::string("Nakomoto"));
    BOOST_REQUIRE(!verify_message(message, address, signature));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

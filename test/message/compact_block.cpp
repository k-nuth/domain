// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

<<<<<<< HEAD
#include <bitcoin/bitcoin.hpp>
=======
#include <kth/domain.hpp>
>>>>>>> dev
#include <boost/test/unit_test.hpp>

using namespace bc;

BOOST_AUTO_TEST_SUITE(compact_block_tests)

BOOST_AUTO_TEST_CASE(compact_block__constructor_1__always__invalid) {
    message::compact_block instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
}

uint64_t convert_to_uint64t(std::string const& rawdata) {
    uint64_t value;
    std::istringstream iss(rawdata);
    iss >> value;
    return value;
}

BOOST_AUTO_TEST_CASE(compact_block__constructor_2__always__equals_params) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;

    const message::compact_block::short_id_list& short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    message::compact_block instance(header, nonce, short_ids, transactions);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(nonce == instance.nonce());
    BOOST_REQUIRE(short_ids == instance.short_ids());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(compact_block__constructor_3__always__equals_params) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);
    chain::header dup_header(header);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};
    message::compact_block::short_id_list dup_short_ids = short_ids;

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};
    message::prefilled_transaction::list dup_transactions = transactions;

    message::compact_block instance(std::move(dup_header), nonce,
                                    std::move(dup_short_ids), std::move(dup_transactions));

    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(nonce == instance.nonce());
    BOOST_REQUIRE(short_ids == instance.short_ids());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(compact_block__constructor_4__always__equals_params) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    const message::compact_block value(header, nonce, short_ids, transactions);

    message::compact_block instance(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(value == instance);
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(nonce == instance.nonce());
    BOOST_REQUIRE(short_ids == instance.short_ids());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(compact_block__constructor_5__always__equals_params) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    message::compact_block value(header, nonce, short_ids, transactions);

    message::compact_block instance(std::move(value));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(nonce == instance.nonce());
    BOOST_REQUIRE(short_ids == instance.short_ids());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(compact_block__from_data__insufficient_bytes__failure) {
    data_chunk const raw{0xab, 0xcd};
    message::compact_block instance{};
    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::compact_block::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(compact_block__from_data__insufficient_bytes_mid_transaction__failure) {
    auto const raw = to_chunk(base16_literal(
        "0a0000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d619000000"
        "00003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
        "221b08003e8a6300240c0100d2040000000000000400000012121212121234343434"
        "3434565656565678789a9a02010000000100000000000001000000010000000"));

    message::compact_block instance{};
    BOOST_REQUIRE_EQUAL(false, instance.from_data(message::compact_block::version_minimum, raw));
}

BOOST_AUTO_TEST_CASE(compact_block__from_data__insufficient_version__failure) {
    auto const raw = to_chunk(base16_literal(
        "0a0000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d619000000"
        "00003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
        "221b08003e8a6300240c0100d2040000000000000400000012121212121234343434"
        "3434565656565678789a9a0201000000010000000000000100000001000000000000"));

    message::compact_block expected;
    expected.from_data(message::compact_block::version_minimum, raw);
    auto const data = expected.to_data(message::compact_block::version_minimum);
    BOOST_REQUIRE(raw == data);

    message::compact_block instance{};
    BOOST_REQUIRE_EQUAL(false, instance.from_data(
                                   message::compact_block::version_minimum - 1, data));
}

BOOST_AUTO_TEST_CASE(compact_block__factory_from_data_1__valid_input__success) {
    auto const raw = to_chunk(base16_literal(
        "0a0000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d619000000"
        "00003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
        "221b08003e8a6300240c0100d2040000000000000400000012121212121234343434"
        "3434565656565678789a9a0201000000010000000000000100000001000000000000"));

    message::compact_block expected;
    expected.from_data(message::compact_block::version_minimum, raw);
    auto const data = expected.to_data(message::compact_block::version_minimum);
    BOOST_REQUIRE(raw == data);

    auto const result = message::compact_block::factory_from_data(
        message::compact_block::version_minimum, data);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::compact_block::version_minimum));
    BOOST_REQUIRE_EQUAL(
        expected.serialized_size(message::compact_block::version_minimum),
        result.serialized_size(message::compact_block::version_minimum));
}

BOOST_AUTO_TEST_CASE(compact_block__factory_from_data_2__valid_input__success) {
    auto const raw = to_chunk(base16_literal(
        "0a0000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d619000000"
        "00003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
        "221b08003e8a6300240c0100d2040000000000000400000012121212121234343434"
        "3434565656565678789a9a0201000000010000000000000100000001000000000000"));

    message::compact_block expected;
    expected.from_data(message::compact_block::version_minimum, raw);
    auto const data = expected.to_data(message::compact_block::version_minimum);
    BOOST_REQUIRE(raw == data);

    data_source istream(data);
    auto result = message::compact_block::factory_from_data(
        message::compact_block::version_minimum, istream);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::compact_block::version_minimum));
    BOOST_REQUIRE_EQUAL(
        expected.serialized_size(message::compact_block::version_minimum),
        result.serialized_size(message::compact_block::version_minimum));
}

BOOST_AUTO_TEST_CASE(compact_block__factory_from_data_3__valid_input__success) {
    auto const raw = to_chunk(base16_literal(
        "0a0000006fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d619000000"
        "00003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a"
        "221b08003e8a6300240c0100d2040000000000000400000012121212121234343434"
        "3434565656565678789a9a0201000000010000000000000100000001000000000000"));

    message::compact_block expected;
    expected.from_data(message::compact_block::version_minimum, raw);
    auto const data = expected.to_data(message::compact_block::version_minimum);
    BOOST_REQUIRE(raw == data);

    data_source istream(data);
    istream_reader source(istream);
    auto const result = message::compact_block::factory_from_data(
        message::compact_block::version_minimum, source);

    BOOST_REQUIRE(result.is_valid());
    BOOST_REQUIRE(expected == result);
    BOOST_REQUIRE_EQUAL(data.size(),
                        result.serialized_size(message::compact_block::version_minimum));
    BOOST_REQUIRE_EQUAL(
        expected.serialized_size(message::compact_block::version_minimum),
        result.serialized_size(message::compact_block::version_minimum));
}

BOOST_AUTO_TEST_CASE(compact_block__header_accessor_1__always__returns_initialized_value) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    message::compact_block instance(header, nonce, short_ids, transactions);
    BOOST_REQUIRE(header == instance.header());
}

BOOST_AUTO_TEST_CASE(compact_block__header_accessor_2__always__returns_initialized_value) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    const message::compact_block instance(header, nonce, short_ids, transactions);
    BOOST_REQUIRE(header == instance.header());
}

BOOST_AUTO_TEST_CASE(compact_block__header_setter_1__roundtrip__success) {
    chain::header const value(10u,
                              hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                              hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                              531234u,
                              6523454u,
                              68644u);

    message::compact_block instance;
    BOOST_REQUIRE(value != instance.header());
    instance.set_header(value);
    BOOST_REQUIRE(value == instance.header());
}

BOOST_AUTO_TEST_CASE(compact_block__header_setter_2__roundtrip__success) {
    chain::header const value(10u,
                              hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                              hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                              531234u,
                              6523454u,
                              68644u);
    chain::header dup(value);

    message::compact_block instance;
    BOOST_REQUIRE(value != instance.header());
    instance.set_header(std::move(dup));
    BOOST_REQUIRE(value == instance.header());
}

BOOST_AUTO_TEST_CASE(compact_block__nonce_accessor__always__returns_initialized_value) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    message::compact_block instance(header, nonce, short_ids, transactions);
    BOOST_REQUIRE(nonce == instance.nonce());
}

BOOST_AUTO_TEST_CASE(compact_block__nonce_setter__roundtrip__success) {
    uint64_t value = 123356u;

    message::compact_block instance;
    BOOST_REQUIRE(value != instance.nonce());
    instance.set_nonce(value);
    BOOST_REQUIRE(value == instance.nonce());
}

BOOST_AUTO_TEST_CASE(compact_block__short_ids_accessor_1__always__returns_initialized_value) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    message::compact_block instance(header, nonce, short_ids, transactions);
    BOOST_REQUIRE(short_ids == instance.short_ids());
}

BOOST_AUTO_TEST_CASE(compact_block__short_ids_accessor_2__always__returns_initialized_value) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    const message::compact_block instance(header, nonce, short_ids, transactions);
    BOOST_REQUIRE(short_ids == instance.short_ids());
}

BOOST_AUTO_TEST_CASE(compact_block__short_ids_setter_1__roundtrip__success) {
    const message::compact_block::short_id_list value = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    message::compact_block instance;
    BOOST_REQUIRE(value != instance.short_ids());
    instance.set_short_ids(value);
    BOOST_REQUIRE(value == instance.short_ids());
}

BOOST_AUTO_TEST_CASE(compact_block__short_ids_setter_2__roundtrip__success) {
    const message::compact_block::short_id_list value = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};
    message::compact_block::short_id_list dup(value);

    message::compact_block instance;
    BOOST_REQUIRE(value != instance.short_ids());
    instance.set_short_ids(std::move(dup));
    BOOST_REQUIRE(value == instance.short_ids());
}

BOOST_AUTO_TEST_CASE(compact_block__transactions_accessor_1__always__returns_initialized_value) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    message::compact_block instance(header, nonce, short_ids, transactions);
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(compact_block__transactions_accessor_2__always__returns_initialized_value) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    const message::compact_block instance(header, nonce, short_ids, transactions);
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(compact_block__transactions_setter_1__roundtrip__success) {
    const message::prefilled_transaction::list value = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    message::compact_block instance;
    BOOST_REQUIRE(value != instance.transactions());
    instance.set_transactions(value);
    BOOST_REQUIRE(value == instance.transactions());
}

BOOST_AUTO_TEST_CASE(compact_block__transactions_setter_2__roundtrip__success) {
    const message::prefilled_transaction::list value = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};
    message::prefilled_transaction::list dup(value);

    message::compact_block instance;
    BOOST_REQUIRE(value != instance.transactions());
    instance.set_transactions(std::move(dup));
    BOOST_REQUIRE(value == instance.transactions());
}

BOOST_AUTO_TEST_CASE(compact_block__operator_assign_equals__always__matches_equivalent) {
    chain::header const header(10u,
                               hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                               hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                               531234u,
                               6523454u,
                               68644u);

    uint64_t nonce = 453245u;
    const message::compact_block::short_id_list short_ids = {
        convert_to_uint64t("aaaaaaaaaaaa"),
        convert_to_uint64t("bbbbbbbbbbbb"),
        convert_to_uint64t("0f0f0f0f0f0f"),
        convert_to_uint64t("f0f0f0f0f0f0")};

    const message::prefilled_transaction::list transactions = {
        message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
        message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
        message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))};

    message::compact_block value(header, nonce, short_ids, transactions);
    BOOST_REQUIRE(value.is_valid());
    message::compact_block instance;
    BOOST_REQUIRE_EQUAL(false, instance.is_valid());
    instance = std::move(value);
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(header == instance.header());
    BOOST_REQUIRE(nonce == instance.nonce());
    BOOST_REQUIRE(short_ids == instance.short_ids());
    BOOST_REQUIRE(transactions == instance.transactions());
}

BOOST_AUTO_TEST_CASE(compact_block__operator_boolean_equals__duplicates__returns_true) {
    const message::compact_block expected(
        chain::header(10u,
                      hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                      hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                      531234u,
                      6523454u,
                      68644u),
        12334u,
        {convert_to_uint64t("aaaaaaaaaaaa"),
         convert_to_uint64t("bbbbbbbbbbbb"),
         convert_to_uint64t("0f0f0f0f0f0f"),
         convert_to_uint64t("f0f0f0f0f0f0")},
        {message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
         message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
         message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))});

    message::compact_block instance(expected);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(compact_block__operator_boolean_equals__differs__returns_false) {
    const message::compact_block expected(
        chain::header(10u,
                      hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                      hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                      531234u,
                      6523454u,
                      68644u),
        12334u,
        {convert_to_uint64t("aaaaaaaaaaaa"),
         convert_to_uint64t("bbbbbbbbbbbb"),
         convert_to_uint64t("0f0f0f0f0f0f"),
         convert_to_uint64t("f0f0f0f0f0f0")},
        {message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
         message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
         message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))});

    message::compact_block instance;
    BOOST_REQUIRE_EQUAL(false, instance == expected);
}

BOOST_AUTO_TEST_CASE(compact_block__operator_boolean_not_equals__duplicates__returns_false) {
    const message::compact_block expected(
        chain::header(10u,
                      hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                      hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                      531234u,
                      6523454u,
                      68644u),
        12334u,
        {convert_to_uint64t("aaaaaaaaaaaa"),
         convert_to_uint64t("bbbbbbbbbbbb"),
         convert_to_uint64t("0f0f0f0f0f0f"),
         convert_to_uint64t("f0f0f0f0f0f0")},
        {message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
         message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
         message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))});

    message::compact_block instance(expected);
    BOOST_REQUIRE_EQUAL(false, instance != expected);
}

BOOST_AUTO_TEST_CASE(compact_block__operator_boolean_not_equals__differs__returns_true) {
    const message::compact_block expected(
        chain::header(10u,
                      hash_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"),
                      hash_literal("4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"),
                      531234u,
                      6523454u,
                      68644u),
        12334u,
        {convert_to_uint64t("aaaaaaaaaaaa"),
         convert_to_uint64t("bbbbbbbbbbbb"),
         convert_to_uint64t("0f0f0f0f0f0f"),
         convert_to_uint64t("f0f0f0f0f0f0")},
        {message::prefilled_transaction(10, chain::transaction(1, 48, {}, {})),
         message::prefilled_transaction(20, chain::transaction(2, 32, {}, {})),
         message::prefilled_transaction(30, chain::transaction(4, 16, {}, {}))});

    message::compact_block instance;
    BOOST_REQUIRE(instance != expected);
}

BOOST_AUTO_TEST_SUITE_END()

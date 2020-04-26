// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::machine;

data_chunk valid_raw_operation = to_chunk(base16_literal("0900ff11ee22bb33aa44"));

// Start Boost Suite: operation tests

TEST_CASE("operation  constructor 1  always  returns default initialized", "[operation]") {
    operation instance;

    REQUIRE(!instance.is_valid());
    REQUIRE(instance.data().empty());
    REQUIRE(instance.code() == opcode::disabled_xor);
}

TEST_CASE("operation  constructor 2  valid input  returns input initialized", "[operation]") {
    auto const data = to_chunk(base16_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"));
    auto dup_data = data;
    operation instance(std::move(dup_data));

    REQUIRE(instance.is_valid());
    REQUIRE(instance.code() == opcode::push_size_32);
    REQUIRE(instance.data() == data);
}

TEST_CASE("operation  constructor 3  valid input  returns input initialized", "[operation]") {
    auto const data = to_chunk(base16_literal("000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f"));
    operation instance(data);

    REQUIRE(instance.is_valid());
    REQUIRE(instance.code() == opcode::push_size_32);
    REQUIRE(instance.data() == data);
}

TEST_CASE("operation  constructor 4  valid input  returns input initialized", "[operation]") {
    operation const expected(to_chunk(base16_literal("23156214")));
    operation instance(expected);

    REQUIRE(instance.is_valid());
    REQUIRE(expected == instance);
}

TEST_CASE("operation  constructor 5  valid input  returns input initialized", "[operation]") {
    operation expected(to_chunk(base16_literal("23156214")));
    operation instance(std::move(expected));

    REQUIRE(instance.is_valid());
}

TEST_CASE("operation  from data  insufficient bytes  failure", "[operation]") {
    data_chunk const data;
    operation instance;

    REQUIRE(!entity_from_data(instance, data));
    REQUIRE(!instance.is_valid());
}

TEST_CASE("operation  from data  roundtrip push size 0  success", "[operation]") {
    auto const data0 = to_chunk(base16_literal(""));
    auto const raw_operation = to_chunk(base16_literal("00"));
    operation instance;

    REQUIRE(entity_from_data(instance, raw_operation));
    REQUIRE(instance.is_valid());
    REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    REQUIRE(entity_from_data(duplicate,instance.to_data()));
    REQUIRE(instance == duplicate);

    REQUIRE(instance.code() == opcode::push_size_0);
    REQUIRE(instance.data() == data0);
}

TEST_CASE("operation  from data  roundtrip push size 75  success", "[operation]") {
    auto const data75 = data_chunk(75, '.');
    auto const raw_operation = build_chunk({base16_literal("4b"), data75});
    operation instance;

    REQUIRE(entity_from_data(instance, raw_operation));
    REQUIRE(instance.is_valid());
    REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    REQUIRE(entity_from_data(duplicate,instance.to_data()));
    REQUIRE(instance == duplicate);

    REQUIRE(instance.code() == opcode::push_size_75);
    REQUIRE(instance.data() == data75);
}

TEST_CASE("operation  from data  roundtrip push negative 1  success", "[operation]") {
    static auto const op_79 = static_cast<uint8_t>(opcode::push_negative_1);
    auto const data1 = data_chunk{op_79};
    auto const raw_operation = data1;
    operation instance;

    // This is read as an encoded operation, not as data.
    // Constructors read (unencoded) data and can select minimal encoding.
    REQUIRE(entity_from_data(instance, raw_operation));
    REQUIRE(instance.is_valid());
    REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    REQUIRE(entity_from_data(duplicate,instance.to_data()));
    REQUIRE(instance == duplicate);

    // The code is the data for numeric push codes.
    REQUIRE(instance.code() == opcode::push_negative_1);
    REQUIRE(instance.data() == data_chunk{});
}

TEST_CASE("operation  from data  roundtrip push positive 1  success", "[operation]") {
    static auto const op_81 = static_cast<uint8_t>(opcode::push_positive_1);
    auto const data1 = data_chunk{op_81};
    auto const raw_operation = data1;
    operation instance;

    // This is read as an encoded operation, not as data.
    // Constructors read (unencoded) data and can select minimal encoding.
    REQUIRE(entity_from_data(instance, raw_operation));
    REQUIRE(instance.is_valid());
    REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    REQUIRE(entity_from_data(duplicate,instance.to_data()));
    REQUIRE(instance == duplicate);

    // The code is the data for numeric push codes.
    REQUIRE(instance.code() == opcode::push_positive_1);
    REQUIRE(instance.data() == data_chunk{});
}

TEST_CASE("operation  from data  roundtrip push positive 16  success", "[operation]") {
    static auto const op_96 = static_cast<uint8_t>(opcode::push_positive_16);
    auto const data1 = data_chunk{op_96};
    auto const raw_operation = data1;
    operation instance;

    // This is read as an encoded operation, not as data.
    // Constructors read (unencoded) data and can select minimal encoding.
    BOOST_REQUIRE(entity_from_data(instance, raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(entity_from_data(duplicate,instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    // The code is the data for numeric push codes.
    BOOST_REQUIRE(instance.code() == opcode::push_positive_16);
    BOOST_REQUIRE(instance.data() == data_chunk{});
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_one_size__success) {
    auto const data255 = data_chunk(255, '.');
    auto const raw_operation = build_chunk({base16_literal("4c"
                                                           "ff"),
                                            data255});
    operation instance;

    BOOST_REQUIRE(entity_from_data(instance, raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(entity_from_data(duplicate,instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_one_size);
    BOOST_REQUIRE(instance.data() == data255);
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_two_size__success) {
    auto const data520 = data_chunk(520, '.');
    auto const raw_operation = build_chunk({base16_literal("4d"
                                                           "0802"),
                                            data520});
    operation instance;

    BOOST_REQUIRE(entity_from_data(instance, raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(entity_from_data(duplicate,instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_two_size);
    BOOST_REQUIRE(instance.data() == data520);
}

BOOST_AUTO_TEST_CASE(operation__from_data__roundtrip_push_four_size__success) {
    auto const data520 = data_chunk(520, '.');
    auto const raw_operation = build_chunk({base16_literal("4e"
                                                           "08020000"),
                                            data520});
    operation instance;

    BOOST_REQUIRE(entity_from_data(instance, raw_operation));
    BOOST_REQUIRE(instance.is_valid());
    BOOST_REQUIRE(raw_operation == instance.to_data());

    operation duplicate;
    BOOST_REQUIRE(entity_from_data(duplicate,instance.to_data()));
    BOOST_REQUIRE(instance == duplicate);

    BOOST_REQUIRE(instance.code() == opcode::push_four_size);
    BOOST_REQUIRE(instance.data() == data520);
}

BOOST_AUTO_TEST_CASE(operation__factory_from_data_1__roundtrip__success) {
    auto operation = create<machine::operation>(valid_raw_operation);

    BOOST_REQUIRE(operation.is_valid());
    data_chunk output = operation.to_data();
    BOOST_REQUIRE(output == valid_raw_operation);
}

BOOST_AUTO_TEST_CASE(operation__factory_from_data_2__roundtrip__success) {
    data_source istream(valid_raw_operation);
    auto operation = create<machine::operation>(istream);

    BOOST_REQUIRE(operation.is_valid());
    data_chunk output = operation.to_data();
    BOOST_REQUIRE(output == valid_raw_operation);
}

BOOST_AUTO_TEST_CASE(operation__factory_from_data_3__roundtrip__success) {
    data_source istream(valid_raw_operation);
    istream_reader source(istream);
    auto operation = create<machine::operation>(source);

    BOOST_REQUIRE(operation.is_valid());
    data_chunk output = operation.to_data();
    BOOST_REQUIRE(output == valid_raw_operation);
}

BOOST_AUTO_TEST_CASE(operation__operator_assign_equals_1__always__matches_equivalent) {
    operation expected;
    BOOST_REQUIRE(entity_from_data(expected, valid_raw_operation));
    operation instance;
    operation value;
    BOOST_REQUIRE(entity_from_data(value, valid_raw_operation));
    instance = std::move(value);
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(operation__operator_assign_equals_2__always__matches_equivalent) {
    operation expected;
    BOOST_REQUIRE(entity_from_data(expected, valid_raw_operation));
    operation instance;
    instance = expected;
    BOOST_REQUIRE(instance == expected);
}

BOOST_AUTO_TEST_CASE(operation__operator_boolean_equals__duplicates__returns_true) {
    operation alpha;
    operation beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_operation));
    BOOST_REQUIRE(entity_from_data(beta, valid_raw_operation));
    BOOST_REQUIRE(alpha == beta);
}

BOOST_AUTO_TEST_CASE(operation__operator_boolean_equals__differs__returns_false) {
    operation alpha;
    operation beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_operation));
    BOOST_REQUIRE_EQUAL(false, alpha == beta);
}

BOOST_AUTO_TEST_CASE(operation__operator_boolean_not_equals__duplicates__returns_false) {
    operation alpha;
    operation beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_operation));
    BOOST_REQUIRE(entity_from_data(beta, valid_raw_operation));
    BOOST_REQUIRE_EQUAL(false, alpha != beta);
}

BOOST_AUTO_TEST_CASE(operation__operator_boolean_not_equals__differs__returns_true) {
    operation alpha;
    operation beta;
    BOOST_REQUIRE(entity_from_data(alpha, valid_raw_operation));
    BOOST_REQUIRE(alpha != beta);
}

// to_string

BOOST_AUTO_TEST_CASE(operation__to_string__push_size_0__zero) {
    operation value(opcode::push_size_0);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "zero");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_size_75__push_75) {
    // Empty data allows the push code to serialize as an op code.
    operation value(opcode::push_size_75);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "push_75");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_positive_7__7) {
    operation value(opcode::push_positive_7);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "7");
}

BOOST_AUTO_TEST_CASE(operation__to_string_minimal__0x07__7) {
    operation value({0x07}, true);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "7");
}

BOOST_AUTO_TEST_CASE(operation__to_string_nominal__0x07__0x07) {
    operation value({0x07}, false);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[07]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__0x42__0x42) {
    operation value({0x42}, true);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[42]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__0x112233__0x112233) {
    operation value({{0x11, 0x22, 0x33}}, true);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_size_3__0x112233__0x112233) {
    static data_chunk const encoded{{0x03, 0x11, 0x22, 0x33}};
    operation value;
    entity_from_data(value, encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_one_size_0x112233__1_0x112233) {
    static data_chunk const encoded{{0x4c, 0x03, 0x11, 0x22, 0x33}};
    operation value;
    entity_from_data(value, encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[1.112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_two_size_0x112233__2_0x112233) {
    static data_chunk const encoded{{0x4d, 0x03, 0x00, 0x11, 0x22, 0x33}};
    operation value;
    entity_from_data(value, encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[2.112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__push_four_size_0x112233__4_0x112233) {
    static data_chunk const encoded{{0x4e, 0x03, 0x00, 0x00, 0x00, 0x11, 0x22, 0x33}};
    operation value;
    entity_from_data(value, encoded);
    BOOST_REQUIRE_EQUAL(value.to_string(0), "[4.112233]");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop2_no_rules__nop2) {
    operation value(opcode::nop2);
    BOOST_REQUIRE_EQUAL(value.to_string(machine::rule_fork::no_rules), "nop2");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop2_bip65_rule__checklocktimeverify) {
    operation value(opcode::nop2);
    BOOST_REQUIRE_EQUAL(value.to_string(machine::rule_fork::bip65_rule), "checklocktimeverify");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop3_no_rules__nop3) {
    operation value(opcode::nop3);
    BOOST_REQUIRE_EQUAL(value.to_string(machine::rule_fork::no_rules), "nop3");
}

BOOST_AUTO_TEST_CASE(operation__to_string__nop3_bip112_rule__checksequenceverify) {
    operation value(opcode::nop3);
    BOOST_REQUIRE_EQUAL(value.to_string(machine::rule_fork::bip112_rule), "checksequenceverify");
}

// from_string

BOOST_AUTO_TEST_CASE(operation__from_string__negative_1__push_negative_1_empty) {
    operation value;
    BOOST_REQUIRE(value.from_string("-1"));
    BOOST_REQUIRE(value.code() == opcode::push_negative_1);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__0__push_size_0_empty) {
    operation value;
    BOOST_REQUIRE(value.from_string("0"));
    BOOST_REQUIRE(value.code() == opcode::push_size_0);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__1__push_positive_1_empty) {
    operation value;
    BOOST_REQUIRE(value.from_string("1"));
    BOOST_REQUIRE(value.code() == opcode::push_positive_1);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__16__push_positive_16_empty) {
    operation value;
    BOOST_REQUIRE(value.from_string("16"));
    BOOST_REQUIRE(value.code() == opcode::push_positive_16);
    BOOST_REQUIRE(value.data().empty());
}

BOOST_AUTO_TEST_CASE(operation__from_string__17__push_size_1_expected) {
    static data_chunk const expected{0x11};
    operation value;
    BOOST_REQUIRE(value.from_string("17"));
    BOOST_REQUIRE(value.code() == opcode::push_size_1);
    bool xxx = value.data() == expected;
    BOOST_REQUIRE(value.data() == expected);
    BOOST_REQUIRE_EQUAL(value.data(), expected);
}

// BOOST_AUTO_TEST_CASE(operation__from_string__negative_2__push_size_1_expected) {
//     static data_chunk const expected{0x82};
//     operation value;
//     BOOST_REQUIRE(value.from_string("-2"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_1);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__9223372036854775807__push_size_8_expected) {
//     static data_chunk const expected{{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f}};
//     operation value;
//     BOOST_REQUIRE(value.from_string("9223372036854775807"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_8);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__negative_9223372036854775807__push_size_8_expected) {
//     static data_chunk const expected{{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
//     operation value;
//     BOOST_REQUIRE(value.from_string("-9223372036854775807"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_8);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__string_empty__push_size_0_empty) {
//     static data_chunk const expected{0x61};
//     operation value;
//     BOOST_REQUIRE(value.from_string("''"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_0);
//     BOOST_REQUIRE(value.data().empty());
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__string_a__push_size_1_expected_byte) {
//     static data_chunk const expected{0x61};
//     operation value;
//     BOOST_REQUIRE(value.from_string("'a'"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_1);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__string_abc__push_size_3_expected_byte_order) {
//     static data_chunk const expected{{0x61, 0x62, 0x63}};
//     operation value;
//     BOOST_REQUIRE(value.from_string("'abc'"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_3);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__negative_1_character__push_size_1_nominal_encoding) {
//     static data_chunk const expected{0x4f};
//     operation value;
//     BOOST_REQUIRE(value.from_string("'O'"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_1);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__push_0__push_size_0) {
//     operation value;
//     BOOST_REQUIRE(value.from_string("push_0"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_0);
//     BOOST_REQUIRE(value.data().empty());
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__push_1__false) {
//     operation value;
//     BOOST_REQUIRE(!value.from_string("push_1"));
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__push_75__false) {
//     operation value;
//     BOOST_REQUIRE(!value.from_string("push_75"));
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__push_one__push_one_size_empty) {
//     operation value;
//     BOOST_REQUIRE(value.from_string("push_one"));
//     BOOST_REQUIRE(value.code() == opcode::push_one_size);
//     BOOST_REQUIRE(value.data().empty());
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__push_two__push_two_size_empty) {
//     operation value;
//     BOOST_REQUIRE(value.from_string("push_two"));
//     BOOST_REQUIRE(value.code() == opcode::push_two_size);
//     BOOST_REQUIRE(value.data().empty());
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__push_four__push_four_size_empty) {
//     operation value;
//     BOOST_REQUIRE(value.from_string("push_four"));
//     BOOST_REQUIRE(value.code() == opcode::push_four_size);
//     BOOST_REQUIRE(value.data().empty());
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__7__push_positive_7) {
//     operation value;
//     BOOST_REQUIRE(value.from_string("7"));
//     BOOST_REQUIRE(value.code() == opcode::push_positive_7);
//     BOOST_REQUIRE(value.data().empty());
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__0x07__push_size_1) {
//     static data_chunk const expected{0x07};
//     operation value;
//     BOOST_REQUIRE(value.from_string("[07]"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_1);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__0x42__push_size_1) {
//     static data_chunk const expected{0x42};
//     operation value;
//     BOOST_REQUIRE(value.from_string("[42]"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_1);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__0x112233__push_size_3) {
//     static data_chunk const expected{{0x11, 0x22, 0x33}};
//     operation value;
//     BOOST_REQUIRE(value.from_string("[112233]"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_3);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__0_0x112233__push_size_3) {
//     static data_chunk const expected{{0x11, 0x22, 0x33}};
//     operation value;
//     BOOST_REQUIRE(value.from_string("[0.112233]"));
//     BOOST_REQUIRE(value.code() == opcode::push_size_3);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__1_0x112233__push_one_size) {
//     static data_chunk const expected{{0x11, 0x22, 0x33}};
//     operation value;
//     BOOST_REQUIRE(value.from_string("[1.112233]"));
//     BOOST_REQUIRE(value.code() == opcode::push_one_size);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__2_0x112233__push_two_size) {
//     static data_chunk const expected{{0x11, 0x22, 0x33}};
//     operation value;
//     BOOST_REQUIRE(value.from_string("[2.112233]"));
//     BOOST_REQUIRE(value.code() == opcode::push_two_size);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__4_0x112233__push_four_size) {
//     static data_chunk const expected{{0x11, 0x22, 0x33}};
//     operation value;
//     BOOST_REQUIRE(value.from_string("[4.112233]"));
//     BOOST_REQUIRE(value.code() == opcode::push_four_size);
//     BOOST_REQUIRE_EQUAL(value.data(), expected);
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__5_0x112233__false) {
//     operation value;
//     BOOST_REQUIRE(!value.from_string("[5.112233]"));
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__empty_0x112233__false) {
//     operation value;
//     BOOST_REQUIRE(!value.from_string("[.112233]"));
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__nop2__nop2_checklocktimeverify) {
//     operation value;
//     BOOST_REQUIRE(value.from_string("nop2"));
//     BOOST_REQUIRE(value.code() == opcode::nop2);
//     BOOST_REQUIRE(value.code() == opcode::checklocktimeverify);
//     BOOST_REQUIRE(value.data().empty());
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__checklocktimeverify__nop2_checklocktimeverify) {
//     operation value;
//     BOOST_REQUIRE(value.from_string("checklocktimeverify"));
//     BOOST_REQUIRE(value.code() == opcode::nop2);
//     BOOST_REQUIRE(value.code() == opcode::checklocktimeverify);
//     BOOST_REQUIRE(value.data().empty());
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__nop3__nop3_checksequenceverify) {
//     operation value;
//     BOOST_REQUIRE(value.from_string("nop3"));
//     BOOST_REQUIRE(value.code() == opcode::nop3);
//     BOOST_REQUIRE(value.code() == opcode::checksequenceverify);
//     BOOST_REQUIRE(value.data().empty());
// }

// BOOST_AUTO_TEST_CASE(operation__from_string__checklocktimeverify__nop3_checksequenceverify) {
//     operation value;
//     BOOST_REQUIRE(value.from_string("checksequenceverify"));
//     BOOST_REQUIRE(value.code() == opcode::nop3);
//     BOOST_REQUIRE(value.code() == opcode::checksequenceverify);
//     BOOST_REQUIRE(value.data().empty());
// }

BOOST_AUTO_TEST_SUITE_END()

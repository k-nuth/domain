// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <test_helpers.hpp>

using namespace kth;
using namespace kd;
using namespace kth::domain::machine;

// Start Boost Suite: opcode tests

// opcode_to_string
//-----------------------------------------------------------------------------
// Partial coverage includes those with aliases and samples of each "range" of codes.

// Use the traditional serializations for all codes (in this case 'zero' vs. 'push_0').
TEST_CASE("opcode  opcode to string  zero any forks  zero", "[None]")
{
    REQUIRE(opcode_to_string(opcode::push_size_0, rule_fork::no_rules) == "zero");
    REQUIRE(opcode_to_string(opcode::push_size_0, rule_fork::all_rules) == "zero");
}

// We formerly serialized all 1-75 as 'special'.
TEST_CASE("opcode  opcode to string  push size 42 any forks  push 42", "[None]")
{
    REQUIRE(opcode_to_string(opcode::push_size_42, rule_fork::no_rules) == "push_42");
    REQUIRE(opcode_to_string(opcode::push_size_42, rule_fork::all_rules) == "push_42");
}

// Use the traditional serializations for all codes (in this case 'pushdata1' vs. 'push_one').
TEST_CASE("opcode  opcode to string  push one size any forks  pushdata1", "[None]")
{
    REQUIRE(opcode_to_string(opcode::push_one_size, rule_fork::no_rules) == "pushdata1");
    REQUIRE(opcode_to_string(opcode::push_one_size, rule_fork::all_rules) == "pushdata1");
}

// Use the traditional serializations for all codes (in this case 'pushdata2' vs. 'push_two').
TEST_CASE("opcode  opcode to string  push two size size any forks  pushdata2", "[None]")
{
    REQUIRE(opcode_to_string(opcode::push_two_size, rule_fork::no_rules) == "pushdata2");
    REQUIRE(opcode_to_string(opcode::push_two_size, rule_fork::all_rules) == "pushdata2");
}

// Use the traditional serializations for all codes (in this case 'pushdata4' vs. 'push_four').
TEST_CASE("opcode  opcode to string  push four size any forks  pushdata4", "[None]")
{
    REQUIRE(opcode_to_string(opcode::push_four_size, rule_fork::no_rules) == "pushdata4");
    REQUIRE(opcode_to_string(opcode::push_four_size, rule_fork::all_rules) == "pushdata4");
}

// Use the traditional serializations for all codes (in this case 'reserved' vs. 0x50).
TEST_CASE("opcode  opcode to string  reserved 80 any forks  reserved", "[None]")
{
    REQUIRE(opcode_to_string(opcode::reserved_80, rule_fork::no_rules) == "reserved");
    REQUIRE(opcode_to_string(opcode::reserved_80, rule_fork::all_rules) == "reserved");
}

TEST_CASE("opcode  opcode to string  push positive 7 any forks  7", "[None]")
{
    REQUIRE(opcode_to_string(opcode::push_positive_7, rule_fork::no_rules) == "7");
    REQUIRE(opcode_to_string(opcode::push_positive_7, rule_fork::all_rules) == "7");
}

// Use the traditional serializations for all codes (in this case 'ver' vs. '0x62').
TEST_CASE("opcode  opcode to string  reserved 98 any forks  ver", "[None]")
{
    REQUIRE(opcode_to_string(opcode::reserved_98, rule_fork::no_rules) == "ver");
    REQUIRE(opcode_to_string(opcode::reserved_98, rule_fork::all_rules) == "ver");
}

// Use the traditional serializations for all codes (in this case 'verif' vs. '0x65').
TEST_CASE("opcode  opcode to string  disabled verif any forks  verif", "[None]")
{
    REQUIRE(opcode_to_string(opcode::disabled_verif, rule_fork::no_rules) == "verif");
    REQUIRE(opcode_to_string(opcode::disabled_verif, rule_fork::all_rules) == "verif");
}

// Use the traditional serializations for all codes (in this case 'vernotif' vs. '0x66').
TEST_CASE("opcode  opcode to string  disabled vernotif any forks  vernotif", "[None]")
{
    REQUIRE(opcode_to_string(opcode::disabled_vernotif, rule_fork::no_rules) == "vernotif");
    REQUIRE(opcode_to_string(opcode::disabled_vernotif, rule_fork::all_rules) == "vernotif");
}

// Use the traditional serializations for all codes (in this case '2drop' vs. 'drop2').
TEST_CASE("opcode  opcode to string  drop2 any forks  2drop", "[None]")
{
    REQUIRE(opcode_to_string(opcode::drop2, rule_fork::no_rules) == "2drop");
    REQUIRE(opcode_to_string(opcode::drop2, rule_fork::all_rules) == "2drop");
}

// Use the traditional serializations for all codes (in this case '2dup' vs. 'dup2').
TEST_CASE("opcode  opcode to string  rdup2 any forks  2dup", "[None]")
{
    REQUIRE(opcode_to_string(opcode::dup2, rule_fork::no_rules) == "2dup");
    REQUIRE(opcode_to_string(opcode::dup2, rule_fork::all_rules) == "2dup");
}

// Use the traditional serializations for all codes (in this case '3dup' vs. 'dup3').
TEST_CASE("opcode  opcode to string  dup3 any forks  3dup", "[None]")
{
    REQUIRE(opcode_to_string(opcode::dup3, rule_fork::no_rules) == "3dup");
    REQUIRE(opcode_to_string(opcode::dup3, rule_fork::all_rules) == "3dup");
}

// Use the traditional serializations for all codes (in this case '2over' vs. 'over2').
TEST_CASE("opcode  opcode to string  over2 any forks  vernotif", "[None]")
{
    REQUIRE(opcode_to_string(opcode::over2, rule_fork::no_rules) == "2over");
    REQUIRE(opcode_to_string(opcode::over2, rule_fork::all_rules) == "2over");
}

// Use the traditional serializations for all codes (in this case '2rot' vs. 'rot2').
TEST_CASE("opcode  opcode to string  rot2 any forks  2rot", "[None]")
{
    REQUIRE(opcode_to_string(opcode::rot2, rule_fork::no_rules) == "2rot");
    REQUIRE(opcode_to_string(opcode::rot2, rule_fork::all_rules) == "2rot");
}

// Use the traditional serializations for all codes (in this case '2swap' vs. 'swap2').
TEST_CASE("opcode  opcode to string  swap2 any forks  2swap", "[None]")
{
    REQUIRE(opcode_to_string(opcode::swap2, rule_fork::no_rules) == "2swap");
    REQUIRE(opcode_to_string(opcode::swap2, rule_fork::all_rules) == "2swap");
}

// Use the traditional serializations for all codes (in this case '1add' vs. 'add1').
TEST_CASE("opcode  opcode to string  add1 any forks  2rot", "[None]")
{
    REQUIRE(opcode_to_string(opcode::add1, rule_fork::no_rules) == "1add");
    REQUIRE(opcode_to_string(opcode::add1, rule_fork::all_rules) == "1add");
}

// Use the traditional serializations for all codes (in this case '1sub' vs. 'sub1').
TEST_CASE("opcode  opcode to string  sub1 any forks  2swap", "[None]")
{
    REQUIRE(opcode_to_string(opcode::sub1, rule_fork::no_rules) == "1sub");
    REQUIRE(opcode_to_string(opcode::sub1, rule_fork::all_rules) == "1sub");
}

// Ensure nop2 still serializes as 'nop2' without bip65 fork.
TEST_CASE("opcode  opcode to string  nop2 or checklocktimeverify any fork except bip65  nop2", "[None]")
{
    static_assert(opcode::checklocktimeverify == opcode::nop2, "nop2 drift");
    REQUIRE(opcode_to_string(opcode::nop2, rule_fork::no_rules) == "nop2");
    REQUIRE(opcode_to_string(opcode::nop2, rule_fork::bip16_rule) == "nop2");
    REQUIRE(opcode_to_string(opcode::nop2, rule_fork::bip30_rule) == "nop2");
    REQUIRE(opcode_to_string(opcode::nop2, rule_fork::bip34_rule) == "nop2");
    REQUIRE(opcode_to_string(opcode::nop2, rule_fork::bip66_rule) == "nop2");
    REQUIRE(opcode_to_string(opcode::nop2, rule_fork::bip112_rule) == "nop2");
}

// Ensure nop2 and checklocktimeverify serialize as 'checklocktimeverify' with bip65 fork.
TEST_CASE("opcode  opcode to string  nop2 or checklocktimeverify bip65 fork  checklocktimeverify", "[None]")
{
    static_assert(opcode::checklocktimeverify == opcode::nop2, "nop2 drift");
    REQUIRE(opcode_to_string(opcode::nop2, rule_fork::bip65_rule) == "checklocktimeverify");
    REQUIRE(opcode_to_string(opcode::nop2, rule_fork::all_rules) == "checklocktimeverify");
}

// Ensure nop3 still serializes as 'nop3' without bip112 fork.
TEST_CASE("opcode  opcode to string  nop3 or checksequenceverify any fork except bip112  nop3", "[None]")
{
    static_assert(opcode::checksequenceverify == opcode::nop3, "nop3 drift");
    REQUIRE(opcode_to_string(opcode::nop3, rule_fork::no_rules) == "nop3");
    REQUIRE(opcode_to_string(opcode::nop3, rule_fork::bip16_rule) == "nop3");
    REQUIRE(opcode_to_string(opcode::nop3, rule_fork::bip30_rule) == "nop3");
    REQUIRE(opcode_to_string(opcode::nop3, rule_fork::bip34_rule) == "nop3");
    REQUIRE(opcode_to_string(opcode::nop3, rule_fork::bip66_rule) == "nop3");
    REQUIRE(opcode_to_string(opcode::nop3, rule_fork::bip65_rule) == "nop3");
}

// Ensure nop3 and checksequenceverify serialize as 'checksequenceverify' with bip112 fork.
TEST_CASE("opcode  opcode to string  nop3 or checksequenceverify bip112 fork  checksequenceverify", "[None]")
{
    static_assert(opcode::checksequenceverify == opcode::nop3, "nop3 drift");
    REQUIRE(opcode_to_string(opcode::nop3, rule_fork::bip112_rule) == "checksequenceverify");
    REQUIRE(opcode_to_string(opcode::nop3, rule_fork::all_rules) == "checksequenceverify");
}

// All codes above 'nop10' serialize as hex.
TEST_CASE("opcode  opcode to string  reserved 186 any forks  0xba", "[None]")
{
    REQUIRE(opcode_to_string(opcode::reserved_186, rule_fork::no_rules) == "0xba");
    REQUIRE(opcode_to_string(opcode::reserved_186, rule_fork::all_rules) == "0xba");
}

// All codes above 'nop10' serialize as hex.
TEST_CASE("opcode  opcode to string  reserved 255 any forks  0xff", "[None]")
{
    REQUIRE(opcode_to_string(opcode::reserved_255, rule_fork::no_rules) == "0xff");
    REQUIRE(opcode_to_string(opcode::reserved_255, rule_fork::all_rules) == "0xff");
}

// opcode_from_string
//-----------------------------------------------------------------------------
// Partial coverage includes all aliases and boundaries of each "range" of codes.

// zero

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__zero__push_size_0)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "zero"));
    BOOST_REQUIRE(out_code == opcode::push_size_0);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__push_0__push_size_0)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "push_0"));
    BOOST_REQUIRE(out_code == opcode::push_size_0);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__0__push_size_0)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "0"));
    BOOST_REQUIRE(out_code == opcode::push_size_0);
}

// push n (special)

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__push_1__push_size_1)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "push_1"));
    BOOST_REQUIRE(out_code == opcode::push_size_1);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__push_75__push_size_75)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "push_75"));
    BOOST_REQUIRE(out_code == opcode::push_size_75);
}

// push n byte size (pushdata)

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__push_one__push_one_size)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "push_one"));
    BOOST_REQUIRE(out_code == opcode::push_one_size);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__push_two__push_two_size)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "push_two"));
    BOOST_REQUIRE(out_code == opcode::push_two_size);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__push_four__push_four_size)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "push_four"));
    BOOST_REQUIRE(out_code == opcode::push_four_size);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__pushdata1__push_one_size)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "pushdata1"));
    BOOST_REQUIRE(out_code == opcode::push_one_size);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__pushdata2_push_two_size)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "pushdata2"));
    BOOST_REQUIRE(out_code == opcode::push_two_size);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__pushdata4__push_four_size)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "pushdata4"));
    BOOST_REQUIRE(out_code == opcode::push_four_size);
}

// negative one

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__minus1__push_negative_1)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "-1"));
    BOOST_REQUIRE(out_code == opcode::push_negative_1);
}

// reserved

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__reserved__reserved_80)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "reserved"));
    BOOST_REQUIRE(out_code == opcode::reserved_80);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__reserved_80__reserved_80)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "reserved_80"));
    BOOST_REQUIRE(out_code == opcode::reserved_80);
}

// positive numbers

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__1__push_positive_1)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "1"));
    BOOST_REQUIRE(out_code == opcode::push_positive_1);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__16__push_positive_16)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "16"));
    BOOST_REQUIRE(out_code == opcode::push_positive_16);
}

// ver/verif/vernotif

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__ver__reserved_98)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "ver"));
    BOOST_REQUIRE(out_code == opcode::reserved_98);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__verif__disabled_verif)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "verif"));
    BOOST_REQUIRE(out_code == opcode::disabled_verif);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__vernotif__disabled_vernotif)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "vernotif"));
    BOOST_REQUIRE(out_code == opcode::disabled_vernotif);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__reserved_98__reserved_98)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "reserved_98"));
    BOOST_REQUIRE(out_code == opcode::reserved_98);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__disabled_verif__disabled_verif)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "disabled_verif"));
    BOOST_REQUIRE(out_code == opcode::disabled_verif);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__disabled_vernotif__disabled_vernotif)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "disabled_vernotif"));
    BOOST_REQUIRE(out_code == opcode::disabled_vernotif);
}

// math

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__drop2__drop2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "drop2"));
    BOOST_REQUIRE(out_code == opcode::drop2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__dup2__dup2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "dup2"));
    BOOST_REQUIRE(out_code == opcode::dup2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__dup3__dup3)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "dup3"));
    BOOST_REQUIRE(out_code == opcode::dup3);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__over2__over2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "over2"));
    BOOST_REQUIRE(out_code == opcode::over2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__rot2__rot2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "rot2"));
    BOOST_REQUIRE(out_code == opcode::rot2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__swap2__swap2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "swap2"));
    BOOST_REQUIRE(out_code == opcode::swap2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__add1__add1)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "add1"));
    BOOST_REQUIRE(out_code == opcode::add1);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__sub1__sub1)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "sub1"));
    BOOST_REQUIRE(out_code == opcode::sub1);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__mul2__mul2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "mul2"));
    BOOST_REQUIRE(out_code == opcode::disabled_mul2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__div2__div2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "div2"));
    BOOST_REQUIRE(out_code == opcode::disabled_div2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__2drop__drop2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "2drop"));
    BOOST_REQUIRE(out_code == opcode::drop2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__2dup__dup2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "2dup"));
    BOOST_REQUIRE(out_code == opcode::dup2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__3dup__dup3)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "3dup"));
    BOOST_REQUIRE(out_code == opcode::dup3);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__2over__over2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "2over"));
    BOOST_REQUIRE(out_code == opcode::over2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__2rot__rot2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "2rot"));
    BOOST_REQUIRE(out_code == opcode::rot2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__2swap__swap2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "2swap"));
    BOOST_REQUIRE(out_code == opcode::swap2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__1add__add1)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "1add"));
    BOOST_REQUIRE(out_code == opcode::add1);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__1sub__sub1)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "1sub"));
    BOOST_REQUIRE(out_code == opcode::sub1);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__2mul__mul2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "2mul"));
    BOOST_REQUIRE(out_code == opcode::disabled_mul2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__2div__div2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "2div"));
    BOOST_REQUIRE(out_code == opcode::disabled_div2);
}

// reserved1/reserved2

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__reserved1__reserved_137)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "reserved1"));
    BOOST_REQUIRE(out_code == opcode::reserved_137);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__reserved2__reserved_138)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "reserved2"));
    BOOST_REQUIRE(out_code == opcode::reserved_138);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__disabled_vernotif__reserved_137)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "reserved_137"));
    BOOST_REQUIRE(out_code == opcode::reserved_137);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__disabled_vernotif__reserved_138)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "reserved_138"));
    BOOST_REQUIRE(out_code == opcode::reserved_138);
}

// nop2/checklocktimeverify

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__nop2__nop2)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "nop2"));
    BOOST_REQUIRE(out_code == opcode::nop2);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__checklocktimeverify__nop2_or_checklocktimeverify)
{
    static_assert(opcode::checklocktimeverify == opcode::nop2, "nop2 drift");
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "checklocktimeverify"));
    BOOST_REQUIRE(out_code == opcode::nop2);
}

// nop3/checksequenceverify

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__nop3__nop3)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "nop3"));
    BOOST_REQUIRE(out_code == opcode::nop3);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_string__checksequenceverify__nop3_or_checksequenceverify)
{
    static_assert(opcode::checksequenceverify == opcode::nop3, "nop3 drift");
    opcode out_code;
    BOOST_REQUIRE(opcode_from_string(out_code, "checksequenceverify"));
    BOOST_REQUIRE(out_code == opcode::nop3);
}

// opcode_to_hexadecimal
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(opcode__opcode_to_hexadecimal__zero__0x00)
{
    BOOST_REQUIRE(opcode_to_hexadecimal(opcode::push_size_0) == "0x00");
}

BOOST_AUTO_TEST_CASE(opcode__opcode_to_hexadecimal__push_size_42__0x2a)
{
    BOOST_REQUIRE(opcode_to_hexadecimal(opcode::push_size_42) == "0x2a");
}

BOOST_AUTO_TEST_CASE(opcode__opcode_to_hexadecimal__reserved_255__0xff)
{
    BOOST_REQUIRE(opcode_to_hexadecimal(opcode::reserved_255) == "0xff");
}

// opcode_from_hexadecimal
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__empty__false)
{
    opcode out_code;
    BOOST_REQUIRE(!opcode_from_hexadecimal(out_code, ""));
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__bogus__false)
{
    opcode out_code;
    BOOST_REQUIRE(!opcode_from_hexadecimal(out_code, "bogus"));
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__9_bits__false)
{
    opcode out_code;
    BOOST_REQUIRE(!opcode_from_hexadecimal(out_code, "0x"));
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__8_bits__false)
{
    opcode out_code;
    BOOST_REQUIRE(!opcode_from_hexadecimal(out_code, "0xf"));
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__16_bits__expected)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_hexadecimal(out_code, "0xff"));
    BOOST_REQUIRE(out_code == opcode::reserved_255);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__24_bits__false)
{
    opcode out_code;
    BOOST_REQUIRE(!opcode_from_hexadecimal(out_code, "0xffe"));
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__48_bits__false)
{
    opcode out_code;
    BOOST_REQUIRE(!opcode_from_hexadecimal(out_code, "0xffee"));
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__upper_case__expected)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_hexadecimal(out_code, "0xFE"));
    BOOST_REQUIRE(out_code == opcode::reserved_254);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__mixed_case__expected)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_hexadecimal(out_code, "0xFe"));
    BOOST_REQUIRE(out_code == opcode::reserved_254);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__numeric__expected)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_hexadecimal(out_code, "0x42"));
    BOOST_REQUIRE(out_code == opcode::push_size_66);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__alphanumeric__expected)
{
    opcode out_code;
    BOOST_REQUIRE(opcode_from_hexadecimal(out_code, "0x4f"));
    BOOST_REQUIRE(out_code == opcode::push_negative_1);
}

BOOST_AUTO_TEST_CASE(opcode__opcode_from_hexadecimal__upper_case_prefix__false)
{
    opcode out_code;
    BOOST_REQUIRE(!opcode_from_hexadecimal(out_code, "0X4f"));
}

BOOST_AUTO_TEST_SUITE_END()

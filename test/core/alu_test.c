#include "gbemutest/core/alu_test.h"
#include "gbemu/core/alu.h"
#include "test/unit.h"

BEFORE_ALL({})
AFTER_ALL({})

TEST(addition, "Should sum values", {
  alu_result_t simple_sum = alu_add(2, 1);

  ASSERT_EQ_NUM(3, simple_sum.result);
  ASSERT_FALSE(simple_sum.has_carry);
  ASSERT_FALSE(simple_sum.has_half_carry);

  alu_result_t half_carry_sum = alu_add(0x0F, 0x01);

  ASSERT_EQ_HEX(0x10, half_carry_sum.result);
  ASSERT_FALSE(half_carry_sum.has_carry);
  ASSERT_TRUE(half_carry_sum.has_half_carry);

  alu_result_t carry_sum = alu_add(0xFF, 0x01);

  ASSERT_EQ_HEX(0x00, carry_sum.result);
  ASSERT_TRUE(carry_sum.has_carry);
  ASSERT_TRUE(carry_sum.has_half_carry);
})

TEST(subtract, "Should subtract values", {
  alu_result_t simple_subtract = alu_sub(2, 1);

  ASSERT_EQ_NUM(1, simple_subtract.result);
  ASSERT_FALSE(simple_subtract.has_carry);
  ASSERT_FALSE(simple_subtract.has_half_carry);

  alu_result_t half_carry_subtract = alu_sub(0x10, 0x0F);

  ASSERT_EQ_HEX(0x01, half_carry_subtract.result);
  ASSERT_FALSE(half_carry_subtract.has_carry);
  ASSERT_TRUE(half_carry_subtract.has_half_carry);

  alu_result_t carry_subtract = alu_sub(0x10, 0x21);

  ASSERT_EQ_HEX(0xEF, carry_subtract.result);
  ASSERT_TRUE(carry_subtract.has_carry);
  ASSERT_TRUE(carry_subtract.has_half_carry);
})

TEST(bitwise_AND, "Should execute bitwise AND in values", {
  alu_result_t and_bitwise = alu_and(0b11111111, 0b10001101);

  ASSERT_EQ_BINARY(0b10001101, and_bitwise.result);
  ASSERT_FALSE(and_bitwise.has_carry);
  ASSERT_FALSE(and_bitwise.has_half_carry);
})

TEST(bitwise_OR, "Should execute bitwise OR in values", {
  alu_result_t or_bitwise = alu_or(0b01010000, 0b10001101);

  ASSERT_EQ_BINARY(0b11011101, or_bitwise.result);
  ASSERT_FALSE(or_bitwise.has_carry);
  ASSERT_FALSE(or_bitwise.has_half_carry);
})

TEST(bitwise_XOR, "Should execute bitwise XOR in values", {
  alu_result_t or_bitwise = alu_xor(0b01011100, 0b10001101);

  ASSERT_EQ_BINARY(0b11010001, or_bitwise.result);
  ASSERT_FALSE(or_bitwise.has_carry);
  ASSERT_FALSE(or_bitwise.has_half_carry);
})

TEST(rotate_left, "Should rotate bits position to left", {
  alu_result_t rotate_left_first_case = alu_rotate_left(0b01011100);

  ASSERT_EQ_BINARY(0b10111000, rotate_left_first_case.result);
  ASSERT_FALSE(rotate_left_first_case.has_carry);
  ASSERT_FALSE(rotate_left_first_case.has_half_carry);

  alu_result_t rotate_left_second_case = alu_rotate_left(0b10111000);

  ASSERT_EQ_BINARY(0b01110000, rotate_left_second_case.result);
  ASSERT_TRUE(rotate_left_second_case.has_carry);
  ASSERT_FALSE(rotate_left_second_case.has_half_carry);
})

TEST(rotate_left_circular, "Should cicle rotate bits position to left", {
  alu_result_t rotate_left_first_case = alu_rotate_left_circular(0b01011100);

  ASSERT_EQ_BINARY(0b10111000, rotate_left_first_case.result);
  ASSERT_FALSE(rotate_left_first_case.has_carry);
  ASSERT_FALSE(rotate_left_first_case.has_half_carry);

  alu_result_t rotate_left_second_case = alu_rotate_left_circular(0b10111000);

  ASSERT_EQ_BINARY(0b01110001, rotate_left_second_case.result);
  ASSERT_TRUE(rotate_left_second_case.has_carry);
  ASSERT_FALSE(rotate_left_second_case.has_half_carry);
})

TEST(rotate_right, "Should rotate bits position to right", {
  alu_result_t rotate_right_first_case = alu_rotate_right(0b01011100);

  ASSERT_EQ_BINARY(0b00101110, rotate_right_first_case.result);
  ASSERT_FALSE(rotate_right_first_case.has_carry);
  ASSERT_FALSE(rotate_right_first_case.has_half_carry);

  alu_result_t rotate_right_second_case = alu_rotate_right(0b10111001);

  ASSERT_EQ_BINARY(0b01011100, rotate_right_second_case.result);
  ASSERT_TRUE(rotate_right_second_case.has_carry);
  ASSERT_FALSE(rotate_right_second_case.has_half_carry);
})

TEST(rotate_right_circular, "Should cicle rotate bits position to right", {
  alu_result_t rotate_right_first_case = alu_rotate_right_circular(0b01011100);

  ASSERT_EQ_BINARY(0b00101110, rotate_right_first_case.result);
  ASSERT_FALSE(rotate_right_first_case.has_carry);
  ASSERT_FALSE(rotate_right_first_case.has_half_carry);

  alu_result_t rotate_right_second_case = alu_rotate_right_circular(0b10111001);

  ASSERT_EQ_BINARY(0b11011100, rotate_right_second_case.result);
  ASSERT_TRUE(rotate_right_second_case.has_carry);
  ASSERT_FALSE(rotate_right_second_case.has_half_carry);
})

RUN_SUITE(
  arithetic_logic_unit,
  test_addition,
  test_subtract,
  test_bitwise_AND,
  test_bitwise_OR,
  test_bitwise_XOR,
  test_rotate_left,
  test_rotate_left_circular,
  test_rotate_right,
  test_rotate_right_circular,
)

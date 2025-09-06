#include "gbemu/core/alu.h"

#define LSB(value) (value & 0x0F)
#define MAX_8BIT_VALUE 0xFF
#define MAX_4BIT_VALUE 0xF

alu_result_t alu_add(uint8_t input1, uint8_t input2) {
  alu_result_t alu_result;
  alu_result.result = input1 + input2;
  alu_result.has_carry = (input1 + input2) > MAX_8BIT_VALUE;
  alu_result.has_half_carry = (LSB(input1) + LSB(input2)) > MAX_4BIT_VALUE;
  return alu_result;
}

alu_result_t alu_sub(uint8_t input1, uint8_t input2) {
  alu_result_t alu_result;
  alu_result.result = input1 - input2;
  alu_result.has_carry = input1 < input2;
  alu_result.has_half_carry = LSB(input1) < LSB(input2);
  return alu_result;
}

alu_result_t alu_and(uint8_t input1, uint8_t input2) {
  alu_result_t alu_result;
  alu_result.result = input1 & input2;
  alu_result.has_carry = false;
  alu_result.has_half_carry = false;
  return alu_result;
}

alu_result_t alu_or(uint8_t input1, uint8_t input2) {
  alu_result_t alu_result;
  alu_result.result = input1 | input2;
  alu_result.has_carry = false;
  alu_result.has_half_carry = false;
  return alu_result;
}

alu_result_t alu_xor(uint8_t input1, uint8_t input2) {
  alu_result_t alu_result;
  alu_result.result = input1 ^ input2;
  alu_result.has_carry = false;
  alu_result.has_half_carry = false;
  return alu_result;
}

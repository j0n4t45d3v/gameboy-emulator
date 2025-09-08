#pragma once

#include <stdint.h>
#include <stdbool.h>

#define LAST_BITE(byte) (byte & 0x01) 
#define FIRST_BITE(byte) (byte & 0b10000000) >> 7 

typedef struct {
  uint8_t result;
  bool has_carry;
  bool has_half_carry;
} alu_result_t; 

//ARITHMETIC
alu_result_t alu_add(uint8_t input1, uint8_t input2); 
alu_result_t alu_sub(uint8_t input1, uint8_t input2); 
alu_result_t alu_and(uint8_t input1, uint8_t input2); 
alu_result_t alu_or(uint8_t input1, uint8_t input2); 
alu_result_t alu_xor(uint8_t input1, uint8_t input2); 

//ROTATION
alu_result_t alu_rotate_left(uint8_t input); 
alu_result_t alu_rotate_left_circular(uint8_t input); 
alu_result_t alu_rotate_right(uint8_t input); 
alu_result_t alu_rotate_right_circular(uint8_t input); 

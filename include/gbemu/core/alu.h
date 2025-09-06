#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint8_t result;
  bool has_carry;
  bool has_half_carry;
} alu_result_t; 

alu_result_t alu_add(uint8_t input1, uint8_t input2); 
alu_result_t alu_sub(uint8_t input1, uint8_t input2); 
alu_result_t alu_and(uint8_t input1, uint8_t input2); 
alu_result_t alu_or(uint8_t input1, uint8_t input2); 
alu_result_t alu_xor(uint8_t input1, uint8_t input2); 

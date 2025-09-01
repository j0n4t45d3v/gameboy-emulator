#ifndef SM83_H
#define SM83_H

#include "gbemu/core/bus.h"
#include <stdint.h>

#define MASK_MSB_8BITS 0xF0
#define MASK_LSB_8BITS 0x0F
#define SM83_FREQUECY 4.194304
#define XOR(r1, r2) r1 ^ r2
#define HAS_HALF_CARRY_IN_SUB(r1, value) (r1 & MASK_LSB_8BITS) < value
#define HAS_HALF_CARRY_IN_ADD(r1, r2) ((r1 & MASK_LSB_8BITS) + (r2 & MASK_LSB_8BITS)) > 0x0F

#define Z_FLAG(flags) (cpu->AF.lsb & 0b10000000)
#define N_FLAG(flags) (cpu->AF.lsb & 0b01000000)
#define H_FLAG(flags) (cpu->AF.lsb & 0b00100000)
#define C_FLAG(flags) (cpu->AF.lsb & 0b00010000)

#define Z_FLAG_VALUE(flags) Z_FLAG(flags) >> 7
#define N_FLAG_VALUE(flags) N_FLAG(flags) >> 6
#define H_FLAG_VALUE(flags) H_FLAG(flags) >> 5
#define C_FLAG_VALUE(flags) C_FLAG(flags) >> 4

// REGISTERS 16 Bits

typedef union {
  struct {
    uint8_t lsb;
    uint8_t msb;
  };
  uint16_t value;
} reg16_t;

// CPU 
typedef struct sm83 {
  reg16_t IR;
  reg16_t IE;

  reg16_t AF;
  reg16_t BC;
  reg16_t DE;
  reg16_t HL;
  reg16_t SP;
  reg16_t PC;

  uint8_t clocks;
  uint8_t opcode;
} sm83_t;

typedef uint8_t (*execute_t)(sm83_t*, bus_t*);

#define NUMBER_OF_THE_INSTRUCTIONS 0x0100

sm83_t* init();
void clock(sm83_t*, bus_t*);
uint8_t fetch(sm83_t*, bus_t*);
uint8_t execute(sm83_t*, bus_t*);

// INSTRUCTIONS
uint8_t NOP(sm83_t*, bus_t*);

uint8_t XXX(sm83_t*, bus_t*);

uint8_t LD_rr_nn(sm83_t*, bus_t*);
uint8_t LD_rr_A(sm83_t*, bus_t*);
uint8_t LD_A_rr(sm83_t*, bus_t*);
uint8_t LD_nn_A(sm83_t*, bus_t*);
uint8_t LD_A_nn(sm83_t*, bus_t*);

uint8_t LD_r_r(sm83_t*, bus_t*);
uint8_t LD_r_n(sm83_t*, bus_t*);

uint8_t LD_r_HL(sm83_t*, bus_t*);
uint8_t LD_HL_r(sm83_t*, bus_t*);
uint8_t LD_HL_n(sm83_t*, bus_t*);

uint8_t LDH_n_A(sm83_t*, bus_t*);
uint8_t LDH_A_n(sm83_t*, bus_t*);

//ARITMETIC 16 bits
uint8_t INC_rr(sm83_t*, bus_t*);

#endif // !SM83_H

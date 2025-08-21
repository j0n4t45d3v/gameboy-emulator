#ifndef SM83_H
#define SM83_H

#include "gbemu/core/bus.h"
#include <stdint.h>

#define MASK_MSB_8BITS 0xF0

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


#endif // !SM83_H

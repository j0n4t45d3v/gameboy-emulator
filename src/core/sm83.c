#include "gbemu/core/sm83.h"
#include "gbemu/core/bus.h"
#include <stdint.h>
#include <stdlib.h>

const execute_t DECODER[NUMBER_OF_THE_INSTRUCTIONS] = {
    [0x00] = NOP,
    /* LOADs 16bits */
    [0x01] = LD_rr_nn,
    [0x11] = LD_rr_nn,
    [0x21] = LD_rr_nn,
    [0x31] = LD_rr_nn,

    /* LOADs 8bits */
    [0x02] = LD_rr_A,
    [0x12] = LD_rr_A,
    [0x22] = LD_rr_A,
    [0x32] = LD_rr_A,

    [0x40] = LD_r_r,
    [0x50] = LD_r_r,
    [0x60] = LD_r_r,
    [0x41] = LD_r_r,
    [0x51] = LD_r_r,
    [0x61] = LD_r_r,
    [0x42] = LD_r_r,
    [0x52] = LD_r_r,
    [0x62] = LD_r_r,
    [0x43] = LD_r_r,
    [0x53] = LD_r_r,
    [0x63] = LD_r_r,
    [0x44] = LD_r_r,
    [0x54] = LD_r_r,
    [0x64] = LD_r_r,
    [0x45] = LD_r_r,
    [0x55] = LD_r_r,
    [0x65] = LD_r_r,
    [0x47] = LD_r_r,
    [0x57] = LD_r_r,
    [0x67] = LD_r_r,
    [0x48] = LD_r_r,
    [0x58] = LD_r_r,
    [0x68] = LD_r_r,
    [0x78] = LD_r_r,
    [0x49] = LD_r_r,
    [0x59] = LD_r_r,
    [0x69] = LD_r_r,
    [0x79] = LD_r_r,
    [0x4A] = LD_r_r,
    [0x5A] = LD_r_r,
    [0x6A] = LD_r_r,
    [0x7A] = LD_r_r,
    [0x4B] = LD_r_r,
    [0x5B] = LD_r_r,
    [0x6B] = LD_r_r,
    [0x7B] = LD_r_r,
    [0x4C] = LD_r_r,
    [0x5C] = LD_r_r,
    [0x6C] = LD_r_r,
    [0x7C] = LD_r_r,
    [0x4D] = LD_r_r,
    [0x5D] = LD_r_r,
    [0x6D] = LD_r_r,
    [0x7D] = LD_r_r,
    [0x4F] = LD_r_r,
    [0x5F] = LD_r_r,
    [0x6F] = LD_r_r,
    [0x7F] = LD_r_r,

    [0x06] = LD_r_n,
    [0x16] = LD_r_n,
    [0x26] = LD_r_n,
    [0x0E] = LD_r_n,
    [0x1E] = LD_r_n,
    [0x2E] = LD_r_n,
    [0x3E] = LD_r_n,

    [0x0A] = LD_A_rr,
    [0x1A] = LD_A_rr,
    [0x2A] = LD_A_rr,
    [0x3A] = LD_A_rr,

    [0x46] = LD_r_HL,
    [0x56] = LD_r_HL,
    [0x66] = LD_r_HL,
    [0x4E] = LD_r_HL,
    [0x5E] = LD_r_HL,
    [0x6E] = LD_r_HL,
    [0x7E] = LD_r_HL,
};

sm83_t *init() {
  sm83_t *cpu = malloc(sizeof(sm83_t));
  cpu->IE.value = 0x0000;
  cpu->IR.value = 0x0000;

  cpu->AF.value = 0x0000;
  cpu->BC.value = 0x0000;
  cpu->DE.value = 0x0000;
  cpu->HL.value = 0x0000;
  cpu->PC.value = 0x0000;
  cpu->SP.value = 0x0000;
  return cpu;
}

void clock(sm83_t *cpu, bus_t *busAddr) {
  cpu->clocks += fetch(cpu, busAddr);
  cpu->clocks += execute(cpu, busAddr);
}

uint8_t fetch(sm83_t *cpu, bus_t *busAddr) {
  cpu->opcode = read_bus(busAddr, cpu->PC.value++);
  return 1;
}

uint8_t execute(sm83_t *cpu, bus_t *busAddr) {
  return DECODER[cpu->opcode](cpu, busAddr);
}

/*
x=============================================x
  INSTRUNCTIONS FUNCTIONS

  NOP - No Operation

 LOADER:
   - LD rr, nn
x=============================================x
*/
uint8_t NOP(sm83_t *cpu, bus_t *busAddr) {
  return 1;
}

uint8_t XXX(sm83_t *cpu, bus_t *busAddr) {
  return 0;
}

// LOADERS
uint8_t LD_rr_nn(sm83_t *cpu, bus_t *busAddr) {
  reg16_t *regs[4] = {&cpu->BC, &cpu->DE, &cpu->HL, &cpu->SP};
  uint8_t reg_index = (cpu->opcode & MASK_MSB_8BITS) >> 4;
  if (reg_index < 4) {
    reg16_t *reg = regs[reg_index];
    reg->lsb = read_bus(busAddr, cpu->PC.value++);
    reg->msb = read_bus(busAddr, cpu->PC.value++);
  }
  return 3;
}

uint8_t LD_rr_A(sm83_t *cpu, bus_t *busAddr) {
  switch (cpu->opcode) {
  case 0x02:
    write_bus(busAddr, cpu->BC.value, cpu->AF.msb);
    break;
  case 0x12:
    write_bus(busAddr, cpu->DE.value, cpu->AF.msb);
    break;
  case 0x22:
    write_bus(busAddr, cpu->HL.value++, cpu->AF.msb);
    break;
  case 0x32:
    write_bus(busAddr, cpu->HL.value--, cpu->AF.msb);
    break;
  }
  return 2;
}

uint8_t LD_A_rr(sm83_t *cpu, bus_t *busAddr) {
  switch (cpu->opcode) {
  case 0x0A:
    cpu->AF.msb = read_bus(busAddr, cpu->BC.value);
    break;
  case 0x1A:
    cpu->AF.msb = read_bus(busAddr, cpu->DE.value);
    break;
  case 0x2A:
    cpu->AF.msb = read_bus(busAddr, cpu->HL.value++);
    break;
  case 0x3A:
    cpu->AF.msb = read_bus(busAddr, cpu->HL.value--);
    break;
  }
  return 2;
}

uint8_t LD_r_r(sm83_t *cpu, bus_t *busAddr) {
  switch (cpu->opcode) {
  case 0x40:
    cpu->BC.msb = cpu->BC.msb;
    break;
  case 0x50:
    cpu->DE.msb = cpu->BC.msb;
    break;
  case 0x60:
    cpu->HL.msb = cpu->BC.msb;
    break;
  case 0x41:
    cpu->BC.msb = cpu->BC.lsb;
    break;
  case 0x51:
    cpu->DE.msb = cpu->BC.lsb;
    break;
  case 0x61:
    cpu->HL.msb = cpu->BC.lsb;
    break;

  case 0x42:
    cpu->BC.msb = cpu->DE.msb;
    break;
  case 0x52:
    cpu->DE.msb = cpu->DE.msb;
    break;
  case 0x62:
    cpu->HL.msb = cpu->DE.msb;
    break;
  case 0x43:
    cpu->BC.msb = cpu->DE.lsb;
    break;
  case 0x53:
    cpu->DE.msb = cpu->DE.lsb;
    break;
  case 0x63:
    cpu->HL.msb = cpu->DE.lsb;
    break;

  case 0x44:
    cpu->BC.msb = cpu->HL.msb;
    break;
  case 0x54:
    cpu->DE.msb = cpu->HL.msb;
    break;
  case 0x64:
    cpu->HL.msb = cpu->HL.msb;
    break;
  case 0x45:
    cpu->BC.msb = cpu->HL.lsb;
    break;
  case 0x55:
    cpu->DE.msb = cpu->HL.lsb;
    break;
  case 0x65:
    cpu->HL.msb = cpu->HL.lsb;

  case 0x47:
    cpu->BC.msb = cpu->AF.msb;
    break;
  case 0x57:
    cpu->DE.msb = cpu->AF.msb;
    break;
  case 0x67:
    cpu->HL.msb = cpu->AF.msb;
    break;

  case 0x48:
    cpu->BC.lsb = cpu->BC.msb;
    break;
  case 0x58:
    cpu->DE.lsb = cpu->BC.msb;
    break;
  case 0x68:
    cpu->HL.lsb = cpu->BC.msb;
    break;
  case 0x78:
    cpu->AF.msb = cpu->BC.msb;
    break;

  case 0x49:
    cpu->BC.lsb = cpu->BC.lsb;
    break;
  case 0x59:
    cpu->DE.lsb = cpu->BC.lsb;
    break;
  case 0x69:
    cpu->HL.lsb = cpu->BC.lsb;
    break;
  case 0x79:
    cpu->AF.msb = cpu->BC.lsb;
    break;

  case 0x4A:
    cpu->BC.lsb = cpu->DE.msb;
    break;
  case 0x5A:
    cpu->DE.lsb = cpu->DE.msb;
    break;
  case 0x6A:
    cpu->HL.lsb = cpu->DE.msb;
    break;
  case 0x7A:
    cpu->AF.msb = cpu->DE.msb;
    break;

  case 0x4B:
    cpu->BC.lsb = cpu->DE.lsb;
    break;
  case 0x5B:
    cpu->DE.lsb = cpu->DE.lsb;
    break;
  case 0x6B:
    cpu->HL.lsb = cpu->DE.lsb;
    break;
  case 0x7B:
    cpu->AF.msb = cpu->DE.lsb;
    break;

  case 0x4C:
    cpu->BC.lsb = cpu->HL.msb;
    break;
  case 0x5C:
    cpu->DE.lsb = cpu->HL.msb;
    break;
  case 0x6C:
    cpu->HL.lsb = cpu->HL.msb;
    break;
  case 0x7C:
    cpu->AF.msb = cpu->HL.msb;
    break;

  case 0x4D:
    cpu->BC.lsb = cpu->HL.lsb;
    break;
  case 0x5D:
    cpu->DE.lsb = cpu->HL.lsb;
    break;
  case 0x6D:
    cpu->HL.lsb = cpu->HL.lsb;
    break;
  case 0x7D:
    cpu->AF.msb = cpu->HL.lsb;
    break;

  case 0x4F:
    cpu->BC.lsb = cpu->AF.msb;
    break;
  case 0x5F:
    cpu->DE.lsb = cpu->AF.msb;
    break;
  case 0x6F:
    cpu->HL.lsb = cpu->AF.msb;
    break;
  case 0x7F:
    cpu->AF.msb = cpu->AF.msb;
    break;
  }
  return 1;
}

uint8_t LD_r_n(sm83_t *cpu, bus_t *busAddr) {
  switch (cpu->opcode) {
  case 0x06:
    cpu->BC.msb = read_bus(busAddr, cpu->PC.value++);
    break;
  case 0x16:
    cpu->DE.msb = read_bus(busAddr, cpu->PC.value++);
    break;
  case 0x26:
    cpu->HL.msb = read_bus(busAddr, cpu->PC.value++);
    break;

  case 0x0E:
    cpu->BC.lsb = read_bus(busAddr, cpu->PC.value++);
    break;
  case 0x1E:
    cpu->DE.lsb = read_bus(busAddr, cpu->PC.value++);
    break;
  case 0x2E:
    cpu->HL.lsb = read_bus(busAddr, cpu->PC.value++);
    break;
  case 0x3E:
    cpu->AF.msb = read_bus(busAddr, cpu->PC.value++);
    break;
  }
  return 2;
}

uint8_t LD_r_HL(sm83_t *cpu, bus_t *busAddr) {
  uint8_t value = read_bus(busAddr, cpu->HL.value);
  switch (cpu->opcode) {
  case 0x46:
    cpu->BC.msb = value;
    break;
  case 0x56:
    cpu->DE.msb = value;
    break;
  case 0x66:
    cpu->HL.msb = value;

  case 0x4E:
    cpu->BC.lsb = value;
    break;
  case 0x5E:
    cpu->DE.lsb = value;
    break;
  case 0x6E:
    cpu->HL.lsb = value;
    break;
  case 0x7E:
    cpu->AF.msb = value;
    break;
  }
  return 2;
}

uint8_t LD_HL_r(sm83_t *cpu, bus_t *busAddr) {
  switch (cpu->opcode) {
    case 0x36:

    break;
  }
  return 2;
}

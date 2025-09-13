#include "gbemu/core/sm83.h"
#include "gbemu/common/bitwise.h"
#include "gbemu/common/logger.h"
#include "gbemu/core/bus.h"
#include <stdint.h>
#include <stdlib.h>

const instruction_t DECODER[NUMBER_OF_THE_INSTRUCTIONS] = {
    [0x00] = INSTRUCTION(NOP),
    /* LOADs 16bits */
    [0x01] = INSTRUCTION(LD_rr_nn),
    [0x11] = INSTRUCTION(LD_rr_nn),
    [0x21] = INSTRUCTION(LD_rr_nn),
    [0x31] = INSTRUCTION(LD_rr_nn),

    /* LOADs 8bits */
    [0x02] = INSTRUCTION(LD_rr_A),
    [0x12] = INSTRUCTION(LD_rr_A),
    [0x22] = INSTRUCTION(LD_rr_A),
    [0x32] = INSTRUCTION(LD_rr_A),

    [0x40] = INSTRUCTION(LD_r_r),
    [0x50] = INSTRUCTION(LD_r_r),
    [0x60] = INSTRUCTION(LD_r_r),
    [0x41] = INSTRUCTION(LD_r_r),
    [0x51] = INSTRUCTION(LD_r_r),
    [0x61] = INSTRUCTION(LD_r_r),
    [0x42] = INSTRUCTION(LD_r_r),
    [0x52] = INSTRUCTION(LD_r_r),
    [0x62] = INSTRUCTION(LD_r_r),
    [0x43] = INSTRUCTION(LD_r_r),
    [0x53] = INSTRUCTION(LD_r_r),
    [0x63] = INSTRUCTION(LD_r_r),
    [0x44] = INSTRUCTION(LD_r_r),
    [0x54] = INSTRUCTION(LD_r_r),
    [0x64] = INSTRUCTION(LD_r_r),
    [0x45] = INSTRUCTION(LD_r_r),
    [0x55] = INSTRUCTION(LD_r_r),
    [0x65] = INSTRUCTION(LD_r_r),
    [0x47] = INSTRUCTION(LD_r_r),
    [0x57] = INSTRUCTION(LD_r_r),
    [0x67] = INSTRUCTION(LD_r_r),
    [0x48] = INSTRUCTION(LD_r_r),
    [0x58] = INSTRUCTION(LD_r_r),
    [0x68] = INSTRUCTION(LD_r_r),
    [0x78] = INSTRUCTION(LD_r_r),
    [0x49] = INSTRUCTION(LD_r_r),
    [0x59] = INSTRUCTION(LD_r_r),
    [0x69] = INSTRUCTION(LD_r_r),
    [0x79] = INSTRUCTION(LD_r_r),
    [0x4A] = INSTRUCTION(LD_r_r),
    [0x5A] = INSTRUCTION(LD_r_r),
    [0x6A] = INSTRUCTION(LD_r_r),
    [0x7A] = INSTRUCTION(LD_r_r),
    [0x4B] = INSTRUCTION(LD_r_r),
    [0x5B] = INSTRUCTION(LD_r_r),
    [0x6B] = INSTRUCTION(LD_r_r),
    [0x7B] = INSTRUCTION(LD_r_r),
    [0x4C] = INSTRUCTION(LD_r_r),
    [0x5C] = INSTRUCTION(LD_r_r),
    [0x6C] = INSTRUCTION(LD_r_r),
    [0x7C] = INSTRUCTION(LD_r_r),
    [0x4D] = INSTRUCTION(LD_r_r),
    [0x5D] = INSTRUCTION(LD_r_r),
    [0x6D] = INSTRUCTION(LD_r_r),
    [0x7D] = INSTRUCTION(LD_r_r),
    [0x4F] = INSTRUCTION(LD_r_r),
    [0x5F] = INSTRUCTION(LD_r_r),
    [0x6F] = INSTRUCTION(LD_r_r),
    [0x7F] = INSTRUCTION(LD_r_r),

    [0x06] = INSTRUCTION(LD_r_n),
    [0x16] = INSTRUCTION(LD_r_n),
    [0x26] = INSTRUCTION(LD_r_n),
    [0x0E] = INSTRUCTION(LD_r_n),
    [0x1E] = INSTRUCTION(LD_r_n),
    [0x2E] = INSTRUCTION(LD_r_n),
    [0x3E] = INSTRUCTION(LD_r_n),

    [0x0A] = INSTRUCTION(LD_A_rr),
    [0x1A] = INSTRUCTION(LD_A_rr),
    [0x2A] = INSTRUCTION(LD_A_rr),
    [0x3A] = INSTRUCTION(LD_A_rr),

    [0x46] = INSTRUCTION(LD_r_HL),
    [0x56] = INSTRUCTION(LD_r_HL),
    [0x66] = INSTRUCTION(LD_r_HL),
    [0x4E] = INSTRUCTION(LD_r_HL),
    [0x5E] = INSTRUCTION(LD_r_HL),
    [0x6E] = INSTRUCTION(LD_r_HL),
    [0x7E] = INSTRUCTION(LD_r_HL),

    [0x36] = INSTRUCTION(LD_HL_n),

    [0x70] = INSTRUCTION(LD_HL_r),
    [0x71] = INSTRUCTION(LD_HL_r),
    [0x72] = INSTRUCTION(LD_HL_r),
    [0x73] = INSTRUCTION(LD_HL_r),
    [0x74] = INSTRUCTION(LD_HL_r),
    [0x75] = INSTRUCTION(LD_HL_r),
    [0x77] = INSTRUCTION(LD_HL_r), 

    [0xEA] = INSTRUCTION(LD_nn_A),
    [0xFA] = INSTRUCTION(LD_A_nn),

    [0xE0] = INSTRUCTION(LDH_n_A),
    [0xF0] = INSTRUCTION(LDH_A_n),

    // ARITHMETIC 16 BITS
    [0x03] = INSTRUCTION(INC_rr),
    [0x13] = INSTRUCTION(INC_rr),
    [0x23] = INSTRUCTION(INC_rr),
    [0x33] = INSTRUCTION(INC_rr),

    // ARITHMETIC 8 BITS
    [0xA8] = INSTRUCTION(XOR_r),
    [0xA9] = INSTRUCTION(XOR_r),
    [0xAA] = INSTRUCTION(XOR_r),
    [0xAB] = INSTRUCTION(XOR_r),
    [0xAC] = INSTRUCTION(XOR_r),
    [0xAD] = INSTRUCTION(XOR_r),
    [0xAF] = INSTRUCTION(XOR_r),

    [0x05] = INSTRUCTION(DEC_r),
    [0x15] = INSTRUCTION(DEC_r),
    [0x25] = INSTRUCTION(DEC_r),
    [0x0D] = INSTRUCTION(DEC_r),
    [0x1D] = INSTRUCTION(DEC_r),
    [0x2D] = INSTRUCTION(DEC_r),
    [0x3D] = INSTRUCTION(DEC_r),

    [0x04] = INSTRUCTION(INC_r),
    [0x14] = INSTRUCTION(INC_r),
    [0x24] = INSTRUCTION(INC_r),
    [0x0C] = INSTRUCTION(INC_r),
    [0x1C] = INSTRUCTION(INC_r),
    [0x2C] = INSTRUCTION(INC_r),
    [0x3C] = INSTRUCTION(INC_r),


    [0x80] = INSTRUCTION(ADD_r),
  //
    //FLOW CONTROLL
    [0xC3] = INSTRUCTION(JP_nn),
    [0x20] = INSTRUCTION(JP_cc_e),
    [0x30] = INSTRUCTION(JP_cc_e),
    [0x28] = INSTRUCTION(JP_cc_e),
    [0x38] = INSTRUCTION(JP_cc_e),

    [0xF3] = INSTRUCTION(DI),

    [0xCF] = INSTRUCTION(RST_n), 
    [0xDF] = INSTRUCTION(RST_n),
    [0xEF] = INSTRUCTION(RST_n),
    [0xFF] = INSTRUCTION(RST_n),
    [0xC9] = INSTRUCTION(RET),
    [0xCD] = INSTRUCTION(CALL_nn),
    [0xCC] = INSTRUCTION(CALL_cc_nn), 
    [0xDC] = INSTRUCTION(CALL_cc_nn),
    [0xC4] = INSTRUCTION(CALL_cc_nn),
    [0xD4] = INSTRUCTION(CALL_cc_nn), 

    //ROTATES, SHIFTS, AND BIT OPERATIONS
    [0x0F] = INSTRUCTION(RRCA),
    [0x07] = INSTRUCTION(RLCA),
};

sm83_t *init() {
  sm83_t *cpu = malloc(sizeof(sm83_t));
  cpu->IE.value = 0x0000;
  cpu->IR.value = 0x0000;

  cpu->AF.value = 0x01B0;
  cpu->BC.value = 0x0000;
  cpu->DE.value = 0x0000;
  cpu->HL.value = 0x0000;
  cpu->PC.value = 0x0100;
  cpu->SP.value = 0xFFFE;
  return cpu;
}

void clock(sm83_t *cpu, bus_t *busAddr) {
  fetch(cpu, busAddr);
  cpu->clocks = execute(cpu, busAddr);
}

uint8_t fetch(sm83_t *cpu, bus_t *busAddr) {
  cpu->opcode = read_bus(busAddr, cpu->PC.value++);
  LOG_INFO("CURRENT OPCODE: 0X%02X", cpu->opcode);
  return 1;
}

uint8_t execute(sm83_t *cpu, bus_t *busAddr) {
  instruction_t instruction = DECODER[cpu->opcode];
  if(instruction.fn == NULL) {
    LOG_WARN("OPCODE UNIMPLEMENTED");
    return 0;
  }

  LOG_INFO("EXECUTE INSTRUCTION: %s", instruction.name);
  return instruction.fn(cpu, busAddr);
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

uint8_t LD_HL_n(sm83_t *cpu, bus_t *busAddr) {
  uint8_t value = read_bus(busAddr, cpu->PC.value);
  write_bus(busAddr, cpu->HL.value, value);
  return 3;
}

#define NUM_ACTIONS 8
#define MASK_LSB_8BIT 0x0F

uint8_t LD_HL_r(sm83_t *cpu, bus_t *busAddr) {
  uint8_t data[NUM_ACTIONS] = {
    [0x00] = cpu->BC.msb,
    [0x01] = cpu->BC.lsb,
    [0x02] = cpu->DE.msb,
    [0x03] = cpu->DE.lsb,
    [0x04] = cpu->HL.msb,
    [0x05] = cpu->HL.lsb,
    [0x07] = cpu->AF.msb
  };

  uint8_t index = cpu->opcode & MASK_LSB_8BIT;
  if(index >= 0 && index < NUM_ACTIONS && index != 0x06) {
    write_bus(busAddr, cpu->HL.value, data[index]);
  }
  return 2;
}

uint8_t LD_nn_A(sm83_t *cpu, bus_t *busAddr) {
  uint8_t nn_lsb = read_bus(busAddr, cpu->PC.value++);
  uint8_t nn_msb = read_bus(busAddr, cpu->PC.value++);
  write_bus(busAddr, UNSIGNED_16(nn_msb, nn_lsb), cpu->AF.msb);
  return 4;
}

uint8_t LD_A_nn(sm83_t *cpu, bus_t *busAddr) {
  uint8_t nn_lsb = read_bus(busAddr, cpu->PC.value++);
  uint8_t nn_msb = read_bus(busAddr, cpu->PC.value++);
  cpu->AF.msb = read_bus(busAddr, UNSIGNED_16(nn_msb, nn_lsb));
  return 4;
}

uint8_t LDH_n_A(sm83_t *cpu, bus_t *busAddr) {
  uint8_t lsb_n = read_bus(busAddr, cpu->PC.value);
  write_bus(busAddr, UNSIGNED_16(0xFF, lsb_n), cpu->AF.msb);
  return 3;
}

uint8_t LDH_A_n(sm83_t *cpu, bus_t *busAddr) {
  uint8_t lsb_n = read_bus(busAddr, cpu->PC.value);
  cpu->AF.msb = read_bus(busAddr, UNSIGNED_16(0xFF, lsb_n));
  return 3;
}

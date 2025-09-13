#include "gbemu/core/sm83.h"
#include "SDL_cpuinfo.h"
#include "gbemu/common/bitwise.h"
#include "gbemu/common/logger.h"
#include "gbemu/core/alu.h"
#include "gbemu/core/bus.h"
#include <stdbool.h>
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

static uint8_t* get_opcode_register(sm83_t *cpu, uint8_t index) {
  uint8_t *registers[8] = {
    [0x0] = &cpu->BC.msb,
    [0x1] = &cpu->BC.lsb,
    [0x2] = &cpu->DE.msb,
    [0x3] = &cpu->DE.lsb,
    [0x4] = &cpu->HL.msb,
    [0x5] = &cpu->HL.lsb,
    [0x7] = &cpu->AF.msb,
  };
  return registers[index];
}

sm83_t *init() {
  sm83_t *cpu = malloc(sizeof(sm83_t));
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
    return 3;
  }
  return 0;
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
  default: 
      return 0;
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
  default: 
      return 0;
  }
  return 2;
}

uint8_t LD_r_r(sm83_t *cpu, bus_t *busAddr) {
  uint8_t *main_reg8bits = get_opcode_register(cpu, MAIN_REGISTER_IDX(cpu->opcode));
  uint8_t *second_reg8bits = get_opcode_register(cpu, SECOND_REGISTER_IDX(cpu->opcode));
  *main_reg8bits = *second_reg8bits;
  return 1;
}

uint8_t LD_r_n(sm83_t *cpu, bus_t *busAddr) {
  uint8_t *reg8bits = get_opcode_register(cpu, MAIN_REGISTER_IDX(cpu->opcode));
  *reg8bits = read_bus(busAddr, cpu->PC.value++);
  return 2;
}

uint8_t LD_r_HL(sm83_t *cpu, bus_t *busAddr) {
  uint8_t value = read_bus(busAddr, cpu->HL.value);
  uint8_t *reg8bits = get_opcode_register(cpu, MAIN_REGISTER_IDX(cpu->opcode));
  *reg8bits = value;
  return 2;
}

uint8_t LD_HL_n(sm83_t *cpu, bus_t *busAddr) {
  uint8_t value = read_bus(busAddr, cpu->PC.value++);
  write_bus(busAddr, cpu->HL.value, value);
  return 3;
}

uint8_t LD_HL_r(sm83_t *cpu, bus_t *busAddr) {
  uint8_t *reg8bits = get_opcode_register(cpu, SECOND_REGISTER_IDX(cpu->opcode));
  write_bus(busAddr, cpu->HL.value, *reg8bits);
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
  uint8_t lsb_n = read_bus(busAddr, cpu->PC.value++);
  cpu->AF.msb = read_bus(busAddr, UNSIGNED_16(0xFF, lsb_n));
  return 3;
}

uint8_t INC_rr(sm83_t *cpu, bus_t *busAddr) {
  switch (cpu->opcode) {
    case 0x03:
      cpu->BC.value++;
    break;
    case 0x13:
      cpu->DE.value++;
    break;
    case 0x23:
      cpu->HL.value++;
    break;
    case 0x33:
      cpu->SP.value++;
    break;
    default:
      return 0;
  }
  return 2;
}

uint8_t XOR_r(sm83_t *cpu, bus_t *busAddr) {
  uint8_t r;

  switch (cpu->opcode) {
    case 0xA8:
      r = cpu->BC.msb;
    break;
    case 0xA9:
      r = cpu->BC.lsb;
    break;
    case 0xAA:
      r = cpu->DE.msb;
    break;
    case 0xAB:
      r = cpu->DE.lsb;
    break;
    case 0xAC:
      r = cpu->HL.msb;
    break;
    case 0xAD:
      r = cpu->HL.lsb;
    break;
    case 0xAF:
      r = cpu->AF.msb;
    break;
    default: 
      return 0;
  }

  uint8_t result = XOR(cpu->AF.msb, r);
  cpu->AF.msb = result;
  cpu->AF.lsb = (result == 0) ? 0b10000000 : 0b00000000;
  return 1;
}

uint8_t DEC_r(sm83_t *cpu, bus_t *busAddr) {
  uint8_t result = 0;
  bool half_carry = false;

  switch (cpu->opcode) {
    case 0x05:
      half_carry = HAS_HALF_CARRY_IN_SUB(cpu->BC.msb, 1);
      cpu->BC.msb--;
      result = cpu->BC.msb;
    break;
    case 0x15:
      half_carry = HAS_HALF_CARRY_IN_SUB(cpu->DE.msb, 1);
      cpu->DE.msb--;
      result = cpu->DE.msb;
    break;
    case 0x25:
      half_carry = HAS_HALF_CARRY_IN_SUB(cpu->HL.msb, 1);
      cpu->HL.msb--;
      result = cpu->HL.msb;
    break;
    case 0x0D:
      half_carry = HAS_HALF_CARRY_IN_SUB(cpu->BC.lsb, 1);
      cpu->BC.lsb--;
      result = cpu->BC.lsb;
    break;
    case 0x1D:
      half_carry = HAS_HALF_CARRY_IN_SUB(cpu->DE.lsb, 1);
      cpu->DE.lsb--;
      result = cpu->DE.lsb;
    break;
    case 0x2D:
      half_carry = HAS_HALF_CARRY_IN_SUB(cpu->HL.lsb, 1);
      cpu->HL.lsb--;
      result = cpu->HL.lsb;
    break;
    case 0x3D:
      half_carry = HAS_HALF_CARRY_IN_SUB(cpu->AF.msb, 1);
      cpu->AF.msb--;
      result = cpu->AF.msb;
    break;
    default: 
      return 0;
  }

  uint8_t z_flag = (result == 0) ? 0b1000 : 0b0000;
  uint8_t n_flag = 0b0100;
  uint8_t h_flag = (half_carry) ? 0b0010 : 0b0000;
  uint8_t c_flag = C_FLAG(cpu->AF.lsb);
  cpu->AF.lsb = (z_flag | n_flag | h_flag | c_flag) << 4;
  return 1;
}

uint8_t INC_r(sm83_t *cpu, bus_t *busAddr) {
  alu_result_t result;
  switch(cpu->opcode) {
    case 0x04:
      result = alu_add(cpu->BC.msb, 1);
      cpu->BC.msb = result.result;
    break;
    case 0x14:
      result = alu_add(cpu->DE.msb, 1);
      cpu->DE.msb = result.result;
    break;
    case 0x24:
      result = alu_add(cpu->HL.msb, 1);
      cpu->HL.msb = result.result;
    break;
    case 0x0C:
      result = alu_add(cpu->BC.lsb, 1);
      cpu->BC.lsb = result.result;
    break;
    case 0x1C:
      result = alu_add(cpu->DE.lsb, 1);
      cpu->DE.lsb = result.result;
    break;
    case 0x2C:
      result = alu_add(cpu->HL.lsb, 1);
      cpu->HL.lsb = result.result;
    break;
    case 0x3C:
      result = alu_add(cpu->AF.msb, 1);
      cpu->AF.msb = result.result;
    break;
    default:
      return 0;
  }
  uint8_t z_flag = (result.result == 0) ? 1 : 0;
  uint8_t n_flag =  0;
  uint8_t h_flag = (result.has_half_carry) ? 1 : 0;
  uint8_t c_flag = C_FLAG_VALUE(cpu->AF.lsb);
  cpu->AF.lsb = JOIN_FLAGS(z_flag, n_flag, h_flag, c_flag);
  return 1;
}

uint8_t ADD_r(sm83_t *cpu, bus_t *busAddr) {
  if(cpu->opcode == 0x80) {
    alu_result_t data = alu_add(cpu->AF.msb, cpu->BC.msb);
    cpu->AF.msb = data.result;
    uint8_t z_flag = (data.result == 0) ? 1 : 0;
    uint8_t n_flag =  0;
    uint8_t h_flag = (data.has_half_carry) ? 1 : 0;
    uint8_t c_flag = (data.has_carry) ? 1 : 0;
    cpu->AF.lsb = JOIN_FLAGS(z_flag, n_flag, h_flag, c_flag);
    return 1;
  }
  return 0;
}

//FLOW CONTROLL
uint8_t JP_nn(sm83_t *cpu, bus_t *busAddr) {
  uint8_t nn_lsb = read_bus(busAddr, cpu->PC.value++);
  uint8_t nn_msb = read_bus(busAddr, cpu->PC.value++);
  cpu->PC.value = UNSIGNED_16(nn_msb, nn_lsb);
  return 4;
}

uint8_t JP_cc_e(sm83_t *cpu, bus_t *busAddr) {
  int8_t signed_value = (int8_t)read_bus(busAddr, cpu->PC.value++);

  bool condition = false;
  if (cpu->opcode == 0x20) {
    condition = !Z_FLAG_VALUE(cpu->AF.value);
  }else if (cpu->opcode == 0x30) {
    condition = !C_FLAG_VALUE(cpu->AF.value);
  }else if (cpu->opcode == 0x28) {
    condition = Z_FLAG_VALUE(cpu->AF.value);
  }else if (cpu->opcode == 0x38) {
    condition = C_FLAG_VALUE(cpu->AF.value);
  }

  if(condition) {
    cpu->PC.value += signed_value;
    return 3;
  }
  return 2;
}


uint8_t RST_n(sm83_t *cpu, bus_t *busAddr) {
  uint8_t n_value = cpu->opcode & 0b00111000;
  write_bus(busAddr, --cpu->SP.value, cpu->PC.msb);
  write_bus(busAddr, --cpu->SP.value, cpu->PC.lsb);
  cpu->PC.value = UNSIGNED_16(0x00, n_value);
  return 4;
}

uint8_t RET(sm83_t *cpu, bus_t *busAddr) {
  uint8_t lsb = read_bus(busAddr, cpu->SP.value++);
  uint8_t msb = read_bus(busAddr, cpu->SP.value++);
  cpu->PC.value = UNSIGNED_16(msb, lsb);
  return 4;
}

uint8_t CALL_nn(sm83_t *cpu, bus_t *busAddr) {
  uint8_t nn_lsb = read_bus(busAddr, cpu->PC.value++);
  uint8_t nn_msb = read_bus(busAddr, cpu->PC.value++);
  uint8_t nn_value = UNSIGNED_16(nn_msb, nn_lsb);
  write_bus(busAddr, --cpu->SP.value, cpu->PC.msb);
  write_bus(busAddr, --cpu->SP.value, cpu->PC.lsb);
  cpu->PC.value = nn_value;
  return 6;
}

uint8_t CALL_cc_nn(sm83_t *cpu, bus_t *busAddr) {
  uint8_t nn_lsb = read_bus(busAddr, cpu->PC.value++);
  uint8_t nn_msb = read_bus(busAddr, cpu->PC.value++);
  uint8_t nn_value = UNSIGNED_16(nn_msb, nn_lsb);
  bool condition = false;
  if (cpu->opcode == 0xCC) {
    condition = Z_FLAG_VALUE(cpu->AF.value);
  } else if(cpu->opcode == 0xDC) {
    condition = C_FLAG_VALUE(cpu->AF.value);
  } else if (cpu->opcode == 0xC4) {
    condition = !Z_FLAG_VALUE(cpu->AF.value);
  } else if(cpu->opcode == 0xD4) {
    condition = !C_FLAG_VALUE(cpu->AF.value);
  }

  if (condition) {
    write_bus(busAddr, --cpu->SP.value, cpu->PC.msb);
    write_bus(busAddr, --cpu->SP.value, cpu->PC.lsb);
    cpu->PC.value = nn_value;
    return 6;
  }
  return 3;
}

//ROTATES, SHIFTS, AND BIT OPERATIONS
uint8_t RLCA(sm83_t *cpu, bus_t *busAddr) {
  alu_result_t result = alu_rotate_left_circular(cpu->AF.msb);
  cpu->AF.msb = result.result;
  cpu->AF.lsb = result.has_carry ? 0x10 : 0x00;
  return 1;
}

uint8_t RRCA(sm83_t *cpu, bus_t *busAddr) {
  uint8_t reg_a_value = cpu->AF.msb;
  uint8_t b0 = (0b00000001 & reg_a_value);
  uint8_t rest = (0b11111110 & reg_a_value) >> 1;
  cpu->AF.msb = (b0 << 7) | rest;
  cpu->AF.lsb = (b0 == 0) ? 0x00 : 0x10;
  return 1;
}

uint8_t DI(sm83_t *cpu, bus_t *busAddr) {
  cpu->IME = false;
  return 1;
}

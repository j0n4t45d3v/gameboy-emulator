#include "gbemutest/core/instructions_test.h"
#include "gbemu/core/bus.h"
#include "gbemu/core/sm83.h"
#include <stddef.h>
#include <stdlib.h>
#include "test/unit.h"

static sm83_t *cpu;
static bus_t *bus;

BEFORE_ALL({
  /* PREPARE ENVIROMENT */
  cpu = malloc(sizeof(sm83_t));
  cpu->PC.value = 0x0000;
  cpu->BC.value = 0x0000;
  cpu->DE.value = 0x0000;
  cpu->HL.value = 0x0000;
  cpu->AF.value = 0x0000;
  cpu->opcode = 0x00;
  cpu->clocks = 0x00;

  bus = malloc(sizeof(bus_t));
  cartridger_t *cart = malloc(sizeof(cartridger_t));
  bus->cart = cart;
  cart->bank00[0] = 0xFF;
  cart->bank00[1] = 0xF0;
  cart->bank00[2] = 0x00;
  cart->bank00[3] = 0xAF;
  cart->bank00[4] = 0x0F;
})

AFTER_ALL({
  /* CLEANUP */
  free(bus);
  free(cpu);
})

TEST(LD_r_r, "Should Load Register 8 bits into Register 8 bits", {
  cpu->opcode = 0x50;
  cpu->BC.value = 0xFFFF;
  uint8_t clocks = LD_r_r(cpu, NULL);
  ASSERT_EQ_NUM(1, clocks);
  ASSERT_EQ_HEX(0XFF00, cpu->DE.value);
  ASSERT_EQ_HEX(0XFF, cpu->DE.msb);
})

TEST(LD_r_n, "Should Load data n into Register 8 bits", {
  cpu->opcode = 0x06;
  cpu->PC.value = 0x0000;
  cpu->BC.value = 0x0000;
  write_bus(bus, cpu->PC.value, 0xE5);
  uint8_t clocks = LD_r_n(cpu, bus);
  ASSERT_EQ_NUM(2, clocks);
  ASSERT_EQ_HEX(0XE5, cpu->BC.msb);
  ASSERT_EQ_NUM(1, cpu->PC.value);
})

TEST(LD_r_HL, "Should Load value in memory address Register HL into Register 8 bits", {
  cpu->opcode = 0x56;
  cpu->HL.value = 0x0000;
  cpu->DE.value = 0x0000;
  write_bus(bus, cpu->HL.value, 0xFF);
  uint8_t clocks = LD_r_HL(cpu, bus);
  ASSERT_EQ_NUM(2, clocks);
  ASSERT_EQ_HEX(0XFF00, cpu->DE.value);
  ASSERT_EQ_HEX(0XFF, cpu->DE.msb);

  cpu->opcode = 0x66;
  cpu->HL.value = 0x0001;
  clocks = LD_r_HL(cpu, bus);
  ASSERT_EQ_NUM(2, clocks);
  ASSERT_EQ_HEX(0XF001, cpu->HL.value);
  ASSERT_EQ_HEX(0XF0, cpu->HL.msb);
})

TEST(LD_rr_nn, "Should Load value in memory address Register PC into Register 16 bits", {
  cpu->opcode = 0x01;
  cpu->PC.value = 0x0000;
  cpu->BC.value = 0x0000;
  uint8_t clocks = LD_rr_nn(cpu, bus);
  ASSERT_EQ_NUM(3, clocks);
  ASSERT_EQ_HEX(0XF0FF, cpu->BC.value);
  ASSERT_EQ_HEX(0XF0, cpu->BC.msb);
  ASSERT_EQ_HEX(0XFF, cpu->BC.lsb);
});

TEST(LD_rr_A, "Should Load value in memory address Register A into Register 16 bits", {
  cpu->opcode = 0x02;
  cpu->BC.value = 0x0000;
  cpu->AF.msb = 0xF3;
  uint8_t clocks = LD_rr_A(cpu, bus);
  ASSERT_EQ_NUM(2, clocks);
  ASSERT_EQ_HEX(0XF3, read_bus(bus, 0x0000));
})

TEST(LD_A_rr, "Should Load value in memory address Register 16 bit into Register 8 bit A", {
  cpu->opcode = 0x0A;
  cpu->BC.value = 0x0000;
  write_bus(bus, cpu->BC.value, 0xA4);
  cpu->AF.msb = 0xF3;
  uint8_t clocks = LD_A_rr(cpu, bus);
  ASSERT_EQ_NUM(2, clocks);
  ASSERT_EQ_HEX(0xA4, cpu->AF.msb);
})

TEST(LD_HL_n, "Should Load value in memory address PC Register 16 bit into HL address", {
  cpu->opcode = 0x36;
  cpu->PC.value = 0x0000;
  cpu->HL.value = 0x0001;
  write_bus(bus, cpu->PC.value, 0xA4);
  // cpu->AF.msb = 0xF3;
  uint8_t clocks = LD_HL_n(cpu, bus);
  ASSERT_EQ_NUM(3, clocks);
  ASSERT_EQ_HEX(0xA4, read_bus(bus, 0x0001));
})

TEST(LD_HL_r, "Should Load value in Register 8 bit into HL address", {
  cpu->opcode = 0x70;
  cpu->BC.value = 0xE600;
  cpu->HL.value = 0x0000;
  uint8_t clocks = LD_HL_r(cpu, bus);
  ASSERT_EQ_NUM(2, clocks);
  ASSERT_EQ_HEX(0xE6, read_bus(bus, cpu->HL.value));
})

TEST(LD_nn_A, "Should Load value of the A Register 8 bit into nn address", {
  cpu->opcode = 0x70;
  cpu->AF.msb = 0x44;
  cpu->PC.value = 0x0000;

  write_bus(bus, 0, 0xFF);
  write_bus(bus, 1, 0x00);
  uint8_t clocks = LD_nn_A(cpu, bus);
  ASSERT_EQ_NUM(4, clocks);
  ASSERT_EQ_HEX(0x44, read_bus(bus, 0x00FF));
})

TEST(LD_A_nn, "Should Load nn address value into A Register 8 bit", {
  cpu->opcode = 0x70;
  cpu->AF.msb = 0x00;
  cpu->PC.value = 0x0000;

  write_bus(bus, 0, 0xFF);
  write_bus(bus, 1, 0x33);
  write_bus(bus, 0x33FF, 0xD3);

  uint8_t clocks = LD_A_nn(cpu, bus);
  ASSERT_EQ_NUM(4, clocks);
  ASSERT_EQ_HEX(0xD3, cpu->AF.msb);
})

TEST(LDH_n_A, "Should Load value of the A Register 8 bit into nn address", {
  cpu->opcode = 0xE0;
  cpu->AF.msb = 0x44;
  cpu->PC.value = 0x0000;

  write_bus(bus, 0, 0x00);
  uint8_t clocks = LDH_n_A(cpu, bus);
  ASSERT_EQ_NUM(3, clocks);
  ASSERT_EQ_HEX(0x44, read_bus(bus, 0xFF00));
})

TEST(LDH_A_n, "Should Load nn address value into A Register 8 bit", {
  cpu->opcode = 0xF0;
  cpu->AF.msb = 0x00;
  cpu->PC.value = 0x0000;

  write_bus(bus, 0, 0x00);
  write_bus(bus, 0xFF00, 0xEE);
  uint8_t clocks = LDH_A_n(cpu, bus);
  ASSERT_EQ_NUM(3, clocks);
  ASSERT_EQ_HEX(0xEE, cpu->AF.msb);
})

//ARITHMETIC 
TEST(INC_rr, "Should Increment rr Register 16 bits", {
  cpu->opcode = 0x03;
  cpu->BC.value = 0x0000;
  cpu->DE.value = 0x0000;
  cpu->HL.value = 0x0000;
  cpu->SP.value = 0x0000;
  uint8_t clocks = INC_rr(cpu, bus);
  ASSERT_EQ_NUM(2, clocks);
  ASSERT_EQ_HEX(0x0001, cpu->BC.value);
  ASSERT_EQ_HEX(0x0000, cpu->DE.value);
  ASSERT_EQ_HEX(0x0000, cpu->HL.value);
  ASSERT_EQ_HEX(0x0000, cpu->SP.value);
})

TEST(XOR_r, "Should perform XOR operatio between A register and the 8-bit register 'r'", {
  cpu->opcode = 0xA8;
  cpu->AF.msb = 0b10110010;
  cpu->BC.msb = 0b00100001;
  cpu->DE.msb = 0x00;
  cpu->HL.msb = 0x00;

  uint8_t clocks = XOR_r(cpu, bus);

  ASSERT_EQ_NUM(1, clocks);
  ASSERT_EQ_BINARY(0b10010011, cpu->AF.msb);
  ASSERT_EQ_BINARY(0b00100001, cpu->BC.msb);
  ASSERT_EQ_NUM(0, Z_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, H_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, H_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, C_FLAG_VALUE(cpu->AF.lsb));

  cpu->opcode = 0xAF;
  cpu->AF.msb = 0b10110010;
  cpu->BC.msb = 0b00100001;
  cpu->DE.msb = 0b00100001;
  cpu->HL.msb = 0b00100001;

  clocks = XOR_r(cpu, bus);

  ASSERT_EQ_NUM(1, clocks);
  ASSERT_EQ_HEX(0x00, cpu->AF.msb);
  ASSERT_EQ_BINARY(0b00100001, cpu->BC.msb);
  ASSERT_EQ_NUM(1, Z_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, N_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, H_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, C_FLAG_VALUE(cpu->AF.lsb));
})

TEST(DEC_r, "Should Decrement r Register 8 bits", {
  cpu->opcode = 0x05;
  cpu->BC.value = 0x0F00;
  cpu->AF.value = 0x0000;
  uint8_t clocks = DEC_r(cpu, bus);
  ASSERT_EQ_NUM(1, clocks);
  uint8_t flags = cpu->AF.lsb >> 4;
  ASSERT_EQ_BINARY(0b0100, flags);
  ASSERT_EQ_HEX(0x0E, cpu->BC.msb);
  ASSERT_EQ_HEX(0x00, cpu->BC.lsb);
  ASSERT_EQ_HEX(0x0E00, cpu->BC.value);

  cpu->opcode = 0x05;
  cpu->BC.value = 0x0100;
  cpu->AF.value = 0x0000;
  clocks = DEC_r(cpu, bus);
  ASSERT_EQ_NUM(1, clocks);
  flags = cpu->AF.lsb >> 4;
  ASSERT_EQ_BINARY(0b1100, flags);
  ASSERT_EQ_HEX(0x00, cpu->BC.msb);
  ASSERT_EQ_HEX(0x00, cpu->BC.lsb);

  cpu->opcode = 0x05;
  cpu->BC.value = 0x0000;
  cpu->AF.value = 0x0000;
  clocks = DEC_r(cpu, bus);
  ASSERT_EQ_NUM(1, clocks);
  flags = cpu->AF.lsb >> 4;
  ASSERT_EQ_BINARY(0b0110, flags);
  ASSERT_EQ_HEX(0xFF, cpu->BC.msb);
  ASSERT_EQ_HEX(0x00, cpu->BC.lsb);
})

TEST(INC_r, "Should increment the 8-bits resgiter 'r' value", {
  cpu->opcode = 0x04;
  cpu->BC.msb = 0x00;

  uint8_t clocks = INC_r(cpu, bus);
  
  ASSERT_EQ_NUM(1, clocks);
  ASSERT_EQ_NUM(1, cpu->BC.msb)
  ASSERT_EQ_NUM(0, Z_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, H_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, H_FLAG_VALUE(cpu->AF.lsb));

  cpu->opcode = 0x14;
  cpu->DE.msb = 0b00001111;

  clocks = INC_r(cpu, bus);
  
  ASSERT_EQ_NUM(1, clocks);
  ASSERT_EQ_NUM(0x10, cpu->DE.msb)
  ASSERT_EQ_NUM(0, Z_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, N_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(1, H_FLAG_VALUE(cpu->AF.lsb));

  cpu->opcode = 0x24;
  cpu->HL.msb = 0b11111111;

  clocks = INC_r(cpu, bus);
  
  ASSERT_EQ_NUM(1, clocks);
  ASSERT_EQ_NUM(0, cpu->HL.msb)
  ASSERT_EQ_NUM(1, Z_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, N_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(1, H_FLAG_VALUE(cpu->AF.lsb));
})

TEST(ADD_r, "Should add to the A register the 8-bits register value", {
  
  cpu->opcode = 0x80;
  cpu->AF.msb = 0x0F;
  cpu->BC.msb = 0x01;

  uint8_t clocks = ADD_r(cpu, bus);
  
  ASSERT_EQ_NUM(1, clocks);
  ASSERT_EQ_HEX(0x10, cpu->AF.msb)
  ASSERT_EQ_NUM(0, Z_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, N_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(1, H_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, C_FLAG_VALUE(cpu->AF.lsb));

  cpu->opcode = 0x80;
  cpu->AF.msb = 0xFF;
  cpu->BC.msb = 0x01;

  clocks = ADD_r(cpu, bus);
  
  ASSERT_EQ_NUM(1, clocks);
  ASSERT_EQ_HEX(0, cpu->AF.msb)
  ASSERT_EQ_NUM(1, Z_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(0, N_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(1, H_FLAG_VALUE(cpu->AF.lsb));
  ASSERT_EQ_NUM(1, C_FLAG_VALUE(cpu->AF.lsb));
})

TEST(JP_nn, "Should jump to nn address", {
  cpu->opcode = 0xC3;
  cpu->PC.value = 0x0000;
  write_bus(bus, 0x0000, 0x00);
  write_bus(bus, 0x0001, 0x01);

  uint8_t clocks = JP_nn(cpu, bus);
  ASSERT_EQ_NUM(4, clocks);
  ASSERT_EQ_HEX(0x01, cpu->PC.msb);
  ASSERT_EQ_HEX(0x00, cpu->PC.lsb);
  ASSERT_EQ_HEX(0x0100, cpu->PC.value);
})

RUN_SUITE(instructions,  
  test_LD_r_r,
  test_LD_r_n,
  test_LD_r_HL,
  test_LD_rr_nn,
  test_LD_rr_A,
  test_LD_A_rr,
  test_LD_HL_n,
  test_LD_HL_r,
  test_LD_nn_A,
  test_LD_A_nn,
  test_LDH_n_A,
  test_LDH_A_n,
  test_INC_rr,
  test_XOR_r,
  test_DEC_r,
  test_INC_r,
  test_ADD_r,
  test_JP_nn,
);


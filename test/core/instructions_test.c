#include "gbemutest/core/instructions_test.h"
#include "gbemu/core/bus.h"
#include "gbemu/core/sm83.h"
#include "gbemu/external/cartridger.h"
#include "test/unit.h"
#include <stddef.h>
#include <stdlib.h>

sm83_t *cpu;
bus_t *bus;
cartridger_t *cart;

void setUp();
void test_LD_r_r(sm83_t *cpu);
void test_LD_r_n(sm83_t *cpu, bus_t *bus);
void test_LD_r_HL(sm83_t *cpu, bus_t *bus);
void test_LD_rr_nn(sm83_t *cpu, bus_t *bus);
void test_LD_rr_A(sm83_t *cpu, bus_t *bus);
void test_LD_A_rr(sm83_t *cpu, bus_t *bus);
void test_LD_HL_r(sm83_t *cpu, bus_t *bus);

void tearDown();

void run_instruction_suite() {
  setUp();
  TEST_SUITE("Instrunctions SUITE", {
    /*  TESTS  */
    test_LD_r_r(cpu);
    test_LD_r_n(cpu, bus);
    test_LD_r_HL(cpu, bus);
    test_LD_rr_nn(cpu, bus);
    test_LD_rr_A(cpu, bus);
    test_LD_A_rr(cpu, bus);
    test_LD_HL_r(cpu, bus);
  });
  tearDown();
}

void setUp() {
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
  cart = malloc(sizeof(cartridger_t));
  bus->cart = cart;
  cart->bank00 = malloc(sizeof(char) * 5);
  cart->bank00[0] = 0xFF;
  cart->bank00[1] = 0xF0;
  cart->bank00[2] = 0x00;
  cart->bank00[3] = 0xAF;
  cart->bank00[4] = 0x0F;
}

void test_LD_r_r(sm83_t *cpu) {
  TEST("Should Load Register 8 bits into Register 8 bits", {
    cpu->opcode = 0x50;
    cpu->BC.value = 0xFFFF;
    uint8_t clocks = LD_r_r(cpu, NULL);
    ASSERT_EQ_NUM(1, clocks);
    ASSERT_EQ_HEX(0XFF00, cpu->DE.value);
    ASSERT_EQ_HEX(0XFF, cpu->DE.msb);
  });
}

void test_LD_r_n(sm83_t *cpu, bus_t *bus) {
  TEST("Should Load data n into Register 8 bits", {
    cpu->opcode = 0x06;
    cpu->PC.value = 0x0000;
    cpu->BC.value = 0x0000;
    write_bus(bus, cpu->PC.value, 0xE5);
    uint8_t clocks = LD_r_n(cpu, bus);
    ASSERT_EQ_NUM(2, clocks);
    ASSERT_EQ_HEX(0XE5, cpu->BC.msb);
    ASSERT_EQ_NUM(1, cpu->PC.value);
  });
}

void test_LD_r_HL(sm83_t *cpu, bus_t *bus) {
  TEST("Should Load value in memory address Register HL into Register 8 bits", {
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
  });
}

void test_LD_rr_nn(sm83_t *cpu, bus_t *bus) {
  TEST("Should Load value in memory address Register PC into Register 16 bits", {
    cpu->opcode = 0x01;
    cpu->PC.value = 0x0000;
    cpu->BC.value = 0x0000;
    uint8_t clocks = LD_rr_nn(cpu, bus);
    ASSERT_EQ_NUM(3, clocks);
    ASSERT_EQ_HEX(0XF0FF, cpu->BC.value);
    ASSERT_EQ_HEX(0XF0, cpu->BC.msb);
    ASSERT_EQ_HEX(0XFF, cpu->BC.lsb);
  });
}

void test_LD_rr_A(sm83_t *cpu, bus_t *bus) {
  TEST("Should Load value in memory address Register A into Register 16 bits", {
    cpu->opcode = 0x02;
    cpu->BC.value = 0x0000;
    cpu->AF.msb = 0xF3;
    uint8_t clocks = LD_rr_A(cpu, bus);
    ASSERT_EQ_NUM(2, clocks);
    ASSERT_EQ_HEX(0XF3, read_bus(bus, 0x0000));
  });
}

void test_LD_A_rr(sm83_t *cpu, bus_t *bus) {
  TEST("Should Load value in memory address Register 16 bit into Register 8 bit A", {
    cpu->opcode = 0x0A;
    cpu->BC.value = 0x0000;
    write_bus(bus, cpu->BC.value, 0xA4);
    cpu->AF.msb = 0xF3;
    uint8_t clocks = LD_A_rr(cpu, bus);
    ASSERT_EQ_NUM(2, clocks);
    ASSERT_EQ_HEX(0xA4, cpu->AF.msb);
  });
}

void test_LD_HL_r(sm83_t *cpu, bus_t *bus) {
  TEST("Should Load value in memory address PC Register 16 bit into HL address", {
    cpu->opcode = 0x36;
    cpu->PC.value = 0x0000;
    cpu->HL.value = 0x0001;
    write_bus(bus, cpu->PC.value, 0xA4);
    // cpu->AF.msb = 0xF3;
    uint8_t clocks = LD_HL_r(cpu, bus);
    ASSERT_EQ_NUM(3, clocks);
    ASSERT_EQ_HEX(0xA4, read_bus(bus, 0x0001));
  });
}

void tearDown() {
  /* CLEANUP */
  free(cart->bank00);
  free(cart);
  free(bus);
  free(cpu);
}

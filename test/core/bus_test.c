#include "gbemutest/core/bus_test.h"
#include "gbemu/common/units.h"
#include "gbemu/core/bus.h"
#include "gbemu/external/cartridger.h"
#include "test/unit.h"
#include <stdint.h>
#include <stdlib.h>

static bus_t *bus;
static cartridger_t cart;

BEFORE_ALL({
  bus = (bus_t *)malloc(sizeof(bus_t));
  cart.ram = (uint8_t *)malloc(sizeof(uint8_t) * (8 * KILO_BYTES));
  bus->cart = &cart;
})

AFTER_ALL({
  free(cart.ram);
  free(bus);
})

TEST(write_bus, "Should write value in address informed", {
  uint8_t clocks = write_bus(bus, 0x0000, 0xFF);
  clocks += write_bus(bus, START_ADDR_BANK01_NN, 0xFF);
  clocks += write_bus(bus, START_ADDR_VIDEO_RAM, 0xFF);
  clocks += write_bus(bus, START_ADDR_EXTERNAL_RAM, 0xFF);
  clocks += write_bus(bus, START_ADDR_WORK_RAM0, 0xFF);
  clocks += write_bus(bus, START_ADDR_WORK_RAM1, 0xFF);
  clocks += write_bus(bus, START_ADDR_OBJECT_ATTR_MEMORY, 0xFF);
  clocks += write_bus(bus, START_ADDR_IO_REGISTERS, 0xFF);
  clocks += write_bus(bus, START_END_ADDR_INTERRUPT_ENABLE_REGISTER, 0xFF);

  ASSERT_EQ_NUM(9, clocks);
  ASSERT_EQ_NUM(0xFF, (uint8_t)cart.bank00[0x0000]);
  ASSERT_EQ_NUM(0xFF, (uint8_t)cart.bank01NN[0x0000]);
  ASSERT_EQ_NUM(0xFF, (uint8_t)bus->vram[0x0000]);
  ASSERT_EQ_NUM(0xFF, (uint8_t)cart.ram[0x0000]);
  ASSERT_EQ_NUM(0xFF, (uint8_t)bus->wram0[0x0000]);
  ASSERT_EQ_NUM(0xFF, (uint8_t)bus->wram1[0x0000]);
  ASSERT_EQ_NUM(0xFF, (uint8_t)bus->oam[0x0000]);
  ASSERT_EQ_NUM(0xFF, (uint8_t)bus->io_register[0x0000]);
  ASSERT_EQ_NUM(0xFF, (uint8_t)bus->ie_register);
})

TEST(read_bus, "Should read value in address informed", {
  cart.bank00[0x0000] = 0x01;
  cart.bank01NN[0x0000] = 0x02;
  bus->vram[0x0000] = 0x03;
  cart.ram[0x0000] = 0x04;
  bus->wram0[0x0000] = 0x05;
  bus->wram1[0x0000] = 0x06;
  bus->oam[0x0000] = 0x07;
  bus->io_register[0x0000] = 0x08;
  bus->ie_register = 0x09;

  ASSERT_EQ_NUM(0x01, read_bus(bus, 0x0000));
  ASSERT_EQ_NUM(0x02, read_bus(bus, START_ADDR_BANK01_NN));
  ASSERT_EQ_NUM(0x03, read_bus(bus, START_ADDR_VIDEO_RAM));
  ASSERT_EQ_NUM(0x04, read_bus(bus, START_ADDR_EXTERNAL_RAM));
  ASSERT_EQ_NUM(0x05, read_bus(bus, START_ADDR_WORK_RAM0));
  ASSERT_EQ_NUM(0x06, read_bus(bus, START_ADDR_WORK_RAM1));
  ASSERT_EQ_NUM(0x07, read_bus(bus, START_ADDR_OBJECT_ATTR_MEMORY));
  ASSERT_EQ_NUM(0x08, read_bus(bus, START_ADDR_IO_REGISTERS));
  ASSERT_EQ_NUM(0x09, read_bus(bus, START_END_ADDR_INTERRUPT_ENABLE_REGISTER));
})

RUN_SUITE(bus, test_read_bus, test_write_bus);

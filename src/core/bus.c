#include "gbemu/core/bus.h"


uint8_t read_bus(bus_t *busAddr, uint16_t addr) {
  if (IS_BANK00(addr)) {
    return (uint8_t)busAddr->cart->bank00[addr];
  }
  if (IS_BANK01_NN(addr)) {
    return (uint8_t)busAddr->cart->bank01NN[addr - START_ADDR_BANK01_NN];
  }
  if (IS_VIDEO_RAM(addr)) {
    return busAddr->vram[addr - START_ADDR_VIDEO_RAM];
  }
  if (IS_EXTERNAL_RAM(addr)) {
    return busAddr->cart->ram[addr - START_ADDR_EXTERNAL_RAM];
  }
  if (IS_WORK_RAM0(addr)) {
    return busAddr->wram0[addr - START_ADDR_WORK_RAM0];
  }
  if (IS_WORK_RAM1(addr)) {
    return busAddr->wram1[addr - START_ADDR_WORK_RAM1];
  }
  if (IS_OBJECT_ATTR_MEMORY(addr)) {
    return busAddr->oam[addr - START_ADDR_OBJECT_ATTR_MEMORY];
  }
  if (IS_IO_REGISTERS(addr)) {
    return busAddr->io_register[addr - START_ADDR_IO_REGISTERS];
  }
  if (IS_INTERRUPT_ENABLE_REGISTER(addr)) {
    return busAddr->ie_register;
  }

  return -1;
}

uint8_t write_bus(bus_t *busAddr, uint16_t addr, uint8_t value) {
  if (IS_BANK00(addr)) {
    busAddr->cart->bank00[addr] = value;
  }
  if (IS_BANK01_NN(addr)) {
    busAddr->cart->bank01NN[addr - START_ADDR_BANK01_NN] = value;
  }
  if (IS_VIDEO_RAM(addr)) {
    busAddr->vram[addr - START_ADDR_VIDEO_RAM] = value;
  }
  if (IS_EXTERNAL_RAM(addr)) {
    busAddr->cart->ram[addr - START_ADDR_EXTERNAL_RAM] = value;
  }
  if (IS_WORK_RAM0(addr)) {
    busAddr->wram0[addr - START_ADDR_WORK_RAM0] = value;
  }
  if (IS_WORK_RAM1(addr)) {
    busAddr->wram1[addr - START_ADDR_WORK_RAM1] = value;
  }
  if (IS_OBJECT_ATTR_MEMORY(addr)) {
    busAddr->oam[addr - START_ADDR_OBJECT_ATTR_MEMORY] = value;
  }
  if (IS_IO_REGISTERS(addr)) {
    busAddr->io_register[addr - START_ADDR_IO_REGISTERS] = value;
  }
  if (IS_INTERRUPT_ENABLE_REGISTER(addr)) {
    busAddr->ie_register = value;
  }
  return 1;
}

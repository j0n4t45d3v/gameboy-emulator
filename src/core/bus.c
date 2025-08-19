#include "gbemu/core/bus.h"
#include "gbemu/common/logger.h"

#define IS_BANK00(addr) addr >= 0x0000 && addr <= 0x3FFF
#define IS_BANK01_NN(addr) addr >= 0x4000 && addr <= 0x7FFF
#define IS_EXTERNAL_RAM(addr) addr >= 0xA000 && addr <= 0xBFFF

uint8_t read_bus(bus_t *busAddr, uint16_t addr) {
  if (IS_BANK00(addr)) {
    return (uint8_t)busAddr->cart->bank00[addr];
  }
  if (IS_BANK01_NN(addr)) {
    return (uint8_t)busAddr->cart->bank01NN[addr];
  }
  if (IS_EXTERNAL_RAM(addr)) {
    LOG_DEBUG("TESTE");
    return busAddr->cart->ram[addr];
  }
  return -1;
}

uint8_t write_bus(bus_t *busAddr, uint16_t addr, uint8_t value) {
  if (IS_BANK00(addr)) {
    busAddr->cart->bank00[addr] = value;
  }
  if (IS_BANK01_NN(addr)) {
    busAddr->cart->bank01NN[addr] = value;
  }
  if (IS_EXTERNAL_RAM(addr)) {
    busAddr->cart->ram[addr] = value;
  }
  return -1;
}

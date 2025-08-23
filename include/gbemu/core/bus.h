#ifndef BUS_H
#define BUS_H

#include "gbemu/external/cartridger.h"
#include <stdint.h>

#define IS_BANK00(addr) addr >= 0x0000 && addr <= 0x3FFF

#define START_ADDR_BANK01_NN 0x4000
#define END_ADDR_BANK01_NN 0x7FFF
#define IS_BANK01_NN(addr) addr >= START_ADDR_BANK01_NN && addr <= END_ADDR_BANK01_NN

#define START_ADDR_VIDEO_RAM 0x8000
#define END_ADDR_VIDEO_RAM 0x9FFF
#define IS_VIDEO_RAM(addr) addr >= START_ADDR_VIDEO_RAM && addr <= END_ADDR_VIDEO_RAM

#define START_ADDR_EXTERNAL_RAM 0xA000
#define END_ADDR_EXTERNAL_RAM 0xBFFF
#define IS_EXTERNAL_RAM(addr) addr >= START_ADDR_EXTERNAL_RAM && addr <= END_ADDR_EXTERNAL_RAM

#define START_ADDR_WORK_RAM0 0xC000
#define END_ADDR_WORK_RAM0 0xCFFF
#define IS_WORK_RAM0(addr) addr >= START_ADDR_WORK_RAM0 && addr <= END_ADDR_WORK_RAM0

#define START_ADDR_WORK_RAM1 0xD000
#define END_ADDR_WORK_RAM1 0xDFFF
#define IS_WORK_RAM1(addr) addr >= START_ADDR_WORK_RAM1 && addr <= END_ADDR_WORK_RAM1

#define START_ADDR_OBJECT_ATTR_MEMORY 0xFE00
#define END_ADDR_OBJECT_ATTR_MEMORY 0xFE9F
#define IS_OBJECT_ATTR_MEMORY(addr) addr >= START_ADDR_OBJECT_ATTR_MEMORY && addr <= END_ADDR_OBJECT_ATTR_MEMORY

#define START_ADDR_IO_REGISTERS 0xFF00
#define END_ADDR_IO_REGISTERS 0xFFFE
#define IS_IO_REGISTERS(addr) addr >= START_ADDR_IO_REGISTERS && addr <= END_ADDR_IO_REGISTERS

#define START_END_ADDR_INTERRUPT_ENABLE_REGISTER 0xFFFF
#define IS_INTERRUPT_ENABLE_REGISTER(addr) addr == START_END_ADDR_INTERRUPT_ENABLE_REGISTER

typedef struct {
  cartridger_t* cart;
  uint8_t vram[END_ADDR_VIDEO_RAM - START_ADDR_VIDEO_RAM];
  uint8_t wram0[END_ADDR_WORK_RAM0 - START_ADDR_WORK_RAM0];
  uint8_t wram1[END_ADDR_WORK_RAM1 - START_ADDR_WORK_RAM1];
  uint8_t oam[END_ADDR_OBJECT_ATTR_MEMORY - START_ADDR_OBJECT_ATTR_MEMORY];
  uint8_t io_register[END_ADDR_IO_REGISTERS - START_ADDR_IO_REGISTERS];
  uint8_t ie_register;
} bus_t;


uint8_t read_bus(bus_t*, uint16_t); 
uint8_t write_bus(bus_t*, uint16_t, uint8_t); 

#endif // !BUS_H

#ifndef BUS_H
#define BUS_H

#include "gbemu/external/cartridger.h"
#include <stdint.h>

typedef struct {
  cartridger_t* cart;
} bus_t;

uint8_t read_bus(bus_t*, uint16_t); 
uint8_t write_bus(bus_t*, uint16_t, uint8_t); 

#endif // !BUS_H

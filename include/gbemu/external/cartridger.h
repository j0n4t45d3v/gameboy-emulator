#ifndef CARTRIDGER_H
#define CARTRIDGER_H

#include "../common/types.h"
#include <stdint.h>

typedef struct {
  char *bank00;
  char *bank01NN;
  uint8_t *ram;
} cartridger_t;

#define ADDRR_CART_TYPE 0x0147

void load_rom(cartridger_t*, string_t);

uint8_t checksum(const char *);

#define INIT_CHECKSUM_ADDR 0x0134
#define END_CHECKSUM_ADDR 0x014C
#define CHECKSUM_EXPECTED_VALUE 0x014D

#endif // !CARTRIDGER_H

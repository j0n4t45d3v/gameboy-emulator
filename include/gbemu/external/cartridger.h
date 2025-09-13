#pragma once

#include "../common/types.h"
#include "../common/units.h"
#include <stdint.h>

#define BANK_SIZE 16 * KILO_BYTES
#define INIT_CHECKSUM_ADDR 0x0134
#define END_CHECKSUM_ADDR 0x014C
#define CHECKSUM_EXPECTED_VALUE 0x014D
#define CART_TYPE_DECODER_SIZE 0x0100
#define ADDR_CART_TYPE 0x0147

typedef struct {
  char bank00[BANK_SIZE];
  char bank01NN[BANK_SIZE];
  uint8_t *ram;
  uint8_t type;
} cartridger_t;


void load_rom(cartridger_t*, string_t);

uint8_t checksum(const char *);

extern const string_t CART_TYPE_DECODER[CART_TYPE_DECODER_SIZE];

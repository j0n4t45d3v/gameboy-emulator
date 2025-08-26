#include "gbemutest/external/cartridger_test.h"
#include "gbemu/external/cartridger.h"
#include "test/unit.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

cartridger_t *cart;
const char * PATH_ROM = "/home/jonatasroot/Documentos/workspace/cpp/clang/gbemu/roms/Tetris.gb";

BEFORE_ALL(cart = (cartridger_t*)malloc(sizeof(cartridger_t));)

AFTER_ALL(free(cart);)

bool is_empty_bank(const char bank[BANK_SIZE]) {
  int counter = 0;
  for (int i = 0; i < BANK_SIZE; i++) {
    if(bank[i] > 0x00) {
      counter++;
    }
  }
  return counter == 0;
}

TEST(load_rom, "Should load rom in banks", {
  load_rom(cart, PATH_ROM);
  const int rom_only_addr = 0x00;
  ASSERT_EQ_STR(CART_TYPE_DECODER[rom_only_addr], CART_TYPE_DECODER[cart->bank00[ADDR_CART_TYPE]]);
  ASSERT_FALSE(is_empty_bank(cart->bank00))
  ASSERT_FALSE(is_empty_bank(cart->bank01NN))
})

TEST(checksum, "Should validate rom", {
  uint8_t checksum_value = checksum(cart->bank00);
  ASSERT_EQ_NUM(cart->bank00[CHECKSUM_EXPECTED_VALUE], checksum_value);
})

RUN_SUITE(cartridger, test_load_rom, test_checksum)


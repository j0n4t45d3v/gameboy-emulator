#include "gbemu/external/cartridger.h"
#include "gbemu/common/logger.h"
#include "gbemu/common/types.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const string_t CART_TYPE_DECODER[0x0100] = {
    [0x00] = "ROM ONLY",
    [0x01] = "MBC1",
    [0x02] = "MBC1+RAM",
    [0x03] = "MBC1+RAM+BATTERY",
    [0x05] = "MBC2",
    [0x06] = "MBC2+BATTERY",
    [0x08] = "ROM+RAM 9",
    [0x09] = "ROM+RAM+BATTERY 9",
    [0x0B] = "MMM01",
    [0x0C] = "MMM01+RAM",
    [0x0D] = "MMM01+RAM+BATTERY",
    [0x0F] = "MBC3+TIMER+BATTERY",
    [0x10] = "MBC3+TIMER+RAM+BATTERY 10",
    [0x11] = "MBC3",
    [0x12] = "MBC3+RAM 10",
    [0x13] = "MBC3+RAM+BATTERY 10",
    [0x19] = "MBC5",
    [0x1A] = "MBC5+RAM",
    [0x1B] = "MBC5+RAM+BATTERY",
    [0x1C] = "MBC5+RUMBLE",
    [0x1D] = "MBC5+RUMBLE+RAM",
    [0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
    [0x20] = "MBC6",
    [0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
    [0xFC] = "POCKET CAMERA",
    [0xFD] = "BANDAI TAMA5",
    [0xFE] = "HuC3",
    [0xFF] = "HuC1+RAM+BATTERY",
};

void load_rom(cartridger_t* cart, string_t path_rom) {
  LOG_INFO("Abrindo a ROM...");
  FILE *rom = fopen(path_rom, "rb");

  if (rom == NULL) {
    LOG_ERROR("Não foi possivel abrir a ROM: %s", path_rom);
    exit(1);
  }

  fseek(rom, 0, SEEK_END);
  size_t romSize = ftell(rom);
  rewind(rom);

  char *buffer = malloc(sizeof(char) * romSize);
  fread(buffer, sizeof(char), romSize, rom);

  uint8_t checksum_value = checksum(buffer);
  if (checksum_value == buffer[CHECKSUM_EXPECTED_VALUE]) {
    LOG_INFO("Checksum:  Rom Valida!");
  } else {
    LOG_INFO("Checksum:  Rom Inválida!");
    LOG_INFO("Checksum Value: %2x!", checksum_value);
    LOG_INFO("0x014D Addr Value: %2x!", buffer[CHECKSUM_EXPECTED_VALUE]);
  }

  LOG_INFO("Tamanho da ROM: %zu kilo bytes", romSize);
  LOG_INFO("Cartucho: %s", CART_TYPE_DECODER[buffer[ADDRR_CART_TYPE]]);

  cart->bank00 = buffer;
}


uint8_t checksum(const char *buffer) {
  uint8_t checksum = 0;
  for (uint16_t address = INIT_CHECKSUM_ADDR; address <= END_CHECKSUM_ADDR; address++) {
    checksum = checksum - buffer[address] - 1;
  }
  return checksum;
}

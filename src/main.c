#include <stdio.h>
#include "gbemu/external/cartridger.h"

int main() {
  load_rom(NULL, "./roms/Tetris.gb");
  return 0;
}

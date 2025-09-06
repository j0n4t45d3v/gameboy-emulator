#include "gbemutest/core/alu_test.h"
#include "gbemutest/core/bus_test.h"
#include "gbemutest/core/instructions_test.h"
#include "gbemutest/external/cartridger_test.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
  if (argc == 3 && strcmp("--suite", argv[1]) == 0) {
    if (strcmp("instruction", argv[2]) == 0) {
      run_instructions_suite();
    } else if (strcmp("bus", argv[2]) == 0) {
      run_bus_suite();
    } else if (strcmp("cartridger", argv[2]) == 0) {
      run_cartridger_suite();
    } else if (strcmp("alu", argv[2]) == 0) {
      run_arithetic_logic_unit_suite();
    } else {
      printf("Invalid Suite!\n");
      return 1;
    }
    return 0;
  }
  run_instructions_suite();
  run_bus_suite();
  run_cartridger_suite();
  run_arithetic_logic_unit_suite();
  return 0;
}

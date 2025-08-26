#ifndef UNIT_H
#define UNIT_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// COLORS
#define RESET         "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_WHITE   "\033[1;37m"

#define BEFORE_ALL(block)\
static void setUp() {\
block\
}

#define AFTER_ALL(block)\
static void tearDown() {\
block\
}

#define RUN_SUITE(suite_name, ...) \
void run_##suite_name##_suite() {\
  printf(COLOR_CYAN "SUITE: %s\n" RESET , #suite_name);\
  setUp();\
  /* TESTS */\
  void (*tests[])(void) = { __VA_ARGS__ }; \
  size_t num_tests = sizeof(tests) / sizeof(tests[0]); \
  for(size_t i = 0; i < num_tests; i++) { \
      tests[i](); \
  } \
  tearDown();\
  printf(COLOR_CYAN "FINISH SUITE: %s\n" RESET , #suite_name);\
}

#define TEST_SUITE(name, block)\
printf(COLOR_CYAN "SUITE: %s\n" RESET , name);\
setUp();\
block;\
tearDown();\
printf(COLOR_CYAN "FINISH SUITE: %s\n" RESET , name)

#define TEST(name, description, block)\
  void test_##name(void) {\
    printf(COLOR_WHITE "\tTEST:" RESET " %s...\n", description);\
    block;\
    printf(COLOR_GREEN "\tPASS:" RESET " %s\n", description);\
  }

// ASSERTS
#define ASSERT_IS_NOT_NULL(target)\
  if(target == NULL)\
  {\
    printf(COLOR_RED "\tFAIL:" COLOR_WHITE " %s != null, (line %d)\n" RESET, #target, __LINE__);\
    printf(COLOR_WHITE "\t      Expected: not null\n" RESET);\
    printf(COLOR_WHITE "\t      Actual: null\n" RESET);\
    exit(1);\
  }

#define ASSERT_IS_NULL(target)\
  if(target != NULL)\
  {\
    printf(COLOR_RED "\tFAIL:" COLOR_WHITE " %s == null, (line %d)\n" RESET, #target, __LINE__);\
    printf(COLOR_WHITE "\t      Expected: null\n" RESET);\
    printf(COLOR_WHITE "\t      Actual: not null\n" RESET);\
    exit(1);\
  }

#define ASSERT_EQ_NUM(expected, target)\
  if(expected != target)\
  {\
    printf(COLOR_RED "\tFAIL:" COLOR_WHITE " %s == %s, (line %d)\n" RESET, #expected, #target, __LINE__);\
    printf(COLOR_WHITE "\t      Expected %d\n" RESET, expected);\
    printf(COLOR_WHITE "\t      Actual: %d\n" RESET, target);\
    exit(1);\
  }

#define ASSERT_EQ_HEX(expected, target)\
  if(expected != target)\
  {\
    printf(COLOR_RED "\tFAIL:" COLOR_WHITE " %s == %s, (line %d)\n" RESET, #expected, #target, __LINE__);\
    printf(COLOR_WHITE "\t      Expected %#X\n" RESET, expected);\
    printf(COLOR_WHITE "\t      Actual: %#X\n" RESET, target);\
    exit(1);\
  }

#define ASSERT_EQ_STR(expected, target)\
if(strcmp(expected,target) != 0)\
  {\
    printf(COLOR_RED "\tFAIL:" COLOR_WHITE " %s == %s, (line %d)\n" RESET, #expected, #target, __LINE__);\
    printf(COLOR_WHITE "\t      Expected %s\n" RESET, expected);\
    printf(COLOR_WHITE "\t      Actual: %s\n" RESET, target);\
    exit(1);\
  }

#define ASSERT_TRUE(target)\
  if(!target)\
  {\
    printf(COLOR_RED "\tFAIL:" COLOR_WHITE " actual is false, (line %d)\n" RESET, __LINE__);\
    printf(COLOR_WHITE "\t      Expected true\n");\
    printf(COLOR_WHITE "\t      Actual: %s\n" RESET, target ? "true" : "false" );\
    exit(1);\
  }

#define ASSERT_FALSE(target)\
  if(target)\
  {\
    printf(COLOR_RED "\tFAIL:" COLOR_WHITE " actual is true, (line %d)\n\n" RESET, __LINE__);\
    printf(COLOR_WHITE "\t      Expected false\n" RESET);\
    printf(COLOR_WHITE "\t      Actual: %s\n" RESET, target ? "true" : "false");\
    exit(1);\
  }

#endif // !UNIT_H

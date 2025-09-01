#ifndef LOGGER_H
#define LOGGER_H

#include "gbemu/common/types.h"
#include <stdlib.h>
#define ENABLE_LOGGING

#include <stdio.h>
#include <stdint.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#ifdef ENABLE_LOGGING
  #define LOG_INFO(fmt, ...)  printf("%s[INFO]%s - " fmt "\n", GREEN, RESET, ##__VA_ARGS__)
  #define LOG_ERROR(fmt, ...) printf("%s[ERROR]%s - " fmt "\n", RED, RESET, ##__VA_ARGS__)
  #define LOG_WARN(fmt, ...)  printf("%s[WARN]%s - " fmt "\n", YELLOW, RESET, ##__VA_ARGS__)
  #define LOG_DEBUG(fmt, ...) printf("%s[DEBUG]%s - " fmt "\n", CYAN, RESET, ##__VA_ARGS__)
#else
  #define LOG_INFO(fmt, ...)
  #define LOG_ERROR(fmt, ...)
  #define LOG_WARN(fmt, ...)
  #define LOG_DEBUG(fmt, ...)
#endif

static string_t bitsToString(uint8_t byte) {
   char* buffer = malloc(sizeof(char) * 9);
    for (int i = 7; i >= 0; i--) {
        buffer[7-i] = ((byte >> i) & 1) + '0';  // converte 0/1 para caractere '0'/'1'
    }
    buffer[8] = '\0';  // null terminator
  return buffer;
}
#endif // LOGGER_H

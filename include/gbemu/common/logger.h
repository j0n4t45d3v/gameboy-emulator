#ifndef LOGGER_H
#define LOGGER_H

#define ENABLE_LOGGING

#include <stdio.h>

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

#endif // LOGGER_H

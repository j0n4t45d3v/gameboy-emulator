#pragma once

#define LSB_8BIT(string_bits) string_bits & 0x0F 
#define MSB_8BIT(string_bits) (string_bits & 0xF0) >> 4
#define UNSIGNED_16(msb_bits, lsb_bits) (msb_bits << 8) | lsb_bits 

#ifndef MYUTILITY_H
#define MYUTILITY_H

#include <stddef.h>
#include <stdint.h>

#define C_PI 3.1415926535897932384626433832795
#define C_E  2.7182818284590452353602874713527

// Counts the number of occurrences of a character in a string
size_t count_chars(const char *str, char ch);

// Counts the number of trailing zeros
int trailing_zeros(uint32_t value);

#endif

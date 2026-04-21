#include "myutility.h"
#include <assert.h>
#include <stdint.h>

inline size_t count_chars(const char *str, char ch) {
    size_t count = 0;
    while (*str) {
        if (*str == ch) { ++count; }
        ++str;
    }
    return count;
}

int trailing_zeros(uint32_t value) {
    value &= ~value + 1;
    int count = 32;
    if (value != 0) { --count; }
    if (value & 0x0000FFFF) { count -= 16; }
    if (value & 0x00FF00FF) { count -= 8; }
    if (value & 0x0F0F0F0F) { count -= 4; }
    if (value & 0x33333333) { count -= 2; }
    if (value & 0x55555555) { count -= 1; }
    return count;
}

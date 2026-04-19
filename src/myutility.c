#include "myutility.h"

inline size_t count_chars(const char *str, char ch) {
    size_t count = 0;
    while (*str) {
        if (*str == ch) { ++count; }
        ++str;
    }
    return count;
}

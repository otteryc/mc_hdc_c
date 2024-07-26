#ifndef __COMMON_H
#define __COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* clang-format off */
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
/* clang-format on */

#define REALLOC(ptr, length)                                                   \
    do {                                                                       \
        ptr = realloc(ptr, length);                                            \
        if (unlikely(!ptr)) {                                                  \
            perror("\nCRITICAL: Unable to reallocate memory, exiting...");     \
            exit(1);                                                           \
        }                                                                      \
    } while (0);

#define MALLOC(ptr, length)                                                    \
    do {                                                                       \
        ptr = malloc(length);                                                  \
        if (unlikely(!ptr)) {                                                  \
            perror("\nCRITICAL: Unable to allocate memory, exiting...");       \
            exit(1);                                                           \
        }                                                                      \
    } while (0);

#define CALLOC(ptr, length, size)                                              \
    do {                                                                       \
        ptr = calloc(length, size);                                            \
        if (unlikely(!ptr)) {                                                  \
            perror("\nCRITICAL: Unable to allocate memory, exiting...");       \
            exit(1);                                                           \
        }                                                                      \
    } while (0);

#define FREE(ptr)                                                              \
    do {                                                                       \
        /* it is legal to free(NULL) */                                        \
        free(ptr);                                                             \
        ptr = NULL;                                                            \
    } while (0);

#define BITS_IN_BYTE 8

static inline bool get_bit_from_byte(uint8_t n, uint8_t index)
{
    return ((n & (1 << (7 - index))) > 0);
}

static inline void set_bit_in_byte(uint8_t *n, uint8_t index, bool value)
{
    *n |= value << (7 - index);
}

#endif

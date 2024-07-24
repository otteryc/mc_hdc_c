#ifndef __HDC_H
#define __HDC_H

#include "common.h"
#include "list.h"
#include <stdint.h>

struct Hypervector {
    union {
        uint8_t *hv;
        uint16_t *hv_16;
        uint32_t *hv_32;
    };
    uint32_t dimension;
    struct list_head list;
};

typedef struct Hypervector hv_t;

#define ITER_HV(hypervector, iterator)                                         \
    for (iterator = 0;                                                         \
         iterator < hypervector->dimension / (BITS_IN_BYTE * sizeof(uint8_t)); \
         iterator++)

#define ITER_HV_16(hypervector, iterator)                                      \
    for (iterator = 0; iterator < hypervector->dimension /                     \
                                      (BITS_IN_BYTE * sizeof(uint16_t));       \
         iterator++)

#define ITER_HV_32(hypervector, iterator)                                      \
    for (iterator = 0; iterator < hypervector->dimension /                     \
                                      (BITS_IN_BYTE * sizeof(uint32_t));       \
         iterator++)
#endif

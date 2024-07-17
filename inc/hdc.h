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

struct AssociativeMemory {
    hv_t *hv;
    char *class_name;
    /* As in some HDC model e.g. [1], there may have multi-HV for a class,
     * therefore, we declare a new `list_head` here for extensibility.
     * [1] M.Imani et al. Hierarchical Hyperdimensional Computing for Energy
     * Efficient Classification, DAC 2018 */
    struct list_head list;
};

typedef struct AssociativeMemory am_t;

struct QueryResult {
    union {
        double cosine;
        uint32_t hamming;
    };
    /* class_name will be a pointer to AssociativeMemory's class name, as
     * AssociativeMemory shall be a static variable in inference time.
     * Therefore, this pointer shall never be freed. */
    char *class_name;
    struct list_head list;
};

typedef struct QueryResult qry_t;

enum similarity_method {
    COSINE,
    HAMMING,
};

/* ForEach Macros */
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

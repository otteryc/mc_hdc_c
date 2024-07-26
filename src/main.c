#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "hdc.h"
#include "lib.h"
#include "list.h"

#define DIMENSION 2048
#define NUM_LEVELS 16
#define NUM_CLASSES 10

#define phv(name)                                                              \
    printf(#name ": \t");                                                      \
    print_hypervector(name);

#define cosine(src1, src2)                                                     \
    printf("Cosine Similarity between " #src1 " and " #src2 " is %lf.\n",      \
           cosine_similarity(src1, src2));

static inline hv_t *hv_from_chunk(uint8_t *chunk)
{
    hv_t *new;
    MALLOC(new, sizeof(hv_t));
    new->hv = chunk;
    new->dimension = DIMENSION;
    return new;
}

extern unsigned char level_hypervectors[][256];
extern unsigned char sample_hypervectors[][256];
extern unsigned char position_hypervectors[][256];

LIST_HEAD(am);
LIST_HEAD(level_hv);
LIST_HEAD(position);

void init()
{
    for (size_t i = 0; i < NUM_CLASSES; i++)
        chain_hypervector(&am, hv_from_chunk(sample_hypervectors[i]));
    for (size_t i = 0; i < NUM_LEVELS; i++)
        chain_hypervector(&level_hv, hv_from_chunk(level_hypervectors[i]));
    chain_hypervector(&position, hv_from_chunk(position_hypervectors[0]));
}
int main()
{
    init();
    cosine(list_first_entry(&level_hv, hv_t, list),
           list_first_entry(&am, hv_t, list));
}

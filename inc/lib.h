#ifndef _HDC__LIB_H
#define _HDC__LIB_H

#include "hdc.h"

hv_t *new_empty_hypervector(uint32_t dimension);
hv_t *new_random_hypervector(uint32_t dimension);
hv_t *new_identity_hypervector(uint32_t dimension);
hv_t *new_negate_hypervector(hv_t *hv);
hv_t *clone_hypervector(hv_t *hv);

hv_t *permute_hypervector(hv_t *hv, uint32_t shift);

void free_hypervector(hv_t *hv);

uint32_t hamming_distance(hv_t *hv1, hv_t *hv2);
uint32_t dot_distance(hv_t *hv1, hv_t *hv2);
double cosine_similarity(hv_t *hv1, hv_t *hv2);

void print_hypervector(hv_t *);

#endif

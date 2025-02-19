#ifndef __HDC_LIB_H
#define __HDC_LIB_H

#include "hdc.h"

hv_t *new_empty_hypervector(uint32_t dimension);
hv_t *new_random_hypervector(uint32_t dimension);
hv_t *new_identity_hypervector(uint32_t dimension);
hv_t *new_negate_hypervector(hv_t *hv);
hv_t *clone_hypervector(hv_t *hv);

hv_t *new_permute_hypervector(hv_t *hv, uint32_t shift);
#define hvops(name) hv_t *new_##name##_hypervector(hv_t *src1, hv_t *src2)
hvops(and);
hvops(or);
hvops(xor);
hvops(bind);
hvops(bundle);
#undef hvops
#define hvops(name) void name##_hypervector(hv_t *dest, hv_t *src1, hv_t *src2)
hvops(and);
hvops(or);
hvops(xor);
#undef hvops
hv_t *new_bundle_multi_hypervector(struct list_head *head);
hv_t *new_bind_multi_hypervector(struct list_head *head);

void free_hypervector(hv_t *hv);

uint32_t hamming_distance(hv_t *hv1, hv_t *hv2);
uint32_t dot_distance(hv_t *hv1, hv_t *hv2);
double cosine_similarity(hv_t *hv1, hv_t *hv2);

void chain_hypervector(struct list_head *head, hv_t *hv1, ...);
void print_hypervector(hv_t *hv);
struct list_head *query_memory(struct list_head *, hv_t *,
                               enum similarity_method);

void add_memory_item(struct list_head *am, hv_t *hv, char *class_name);
#endif

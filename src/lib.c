#include "common.h"
#include "hdc.h"
#include <stdlib.h>
#include <time.h>

hv_t *new_empty_hypervector(uint32_t dimension) {
  hv_t *new;
  MALLOC(new, sizeof(hv_t));
  CALLOC(new->hv, sizeof(uint8_t), dimension / sizeof(uint8_t));
  new->dimension = dimension;
  return new;
}

hv_t *new_random_hypervector(uint32_t dimensional) {
  hv_t *new = new_empty_hypervector(dimensional);
  uint32_t i = 0;
  srand(time(NULL));
  ITER_HV(new, i) { new->hv[i] = rand(); }
  return new;
}

hv_t *new_negate_hypervector(hv_t *hv) {
  hv_t *new = new_empty_hypervector(hv->dimension);
  uint32_t i = 0;
  ITER_HV(hv, i) { new->hv[i] = ~hv->hv[i]; }
  new->dimension = hv->dimension;
  return new;
}

void print_hypervector(hv_t *hv) {
  uint32_t i = 0;
  ITER_HV(hv, i) { printf("%x", hv->hv[i]); }
  printf("\n");
}

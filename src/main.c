#include <stdio.h>

#include "hdc.h"
#include "lib.h"

int main() {
  hv_t *random = new_random_hypervector(10000);
  hv_t *inverse_random = new_negate_hypervector(random);
  double similarity = cosine_similarity(random, random);
  uint32_t hamming = hamming_distance(random, inverse_random);
  printf("similarity: %lf\n", similarity);
  printf("hamming: %u\n", hamming);
}

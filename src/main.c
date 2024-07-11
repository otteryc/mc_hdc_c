#include <stdio.h>

#include "hdc.h"
#include "lib.h"

int main() {
  hv_t *random = new_random_hypervector(64);
  hv_t *permute = new_permute_hypervector(random, 1);
  hv_t *permute7 = new_permute_hypervector(random, 7);
  hv_t *permute8 = new_permute_hypervector(random, 8);
  hv_t *permute16 = new_permute_hypervector(random, 16);
  hv_t *permute17 = new_permute_hypervector(random, 17);
  hv_t *inverse = new_negate_hypervector(random);

  printf("random:  ");
  print_hypervector(random);
  printf("inverse: ");
  print_hypervector(inverse);

  printf("permute7:  ");
  print_hypervector(permute7);
  printf("permute8:  ");
  print_hypervector(permute8);
  printf("permute16: ");
  print_hypervector(permute16);
  printf("permute17: ");
  print_hypervector(permute17);

  printf("Cosine Similarity between random and itself: %lf\n",
         cosine_similarity(random, random));
  printf("Cosine Similarity between random and its permute: %lf\n",
         cosine_similarity(random, permute));
  printf("Cosine Similarity between random and its inverse: %lf\n",
         cosine_similarity(random, inverse));

  free_hypervector(random);
  free_hypervector(permute);
  free_hypervector(permute7);
  free_hypervector(permute8);
  free_hypervector(permute16);
  free_hypervector(permute17);
  free_hypervector(inverse);
}

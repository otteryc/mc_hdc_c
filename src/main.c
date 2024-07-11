#include <stdio.h>

#include "hdc.h"
#include "lib.h"

int main() {
  hv_t *random = new_random_hypervector(64);
  hv_t *permute7 = permute_hypervector(random, 7);
  hv_t *permute8 = permute_hypervector(random, 8);
  hv_t *permute16 = permute_hypervector(random, 16);
  hv_t *permute17 = permute_hypervector(random, 17);
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
}

#include <stdio.h>
#include <unistd.h>

#include "hdc.h"
#include "lib.h"

#define phv(name)                                                              \
    printf(#name ": \t");                                                      \
    print_hypervector(name);

int main()
{
    hv_t *random = new_random_hypervector(96);
    hv_t *random1 = new_random_hypervector(96);
    hv_t *permute = new_permute_hypervector(random, 1);
    hv_t *inverse = new_negate_hypervector(random);
    hv_t *bind = new_bind_hypervector(random, random1);
    hv_t *bundle = new_bundle_hypervector(random, random1);

    phv(random);
    phv(random1);
    phv(bind);
    phv(bundle);
    phv(permute);

    printf("Cosine Similarity between random and bind: %lf\n",
           cosine_similarity(random, bind));
    printf("Cosine Similarity between random and bundle: %lf\n",
           cosine_similarity(random, bundle));
    printf("Cosine Similarity between random and its permute: %lf\n",
           cosine_similarity(random, permute));
    printf("Cosine Similarity between random and its inverse: %lf\n",
           cosine_similarity(random, inverse));

    free_hypervector(random);
    free_hypervector(permute);
    free_hypervector(inverse);
}

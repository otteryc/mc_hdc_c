#include <stdio.h>
#include <unistd.h>

#include "hdc.h"
#include "lib.h"

#define phv(name)                                                              \
    printf(#name ": \t");                                                      \
    print_hypervector(name);

#define cosine(src1, src2)                                                     \
    printf("Cosine Similarity between " #src1 " and " #src2 " is %lf.\n",      \
           cosine_similarity(src1, src2));

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

    cosine(random, bind);

    free_hypervector(random);
    free_hypervector(permute);
    free_hypervector(inverse);
}

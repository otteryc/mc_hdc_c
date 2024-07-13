#include <stdio.h>
#include <unistd.h>

#include "hdc.h"
#include "lib.h"
#include "list.h"

#define DIMENSION 10000

#define phv(name)                                                              \
    printf(#name ": \t");                                                      \
    print_hypervector(name);

#define cosine(src1, src2)                                                     \
    printf("Cosine Similarity between " #src1 " and " #src2 " is %lf.\n",      \
           cosine_similarity(src1, src2));

int main()
{
    hv_t *random = new_random_hypervector(DIMENSION);
    hv_t *random1 = new_random_hypervector(DIMENSION);
    hv_t *permute = new_permute_hypervector(random, 1);
    hv_t *inverse = new_negate_hypervector(random);
    hv_t *bind = new_bind_hypervector(random, random1);
    hv_t *bundle = new_bundle_hypervector(random, random1);

    LIST_HEAD(head);
    list_add_tail(&random->list, &head);
    list_add_tail(&random1->list, &head);
    list_add_tail(&permute->list, &head);
    list_add_tail(&bundle->list, &head);

    hv_t *bundle_together = new_bundle_multi_hypervector(&head);
    hv_t *bind_together = new_bind_multi_hypervector(&head);
    phv(random);
    phv(random1);
    phv(bundle);
    phv(permute);
    phv(bundle_together);
    phv(bind_together);

    cosine(random, bind_together);
    cosine(random, bundle_together);

    free_hypervector(random);
    free_hypervector(permute);
    free_hypervector(inverse);
}

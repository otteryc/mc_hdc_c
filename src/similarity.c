#include <math.h>

#include "hdc.h"

uint32_t hamming_distance(hv_t *hv1, hv_t *hv2)
{
    uint32_t i, distance = 0, tmp;
    ITER_HV_32(hv1, i)
    {
        tmp = hv1->hv_32[i] ^ hv2->hv_32[i];
        distance += __builtin_popcount(tmp);
    }
    return distance;
}

uint32_t dot_distance(hv_t *hv1, hv_t *hv2)
{
    uint32_t i, dot_product = 0, tmp;
    ITER_HV_32(hv1, i)
    {
        tmp = hv1->hv_32[i] & hv2->hv_32[i];
        dot_product += __builtin_popcount(tmp);
    }
    return dot_product;
}

double cosine_similarity(hv_t *hv1, hv_t *hv2)
{
    uint32_t i, dot_product = 0, length_hv1 = 0, length_hv2 = 0, tmp;
    ITER_HV_32(hv1, i)
    {
        tmp = hv1->hv_32[i] & hv2->hv_32[i];
        dot_product += __builtin_popcount(tmp);
        length_hv1 += __builtin_popcount(hv1->hv_32[i]);
        length_hv2 += __builtin_popcount(hv2->hv_32[i]);
    }
    return (double)dot_product / (double)sqrt(length_hv1 * length_hv2);
}

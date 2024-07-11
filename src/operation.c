#include "common.h"
#include "hdc.h"
#include "lib.h"
#include <stdint.h>
#include <string.h>

/* For readability, compiler will optimize it */
static hv_t *fast_permute_32(hv_t *hv)
{
    memmove(hv->hv + 4, hv->hv, hv->dimension / BITS_IN_BYTE);
    hv->hv[0] = hv->hv[hv->dimension / BITS_IN_BYTE + 0];
    hv->hv[1] = hv->hv[hv->dimension / BITS_IN_BYTE + 1];
    hv->hv[2] = hv->hv[hv->dimension / BITS_IN_BYTE + 2];
    hv->hv[3] = hv->hv[hv->dimension / BITS_IN_BYTE + 3];
    hv->hv[hv->dimension / BITS_IN_BYTE + 0] = 0;
    hv->hv[hv->dimension / BITS_IN_BYTE + 1] = 0;
    hv->hv[hv->dimension / BITS_IN_BYTE + 2] = 0;
    hv->hv[hv->dimension / BITS_IN_BYTE + 3] = 0;
    return hv;
}
static hv_t *fast_permute_16(hv_t *hv)
{
    memmove(hv->hv + 2, hv->hv, hv->dimension / BITS_IN_BYTE);
    hv->hv[0] = hv->hv[hv->dimension / BITS_IN_BYTE + 0];
    hv->hv[1] = hv->hv[hv->dimension / BITS_IN_BYTE + 1];
    hv->hv[hv->dimension / BITS_IN_BYTE + 0] = 0;
    hv->hv[hv->dimension / BITS_IN_BYTE + 1] = 0;
    return hv;
}

static hv_t *fast_permute_8(hv_t *hv)
{
    memmove(hv->hv + 1, hv->hv, hv->dimension / BITS_IN_BYTE);
    hv->hv[0] = hv->hv[hv->dimension / BITS_IN_BYTE];
    hv->hv[hv->dimension / BITS_IN_BYTE] = 0;
    return hv;
}

static hv_t *fast_permute(hv_t *hv, uint8_t shift)
{
    if (!shift)
        return hv;
    uint32_t i;
    uint8_t prev_lower = 0, current_lower;
    ITER_HV(hv, i)
    {
        current_lower = hv->hv[i] & ((i << shift) - 1);
        hv->hv[i] = hv->hv[i] >> shift;
        hv->hv[i] |= prev_lower;
        prev_lower = current_lower << (8 - shift);
    }
    hv->hv[0] |= prev_lower;
    return hv;
}

hv_t *new_permute_hypervector(hv_t *hv, uint32_t shift)
{
    hv_t *clone = clone_hypervector(hv);
    while (shift >= 8) {
        if (shift >= 32) {
            fast_permute_32(clone);
            shift -= 32;
        } else if (shift >= 16) {
            fast_permute_16(clone);
            shift -= 16;
        } else {
            fast_permute_8(clone);
            shift -= 8;
        }
    }

    return fast_permute(clone, shift);
}

void negate_hypervector(hv_t *hv)
{
    uint32_t i;
    ITER_HV_32(hv, i)
    {
        hv->hv_32[i] = ~hv->hv_32[i];
    }
}

#define hvops(name, op)                                                        \
    void name##_hypervector(hv_t *dest, hv_t *src1, hv_t *src2)                \
    {                                                                          \
        uint32_t i;                                                            \
        ITER_HV_32(src1, i)                                                    \
        {                                                                      \
            dest->hv_32[i] = src1->hv_32[i] op src2->hv_32[i];                 \
        }                                                                      \
    }

hvops(xor, ^);
hvops(or, |);
hvops(and, &);
#undef hvops

#define hvops(name, op)                                                        \
    hv_t *new_##name##_hypervector(hv_t *src1, hv_t *src2)                     \
    {                                                                          \
        uint32_t i;                                                            \
        hv_t *dest = clone_hypervector(src1);                                  \
        ITER_HV_32(src2, i)                                                    \
        {                                                                      \
            dest->hv_32[i] op## = src2->hv_32[i];                              \
        }                                                                      \
        return dest;                                                           \
    }

hvops(xor, ^);
hvops(or, |);
hvops(and, &);
#undef hvops

/* BIND: dissimilar to both */
hv_t *new_bind_hypervector(hv_t *src1, hv_t *src2)
{
    return new_xor_hypervector(src1, src2);
}

/* BUNDLE: maximally similar to both */
hv_t *new_bundle_hypervector(hv_t *src1, hv_t *src2)
{
    hv_t * or = new_or_hypervector(src1, src2);
    hv_t * xor = new_xor_hypervector(src1, src2);
    hv_t *tiebreaker = new_random_hypervector(src1->dimension);
    and_hypervector(tiebreaker, tiebreaker, xor);
    negate_hypervector(tiebreaker);
    and_hypervector(or, tiebreaker, or);
    free_hypervector(tiebreaker);
    free_hypervector(xor);
    return or ;
}

#include "common.h"
#include "hdc.h"
#include "list.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* This function returns an uninitiallized Hypervector, which should not be used
 * anywhere except in this file. */
static hv_t *new_hypervector(uint32_t dimension)
{
    hv_t *new;
    MALLOC(new, sizeof(hv_t));
    /* +4 byte padding for accelerating permute operation */
    MALLOC(new->hv, dimension / (sizeof(uint8_t) * BITS_IN_BYTE) + 4);
    new->dimension = dimension;
    INIT_LIST_HEAD(&new->list);
    return new;
}

hv_t *new_empty_hypervector(uint32_t dimension)
{
    hv_t *new;
    MALLOC(new, sizeof(hv_t));
    /* +4 byte padding for accelerating permute operation */
    CALLOC(new->hv, sizeof(uint8_t),
           dimension / (sizeof(uint8_t) * BITS_IN_BYTE) + 4);
    new->dimension = dimension;
    INIT_LIST_HEAD(&new->list);
    return new;
}

hv_t *new_random_hypervector(uint32_t dimensional)
{
    hv_t *new = new_hypervector(dimensional);
    arc4random_buf(new->hv, new->dimension / BITS_IN_BYTE);
    return new;
}

hv_t *new_identity_hypervector(uint32_t dimension)
{
    hv_t *new = new_hypervector(dimension);
    memset(new->hv, 0xFF, new->dimension / sizeof(uint8_t));
    return new;
}

hv_t *new_negate_hypervector(hv_t *hv)
{
    hv_t *new = new_hypervector(hv->dimension);
    uint32_t i = 0;
    ITER_HV_32 (hv, i) {
        new->hv_32[i] = ~hv->hv_32[i];
    }
    return new;
}

hv_t *clone_hypervector(hv_t *hv)
{
    hv_t *clone = new_hypervector(hv->dimension);
    memcpy(clone->hv, hv->hv, hv->dimension / BITS_IN_BYTE);
    return clone;
}

void free_hypervector(hv_t *hv)
{
    FREE(hv->hv);
    FREE(hv);
}

static void pbin(uint8_t byte)
{
    for (int i = 7; i >= 0; i--) {
        printf("%c", (byte & (1 << i)) ? '1' : '0');
    }
    putchar(' ');
}

void print_hypervector(hv_t *hv)
{
    uint32_t i = 0;
    ITER_HV (hv, i) {
        pbin(hv->hv[i]);
    }
    printf("\n");
}

/* End with a pointer to NULL */
void chain_hypervector(struct list_head *head, hv_t *hv1, ...)
{
    va_list va;
    hv_t *hv;

    va_start(va, hv1);
    for (hv = hv1; hv != NULL; hv = va_arg(va, hv_t *)) {
        list_add_tail(&hv1->list, head);
    }
    va_end(va);
}

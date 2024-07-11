#include "common.h"
#include "hdc.h"
#include "lib.h"
#include <stdint.h>
#include <string.h>

static uint8_t power_of_2[] = {
    [1] = 0b1,     [2] = 0b11,     [3] = 0b111,     [4] = 0b1111,
    [5] = 0b11111, [6] = 0b111111, [7] = 0b1111111,
};

/* For readability, compiler will optimize it */
static hv_t *fast_permute_32(hv_t *hv) {
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
static hv_t *fast_permute_16(hv_t *hv) {
  memmove(hv->hv + 2, hv->hv, hv->dimension / BITS_IN_BYTE);
  hv->hv[0] = hv->hv[hv->dimension / BITS_IN_BYTE + 0];
  hv->hv[1] = hv->hv[hv->dimension / BITS_IN_BYTE + 1];
  hv->hv[hv->dimension / BITS_IN_BYTE + 0] = 0;
  hv->hv[hv->dimension / BITS_IN_BYTE + 1] = 0;
  return hv;
}

static hv_t *fast_permute_8(hv_t *hv) {
  memmove(hv->hv + 1, hv->hv, hv->dimension / BITS_IN_BYTE);
  hv->hv[0] = hv->hv[hv->dimension / BITS_IN_BYTE];
  hv->hv[hv->dimension / BITS_IN_BYTE] = 0;
  return hv;
}

static hv_t *fast_permute(hv_t *hv, uint8_t shift) {
  if (!shift)
    return hv;
  uint32_t i;
  uint8_t prev_lower = 0, current_lower;
  ITER_HV(hv, i) {
    current_lower = hv->hv[i] & power_of_2[shift];
    current_lower = current_lower << (8 - shift);
    hv->hv[i] = hv->hv[i] >> shift;
    hv->hv[i] |= prev_lower;
    prev_lower = current_lower;
  }
  hv->hv[0] |= prev_lower;
  return hv;
}

hv_t *new_permute_hypervector(hv_t *hv, uint32_t shift) {
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

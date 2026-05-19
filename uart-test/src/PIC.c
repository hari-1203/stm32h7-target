#include "PIC.h"
#include "libopencm3/stm32/h7/flash.h"
#include "rng.h"
#include <stdint.h>

void pic_tag_gen(pic_ctx_t *ctx, const void *ptr) {
  if (ctx == NULL)
    return;

  ctx->ptr = ptr;
  rng_read_word(&ctx->cookie);
  ctx->tag = (uint32_t)(uintptr_t)ptr ^ ctx->cookie;
}

int pic_tag_verify(const pic_ctx_t *ctx, const void *ptr) {
  if (ctx == NULL)
    return -1;

  uint32_t expected = (uint32_t)(uintptr_t)ptr ^ ctx->cookie;
  return (expected == ctx->tag) ? 0 : -1;
}

int seed_compare(const void *ptr1, const void *ptr2) {
  return ((uint32_t)(uintptr_t)ptr1 == (uint32_t)(uintptr_t)ptr2) ? -1 : 0;
}

static inline uint32_t popcount8(uint8_t x) {
  return (uint32_t)__builtin_popcount((unsigned)x);
}

int seed_entropy_reject(const uint8_t *seed, size_t n) {
  if (seed == 0 || n == 0)
    return -1; /* invalid input => reject */

  uint32_t weight = 0;
  for (size_t i = 0; i < n; i++) {
    weight += popcount8(seed[i]);
  }

  /* Expected ~4 bits set per byte on average => ~4*n total.
     Use very wide bounds to avoid false rejects.
     For n=32: expected ~128; bounds below accept [32..224].
  */
  const uint32_t min_w = (uint32_t)(1U * n); /* avg 1 bit/byte */
  const uint32_t max_w = (uint32_t)(7U * n); /* avg 7 bits/byte */

  return (weight < min_w || weight > max_w) ? -1 : 0;
}

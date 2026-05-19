#ifndef PIC_H
#define PIC_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  const void *ptr; /* protected pointer */
  uint32_t cookie; /* 32-bit random cookie */
  uint32_t tag;    /* tag = (uint32_t)ptr ^ cookie */
} pic_ctx_t;

void pic_tag_gen(pic_ctx_t *ctx, const void *ptr);

int pic_tag_verify(const pic_ctx_t *ctx, const void *ptr);

int seed_compare(const void *ptr1, const void *ptr2);

int seed_is_degenerate(const uint8_t *seed, size_t n);

int seed_entropy_reject(const uint8_t *seed, size_t n);

#endif // !PIC_H

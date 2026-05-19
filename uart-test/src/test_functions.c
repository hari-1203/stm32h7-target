#include "test_functions.h"
#include "PIC.h"
#include "config.h"
#include "notrandombytes.h"
#include "sha3.h"
#include "usart.h"
#include <stdint.h>

void test_function_skip(void) {
  uint8_t a[10] = {95, 13, 185, 55, 246, 1, 161, 158, 53, 251};
  uint8_t b[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  gpio_set(TRIGGER_PORT, TRIGGER_PIN);
  __asm__("nop");
  __asm__("nop");
  __asm__("nop");
  __asm__("nop");
  __asm__("nop");
  function_to_skip(a, b);

  for (int i = 0; i < 10000; i++) {
    __asm__("nop");
  }

  usart_write(a, sizeof(a));
}

void function_to_skip(uint8_t *a, uint8_t *b) {
  __asm__("nop");
  __asm__("nop");
  __asm__("nop");
  __asm__("nop");
  __asm__("nop");
  gpio_clear(TRIGGER_PORT, TRIGGER_PIN);

  for (int i = 0; i < 10; i++) {
    a[i] = a[i] - b[i];
  }
}

static int dilithium_squeeze256(wc_Shake *shake256, const byte *in,
                                word32 inLen, byte *out, word32 outBlocks) {
  int ret;
  /* Initialize SHAKE-256 operation. */
  ret = wc_InitShake256(shake256, NULL, INVALID_DEVID);
  if (ret == 0) {
    /* Absorb data - update plus final. */
    ret = wc_Shake256_Absorb(shake256, in, inLen);
  }
  if (ret == 0) {
    /* Squeeze out hash data. */
    ret = wc_Shake256_SqueezeBlocks(shake256, out, outBlocks);
  }

  return ret;
}

void dilithium_function_test(wc_Shake *shake256, byte *seed, word16 kappa,
                             byte gamma1_bits, sword32 *y, byte l) {

  __asm__("nop");
  __asm__("nop");
  __asm__("nop");
  __asm__("nop");
  __asm__("nop");
  gpio_clear(TRIGGER_PORT, TRIGGER_PIN);

  byte v[DILITHIUM_MAX_V];

  dilithium_squeeze256(shake256, seed, DILITHIUM_Y_SEED_SZ, v,
                       DILITHIUM_MAX_V_BLOCKS);

  usart_write(seed, DILITHIUM_Y_SEED_SZ);
  // usart_write(v, DILITHIUM_MAX_V);
}

int test_countermeasures(void) {
  uint8_t buf[2 * 32];
  uint8_t *rho = buf;
  uint8_t *sigma = buf + 32;

  randstate = 1;
  // randombytes(buf, 64);
  for (int i = 0; i < 64; i++) {
    buf[i] = 0;
  }

  gpio_set(TRIGGER_PORT, TRIGGER_PIN);
  if (seed_entropy_reject(sigma, 32) == 0) {
    for (int i = 0; i < 64; i++) {
      buf[i] = 2 * i;
    }
  } else {
    gpio_clear(TRIGGER_PORT, TRIGGER_PIN);
    return -1;
  }
  gpio_clear(TRIGGER_PORT, TRIGGER_PIN);

  return 0;
}

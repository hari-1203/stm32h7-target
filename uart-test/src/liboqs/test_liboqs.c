#include "test_liboqs.h"
#include "config.h"
#include "indcpa.h"
#include "kem.h"
#include "liboqs/notrandombytes.h"
#include "mldsa_native.h"
#include "mlkem_native.h"
#include "params.h"
#include "sign.h"
#include "usart.h"
#include <stddef.h>
#include <stdint.h>

int loopstate = 0;

void test_liboqs_keygen(uint8_t c) {
  uint8_t pk[MLKEM768_PUBLICKEYBYTES], sk[MLKEM768_SECRETKEYBYTES];
  uint8_t coins[MLKEM_SYMBYTES] = {
      0 ^ c, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
      16,    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

  randstate = 0;
  gpio_set(TRIGGER_PORT, TRIGGER_PIN);
  // PQCP_MLKEM_NATIVE_MLKEM768_keypair(pk, sk);
  PQCP_MLKEM_NATIVE_MLKEM768_indcpa_keypair_derand(pk, sk, coins);
}

void test_liboqs_encaps(uint8_t c, uint8_t *pk) {

  uint8_t ct[MLKEM768_CIPHERTEXTBYTES], ss[MLKEM_SSBYTES];

  randstate = c;

  gpio_set(TRIGGER_PORT, TRIGGER_PIN);
  PQCP_MLKEM_NATIVE_MLKEM768_enc(ct, ss, pk);
  gpio_clear(TRIGGER_PORT, TRIGGER_PIN);

  usart_write(ct, 4);
}

void test_liboqs_dilithium_sign(uint8_t c, uint8_t *sk) {

  uint8_t sig[MLDSA44_BYTES], msg[32];
  size_t siglen = MLDSA44_BYTES;

  loopstate = 0;
  randstate = c;
  randombytes(msg, sizeof(msg));

  PQCP_MLDSA_NATIVE_MLDSA44_signature(sig, &siglen, msg, 32, NULL, 0, sk);
}

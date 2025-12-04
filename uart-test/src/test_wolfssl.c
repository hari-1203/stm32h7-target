#include "test_wolfssl.h"

static unsigned char rand_seed[64] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA,
    0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
    0xF0, 0x0D, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x13,
    0x37, 0xC0, 0xDE, 0xCA, 0xFE, 0xBA, 0xBE, 0xFE, 0xED, 0xFA, 0xCE,
    0x55, 0xAA, 0x55, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x22, 0x33, 0x44,
    0x55, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

void test_keygen(byte c) {

  MlKemKey key;
  /* --- Key generation --- */
  __builtin_memset(&key, 0, sizeof(MlKemKey));

  rand_seed[0] = c;

  gpio_set(TRIGGER_PORT, TRIGGER_PIN);

  int ret = wc_KyberKey_MakeKeyWithRandom(&key, rand_seed, sizeof(rand_seed));

  // if (ret == 0) {
  //   usart_write(key.pub, sizeof(key.pub));
  // } else {
  //   const uint8_t fail[] = "FAIL\r\n";
  //   putstr(fail);
  // }
  wc_KyberKey_Free(&key);
}

void test_encaps(byte c) {

  int ret;

  MlKemKey key;
  __builtin_memset(&key, 0, sizeof(MlKemKey));

  rand_seed[0] = 0x00;

  do {
    ret = wc_KyberKey_MakeKeyWithRandom(&key, rand_seed, sizeof(rand_seed));
  } while (ret != 0);

  unsigned char m[WC_ML_KEM_ENC_RAND_SZ] = {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
      16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

  m[0] = c;

  unsigned char ct[WC_ML_KEM_MAX_CIPHER_TEXT_SIZE];
  unsigned char k[WC_ML_KEM_SS_SZ];

  ret = wc_KyberKey_EncapsulateWithRandom(&key, ct, k, m, sizeof(m));
}

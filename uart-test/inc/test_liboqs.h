#ifndef TEST_LIBOQS_H
#define TEST_LIBOQS_H

#include <stdint.h>
#define MLK_DEFAULT_NAMESPACE_PREFIX mlkem

#include "config.h"
#include "liboqs/mldsa/src/common.h"
#include "liboqs/mldsa_native.h"
#include "liboqs/mlkem/src/common.h"
#include "liboqs/mlkem_native.h"
#include "liboqs/notrandombytes.h"

extern int loopstate;

void test_liboqs_keygen(uint8_t);
void test_liboqs_encaps(uint8_t, uint8_t *);
void test_liboqs_dilithium_sign(uint8_t, uint8_t *);

#endif // !TEST_LIBOQS_H

#ifndef TEST_WOLFSSL_H
#define TEST_WOLFSSL_H

#include "config.h"
#include "dilithium.h"
#include "error-crypt.h"
#include "wolfssl/wolfcrypt/mlkem.h"
#include "wolfssl/wolfcrypt/wc_mlkem.h"

extern int wolfssl_loopstate;
extern int dsa_kappa;

void test_keygen(byte);
void test_encaps(byte);

void wolfssl_dsa_keygen(dilithium_key *key);
void test_dilithium_sign(byte, dilithium_key *);
#endif // !TEST_FUNCTIONS_H

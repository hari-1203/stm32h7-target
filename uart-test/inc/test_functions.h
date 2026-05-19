#ifndef TEST_FUNC_H
#define TEST_FUNC_H

#include "dilithium.h"
#include "sha3.h"
#include <stdint.h>

/* MAX: (256 * 8 / (17 + 1)) = 576, or ((256 * 8 / (19 + 1)) = 640
 * but need blocks of 17 * 8 bytes: 5 * 17 * 8 = 680 */
#define DILITHIUM_MAX_V_BLOCKS 5
/* Maximum number of bytes to generate into v to make y. */
#define DILITHIUM_MAX_V (DILITHIUM_MAX_V_BLOCKS * 8 * 17)
#define DILITHIUM_Y_SEED_SZ (DILITHIUM_PRIV_RAND_SEED_SZ + 2)

void test_function_skip(void);
void function_to_skip(uint8_t *, uint8_t *);
void dilithium_function_test(wc_Shake *shake256, byte *seed, word16 kappa,
                             byte gamma1_bits, sword32 *y, byte l);
int test_countermeasures(void);

#endif // !TEST_FUNC_H

/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/h7/rng.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <stdint.h>

#include "PIC.h"
#include "config.h"
#include "core/system.h"
#include "core/usart.h"
#include "dilithium.h"
#include "kem.h"
#include "liboqs/notrandombytes.h"
#include "mlkem_native.h"
#include "rng.h"
#include "sha3.h"
#include "sign.h"
#include "test_functions.h"
#include "test_liboqs.h"
#include "test_wolfssl.h"

// static void putstr(const char *s) {
//   while (*s)
//     usart_send_blocking(USARTx, *s++);
// }

static inline void usart_write_u32_le(uint32_t v) {
  uint8_t b[4] = {(uint8_t)(v & 0xFF), (uint8_t)((v >> 8) & 0xFF),
                  (uint8_t)((v >> 16) & 0xFF), (uint8_t)((v >> 24) & 0xFF)};
  usart_write(b, 4);
}

int main(void) {
  //  int i, j = 0, c = 0;

  clock_setup();
  gpio_setup();
  usart_setup();
  rng_init();

  int ret;
  byte sig_seed[DILITHIUM_RND_SZ] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2,
                                     2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5,
                                     5, 5, 6, 6, 6, 6, 7, 7, 7, 7};

  // pic_ctx_t tag;

  // uint8_t *publicseed = sig_seed;
  // uint8_t *secretseed = sig_seed + 16;

  // pic_tag_gen(&tag, sig_seed);
  //
  dilithium_key key;

  wolfssl_dsa_keygen(&key);

  while (1) {

    int c = usart_read_byte();

    test_dilithium_sign(c, &key);

    // dilithium_function_test(&key.shake, seed, 0, params->gamma1_bits, y,
    // params->l);

    // usart_write_u32_le(ret);
    // uint8_t arr[20];
    // usart_write_u32_le((uint32_t)arr);
  }

  return 0;
}

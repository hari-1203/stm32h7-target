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

#include "config.h"
#include "core/system.h"
#include "core/usart.h"
#include "kem.h"
#include "liboqs/notrandombytes.h"
#include "mlkem_native.h"
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

  // uint8_t pk[MLKEM768_PUBLICKEYBYTES], sk[MLKEM768_SECRETKEYBYTES];
  // randstate = 0;
  // PQCP_MLKEM_NATIVE_MLKEM768_keypair(pk, sk);

  uint8_t pk[MLDSA44_PUBLICKEYBYTES], sk[MLDSA44_SECRETKEYBYTES];
  randstate = 0;
  PQCP_MLDSA_NATIVE_MLDSA44_keypair(pk, sk);

  /* Blink the LED (PD12) on the board with every transmitted byte. */
  while (1) {

    // gpio_toggle(LED_PORT, LED_PIN);

    int c = usart_read_byte();

    test_liboqs_dilithium_sign(c, sk);

    // uint8_t data[20];
    // rng_generate_data(data, sizeof(data));

    // usart_write(data, sizeof(data));
  }

  return 0;
}

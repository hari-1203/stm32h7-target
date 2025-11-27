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

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/h7/rng.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <stdint.h>

#include "config.h"
#include "core/rng.h"
#include "core/system.h"
#include "core/usart.h"
#include "libopencm3/stm32/h7/rcc.h"

// #include "wolfssl/wolfcrypt/mlkem.h"
// #include "wolfssl/wolfcrypt/wc_mlkem.h"

static void putstr(const char *s) {
  while (*s)
    usart_send_blocking(USARTx, *s++);
}

// static void test_keygen(void) {
//
//   byte rand[64] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
//   0x99,
//                    0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xDE, 0xAD, 0xBE,
//                    0xEF, 0xBA, 0xAD, 0xF0, 0x0D, 0x12, 0x34, 0x56, 0x78,
//                    0x9A, 0xBC, 0xDE, 0xF0, 0x13, 0x37, 0xC0, 0xDE, 0xCA,
//                    0xFE, 0xBA, 0xBE, 0xFE, 0xED, 0xFA, 0xCE, 0x55, 0xAA,
//                    0x55, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x22, 0x33, 0x44,
//                    0x55, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
//
//   MlKemKey key;
//   /* --- Key generation --- */
//   __builtin_memset(&key, 0, sizeof(KyberKey));
//
//   int ret = wc_KyberKey_MakeKeyWithRandom(&key, rand, sizeof(rand));
//
//   if (ret == 0) {
//     const uint8_t ok[] = "OK\r\n";
//     putstr(ok);
//   } else {
//     const uint8_t fail[] = "FAIL\r\n";
//     putstr(fail);
//   }
//   wc_KyberKey_Free(&key);
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

  /* Blink the LED (PD12) on the board with every transmitted byte. */
  while (1) {

    gpio_toggle(LED_PORT, LED_PIN);

    int c = usart_read_byte();
    // usart_write_byte(c + 1);

    uint8_t data[20];
    rng_generate_data(data, sizeof(data));

    usart_write(data, sizeof(data));
  }

  return 0;
}

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

/*
 * Test protocol (single-byte command dispatch):
 *
 *  'e' + N + <N bytes>  — echo: read N bytes, send them back
 *  'r' + N              — rng:  send N random bytes
 *  'p'                  — ping: reply with 'P' (checks UART is alive)
 *
 * Trigger pin toggles on every command, giving a scope edge to latch on.
 */

#define CMD_ECHO 'e'
#define CMD_RNG 'r'
#define CMD_PING 'p'

int main(void) {
  clock_setup();
  gpio_setup();
  usart_setup();
  rng_init();

  while (1) {
    uint8_t cmd = usart_read_byte();
    gpio_toggle(TRIGGER_PORT, TRIGGER_PIN);

    if (cmd == CMD_PING) {
      usart_write_byte('P');

    } else if (cmd == CMD_ECHO) {
      uint8_t len = usart_read_byte();
      uint8_t buf[256];
      usart_read(buf, len);
      usart_write(buf, len);

    } else if (cmd == CMD_RNG) {
      uint8_t len = usart_read_byte();
      uint8_t buf[256];
      rng_generate_data(buf, len);
      usart_write(buf, len);
    }
  }

  return 0;
}

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
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include "config.h"
#include "core/system.h"
#include "core/usart.h"

int main(void) {
  int i, j = 0, c = 0;

  clock_setup();
  gpio_setup();
  usart_setup();

  /* Blink the LED (PD12) on the board with every transmitted byte. */
  while (1) {

    gpio_toggle(LED_PORT, LED_PIN);       /* LED on/off */
    usart_send_blocking(USARTx, c + '0'); /* USART2: Send byte. */
    c = (c >= 9) ? 0 : c + 2;             /* Increment c. */
    if ((j++ % 80) == 0) {                /* Newline after line full. */
      usart_send_blocking(USARTx, '\r');
      usart_send_blocking(USARTx, '\n');
    }

    for (i = 0; i < 10000000; i++) { /* Wait a bit. */
      __asm__("NOP");
    }
  }

  return 0;
}

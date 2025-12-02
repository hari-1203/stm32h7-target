#include "core/system.h"
#include "config.h"
#include "libopencm3/stm32/h7/rcc.h"
#include <libopencm3/stm32/h7/rng.h>
#include <stdint.h>

void clock_setup(void) {

  rcc_clock_setup_pll(&rcc_cfg_64mhz);
  // rcc_clock_setup_pll(&rcc_cfg_400mhz_hsi);

  /* Enable GPIOD clock for LED & USARTs. */
  rcc_periph_clock_enable(CLK_TRIGGER_PORT);
  rcc_periph_clock_enable(CLK_USART_RX_PORT);
  rcc_periph_clock_enable(CLK_USART_TX_PORT);

  /* Enable clocks for USART. */
  rcc_periph_clock_enable(CLK_USARTx);
}

void gpio_setup(void) {
  /* Setup GPIO pin GPIO12 on GPIO port D for LED. */
  gpio_mode_setup(TRIGGER_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, TRIGGER_PIN);

  /* Setup GPIO pins for USART3 transmit. */
  gpio_mode_setup(USART_PORT_RX, GPIO_MODE_AF, GPIO_PUPD_NONE, USART_RX);

  /* Setup GPIO pins for USART3 receive. */
  gpio_mode_setup(USART_PORT_TX, GPIO_MODE_AF, GPIO_PUPD_NONE, USART_TX);

  /* Setup USART2 TX pin as alternate function. */
  gpio_set_af(USART_PORT_RX, GPIO_AF7, USART_RX);
  gpio_set_af(USART_PORT_TX, GPIO_AF7, USART_TX);

  gpio_mode_setup(USART_PORT_RX, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_RX);
  gpio_mode_setup(USART_PORT_TX, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_TX);

  gpio_set_output_options(USART_PORT_RX, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                          USART_RX);
  gpio_set_output_options(USART_PORT_TX, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                          USART_TX);
}

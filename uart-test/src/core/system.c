#include "core/system.h"
#include "config.h"

void clock_setup(void) {

  /* Enable GPIOD clock for LED & USARTs. */
  rcc_periph_clock_enable(CLK_LED_PORT);
  rcc_periph_clock_enable(CLK_USART_PORT);

  /* Enable clocks for USART2. */
  rcc_periph_clock_enable(CLK_USARTx);
}

void gpio_setup(void) {
  /* Setup GPIO pin GPIO12 on GPIO port D for LED. */
  gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);

  /* Setup GPIO pins for USART3 transmit. */
  gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);

  /* Setup GPIO pins for USART3 receive. */
  gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);

  /* Setup USART2 TX pin as alternate function. */
  gpio_set_af(GPIOD, GPIO_AF7, GPIO9);
  gpio_set_af(GPIOD, GPIO_AF7, GPIO8);
}

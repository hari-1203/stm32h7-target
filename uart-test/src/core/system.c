#include "core/system.h"
#include "config.h"
#include "libopencm3/stm32/h7/rcc.h"

#include <libopencm3/stm32/h7/rng.h>
#include <stdint.h>

#ifndef RNG_CR_CONDRST /* Conditioning soft-reset bit */
#define RNG_CR_CONDRST (1u << 18)
#endif
#ifndef RNG_SR_SEIS /* Seed error interrupt status */
#define RNG_SR_SEIS (1u << 6)
#endif
#ifndef RNG_SR_CEIS /* Clock error interrupt status */
#define RNG_SR_CEIS (1u << 5)
#endif

void clock_setup(void) {

  rcc_clock_setup_pll(&rcc_cfg_64mhz);
  // rcc_clock_setup_pll(&rcc_cfg_400mhz_hsi);

  /* Enable GPIOD clock for LED & USARTs. */
  rcc_periph_clock_enable(CLK_LED_PORT);
  rcc_periph_clock_enable(CLK_USART_PORT);

  /* Enable clocks for USART. */
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

  gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO8 | GPIO9);
  gpio_set_output_options(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                          GPIO9 | GPIO8);
}

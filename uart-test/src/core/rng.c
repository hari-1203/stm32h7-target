#include "core/rng.h"
#include "libopencm3/stm32/h7/rcc.h"
#include "libopencm3/stm32/h7/rng.h"

void rng_init(void) {

  rcc_periph_clock_enable(RCC_RNG);

  // Select PLL1Q as the clock for the RNG Peripheral
  uint32_t reg = RCC_D2CCIP2R;
  reg &= ~(RCC_D2CCIP2R_RNGSEL_MASK << RCC_D2CCIP2R_RNGSEL_SHIFT);
  reg |= (RCC_D2CCIP2R_RNGSEL_PLL1Q << RCC_D2CCIP2R_RNGSEL_SHIFT);
  RCC_D2CCIP2R = reg;

  rng_enable();
}

bool rng_read_word(uint32_t *data) {
  while ((RNG_SR & RNG_SR_DRDY) == 0) {
    if (RNG_SR & (RNG_SR_CEIS | RNG_SR_SEIS)) {
      return false;
    }
  }
  *data = RNG_DR;
  return true;
}

void rng_generate_data(uint8_t *out, unsigned long size) {
  while (size) {
    uint32_t word;
    while (!rng_read_word(&word))
      ;
    unsigned n = (size > 4) ? 4 : size;
    __builtin_memcpy(out, &word, n);
    out += n;
    size -= n;
  }
}

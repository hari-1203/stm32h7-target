#ifndef RNG_H
#define RNG_H

#include <libopencm3/stm32/h7/rcc.h>
#include <libopencm3/stm32/h7/rng.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rng.h>
#include <stdbool.h>
#include <stdint.h>

void rng_init(void);
bool rng_read_word(uint32_t *);
void rng_generate_data(uint8_t *, unsigned long);

#endif // !RNG_H

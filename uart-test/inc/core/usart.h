#ifndef USART_H
#define USART_H

/* Public API for USART/GPIO initialization */
#include <stdbool.h>
#include <stdint.h>

void usart_setup(void);
void usart_write(uint8_t *, const uint32_t);
void usart_write_byte(uint8_t data);
void usart_read(uint8_t *, const uint32_t);
uint8_t usart_read_byte(void);
bool usart_data_available(void);

#endif

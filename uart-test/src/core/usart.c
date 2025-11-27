#include "core/usart.h"
#include "config.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/h7/nvic.h"
#include <stdint.h>

void usart_setup(void) {
  /* Setup USART2 parameters. */
  usart_set_baudrate(USARTx, 115200);
  usart_set_databits(USARTx, 8);
  usart_set_stopbits(USARTx, USART_STOPBITS_1);
  usart_set_mode(USARTx, USART_MODE_TX_RX);
  usart_set_parity(USARTx, USART_PARITY_NONE);
  usart_set_flow_control(USARTx, USART_FLOWCONTROL_NONE);

  // usart_enable_rx_interrupt(USARTx);
  // nvic_enable_irq(USART_NVIC);

  /* Finally enable the USART. */
  usart_enable(USARTx);
}

void usart2_isr(void) {}

void usart_write(uint8_t *data, const uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    usart_write_byte(data[i]);
  }
}

void usart_write_byte(uint8_t data) {
  usart_send_blocking(USARTx, (uint16_t)data);
}

void usart_read(uint8_t *data, uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    data[i] = usart_read_byte();
  }
}

uint8_t usart_read_byte(void) { return (uint8_t)usart_recv_blocking(USARTx); }

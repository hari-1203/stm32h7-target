#include "core/usart.h"
#include "config.h"
#include "libopencm3/cm3/nvic.h"
#include "libopencm3/stm32/h7/nvic.h"
#include <stdbool.h>
#include <stdint.h>

#define RX_BUF_SIZE 1024

static volatile uint8_t rx_buf[RX_BUF_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;

void usart_setup(void) {
  usart_set_baudrate(USARTx, 115200);
  usart_set_databits(USARTx, 8);
  usart_set_stopbits(USARTx, USART_STOPBITS_1);
  usart_set_mode(USARTx, USART_MODE_TX_RX);
  usart_set_parity(USARTx, USART_PARITY_NONE);
  usart_set_flow_control(USARTx, USART_FLOWCONTROL_NONE);

  usart_enable_rx_interrupt(USARTx);
  nvic_enable_irq(USART_NVIC);

  usart_enable(USARTx);
}

void USART_IRQ_HANDLER(void) {
  uint8_t byte = (uint8_t)usart_recv(USARTx);
  uint8_t next_tail = rx_tail + 1;
  if (next_tail != rx_head) {
    rx_buf[rx_tail] = byte;
    rx_tail = next_tail;
  }
}

void usart_write(uint8_t *data, const uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    usart_write_byte(data[i]);
  }
}

void usart_write_byte(uint8_t data) {
  usart_send_blocking(USARTx, (uint16_t)data);
}

bool usart_data_available(void) { return rx_head != rx_tail; }

uint8_t usart_read_byte(void) {
  while (!usart_data_available())
    ;
  uint8_t byte = rx_buf[rx_head];
  rx_head++;
  return byte;
}

void usart_read(uint8_t *data, uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    data[i] = usart_read_byte();
  }
}

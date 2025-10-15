#include "core/usart.h"
#include "config.h"

void usart_setup(void) {
  /* Setup USART2 parameters. */
  usart_set_baudrate(USARTx, 115200);
  usart_set_databits(USARTx, 8);
  usart_set_stopbits(USARTx, USART_STOPBITS_1);
  usart_set_mode(USARTx, USART_MODE_TX_RX);
  usart_set_parity(USARTx, USART_PARITY_NONE);
  usart_set_flow_control(USARTx, USART_FLOWCONTROL_NONE);

  /* Finally enable the USART. */
  usart_enable(USARTx);
}

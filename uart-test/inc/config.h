#ifndef CONFIG_H
#define CONFIG_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#define CLK_LED_PORT RCC_GPIOB
#define CLK_USART_PORT RCC_GPIOD
#define CLK_USARTx RCC_USART3

#define USARTx USART3
#define USART_PORT GPIOD
#define USART_RX GPIO8
#define USART_TX GPIO9

#define LED_PORT GPIOB
#define LED_PIN GPIO0

#endif

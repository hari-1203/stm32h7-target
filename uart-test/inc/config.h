#ifndef CONFIG_H
#define CONFIG_H

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/h7/flash.h>
#include <libopencm3/stm32/h7/pwr.h>
#include <libopencm3/stm32/h7/rcc.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <stdbool.h>
#include <stdint.h>

// #include "user_settings.h"

/* ============================================================
 * Clock Configuration
 * ============================================================ */
static const struct rcc_pll_config rcc_cfg_64mhz = {
    .sysclock_source = RCC_PLL,             // we will switch to PLL1
    .pll_source = RCC_PLLCKSELR_PLLSRC_HSI, // 64 MHz HSI
    .hse_frequency = 0,                     // not used

    /* Make PLL1P = 64 MHz:
       CLKIN = 64 / M = 64/4 = 16 MHz
       VCO   = 16 * N = 16 * 32 = 512 MHz
       SYSCLK(PLL1P) = VCO / P = 512 / 8 = 64 MHz
    */
    .pll1 =
        {
            .divm = 4,
            .divn = 32,
            .divp = 8, // <- SYSCLK source (PLL1P)
            .divq = 8, // optional
            .divr = 8, // optional
        },

    // All domains at ÷1
    .core_pre = RCC_D1CFGR_D1CPRE_BYP,
    .hpre = RCC_D1CFGR_D1HPRE_BYP,
    .ppre1 = RCC_D2CFGR_D2PPRE_BYP,
    .ppre2 = RCC_D2CFGR_D2PPRE_BYP,
    .ppre3 = RCC_D1CFGR_D1PPRE_BYP,
    .ppre4 = RCC_D3CFGR_D3PPRE_BYP,

    .voltage_scale = PWR_VOS_SCALE_1,
    .power_mode = PWR_SYS_LDO,
    .flash_waitstates = FLASH_ACR_LATENCY_0WS, // ok for 64 MHz @ VOS1
};

static const struct rcc_pll_config rcc_cfg_400mhz_hsi = {
    .sysclock_source = RCC_PLL,             // switch SYSCLK to PLL1
    .pll_source = RCC_PLLCKSELR_PLLSRC_HSI, // 64 MHz HSI
    .hse_frequency = 0,

    .pll1 =
        {
            .divm = 4,  // 64/4 = 16 MHz VCO_in (valid)
            .divn = 50, // 16*50 = 800 MHz VCO   (valid)
            .divp = 2,  // 800/2 = 400 MHz (SYSCLK)
            .divq = 4,
            .divr = 8,
        },

    // Conservative bus clocks (safe for all H7 busses)
    .core_pre = RCC_D1CFGR_D1CPRE_BYP, // CPU   = 400 MHz
    .hpre = RCC_D1CFGR_D1HPRE_DIV2,    // AHB   = 200 MHz
    .ppre1 = RCC_D2CFGR_D2PPRE_DIV2,   // APB1  = 100 MHz
    .ppre2 = RCC_D2CFGR_D2PPRE_DIV2,   // APB2  = 100 MHz
    .ppre3 = RCC_D1CFGR_D1PPRE_DIV2,   // APB3  = 100 MHz
    .ppre4 = RCC_D3CFGR_D3PPRE_DIV2,   // APB4  = 100 MHz

    .voltage_scale = PWR_VOS_SCALE_1, // VOS1 is required for 400 MHz
    .power_mode = PWR_SYS_LDO,        // or the SMPS mode you use
    .flash_waitstates = FLASH_ACR_LATENCY_2WS,
};

#define CLK_TRIGGER_PORT RCC_GPIOB
#define CLK_USART_RX_PORT RCC_GPIOD
#define CLK_USART_TX_PORT RCC_GPIOD
#define CLK_USARTx RCC_USART3

/* ============================================================
 * USART Configuration
 * ============================================================ */

#define USARTx USART3
#define USART_PORT_RX GPIOD
#define USART_PORT_TX GPIOD
#define USART_RX GPIO8
#define USART_TX GPIO9
#define USART_NVIC NVIC_USART3_IRQ

/* ============================================================
 * GPIO Configuration
 * ============================================================ */

#define TRIGGER_PORT GPIOB
#define TRIGGER_PIN GPIO0

#endif

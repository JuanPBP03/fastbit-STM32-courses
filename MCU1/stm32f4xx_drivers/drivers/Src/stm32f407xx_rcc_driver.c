/*
 * stm32f407xx_rcc_driver.c
 *
 *  Created on: Jun 16, 2025
 *      Author: JuanP
 */


#include "stm32f407xx.h"

static const uint16_t AHB_Prescaler[16] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 4, 8, 16, 64, 128, 256, 512
};

static const uint8_t APB_Prescaler[8] = {
    1, 1, 1, 1, 2, 4, 8, 16
};

uint32_t RCC_GetPLLCLKFreq(void);

uint32_t RCC_GetPCLK1Freq(void)
{
    uint32_t sysclk;

    uint32_t clk_src = (RCC->CFGR >> 2) & 0x3;

    switch (clk_src)
    {
        case 0x00:
            sysclk = 16000000; // HSI
            break;
        case 0x01:
            sysclk = 8000000;
            break;
        case 0x02:
            // PLL for now just hardcoded
            sysclk = 84000000; // Default PLL output on F407
            break;
        default:
            sysclk = 16000000; // fallback
            break;
    }

    uint32_t ahb_code  = (RCC->CFGR >> 4) & 0xF;
    uint32_t apb1_code = (RCC->CFGR >> 10) & 0x7;

    uint32_t AHBPre  = AHB_Prescaler[ahb_code];
    uint32_t APB1Pre = APB_Prescaler[apb1_code];

    return sysclk / (AHBPre * APB1Pre);
}

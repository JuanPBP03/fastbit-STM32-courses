/*
 * stm32407xx.h
 *
 *  Created on: Jun 2, 2025
 *      Author: JuanP
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#define __vo volatile
/*
 * Flash and SRAM base addresses
 */

#define FLASH_BASEADDR							0X08000000UL		// Flash memory base address
#define SRAM1_BASEADDR							0X20000000UL		// SRAM1 base address
#define SRAM2_BASEADDR							0x2001C000UL		// SRAM2 base address
#define ROM_BASEADDR							0x1FFF0000UL		// System memory base address
#define SRAM 									SRAM1_BASEADDR		// SRAM base address


/*
 * AHBx and APBx Peripheral base addresses
 */

#define PERIPHERALS_BASEADDR					0x40000000UL		// Peripherals base address
#define APB1_BASEADDR							PERIPHERALS_BASE	// APB1 base address
#define APB2_BASEADDR							0x40010000UL		// APB2 base address
#define AHB1_BASEADDR							0x40020000UL		// AHB1 base address
#define AHB2_BASEADDR							0x50000000UL		// AHB2 base address


/*
 * AHB1 Peripheral base addresses
 * TODO: Complete for all other AHB1 Peripherals
 */

#define GPIOA_BASEADDR							(AHB1_BASEADDR + 0x0000)
#define GPIOB_BASEADDR							(AHB1_BASEADDR + 0x0400)
#define GPIOC_BASEADDR							(AHB1_BASEADDR + 0x0800)
#define GPIOD_BASEADDR							(AHB1_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR							(AHB1_BASEADDR + 0x1000)
#define GPIOF_BASEADDR							(AHB1_BASEADDR + 0x1400)
#define GPIOG_BASEADDR							(AHB1_BASEADDR + 0x1800)
#define GPIOH_BASEADDR							(AHB1_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR							(AHB1_BASEADDR + 0x2000)
#define RCC_BASEADDR							(AHB1_BASEADDR + 0x3800)

/*
 * APB1 Peripheral base addresses
 * TODO: Complete for all other APB1 Peripherals
 */

#define I2C1_BASEADDR							(APB1_BASEADDR + 0X5400)
#define I2C2_BASEADDR							(APB1_BASEADDR + 0X5800)
#define I2C3_BASEADDR							(APB1_BASEADDR + 0X5C00)

#define SPI2_BASEADDR							(APB1_BASEADDR + 0X3800)
#define SPI3_BASEADDR							(APB1_BASEADDR + 0X3C00)

#define USART2_BASEADDR							(APB1_BASEADDR + 0X4400)
#define USART3_BASEADDR							(APB1_BASEADDR + 0X4800)
#define UART4_BASEADDR							(APB1_BASEADDR + 0X4C00)
#define UART5_BASEADDR							(APB1_BASEADDR + 0X5000)


/*
 * APB2 Peripheral base addresses
 * TODO: Complete for all other APB2 Peripherals
 */

#define SPI1_BASEADDR 							(APB2_BASEADDR + 0X3000)

#define USART1_BASEADDR 						(APB2_BASEADDR + 0X1000)
#define USART6_BASEADDR 						(APB2_BASEADDR + 0X1400)

#define EXTI_BASEADDR 							(APB2_BASEADDR + 0X3C00)
#define SYSCFG_BASEADDR 						(APB2_BASEADDR + 0X3800)

/***************************************************** Peripheral Register Definition Structures ************************************************/
/*
 *
 */

typedef struct {
	__vo uint32_t MODER;
	__vo uint32_t OTYPER;
	__vo uint32_t OSPEEDR;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSSR;
	__vo uint32_t LCKR;
	__vo uint32_t AFR[2];
}GPIO_RegDef_t;

typedef struct {
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;
	__vo uint32_t RESERVED0;
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
	__vo uint32_t RESERVED1[2];
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;
	__vo uint32_t RESERVED2;
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
	__vo uint32_t RESERVED3[2];
	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;
	__vo uint32_t RESERVED4;
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
	__vo uint32_t RESERVED5[2];
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	__vo uint32_t RESERVED6[2];
	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;

}RCC_RegDef_t;


/*
 * Peripheral definitions (base addresses type-casted to xxxRegDef_t)
 */
#define GPIOA		 	(GPIO_RegDef_t*)GPIOA_BASEADDR
#define GPIOB		 	(GPIO_RegDef_t*)GPIOB_BASEADDR
#define GPIOC		 	(GPIO_RegDef_t*)GPIOC_BASEADDR
#define GPIOD		 	(GPIO_RegDef_t*)GPIOD_BASEADDR
#define GPIOE		 	(GPIO_RegDef_t*)GPIOE_BASEADDR
#define GPIOF			(GPIO_RegDef_t*)GPIOF_BASEADDR
#define GPIOG			(GPIO_RegDef_t*)GPIOG_BASEADDR
#define GPIOH			(GPIO_RegDef_t*)GPIOH_BASEADDR
#define GPIOI			(GPIO_RegDef_t*)GPIOI_BASEADDR


#define RCC				(RCC_RegDef_t*)RCC_BASEADDR


/*
 * Clock enable macros for GPIO peripherals
 */

#define GPIOA_PCLK_EN()			(RCC->AHB1ENR |= (1<<0))
#define GPIOB_PCLK_EN()			(RCC->AHB1ENR |= (1<<1))
#define GPIOC_PCLK_EN()			(RCC->AHB1ENR |= (1<<2))
#define GPIOD_PCLK_EN()			(RCC->AHB1ENR |= (1<<3))
#define GPIOE_PCLK_EN()			(RCC->AHB1ENR |= (1<<4))
#define GPIOF_PCLK_EN()			(RCC->AHB1ENR |= (1<<5))
#define GPIOG_PCLK_EN()			(RCC->AHB1ENR |= (1<<6))
#define GPIOH_PCLK_EN()			(RCC->AHB1ENR |= (1<<7))
#define GPIOI_PCLK_EN()			(RCC->AHB1ENR |= (1<<8))

/*
 * Clock enable macros for I2C peripherals
 */

#define I2C1_PCLK_EN()			(RCC->APB1ENR |= (1<<21))
#define I2C2_PCLK_EN()			(RCC->APB1ENR |= (1<<22))
#define I2C3_PCLK_EN()			(RCC->APB1ENR |= (1<<23))

/*
 * Clock enable macros for SPI peripherals
 */

#define SPI1_PCLK_EN()			(RCC->APB2ENR |= (1<<12))
#define SPI2_PCLK_EN()			(RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN()			(RCC->APB1ENR |= (1<<15))


/*
 * Clock enable macros for U(S)ART peripherals
 */

#define USART1_PCLK_EN()			(RCC->APB2ENR |= (1<<4))
#define USART1_PCLK_EN()			(RCC->APB2ENR |= (1<<17))
#define USART1_PCLK_EN()			(RCC->APB2ENR |= (1<<18))
#define UART4_PCLK_EN()				(RCC->APB2ENR |= (1<<19))
#define UART5_PCLK_EN()				(RCC->APB2ENR |= (1<<20))
#define USART6_PCLK_EN()			(RCC->APB2ENR |= (1<<5))


/*
 * Clock enable macros for SYSCFG peripherals
 */

#define SYSCFG_PCLK_EN()			(RCC->APB2ENR |= (1<<14))







/*
 * Clock disable macros for GPIO peripherals
 */

#define GPIOA_PCLK_DI()			(RCC->AHB1DIR &= ~(1<<0))
#define GPIOB_PCLK_DI()			(RCC->AHB1DIR &= ~(1<<1))
#define GPIOC_PCLK_DI()			(RCC->AHB1DIR &= ~(1<<2))
#define GPIOD_PCLK_DI()			(RCC->AHB1DIR &= ~(1<<3))
#define GPIOE_PCLK_DI()			(RCC->AHB1DIR &= ~(1<<4))
#define GPIOF_PCLK_DI()			(RCC->AHB1DIR &= ~(1<<5))
#define GPIOG_PCLK_DI()			(RCC->AHB1DIR &= ~(1<<6))
#define GPIOH_PCLK_DI()			(RCC->AHB1DIR &= ~(1<<7))
#define GPIOI_PCLK_DI()			(RCC->AHB1DIR &= ~(1<<8))

/*
 * Clock disable macros for I2C peripherals
 */

#define I2C1_PCLK_DI()			(RCC->APB1DIR &= ~(1<<21))
#define I2C2_PCLK_DI()			(RCC->APB1DIR &= ~(1<<22))
#define I2C3_PCLK_DI()			(RCC->APB1DIR &= ~(1<<23))

/*
 * Clock disable macros for SPI peripherals
 */

#define SPI1_PCLK_DI()			(RCC->APB2DIR &= ~(1<<12))
#define SPI2_PCLK_DI()			(RCC->APB1DIR &= ~(1<<14))
#define SPI3_PCLK_DI()			(RCC->APB1DIR &= ~(1<<15))


/*
 * Clock disable macros for U(S)ART peripherals
 */

#define USART1_PCLK_DI()			(RCC->APB2DIR &= ~(1<<4))
#define USART1_PCLK_DI()			(RCC->APB2DIR &= ~(1<<17))
#define USART1_PCLK_DI()			(RCC->APB2DIR &= ~(1<<18))
#define UART4_PCLK_DI()				(RCC->APB2DIR &= ~(1<<19))
#define UART5_PCLK_DI()				(RCC->APB2DIR &= ~(1<<20))
#define USART6_PCLK_DI()			(RCC->APB2DIR &= ~(1<<5))


/*
 * Clock disable macro for SYSCFG peripherals
 */

#define SYSCFG_PCLK_DI()			(RCC->APB2DIR &= ~(1<<14))











#endif /* INC_STM32F407XX_H_ */

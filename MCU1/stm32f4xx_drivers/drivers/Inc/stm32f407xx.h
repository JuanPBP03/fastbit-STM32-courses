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


/***********************************Processor Specific Details*****************************
 *
 * ARM Cortex Mx Processor NVIC ISERx register addresses
 */

#define NVIC_BASEADDR						0xE000E100UL

typedef struct{
	__vo uint32_t ISER[8U];
	uint32_t RESERVED[24U];
	__vo uint32_t ICER[8U];
	uint32_t RESERVED1[24U];
	__vo uint32_t ISPR[8U];
	uint32_t RESERVED2[24U];
	__vo uint32_t ICPR[8U];
	uint32_t RESERVED3[24U];
	__vo uint32_t IABR[8U];
	uint32_t RESERVED4[56U];
	__vo uint32_t IPR[60U];
}NVIC_RegDef_t;


#define NVIC_STIR		(*((__vo uint32_t*)0xE000EF00UL))
#define NVIC			((NVIC_RegDef_t*)NVIC_BASEADDR)

#define __NVIC_PRIO_BITS	4U

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
#define APB1_BASEADDR							PERIPHERALS_BASEADDR	// APB1 base address
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
#define GPIO_ADDR_STRIDE						(0x400U)

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

	__vo uint32_t IMR;
	__vo uint32_t EMR;
	__vo uint32_t RTSR;
	__vo uint32_t FTSR;
	__vo uint32_t SWIER;
	__vo uint32_t PR;

}EXTI_RegDef_t;

typedef struct {
	__vo uint32_t MEMRMP;
	__vo uint32_t PMC;
	__vo uint32_t EXTICR[4];
	uint32_t RESERVED[2];
	__vo uint32_t CMPCR;
}SYSCFG_RegDef_t;

typedef struct {
	__vo uint32_t MODER;
	__vo uint32_t OTYPER;
	__vo uint32_t OSPEEDR;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t LCKR;
	__vo uint32_t AFRL;
	__vo uint32_t AFRH;
}GPIO_RegDef_t;

typedef struct {
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;
	uint32_t RESERVED0;
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
	uint32_t RESERVED1[2];
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;
	uint32_t RESERVED2;
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
	uint32_t RESERVED3[2];
	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;
	uint32_t RESERVED4;
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
	uint32_t RESERVED5[2];
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	uint32_t RESERVED6[2];
	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;

}RCC_RegDef_t;

typedef struct {
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;
	__vo uint32_t RXCRCR;
	__vo uint32_t TXCRCR;
	__vo uint32_t CFGR;
	__vo uint32_t PR;
}SPI_I2S_RegDef_t;


/*
 * Peripheral definitions (base addresses type-casted to xxxRegDef_t)
 */
#define GPIOA		 	((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB		 	((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC		 	((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD		 	((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE		 	((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF			((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG			((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH			((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI			((GPIO_RegDef_t*)GPIOI_BASEADDR)

#define RCC				((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI			((EXTI_RegDef_t*)EXTI_BASEADDR)

#define SYSCFG			((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define SPI1			((SPI_I2S_RegDef_t*)SPI1_BASEADDR)
#define SPI2			((SPI_I2S_RegDef_t*)SPI2_BASEADDR)
#define SPI3			((SPI_I2S_RegDef_t*)SPI3_BASEADDR)


/*
 * Clock enable macros for SYSCFG peripherals
 */

#define SYSCFG_PCLK_EN()			(RCC->APB2ENR |= (1<<14))

/*
 * Clock disable macro for SYSCFG peripherals
 */

#define SYSCFG_PCLK_DI()			(RCC->APB2DIR &= ~(1<<14))


/*
 * 	IRQ numbers of STM32F407xx MCU
 * 	TODO: fill the rest of the IRQn
 */
typedef enum{

	IRQn_EXTI0 = 6,
	IRQn_EXTI1,
	IRQn_EXTI2,
	IRQn_EXTI3,
	IRQn_EXTI4,
	IRQn_EXTI9_5 = 23,
	IRQn_SPI1 = 35,
	IRQn_SPI2,
	IRQn_EXTI15_10 = 40,
	IRQn_SPI3 = 51

}IRQn_t;



/*
 * Utility Macros
 */

#define ENABLE 	1
#define DISABLE 0

#define SET 	1
#define RESET 	0


/**********************
 * 	SPI BIT POSITIONS
 **********************/

#define SPI_CR1_CPHA_Pos         0U  // Clock Phase
#define SPI_CR1_CPOL_Pos         1U  // Clock Polarity
#define SPI_CR1_MSTR_Pos         2U  // Master Selection
#define SPI_CR1_BR_Pos           3U  // Baud Rate Control (3 bits: [5:3])
#define SPI_CR1_SPE_Pos          6U  // SPI Enable
#define SPI_CR1_LSBFIRST_Pos     7U  // Frame Format (0 = MSB first)
#define SPI_CR1_SSI_Pos          8U  // Internal Slave Select
#define SPI_CR1_SSM_Pos          9U  // Software Slave Management
#define SPI_CR1_RXONLY_Pos       10U // Receive Only
#define SPI_CR1_DFF_Pos          11U // Data Frame Format (0 = 8-bit, 1 = 16-bit)
#define SPI_CR1_CRCNEXT_Pos      12U // Transmit CRC Next
#define SPI_CR1_CRCEN_Pos        13U // Hardware CRC Enable
#define SPI_CR1_BIDIOE_Pos       14U // Output Enable in Bidirectional Mode
#define SPI_CR1_BIDIMODE_Pos     15U // Bidirectional Data Mode Enable

#define SPI_CR2_RXDMAEN_Pos      0U  // Rx Buffer DMA Enable
#define SPI_CR2_TXDMAEN_Pos      1U  // Tx Buffer DMA Enable
#define SPI_CR2_SSOE_Pos         2U  // SS Output Enable
#define SPI_CR2_ERRIE_Pos        5U  // Error Interrupt Enable
#define SPI_CR2_RXNEIE_Pos       6U  // RX buffer Not Empty Interrupt Enable
#define SPI_CR2_TXEIE_Pos        7U  // Tx buffer Empty Interrupt Enable
#define SPI_CR2_FRF_Pos          4U  // Frame Format (0 = Motorola, 1 = TI)
#define SPI_CR2_DS_Pos           8U  // Data Size (for SPI with variable frame sizes)





















#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"


#endif /* INC_STM32F407XX_H_ */

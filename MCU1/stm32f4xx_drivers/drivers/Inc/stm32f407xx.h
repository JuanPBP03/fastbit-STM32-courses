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

#define __NVIC_PRIO_BITS	(4U)

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

typedef struct {

	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t OAR1;
	__vo uint32_t OAR2;
	__vo uint32_t DR;
	__vo uint32_t SR1;
	__vo uint32_t SR2;
	__vo uint32_t CCR;
	__vo uint32_t TRISE;
	__vo uint32_t FLTR;

}I2C_RegDef_t;

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

#define I2C1			((I2C_RegDef_t *)I2C1_BASEADDR)
#define I2C2			((I2C_RegDef_t *)I2C2_BASEADDR)
#define I2C3			((I2C_RegDef_t *)I2C3_BASEADDR)



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

	IRQn_I2C1_EV = 31,
	IRQn_I2C1_ER,
	IRQn_I2C2_EV,
	IRQn_I2C2_ER,

	IRQn_SPI1 = 35,
	IRQn_SPI2,
	IRQn_EXTI15_10 = 40,
	IRQn_SPI3 = 51,

	IRQn_I2C3_EV = 72,
	IRQn_I2C3_ER

}IRQn_t;


/******************************
 * 		I2C BIT DEFINITIONS
 ******************************/

#define I2C_CR1_PE_Pos			(0U)
#define I2C_CR1_PE_Msk			(0x1UL<<I2C_CR1_PE_Pos)
#define I2C_CR1_PE				I2C_CR1_PE_Msk
#define I2C_CR1_SMBUS_Pos		(1U)
#define I2C_CR1_SMBUS_Msk		(0x1UL<<I2C_CR1_SMBUS_Pos)
#define I2C_CR1_SMBUS			I2C_CR1_SMBUS_Msk
#define I2C_CR1_SMBTYPE_Pos		(3U)
#define I2C_CR1_SMBTYPE_Msk		(0x1UL<<I2C_CR1_SMBTYPE_Pos)
#define I2C_CR1_SMBTYPE			I2C_CR1_SMBTYPE_Msk
#define I2C_CR1_ENARP_Pos		(4U)
#define I2C_CR1_ENARP_Msk		(0x1UL<<I2C_CR1_ENARP_Pos)
#define I2C_CR1_ENARP			I2C_CR1_ENARP_Msk
#define I2C_CR1_ENGC_Pos		(5U)
#define I2C_CR1_ENGC_Msk		(0x1UL<<I2C_CR1_ENGC_Pos)
#define I2C_CR1_ENGC			I2C_CR1_ENGC_Msk
#define I2C_CR1_ENPEC_Pos		(6U)
#define I2C_CR1_ENPEC_Msk		(0x1UL<<I2C_CR1_ENPEC_Pos)
#define I2C_CR1_ENPEC			I2C_CR1_ENPEC_Msk
#define I2C_CR1_NOSTRETCH_Pos	(7U)
#define I2C_CR1_NOSTRETCH_Msk	(0x1UL<<I2C_CR1_NOSTRETCH_Pos)
#define I2C_CR1_NOSTRETCH		I2C_CR1_NOSTRETCH_Msk
#define I2C_CR1_START_Pos		(8U)
#define I2C_CR1_START_Msk		(0x1UL<<I2C_CR1_START_Pos)
#define I2C_CR1_START			I2C_CR1_START_Msk
#define I2C_CR1_STOP_Pos		(9U)
#define I2C_CR1_STOP_Msk		(0x1UL<<I2C_CR1_STOP_Pos)
#define I2C_CR1_STOP			I2C_CR1_STOP_Msk
#define I2C_CR1_ACK_Pos			(10U)
#define I2C_CR1_ACK_Msk			(0x1UL<<I2C_CR1_ACK_Pos)
#define I2C_CR1_ACK				I2C_CR1_ACK_Msk
#define I2C_CR1_POS_Pos			(11U)
#define I2C_CR1_POS_Msk			(0x1UL<<I2C_CR1_POS_Pos)
#define I2C_CR1_POS				I2C_CR1_POS_Msk
#define I2C_CR1_PEC_Pos			(12U)
#define I2C_CR1_PEC_Msk			(0x1UL<<I2C_CR1_PEC_Pos)
#define I2C_CR1_PEC				I2C_CR1_PEC_Msk
#define I2C_CR1_ALERT_Pos		(13U)
#define I2C_CR1_ALERT_Msk		(0x1UL<<I2C_CR1_ALERT_Pos)
#define I2C_CR1_ALERT			I2C_CR1_ALERT_Msk
#define I2C_CR1_SWRST_Pos		(15U)
#define I2C_CR1_SWRST_Msk		(0x1UL<<I2C_CR1_SWRST_Pos)
#define I2C_CR1_SWRST			I2C_CR1_SWRST_Msk

#define I2C_CR2_FREQ_Pos		(0U)
#define I2C_CR2_FREQ_Msk		(0x3FUL<<I2C_CR2_FREQ_Pos)
#define I2C_CR2_FREQ			I2C_CR2_FREQ_Msk
#define I2C_CR2_ITERREN_Pos		(8U)
#define I2C_CR2_ITERREN_Msk		(0x1UL<<I2C_CR2_ITERREN_Pos)
#define I2C_CR2_ITERREN			I2C_CR2_ITERREN_Msk
#define I2C_CR2_ITEVTEN_Pos		(9U)
#define I2C_CR2_ITEVTEN_Msk		(0x1UL<<I2C_CR2_ITEVTEN_Pos)
#define I2C_CR2_ITEVTEN			I2C_CR2_ITEVTEN_Msk
#define I2C_CR2_ITBUFFN_Pos		(10U)
#define I2C_CR2_ITBUFFN_Msk		(0x1UL<<I2C_CR2_ITBUFFN_Pos)
#define I2C_CR2_ITBUFFN			I2C_CR2_ITBUFFN_Msk
#define I2C_CR2_DMAEN_Pos		(11U)
#define I2C_CR2_DMAEN_Msk		(0x1UL<<I2C_CR2_DMAEN_Pos)
#define I2C_CR2_DMAEN			I2C_CR2_DMAEN_Msk
#define I2C_CR2_LAST_Pos		(12U)
#define I2C_CR2_LAST_Msk		(0x1UL<<I2C_CR2_LAST_Pos)
#define I2C_CR2_LAST			I2C_CR2_LAST_Msk

#define I2C_SR1_SB_Pos			(0U)
#define I2C_SR1_SB_Msk			(0x1UL<<I2C_SR1_SB_Pos)
#define I2C_SR1_SB				I2C_SR1_SB_Msk
#define I2C_SR1_ADDR_Pos		(1U)
#define I2C_SR1_ADDR_Msk		(0x1UL<<I2C_SR1_ADDR_Pos)
#define I2C_SR1_ADDR			I2C_SR1_ADDR_Msk
#define I2C_SR1_BTF_Pos			(2U)
#define I2C_SR1_BTF_Msk			(0x1UL<<I2C_SR1_BTF_Pos)
#define I2C_SR1_BTF				I2C_SR1_BTF_Msk
#define I2C_SR1_ADD10_Pos		(3U)
#define I2C_SR1_ADD10_Msk		(0x1UL<<I2C_SR1_ADD10_Pos)
#define I2C_SR1_ADD10			I2C_SR1_ADD10_Msk
#define I2C_SR1_STOPF_Pos		(4U)
#define I2C_SR1_STOPF_Msk		(0x1UL<<I2C_SR1_STOPF_Pos)
#define I2C_SR1_STOPF			I2C_SR1_STOPF_Msk
#define I2C_SR1_RXNE_Pos		(6U)
#define I2C_SR1_RXNE_Msk		(0x1UL<<I2C_SR1_RXNE_Pos)
#define I2C_SR1_RXNE			I2C_SR1_RXNE_Msk
#define I2C_SR1_TXE_Pos			(7U)
#define I2C_SR1_TXE_Msk			(0x1UL<<I2C_SR1_TXE_Pos)
#define I2C_SR1_TXE				I2C_SR1_TXE_Msk
#define I2C_SR1_BERR_Pos		(8U)
#define I2C_SR1_BERR_Msk		(0x1UL<<I2C_SR1_BERR_Pos)
#define I2C_SR1_BERR			I2C_SR1_BERR_Msk
#define I2C_SR1_ARLO_Pos		(9U)
#define I2C_SR1_ARLO_Msk		(0x1UL<<I2C_SR1_ARLO_Pos)
#define I2C_SR1_ARLO			I2C_SR1_ARLO_Msk
#define I2C_SR1_AF_Pos			(10U)
#define I2C_SR1_AF_Msk			(0x1UL<<I2C_SR1_AF_Pos)
#define I2C_SR1_AF				I2C_SR1_AF_Msk
#define I2C_SR1_OVR_Pos			(11U)
#define I2C_SR1_OVR_Msk			(0x1UL<<I2C_SR1_OVR_Pos)
#define I2C_SR1_OVR				I2C_SR1_OVR_Msk
#define I2C_SR1_PECERR_Pos		(12U)
#define I2C_SR1_PECERR_Msk		(0x1UL<<I2C_SR1_PECERR_Pos)
#define I2C_SR1_PECERR			I2C_SR1_PECERR_Msk
#define I2C_SR1_TIMEOUT_Pos		(14U)
#define I2C_SR1_TIMEOUT_Msk		(0x1UL<<I2C_SR1_TIMEOUT_Pos)
#define I2C_SR1_TIMEOUT			I2C_SR1_TIMEOUT_Msk
#define I2C_SR1_SMBALERT_Pos	(15U)
#define I2C_SR1_SMBALERT_Msk	(0x1UL<<I2C_SR1_SMBALERT_Pos)
#define I2C_SR1_SMBALERT		I2C_SR1_SMBALERT_Msk

#define I2C_SR2_MSL_Pos			(0U)
#define I2C_SR2_MSL_Msk			(0x1UL << I2C_SR2_MSL_Pos)
#define I2C_SR2_MSL				I2C_SR2_MSL_Msk
#define I2C_SR2_BUSY_Pos		(1U)
#define I2C_SR2_BUSY_Msk		(0x1UL << I2C_SR2_BUSY_Pos)
#define I2C_SR2_BUSY			I2C_SR2_BUSY_Msk
#define I2C_SR2_TRA_Pos			(2U)
#define I2C_SR2_TRA_Msk			(0x1UL << I2C_SR2_TRA_Pos)
#define I2C_SR2_TRA				I2C_SR2_TRA_Msk
#define I2C_SR2_GENCALL_Pos		(4U)
#define I2C_SR2_GENCALL_Msk		(0x1UL << I2C_SR2_GENCALL_Pos)
#define I2C_SR2_GENCALL			I2C_SR2_GENCALL_Msk
#define I2C_SR2_SMBDEFAULT_Pos	(5U)
#define I2C_SR2_SMBDEFAULT_Msk	(0x1UL << I2C_SR2_SMBDEFAULT_Pos)
#define I2C_SR2_SMBDEFAULT		I2C_SR2_SMBDEFAULT_Msk
#define I2C_SR2_SMBHOST_Pos		(6U)
#define I2C_SR2_SMBHOST_Msk		(0x1UL << I2C_SR2_SMBHOST_Pos)
#define I2C_SR2_SMBHOST			I2C_SR2_SMBHOST_Msk
#define I2C_SR2_DUALF_Pos		(7U)
#define I2C_SR2_DUALF_Msk		(0x1UL << I2C_SR2_DUALF_Pos)
#define I2C_SR2_DUALF			I2C_SR2_DUALF_Msk
#define I2C_SR2_PEC_Pos			(8U)
#define I2C_SR2_PEC_Msk			(0xFFUL << I2C_SR2_PEC_Pos)
#define I2C_SR2_PEC				I2C_SR2_PEC_Msk


#define I2C_CCR_CCR_Pos			(0U)
#define I2C_CCR_CCR_Msk			(0xFFFUL<<I2C_CCR_CCR_Pos)
#define I2C_CCR_CCR				I2C_CCR_CCR_Msk
#define I2C_CCR_DUTY_Pos		(14U)
#define I2C_CCR_DUTY_Msk		(0x1UL<<I2C_CCR_DUTY_Pos)
#define I2C_CCR_DUTY			I2C_CCR_DUTY_Msk
#define I2C_CCR_FS_Pos			(15U)
#define I2C_CCR_FS_Msk			(0x1UL<<I2C_CCR_FS_Pos)
#define I2C_CCR_FS				I2C_CCR_FS_Msk



/**********************
 * 		SPI BIT DEFINITIONS
 **********************/

#define SPI_CR1_CPHA_Pos        (0U)  // Clock Phase
#define SPI_CR1_CPHA_Msk		(0x1UL<<SPI_CR1_CPHA_Pos)
#define SPI_CR1_CPHA			SPI_CR1_CPHA_Msk
#define SPI_CR1_CPOL_Pos        (1U)  // Clock Polarity
#define SPI_CR1_CPOL_Msk		(0x1UL<<SPI_CR1_CPOL_Pos)
#define SPI_CR1_CPOL			SPI_CR1_CPOL_Msk
#define SPI_CR1_MSTR_Pos        (2U)  // Master Selection
#define SPI_CR1_MSTR_Msk		(0x1UL<<SPI_CR1_MSTR_Pos)
#define SPI_CR1_MSTR			SPI_CR1_MSTR_Msk
#define SPI_CR1_BR_Pos          (3U)  // Baud Rate Control(3 bits: [5:3])
#define SPI_CR1_BR_Msk			(0x7UL<<SPI_CR1_BR_Pos)
#define SPI_CR1_BR				SPI_CR1_BR_Msk
#define SPI_CR1_SPE_Pos         (6U)  // SPI Enable
#define SPI_CR1_SPE_Msk			(0x1UL<<SPI_CR1_SPE_Pos)
#define SPI_CR1_SPE				SPI_CR1_SPE_Msk
#define SPI_CR1_LSBFIRST_Pos    (7U)  // Frame Format(0 = MSB first)
#define SPI_CR1_LSBFIRST_Msk	(0x1UL<<SPI_CR1_LSBFIRST_Pos)
#define SPI_CR1_LSBFIRST		SPI_CR1_LSBFIRST_Msk
#define SPI_CR1_SSI_Pos         (8U)  // Internal Slave Select
#define SPI_CR1_SSI_Msk			(0x1UL<<SPI_CR1_SSI_Pos)
#define SPI_CR1_SSI				SPI_CR1_SSI_Msk
#define SPI_CR1_SSM_Pos         (9U)  // Software Slave Management
#define SPI_CR1_SSM_Msk			(0x1UL<<SPI_CR1_SSM_Pos)
#define SPI_CR1_SSM				SPI_CR1_SSM_Msk
#define SPI_CR1_RXONLY_Pos      (10U) // Receive Only
#define SPI_CR1_RXONLY_Msk		(0x1UL<<SPI_CR1_RXONLY_Pos)
#define SPI_CR1_RXONLY			SPI_CR1_RXONLY_Msk
#define SPI_CR1_DFF_Pos         (11U) // Data Frame Format(0 = 8-bit, 1 = 16-bit)
#define SPI_CR1_DFF_Msk			(0x1UL<<SPI_CR1_DFF_Pos)
#define SPI_CR1_DFF				SPI_CR1_DFF_Msk
#define SPI_CR1_CRCNEXT_Pos     (12U) // Transmit CRC Next
#define SPI_CR1_CRCNEXT_Msk		(0x1UL<<SPI_CR1_CRCNEXT_Pos)
#define SPI_CR1_CRCNEXT			SPI_CR1_CRCNEXT_Msk
#define SPI_CR1_CRCEN_Pos       (13U) // Hardware CRC Enable
#define SPI_CR1_CRCEN_Msk		(0x1UL<<SPI_CR1_CRCEN_Pos)
#define SPI_CR1_CRCEN			SPI_CR1_CRCEN_Msk
#define SPI_CR1_BIDIOE_Pos      (14U) // Output Enable in Bidirectional Mode
#define SPI_CR1_BIDIOE_Msk		(0x1UL<<SPI_CR1_BIDIOE_Pos)
#define SPI_CR1_BIDIOE			SPI_CR1_BIDIOE_Msk
#define SPI_CR1_BIDIMODE_Pos    (15U) // Bidirectional Data Mode Enable
#define SPI_CR1_BIDIMODE_Msk	(0x1UL<<SPI_CR1_BIDIMODE_Pos)
#define SPI_CR1_BIDIMODE		SPI_CR1_BIDIMODE_Msk

#define SPI_CR2_RXDMAEN_Pos     (0U)  // Rx Buffer DMA Enable
#define SPI_CR2_RXDMAEN_Msk		(0x1UL<<SPI_CR2_RXDMAEN_Pos)
#define SPI_CR2_RXDMAEN			SPI_CR2_RXDMAEN_Msk
#define SPI_CR2_TXDMAEN_Pos     (1U)  // Tx Buffer DMA Enable
#define SPI_CR2_TXDMAEN_Msk		(0x1UL<<SPI_CR2_TXDMAEN_Pos)
#define SPI_CR2_TXDMAEN			SPI_CR2_TXDMAEN_Msk
#define SPI_CR2_SSOE_Pos        (2U)  // SS Output Enable
#define SPI_CR2_SSOE_Msk		(0x1UL<<SPI_CR2_SSOE_Pos)
#define SPI_CR2_SSOE			SPI_CR2_SSOE_Msk
#define SPI_CR2_ERRIE_Pos       (5U)  // Error Interrupt Enable
#define SPI_CR2_ERRIE_Msk		(0x1UL<<SPI_CR2_ERRIE_Pos)
#define SPI_CR2_ERRIE			SPI_CR2_ERRIE_Msk
#define SPI_CR2_RXNEIE_Pos      (6U)  // RX buffer Not Empty Interrupt Enable
#define SPI_CR2_RXNEIE_Msk		(0x1UL<<SPI_CR2_RXNEIE_Pos)
#define SPI_CR2_RXNEIE			SPI_CR2_RXNEIE_Msk
#define SPI_CR2_TXEIE_Pos       (7U)  // Tx buffer Empty Interrupt Enable
#define SPI_CR2_TXEIE_Msk		(0x1UL<<SPI_CR2_TXEIE_Pos)
#define SPI_CR2_TXEIE			SPI_CR2_TXEIE_Msk
#define SPI_CR2_FRF_Pos         (4U)  // Frame Format(0 = Motorola, 1 = TI)
#define SPI_CR2_FRF_Msk			(0x1UL<<SPI_CR2_FRF_Pos)
#define SPI_CR2_FRF				SPI_CR2_FRF_Msk
#define SPI_CR2_DS_Pos          (8U)  // Data Size(for SPI with variable frame sizes)
#define SPI_CR2_DS_Msk			(0x1UL<<SPI_CR2_DS_Pos)
#define SPI_CR2_DS				SPI_CR2_DS_Msk

/*
 * Utility Macros
 */

#define ENABLE 	1
#define DISABLE 0

#define SET 	1
#define RESET 	0




#include "stm32f407xx_rcc_driver.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx_i2c_driver.h"


#endif /* INC_STM32F407XX_H_ */

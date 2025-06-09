/*
 * stm32f407xx_driver.h
 *
 *  Created on: Jun 2, 2025
 *      Author: JuanP
 */

#ifndef INC_STM32F407XX_DRIVER_H_
#define INC_STM32F407XX_DRIVER_H_

#include "stm32f407xx.h"




/*
 *  GPIO configuration helpers
 */

typedef enum{
	GPIO_PORT_A = 0,
	GPIO_PORT_B,
	GPIO_PORT_C,
	GPIO_PORT_D,
	GPIO_PORT_E,
	GPIO_PORT_F,
	GPIO_PORT_G,
	GPIO_PORT_H,
	GPIO_PORT_I,
	GPIO_PORTS
}GPIO_Port_t;

typedef enum {
	GPIO_MODE_IN,
	GPIO_MODE_OUT,
	GPIO_MODE_ALTFN,
	GPIO_MODE_ANALOG,
	GPIO_MODE_IT_FT,
	GPIO_MODE_IT_RT,
	GPIO_MODE_IT_RFT

}GPIO_Mode_t;

typedef enum {
	GPIO_OTYPE_PUSHPULL,
	GPIO_OTYPE_ODRAIN
}GPIO_OType_t;

typedef enum{
	GPIO_SPEED_LOW,
	GPIO_SPEED_MEDIUM,
	GPIO_SPEED_HIGH,
	GPIO_SPEED_VERYHIGH
}GPIO_Speed_t;

typedef enum{
	GPIO_PUPD_NONE,
	GPIO_PUPD_UP,
	GPIO_PUPD_DOWN,
}GPIO_PUPD_t;

typedef enum{
	AF_SYS,
	AF_TIMER1to2,
	AF_TIMER3to5,
	AF_TIMER8to11,
	AF_I2C1to3,
	AF_SPI1to2_I2S2,
	AF_SPI3_I2Sext_3,
	AF_USART1to3_I2S3ext,
	AF_UART4to5_USART6,
	AF_CAN1to2_TIM12to14,
	AF_OTGFS_HS,
	AF_ETH,
	AF_FSMC_SDIO_OTGFS,
	AF_DCMI,
	AF_14,
	AF_15,
}GPIO_AFMode_t;

typedef enum{
	GPIO_PIN_NO_0,
	GPIO_PIN_NO_1,
	GPIO_PIN_NO_2,
	GPIO_PIN_NO_3,
	GPIO_PIN_NO_4,
	GPIO_PIN_NO_5,
	GPIO_PIN_NO_6,
	GPIO_PIN_NO_7,
	GPIO_PIN_NO_8,
	GPIO_PIN_NO_9,
	GPIO_PIN_NO_10,
	GPIO_PIN_NO_11,
	GPIO_PIN_NO_12,
	GPIO_PIN_NO_13,
	GPIO_PIN_NO_14,
	GPIO_PIN_NO_15,
}GPIO_PinNum_t;


typedef struct{

	GPIO_PinNum_t GPIO_PinNumber;
	GPIO_Mode_t GPIO_PinMode;
	GPIO_Speed_t GPIO_PinSpeed;
	GPIO_PUPD_t	GPIO_PinPuPdControl;
	GPIO_OType_t GPIO_PinOType;
	GPIO_AFMode_t GPIO_PinAltFnMode;

}GPIO_PinConfig_t;



extern const GPIO_RegDef_t *GPIO[GPIO_PORTS];


/*
 * Handle structure for GPIO pins
 */

typedef struct{

	GPIO_RegDef_t *p_GPIOx; 			/* Base address of GPIO port that the pin belongs to */
	GPIO_PinConfig_t GPIO_PinConfig;	/* Pin configuration settings */

}GPIO_Handle_t;




/**********************************************************************
 * 						APIs supported by this driver
 **********************************************************************/

uint8_t GPIO_portToIndex(GPIO_RegDef_t *p_GPIOx);

/*
 * Peripheral clock setup
 */
void GPIO_periClockControl(GPIO_Port_t port, uint8_t EnDi);
/*
 *  Init and de-init
 */
void GPIO_PinInit(GPIO_Handle_t *p_GPIOHandle);
void GPIO_Reset(GPIO_RegDef_t *p_GPIOx);

/*
 * Data read/write
 */
uint8_t GPIO_ReadPin(GPIO_RegDef_t *p_GPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadPort(GPIO_RegDef_t *p_GPIOx);
void GPIO_WritePin(GPIO_RegDef_t *p_GPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WritePort(GPIO_RegDef_t *p_GPIOx, uint16_t Value);
void GPIO_TogglePin(GPIO_RegDef_t *p_GPIOx, uint8_t PinNumber);

/*
 * IRQ config and ISR handling
 */
void GPIO_IRQInterruptConfig(IRQn_t IRQn, uint8_t EnDi);
void GPIO_IRQPriorityConfig(IRQn_t IRQn, uint8_t priority);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F407XX_DRIVER_H_ */

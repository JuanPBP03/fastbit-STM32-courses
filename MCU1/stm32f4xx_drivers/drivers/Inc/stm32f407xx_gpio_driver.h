/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Jun 2, 2025
 *      Author: JuanP
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"


/*
 *  GPIO pin configuration
 */
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
	GPIO_PushPull,
	GPIO_OpenDrain
}GPIO_OType_t;

typedef enum{
	GPIO_LowSpeed,
	GPIO_MediumSpeed,
	GPIO_HighSpeed,
	GPIO_VeryHighSpeed
}GPIO_Speed_t;

typedef enum{
	GPIO_NoPUPD,
	GPIO_PullUp,
	GPIO_PullDown,
}GPIO_PUPD_t;

typedef enum{
	GPIO_AF0,
	GPIO_AF1,
	GPIO_AF2,
	GPIO_AF3,
	GPIO_AF4,
	GPIO_AF5,
	GPIO_AF6,
	GPIO_AF7,
	GPIO_AF8,
	GPIO_AF9,
	GPIO_AF10,
	GPIO_AF11,
	GPIO_AF12,
	GPIO_AF13,
	GPIO_AF14,
	GPIO_AF15,
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
	GPIO_OType_t GPIO_PinOPType;
	GPIO_AFMode_t GPIO_PinAltFunMode;

}GPIO_PinConfig_t;

/*
 * Handle structure for GPIO pins
 */

typedef struct{

	GPIO_RegDef_t *p_GPIOx; 			/* Base address of GPIO port that the pin belongs to */
	GPIO_PinConfig_t GPIO_PinConfig;	/* Pin configuration settings */

}GPIO_Handle_t;


/*
 * GPIO pin modes
 */


/**********************************************************************
 * 						APIs supported by this driver
 **********************************************************************/

/*
 * Peripheral clock setup
 */
void GPIO_periClockControl(GPIO_RegDef_t *p_GPIOx, uint8_t EnDi);
/*
 *  Init and de-init
 */
void GPIO_Init(GPIO_Handle_t *p_GPIOHandle);
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
void GPIO_IRQConfig(uint8_t IRQNum, uint8_t IRQPriority, uint8_t EnDi);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */

/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Jun 2, 2025
 *      Author: JuanP
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

typedef struct{

	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t	GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;

}GPIO_PinConfig_t;

/*
 * Handle structure for GPIO pins
 */

typedef struct{

	GPIO_RegDef_t *p_GPIOx; 			/* Base address of GPIO port that the pin belongs to */
	GPIO_PinConfig_t GPIO_PinConfig;	/* Pin configuration settings */

}GPIO_Handle_t;



#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */

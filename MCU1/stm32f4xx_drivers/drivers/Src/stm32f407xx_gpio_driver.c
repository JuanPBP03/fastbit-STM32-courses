/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Jun 2, 2025
 *      Author: JuanP
 */

#include "stm32f407xx_gpio_driver.h"


/*
 * Peripheral clock setup
 */

/*********************************************************
 * @fn					- GPIO_periClockControl
 *
 * @brief				- Enable or disable peripheral clock for given GPIO port
 *
 * @param[in]			- base address of GPIO port
 * @param[in]			- ENABLE or DISABLE macros (1 or 0)
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_periClockControl(GPIO_RegDef_t *p_GPIOx, uint8_t EnDi)
{
    if (EnDi == ENABLE)
    {
        if (p_GPIOx == GPIOA)      GPIOA_PCLK_EN();
        else if (p_GPIOx == GPIOB) GPIOB_PCLK_EN();
        else if (p_GPIOx == GPIOC) GPIOC_PCLK_EN();
        else if (p_GPIOx == GPIOD) GPIOD_PCLK_EN();
        else if (p_GPIOx == GPIOE) GPIOE_PCLK_EN();
        else if (p_GPIOx == GPIOF) GPIOF_PCLK_EN();
        else if (p_GPIOx == GPIOG) GPIOG_PCLK_EN();
        else if (p_GPIOx == GPIOH) GPIOH_PCLK_EN();
        else if (p_GPIOx == GPIOI) GPIOI_PCLK_EN();
    }
    else
    {
        if (p_GPIOx == GPIOA)      GPIOA_PCLK_DI();
        else if (p_GPIOx == GPIOB) GPIOB_PCLK_DI();
        else if (p_GPIOx == GPIOC) GPIOC_PCLK_DI();
        else if (p_GPIOx == GPIOD) GPIOD_PCLK_DI();
        else if (p_GPIOx == GPIOE) GPIOE_PCLK_DI();
        else if (p_GPIOx == GPIOF) GPIOF_PCLK_DI();
        else if (p_GPIOx == GPIOG) GPIOG_PCLK_DI();
        else if (p_GPIOx == GPIOH) GPIOH_PCLK_DI();
        else if (p_GPIOx == GPIOI) GPIOI_PCLK_DI();
    }
}


/*
 *  Init and de-init
 */

/*********************************************************
 * @fn					- GPIO_Init
 *
 * @brief				- Initialize GPIO pin through GPIOHandle
 *
 * @param[in]			- GPIO_Handle structure
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_Init(GPIO_Handle_t *p_GPIOHandle){

	GPIO_RegDef_t *PORT = p_GPIOHandle->p_GPIOx;
	GPIO_PinConfig_t CONFIG = p_GPIOHandle->GPIO_PinConfig;
	GPIO_PinNum_t PinNum = CONFIG.GPIO_PinNumber;
	PORT->MODER &= ~(0x3 << (PinNum * 2));

	if(CONFIG.GPIO_PinMode <= GPIO_MODE_ANALOG){
		// 1. Configure pin mode
		PORT->MODER |= ((CONFIG.GPIO_PinMode)<<(PinNum*2));

		// Configure alt. function if applicable
		if(CONFIG.GPIO_PinMode == GPIO_MODE_ALTFN){
			if(PinNum <= 7){
				PORT->AFRL &= ~(0xF<<PinNum*4);
				PORT->AFRL |= ((CONFIG.GPIO_PinAltFnMode)<<(PinNum*4));
			}else{
				PORT->AFRH &= ~(0xF<<((PinNum-8)*4));
				PORT->AFRH |= ((CONFIG.GPIO_PinAltFnMode)<<((PinNum-8)*4));
			}
		}
	}else{

	}
	// 2. Configure pin speed
	PORT->OSPEEDR &= ~(0x3<<(PinNum*2));
	PORT->OSPEEDR |= ((CONFIG.GPIO_PinSpeed)<<(PinNum*2));

	// 3. Configure pull up/down setting
	PORT->PUPDR &= ~(0x3<<(PinNum*2));
	PORT->PUPDR |= ((CONFIG.GPIO_PinPuPdControl)<<(2*PinNum));

	// 4. Configure output type
	PORT->OTYPER &= ~(1<<(PinNum));
	PORT->OTYPER |= ((CONFIG.GPIO_PinOType)<<PinNum);



}

/*********************************************************
 * @fn					- GPIO_Reset
 *
 * @brief				- Reset given GPIO port
 *
 * @param[in]			- base address of GPIO port
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_Reset(GPIO_RegDef_t *p_GPIOx){
	uint8_t PORTNUM = (uint8_t)(((uint32_t)p_GPIOx - AHB1_BASEADDR)/0x400);
	RCC->AHB1RSTR |= (1 << PORTNUM);   // Assert reset
	RCC->AHB1RSTR &= ~(1 << PORTNUM);  // Deassert reset
}

/*
 * Data read/write
 */

/*********************************************************
 * @fn					- GPIO_ReadPin
 *
 * @brief				- Read a single pin from given GPIO port
 *
 * @param[in]			- base address of GPIO port
 * @param[in]			- Pin number to read
 * @param[in]			-
 *
 * @return				- uint8_t
 *
 * @Note				- none
 */
uint8_t GPIO_ReadPin(GPIO_RegDef_t *p_GPIOx, uint8_t PinNumber){
	return 0;
}

/*********************************************************
 * @fn					- GPIO_ReadPort
 *
 * @brief				- Simultaneously read all pins from given GPIO port
 *
 * @param[in]			- base address of GPIO port
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- uint16_t
 *
 * @Note				- none
 */
uint16_t GPIO_ReadPort(GPIO_RegDef_t *p_GPIOx){
	return 0;
}

/*********************************************************
 * @fn					- GPIO_WritePin
 *
 * @brief				- Write to a single GPIO pin
 *
 * @param[in]			- base address of GPIO port
 * @param[in]			- SET or RESET (1 or 0)
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_WritePin(GPIO_RegDef_t *p_GPIOx, uint8_t PinNumber, uint8_t Value){

}

/*********************************************************
 * @fn					- GPIO_WritePort
 *
 * @brief				- Simultaneously write to all pins froma given GPIO port
 *
 * @param[in]			- base address of GPIO port
 * @param[in]			- pin number to write to
 * @param[in]			- SET or RESET macros (1 or 0)
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_WritePort(GPIO_RegDef_t *p_GPIOx, uint16_t Value){

}

/*********************************************************
 * @fn					- GPIO_TogglePin
 *
 * @brief				- Toggle a GPIO pin
 *
 * @param[in]			- base address of GPIO port
 * @param[in]			- SET or RESET macros (1 or 0)
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_TogglePin(GPIO_RegDef_t *p_GPIOx, uint8_t PinNumber){

}

/*
 * IRQ config and ISR handling
 */

/*********************************************************
 * @fn					- GPIO_IRQConfig
 *
 * @brief				- Configure IRQ for GPIO pin
 *
 * @param[in]			- IRQ number
 * @param[in]			- IRQ priority
 * @param[in]			- ENABLE or DISABLE (1 or 0)
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_IRQConfig(uint8_t IRQNum, uint8_t IRQPriority, uint8_t EnDi){

}

/*********************************************************
 * @fn					- GPIO_IRQHandling
 *
 * @brief				- Handle an interrupt request for a GPIO pin
 *
 * @param[in]			- Pin number
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_IRQHandling(uint8_t PinNumber){

}

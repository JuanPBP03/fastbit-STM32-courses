/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Jun 2, 2025
 *      Author: JuanP
 */

#include "stm32f407xx_gpio_driver.h"


const GPIO_RegDef_t *GPIO[GPIO_PORTS] = {
		GPIOA,
		GPIOB,
		GPIOC,
		GPIOD,
		GPIOE,
		GPIOF,
		GPIOG,
		GPIOH,
		GPIOI
};

/*
 * Helper functions
 */

/*********************************************************
 * @fn					- GPIO_portToIndex
 *
 * @brief				- Convert GPIO base addr to port index
 *
 * @param[in]			- base address of GPIO port
 * @param[in]			-
 * @param[in]			-
 *
 * @return				- index of GPIO port
 *
 * @Note				- none
 */
uint8_t GPIO_portToIndex(GPIO_RegDef_t *p_GPIOx){
	uint8_t Index = (uint8_t)(((uint32_t)p_GPIOx - GPIOA_BASEADDR) / GPIO_ADDR_STRIDE);
	return Index;
}





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
void GPIO_periClockControl(GPIO_Port_t port, uint8_t EnDi)
{
	if (port < GPIO_PORTS){
		if (EnDi == ENABLE)
		{
			RCC->AHB1ENR |= (1U<<((uint32_t)port));
		}
		else
		{
			RCC->AHB1ENR &= ~(1U<<((uint32_t)port));
		}
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
void GPIO_PinInit(GPIO_Handle_t *p_GPIOHandle){

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
				PORT->AFRL &= ~(0xF<<(PinNum*4));
				PORT->AFRL |= ((CONFIG.GPIO_PinAltFnMode)<<(PinNum*4));
			}else{
				PORT->AFRH &= ~(0xF<<((PinNum-8)*4));
				PORT->AFRH |= ((CONFIG.GPIO_PinAltFnMode)<<((PinNum-8)*4));
			}
		}
	}else{
		switch(CONFIG.GPIO_PinMode){
		case GPIO_MODE_IT_FT:
			EXTI->FTSR |= (1<<PinNum);
			EXTI->RTSR &= ~(1<<PinNum);
			break;
		case GPIO_MODE_IT_RT:
			EXTI->RTSR |= (1<<PinNum);
			EXTI->FTSR &= ~(1<<PinNum);
			break;
		case GPIO_MODE_IT_RFT:
			EXTI->FTSR |= (1<<PinNum);
			EXTI->RTSR |= (1<<PinNum);
			break;
		default: break;
		}
		uint8_t EXTI_reg = PinNum/4;
		uint8_t EXTI_index = (PinNum % 4)*4;
		uint8_t GPIO_Index = GPIO_portToIndex(PORT);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[EXTI_reg] |= GPIO_Index << EXTI_index;
		EXTI->IMR |= (1U<<PinNum);


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
	uint8_t port_index = GPIO_portToIndex(p_GPIOx);
	RCC->AHB1RSTR |= (1U << port_index);   // Assert reset
	RCC->AHB1RSTR &= ~(1U << port_index);  // Deassert reset
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

	uint8_t bit = (uint8_t)((p_GPIOx->IDR >> PinNumber) & 1);
	return bit;
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

	uint16_t port = (uint16_t)(p_GPIOx->IDR);
	return port;

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

	if(Value) p_GPIOx->ODR |= (1U<<PinNumber);
	else p_GPIOx->ODR &= ~(1U<<PinNumber);

}

/*********************************************************
 * @fn					- GPIO_WritePort
 *
 * @brief				- Simultaneously write to all pins froma given GPIO port
 *
 * @param[in]			- base address of GPIO port
 * @param[in]			- Value to set port to
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_WritePort(GPIO_RegDef_t *p_GPIOx, uint16_t Value){

	p_GPIOx->ODR = Value;


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

	p_GPIOx->ODR ^= (1U<<PinNumber);
}

/*
 * IRQ config and ISR handling
 */

/*********************************************************
 * @fn					- GPIO_IRQInterruptConfig
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
void GPIO_IRQInterruptConfig(IRQn_t IRQn, uint8_t EnDi)
{

	if((int32_t)IRQn >= 0){
		if(EnDi == ENABLE){
		NVIC->ISER[(uint32_t)IRQn>>5U] = (uint32_t)(1UL << ((uint32_t)(IRQn) & 0x1FU));
		}
		else
		{
		NVIC->ICER[(uint32_t)IRQn>>5U] = (uint32_t)(1UL << ((uint32_t)(IRQn) & 0x1FU));
		}
	}

}
/*********************************************************
 * @fn					- GPIO_IRQPriorityConfig
 *
 * @brief				- Configure IRQ priority for GPIO pin
 *
 * @param[in]			- IRQ number
 * @param[in]			- IRQ priority
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 */
void GPIO_IRQPriorityConfig(IRQn_t IRQn, uint8_t priority)
{
	NVIC->IPR[((uint32_t)IRQn) >> 2U] &= ~(0xFFU << (((uint32_t)(IRQn) & 0x3U)*8));
	NVIC->IPR[((uint32_t)IRQn) >> 2U] |= (uint32_t)(((priority&0x0F)<<__NVIC_PRIO_BITS)<<(((uint32_t)(IRQn) & 0x3U)*8));
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
	if(EXTI->PR & 1<<PinNumber)
	{
		EXTI->PR |= 1<<PinNumber;
	}
}

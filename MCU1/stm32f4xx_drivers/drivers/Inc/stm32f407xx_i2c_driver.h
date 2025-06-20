/*
 * stm32f4xx_i2c_driver.h
 *
 *  Created on: Jun 10, 2025
 *      Author: juanp
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"


typedef enum{
	I2C_CH1 = 0,
	I2C_CH2,
	I2C_CH3,

	I2C_CHANNELS
}I2C_Channel_t;



#define I2C_MODE_FM 		1
#define I2C_MODE_SM			0

#define	I2C_SPEED_STANDARD		100000
#define	I2C_SPEED_FAST2K	200000
#define	I2C_SPEED_FAST4K	400000

#define I2C_FM_DUTY_2		0
#define I2C_FM_DUTY_16_9	1

#define I2C_ACK_ENABLE		1
#define I2c_ACK_DISABLE		0

#define	I2C_FLAG_SB			I2C_SR1_SB
#define	I2C_FLAG_ADDR		I2C_SR1_ADDR
#define	I2C_FLAG_BTF		I2C_SR1_BTF
#define	I2C_FLAG_ADD10		I2C_SR1_ADD10
#define	I2C_FLAG_STOPF		I2C_SR1_STOPF
#define	I2C_FLAG_RXNE		I2C_SR1_RXNE
#define	I2C_FLAG_TXE		I2C_SR1_TXE
#define	I2C_FLAG_BERR		I2C_SR1_BERR
#define	I2C_FLAG_ARLO		I2C_SR1_ARLO
#define	I2C_FLAG_AF			I2C_SR1_AF
#define	I2C_FLAG_OVR		I2C_SR1_OVR
#define	I2C_FLAG_PECERR		I2C_SR1_PECERR
#define	I2C_FLAG_TIMEOUT	I2C_SR1_TIMEOUT
#define	I2C_FLAG_SMBALERT	I2C_SR1_SMBALERT

#ifdef USE_SINGLETON_HANDLES
typedef struct I2C_Handle I2C_Handle_t;

I2C_Handle_t I2C1_getHandle(void);
I2C_Handle_t I2C1_getHandle(void);
I2C_Handle_t I2C1_getHandle(void);
#endif

typedef struct{
	uint32_t ClockSpeed;
	uint8_t DutyCycle;
	uint8_t OwnAddr;
	uint8_t Ack;
	uint8_t Mode;
}I2C_Config_t;

typedef struct{
	I2C_RegDef_t *instance;
	I2C_Channel_t channel;
	I2C_Config_t config;
}I2C_Handle_t;

void I2C_periClockCtrl(I2C_Channel_t ch, uint8_t ENorDI);

void I2C_Init(I2C_Handle_t *p_I2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_MasterTx(I2C_Handle_t *hI2C, uint8_t *pTxBuffer, uint32_t len, uint8_t addr  );

void I2C_MasterRx(void);

void I2C_SlaveTx(void);

void I2C_SlaveRx(void);

void I2C_ErrIRQHandler(void);

void I2c_EventIRQHandler(void);

void I2C_IRQInterruptConfig(IRQn_t IRQNumber, uint8_t ENorDI);
void I2C_IRQInterruptConfig(IRQn_t IRQn, uint8_t priority);


void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t ENorDI);
uint8_t I2C_getFlag(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv);

#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */

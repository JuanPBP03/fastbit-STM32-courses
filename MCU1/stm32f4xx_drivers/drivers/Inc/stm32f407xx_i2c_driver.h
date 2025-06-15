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

};


typedef enum{

};
typedef enum{

};
typedef enum{

};
typedef enum{

};

#define	I2C_SPEED_SLOW		100000
#define	I2C_SPEED_FAST2K	200000
#define	I2C_SPEED_FAST4K	400000


typedef struct{
	uint32_t ClockSpeed;
	uint8_t DutyCycle;
	uint8_t OwnAddr;
	uint8_t Ack;
}I2C_Config_t;

typedef struct{
	I2C_RegDef_t instance;
	I2C_Config_t config;


}I2C_Handle_t;



void I2C_init(void);

void I2C_MasterTx(void);

void I2C_MasterRx(void);

void I2C_SlaveTx(void);

void I2C_SlaveRx(void);

void I2C_ErrIRQHandler(void);

void I2c_EventIRQHandler(void);

#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */

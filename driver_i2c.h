#ifndef __DRIVER_I2C_H
#define __DRIVER_I2C_H

#define SCL_PIN	GPIO_PIN_10
#define SDA_PIN	GPIO_PIN_11

#define SCL_PORT	GPIOF
#define SDA_PORT	GPIOF

#define SCL_H	HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, 1)
#define SCL_L	HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, 0)

#define SDA_H	HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, 1)
#define SDA_L	HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, 0)

#define SDA_IN	HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN)


void I2C_GPIO_ReInit(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_ack(void);
void i2c_nack(void);
int i2c_GetAck(void);
void i2c_writebyte(uint8_t data);
uint8_t i2c_readbyte(uint8_t ack);



#endif


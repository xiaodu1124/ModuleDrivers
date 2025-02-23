#include "driver_i2c.h"

void I2C_delay(uint32_t t)
{
	volatile uint32_t tmp = t;
	while(tmp--);
}

void I2C_GPIO_ReInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_GPIO_DeInit(SCL_PORT, SCL_PIN);
	HAL_GPIO_DeInit(SDA_PORT, SDA_PIN);

	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStruct.Pin = SCL_PIN;

	HAL_GPIO_Init(SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);
}

void i2c_start(void)
{
	SCL_H;
	SDA_H;
	I2C_delay(10);
	SDA_L;
	I2C_delay(10);
}

void i2c_stop(void)
{
	SDA_L;
	SCL_H;
	I2C_delay(10);
	SDA_H;
	I2C_delay(10);
}

void i2c_ack(void)
{
	SCL_L;
	SDA_L;
	I2C_delay(10);
	SCL_H;
	I2C_delay(10);
}

void i2c_nack(void)
{
	SCL_L;
	SDA_H;
	I2C_delay(10);
	SCL_H;
	I2C_delay(10);
}

int i2c_GetAck(void)
{
	int i = 0;
	SCL_L;
	SDA_H;
	I2C_delay(10);
	SCL_H;
	while(SDA_IN != 0)
	{
		i++;
		if(i == 5)
		{
			SCL_L;
			return -1;
		}
		I2C_delay(10);
	}
	SCL_L;
	return 0;
}

void i2c_writebyte(uint8_t data)
{
	uint8_t i = 0;
	for(i = 0;i < 8;i++)
	{
		SCL_L;
		I2C_delay(10);
		if(data & 0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}
		data <<= 1;
		SCL_H;
		I2C_delay(10);
	}
	i2c_GetAck();
}

uint8_t i2c_readbyte(uint8_t ack)
{
	int i = 0;
	uint8_t data = 0;
	SDA_H;
	for(i = 0; i < 8;i++)
	{
		SCL_L;
		I2C_delay(10);
		SCL_H;
		I2C_delay(10);
		data <<= 1;
		if(SDA_IN == 1)
		{
			data++;
		}
		else
		{
			data = data;
		}
	}
	if(ack)
	{
		i2c_ack();
	}
	else if(ack == 1)
	{
		i2c_nack();
	}
	return data;
}


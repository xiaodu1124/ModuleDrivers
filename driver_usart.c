#include "driver_usart.h"
#include "ring_buffer.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "uart_device.h"

static volatile uint8_t txcplt_flag = 0;
static volatile uint8_t rxcplt_flag = 0;

ring_buffer test_buff;

typedef void (*NetInputProcessCallback)(char c);
static NetInputProcessCallback g_fNetInputProcessCallback;

void SetNetInputProcesscallback(NetInputProcessCallback func)
{
	g_fNetInputProcessCallback = func;
}

void EnableDebugIRQ(void)
{
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_TC | UART_IT_RXNE);
}

void EnableUART3IRQ(void)
{
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_TC | UART_IT_RXNE);
}

int fputc(int ch, FILE *f)
{
	txcplt_flag = 0;
	HAL_UART_Transmit_IT(&huart1, (uint8_t *)&ch, 1);
	while(txcplt_flag == 0);
	return 0;
}

int fgetc(FILE *f)
{
	char c = 0;
	while(ring_buffer_read((unsigned char *)&c, &test_buff) != 0);
	return c;
}

void USART1_IRQHandler(void)
{
	unsigned char c = 0;
	if((USART1->SR & (1 << 5)) != 0)
	{
		c = USART1->DR;
		ring_buffer_write(c, &test_buff);
	}
	HAL_UART_IRQHandler(&huart1);
}

void USART3_IRQHandler(void)
{
	static ring_buffer *uart3_ringbuffer = NULL;
	unsigned char c = 0;
	if(!uart3_ringbuffer)
		uart3_ringbuffer = GetUART3RingBuffer();
	if((USART3->SR & (1<<5)) != 0)
	{
		c = USART3->DR;
		ring_buffer_write(c, uart3_ringbuffer);
		if(g_fNetInputProcessCallback)
		{
			g_fNetInputProcessCallback(c);
		}
	}
	HAL_UART_IRQHandler(&huart3);	
}

void USART3_SendBytes(char *buf, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		while((USART3->SR & (1 << 7)) == 0);
		USART3->DR = buf[i];
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		rxcplt_flag = 1;
	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance ==  USART1)
	{
		txcplt_flag = 1;
	}
}

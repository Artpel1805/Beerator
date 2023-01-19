/*
 * drv_uart1.c
 *
 *  Created on: 7 nov. 2022
 *      Author: laurentf
 */

#include "drv_uart1.h"

#include "usart.h"
#include "gpio.h"

uint8_t drv_uart1_receive(char * pData, uint16_t size)
{
	HAL_UART_Receive(&huart1, (uint8_t*)(pData), size, HAL_MAX_DELAY);

	return 0;	// Life's too short for error management
}

uint8_t drv_uart1_transmit(const char * pData, uint16_t size)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)pData, size, HAL_MAX_DELAY);

	return 0;	// Srsly, don't do that kids
}

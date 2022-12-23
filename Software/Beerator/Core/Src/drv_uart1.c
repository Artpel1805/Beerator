/*
 * drv_uart1.c
 *
 *  Created on: 18 dec. 2022
 *      Author: artpel1805
 */

#include <stdio.h>
#include "drv_uart1.h"
#include "usart.h"
#include "shell.h"

uint8_t drv_uart1_receive(char * pData, uint16_t size)
{
	HAL_StatusTypeDef Hal_Response = HAL_UART_Receive_IT(&UART_DEVICE, (uint8_t*)(pData), size);
	return Hal_Response;
}

uint8_t drv_uart1_transmit(const char * pData, uint16_t size)
{
	HAL_StatusTypeDef HAL_Response = HAL_UART_Transmit(&UART_DEVICE, (uint8_t*)pData, size, HAL_MAX_DELAY);
	return HAL_Response;
}

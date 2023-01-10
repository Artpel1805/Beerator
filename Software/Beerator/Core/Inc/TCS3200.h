/*
 * TCS3200.h
 *
 *  Created on: Nov 21, 2022
 *      Author: arthurpellegrin
 */

#include "main.h"
#include <stdio.h>
#include "cmsis_os.h"

#ifndef INC_TCS3200_H_
#define INC_TCS3200_H_

typedef struct color_struct
{

	uint32_t red;
	uint32_t green;

	SemaphoreHandle_t TCS3200_sem;
} TCS3200_struct;

typedef enum TCS3200_outputFS_enum
{
	outputFS_0     = 0,
	outputFS_2     = 1,
	outputFS_20    = 2,
	outputFS_100   = 3
} TCS3200_FO;

typedef enum TCS3200_filter_enum
{
	RED     = 0,
	BLUE    = 1,
	CLEAR  	= 2,
	GREEN 	= 3
} TCS3200_filter;

//uint8_t TCS3200_Init(TCS3200_struct *TCS3200);
uint8_t TCS3200_Init();
uint8_t TCS3200_Set_outputFS(TCS3200_FO outputFS);
void TCS3200_Display_Color();
void TCS3200_Read_Color();




#endif /* INC_TCS3200_H_ */





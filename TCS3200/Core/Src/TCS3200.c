/*
 * TCS3200.c
 *
 *  Created on: Dec 31, 2022
 *      Author: nassi
 */

#include "TCS3200.h"
#include "main.h"

extern  float frequency ;
extern 	float red_freq;
extern  float green_freq;

uint8_t TCS3200_OE()
{
	HAL_GPIO_WritePin(OE_GPIO_Port,OE_Pin,GPIO_PIN_RESET);
	return 1;
}
uint8_t TCS3200_Set_outputFS(TCS3200_FO outputFS)
{
	switch(outputFS)
	{
	case outputFS_0 :
		HAL_GPIO_WritePin(S0_GPIO_Port, S0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_RESET);
		break;
	case outputFS_2:
		HAL_GPIO_WritePin(S0_GPIO_Port, S0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_SET);
		break;
	case outputFS_20:
		HAL_GPIO_WritePin(S0_GPIO_Port, S0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_RESET);
		break;
	case outputFS_100:
		HAL_GPIO_WritePin(S0_GPIO_Port, S0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_SET);
	default:
		HAL_GPIO_WritePin(S0_GPIO_Port, S0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(S1_GPIO_Port, S1_Pin, GPIO_PIN_SET);
		break;
	}
	return 1;
}
uint8_t TCS3200_Filter(TCS3200_filter filter)
{
	switch(filter)
	{
	case RED:
		HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);
		break;
	case GREEN:
		HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_SET);
	case BLUE:
		HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin,GPIO_PIN_SET);
		break;
	case CLEAR:
		HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);

	default:
		HAL_GPIO_WritePin(S2_GPIO_Port, S2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(S3_GPIO_Port, S3_Pin, GPIO_PIN_RESET);
		break;
	}
	return 1;
}
uint8_t TCS3200_OE_outputFS_Init()
{
	if(TCS3200_OE()!=1)
	{
		printf("\r\nError Enabling output\r\n");
		return 0;
	}
	if(TCS3200_Filter(CLEAR)!=1)
	{
		printf("\r\nError Enabling Filter\r\n");
		return 0;
	}
	if(TCS3200_Set_outputFS(outputFS_2)!=1)
	{
		printf("\r\nError setting output FS\r\n");
		return 0;
	}
	return 1;

}
void TCS3200_Display_Color()
{
	TCS3200_Filter(RED);
	HAL_Delay(10);
	printf("\r\n RED : %f\r\n ",frequency);
	HAL_Delay(1000);

	TCS3200_Filter(GREEN);
	HAL_Delay(10);
	printf("\r\n GREEN : %f\r\n ",frequency);
	HAL_Delay(1000);

}


uint8_t TCS3200_Init(TCS3200_struct *TCS3200)
{

	TCS3200->TCS3200_sem=xSemaphoreCreateBinary();
	if (TCS3200->TCS3200_sem == NULL)
	{
		printf("Error semaphore color sensor\r\n");
		while(1);
	}
    if(TCS3200_OE_outputFS_Init()!=1)
    {
    	printf("Error Configuration TCS3200 ......... :( \r\n");
    	return 0;
    }


	return 1;
}
void TCS3200_Read_Color(TCS3200_struct *TCS3200_st_color)
{
	float red_freq;
	float green_freq;

	if(TCS3200_Filter(RED)!=1)
	{
	  printf("\r\nRed Filer not enabled !! \r\n");
	}
	HAL_Delay(1);
	red_freq=frequency;
	if(TCS3200_Filter(GREEN)!=1)
	{
	  printf("\r\nGreen  Filer not enabled !! \r\n");
	}
	HAL_Delay(1);
	green_freq=frequency;
	if(red_freq > green_freq)
	{
		TCS3200_st_color->red=1;
		TCS3200_st_color->green=0;
	}
	else {
		TCS3200_st_color->red=0;
		TCS3200_st_color->green=1;
	}



}



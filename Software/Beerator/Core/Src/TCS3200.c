/*
 * TCS3200.c
 *
 *  Created on: Nov 21, 2022
 *      Author: arthurpellegrin
 */


#include "TCS3200.h"
#include "main.h"

extern  float frequency ;
extern 	float red_freq;
extern  float green_freq;



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

	if(TCS3200_Filter(CLEAR)!=1)
	{
		printf("\r\nError Enabling Filter\r\n");
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
	HAL_GPIO_WritePin(TCS_LED_GPIO_Port, TCS_LED_Pin, GPIO_PIN_SET);
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
		printf("REDD COLOOOR ");
	}
	else {
		TCS3200_st_color->red=0;
		TCS3200_st_color->green=1;
		printf("GREEN COLOOOR ");
	}



}


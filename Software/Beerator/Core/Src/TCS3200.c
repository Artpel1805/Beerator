/*
 * TCS3200.c
 *
 *  Created on: Nov 21, 2022
 *      Author: arthurpellegrin
 */


#include "TCS3200.h"
#include "main.h"



void TCS3200_Init()
{

	TCS3200_Switch_S2(1);
	TCS3200_Switch_S3(0);


}


void TCS3200_Switch_Led(int state){
	if(state == 1){
		HAL_GPIO_WritePin(TCS_LED_GPIO_Port, TCS_LED_Pin, GPIO_PIN_SET);
	}
	else if(state == 0){
		HAL_GPIO_WritePin(TCS_LED_GPIO_Port, TCS_LED_Pin, GPIO_PIN_RESET);
	}
	else{
		Error_Handler();
	}
}

void TCS3200_Switch_S2(int state){
	if(state == 1){
		HAL_GPIO_WritePin(TCS_S2_GPIO_Port, TCS_S2_Pin, GPIO_PIN_SET);
	}
	else if(state == 0){
		HAL_GPIO_WritePin(TCS_S2_GPIO_Port, TCS_S2_Pin, GPIO_PIN_RESET);
	}
	else{
		Error_Handler();
	}
}

void TCS3200_Switch_S3(int state){
	if(state == 1){
		HAL_GPIO_WritePin(TCS_S3_GPIO_Port, TCS_S3_Pin, GPIO_PIN_SET);
	}
	else if(state == 0){
		HAL_GPIO_WritePin(TCS_S3_GPIO_Port, TCS_S3_Pin, GPIO_PIN_RESET);
	}
	else{
		Error_Handler();
	}
}

/*void TCS3200_Read_Color(TCS3200_struct *TCS3200_st_color)
{
	float red_freq;
	float green_freq;

	TCS3200_Switch_S2(0);
	TCS3200_Switch_S3(0);
	HAL_Delay(1);
	red_freq=frequency;
	HAL_Delay(1);

	TCS3200_Switch_S2(1);
	TCS3200_Switch_S3(1);

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
*/




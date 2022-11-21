/*
 * TCS3200.c
 *
 *  Created on: Nov 21, 2022
 *      Author: arthurpellegrin
 */


#include "TCS3200.h"
#include "main.h"


int flag=1;
int Red_Freq;
int Green_Freq;
int Blue_Freq;

void TCS3200_Init()
{
	TCS3200_Switch_Led(0);
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




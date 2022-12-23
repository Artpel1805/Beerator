/*
 * motor_drive.c
 *
 *  Created on: Dec 5, 2022
 *      Author: arthurpellegrin
 */


#include "motor_drive.h"
#include <stdio.h>


// CONTROL MOTOR


void motor_start_forward(h_motor_t * motor){
	HAL_TIMEx_PWMN_Stop(motor->htim_motor, motor->channel_motor);
	HAL_TIM_PWM_Start(motor->htim_motor, motor->channel_motor);
}

void motor_start_reverse(h_motor_t * motor){
	HAL_TIM_PWM_Stop(motor->htim_motor, motor->channel_motor);
	HAL_TIMEx_PWMN_Start(motor->htim_motor, motor->channel_motor);
}

void motor_stop(h_motor_t * motor){
	HAL_TIM_PWM_Stop(motor->htim_motor, motor->channel_motor);
	HAL_TIMEx_PWMN_Stop(motor->htim_motor, motor->channel_motor);

}

void motors_stop(h_motors_t * motors){
	motor_stop(motors->motor1);
	motor_stop(motors->motor2);
}

//

void motors_init(h_motors_t * motors_struc){

}



//
//static void change_speed(TIM_HandleTypeDef *htim, int dutyCycle){
//	int goalCCR = get_ccr_value(htim, dutyCycle);
//	if(goalCCR == -1){
//		return;
//	}
//	if( goalCCR > htim -> Instance -> CCR1){
//		while(htim -> Instance -> CCR1 != goalCCR){
//			htim -> Instance -> CCR1 ++;
//		}
//		return;
//	}
//	if( goalCCR < htim -> Instance -> CCR1){
//		while(htim -> Instance -> CCR1 != goalCCR){
//			htim -> Instance -> CCR1 --;
//		}
//		return;
//	}
//}
//
//static int get_ccr_value(TIM_HandleTypeDef *htim, int dutyCycle){
//	if(dutyCycle > MAX_DUTY){
//			return -1;
//		}
//		if(dutyCycle< MIN_DUTY){
//			return -1;
//		}
//		int arr = htim->Instance->ARR;
//		return (dutyCycle * arr) / 100;
//}



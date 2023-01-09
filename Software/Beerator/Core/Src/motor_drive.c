/*
 * motor_drive.c
 *
 *  Created on: Dec 5, 2022
 *      Author: arthurpellegrin
 */


#include "motor_drive.h"


// CONTROL MOTOR


int motor_run_forward(h_motor_t * motor){

	if(motor->isReverse == 1){
		if(	HAL_TIM_PWM_Stop_IT(motor->htim_motor, motor->Channel_Motor_Forward) != HAL_OK){
			return MOTOR_UNKNOWN_ERROR;
		}
		if(	HAL_TIMEx_PWMN_Start_IT(motor->htim_motor, motor->Channel_Motor_Reverse) != HAL_OK){
			return MOTOR_UNKNOWN_ERROR;
		}
		return MOTOR_OK;
	}


	if(HAL_TIMEx_PWMN_Stop_IT(motor->htim_motor, motor->Channel_Motor_Reverse) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	if(HAL_TIM_PWM_Start_IT(motor->htim_motor, motor->Channel_Motor_Forward) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	return MOTOR_OK;
}

int motor_run_reverse(h_motor_t * motor){
	if(motor->isReverse == 1){
		if(HAL_TIMEx_PWMN_Stop_IT(motor->htim_motor, motor->Channel_Motor_Reverse) != HAL_OK){
			return MOTOR_UNKNOWN_ERROR;
		}
		if(HAL_TIM_PWM_Start_IT(motor->htim_motor, motor->Channel_Motor_Forward) != HAL_OK){
			return MOTOR_UNKNOWN_ERROR;
		}
		return MOTOR_OK;
	}
	if(	HAL_TIM_PWM_Stop_IT(motor->htim_motor, motor->Channel_Motor_Forward) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	if(	HAL_TIMEx_PWMN_Start_IT(motor->htim_motor, motor->Channel_Motor_Reverse) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	return MOTOR_OK;
}

int motor_stop(h_motor_t * motor){
	if(	HAL_TIM_PWM_Stop_IT(motor->htim_motor, motor->Channel_Motor_Forward) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	if(	HAL_TIMEx_PWMN_Stop_IT(motor->htim_motor, motor->Channel_Motor_Reverse) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	return MOTOR_OK;
}

//

int motor_change_dutyCycle(h_motor_t * motor, int newDutyCycle){
	if((newDutyCycle > MAX_DUTY) || (newDutyCycle < MIN_DUTY)){
		return OUT_OF_RANGE;
	}
	motor->dutyCycle = newDutyCycle;
	return MOTOR_OK;
}

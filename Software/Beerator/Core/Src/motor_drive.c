/*
 * motor_drive.c
 *
 *  Created on: Dec 5, 2022
 *      Author: arthurpellegrin
 */


#include "motor_drive.h"
#include "asserv.h"

// CONTROL MOTOR


int motor_run_forward(h_motor_t * motor){
	if(motor->isReverse == 1){
		if(	HAL_TIM_PWM_Stop_IT(motor->htim_motor, motor->Channel_Motor_Forward) != HAL_OK){
			return MOTOR_UNKNOWN_ERROR;
		}
		if(	HAL_TIMEx_PWMN_Start_IT(motor->htim_motor, motor->Channel_Motor_Reverse) != HAL_OK){
			return MOTOR_UNKNOWN_ERROR;
		}
		motor->speedInstruction = SPEED_COMMAND;
		motor->status = MOTOR_FORWARD;
		return MOTOR_OK;
	}
	if(HAL_TIMEx_PWMN_Stop_IT(motor->htim_motor, motor->Channel_Motor_Reverse) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	if(HAL_TIM_PWM_Start_IT(motor->htim_motor, motor->Channel_Motor_Forward) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	motor->speedInstruction = SPEED_COMMAND;
	motor->status = MOTOR_FORWARD;
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
		motor->speedInstruction = - SPEED_COMMAND;
		motor->status = MOTOR_REVERSE;
		return MOTOR_OK;
	}
	if(	HAL_TIM_PWM_Stop_IT(motor->htim_motor, motor->Channel_Motor_Forward) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	if(	HAL_TIMEx_PWMN_Start_IT(motor->htim_motor, motor->Channel_Motor_Reverse) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	motor->speedInstruction = - SPEED_COMMAND;
	motor->status = MOTOR_REVERSE;
	return MOTOR_OK;
}

int motor_stop(h_motor_t * motor){
	motor->speedInstruction = STOP_COMMAND;
	if(HAL_TIMEx_PWMN_Stop_IT(motor->htim_motor, motor->Channel_Motor_Reverse) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	if(	HAL_TIM_PWM_Stop_IT(motor->htim_motor, motor->Channel_Motor_Forward) != HAL_OK){
		return MOTOR_UNKNOWN_ERROR;
	}
	return MOTOR_OK;
}

/*
 * motor_drive.h
 *
 *  Created on: Dec 5, 2022
 *      Author: arthurpellegrin
 */
#include "main.h"
#include "tim.h"

#define ENCODER_MOTOR_1_TIM &htim1
#define ENCODER_MOTOR_2_TIM &htim3

#define MOTOR1_TIM &htim16
#define MOTOR1_CHANNEL_FORWARD TIM_CHANNEL_1
#define MOTOR1_CHANNEL_REVERSE TIM_CHANNEL_1

#define MOTOR2_TIM &htim17
#define MOTOR2_CHANNEL_FORWARD TIM_CHANNEL_1
#define MOTOR2_CHANNEL_REVERSE TIM_CHANNEL_1

#define DEFAULT_PERIOD 100
#define MAX_DUTY 99
#define MIN_DUTY 0

#ifndef INC_MOTOR_DRIVE_H_
#define INC_MOTOR_DRIVE_H_

#endif /* INC_MOTOR_DRIVE_H_ */

typedef enum MOTOR_STATUS {
	MOTOR_FORWARD,
	MOTOR_REVERSE,
	MOTOR_PAUSED,
} MOTOR_STATUS_TYPE;

typedef enum MOTOR_ERRORS {
	MOTOR_NO_CHANGES = 2,
	MOTOR_OK = 1,
	MOTOR_UNKNOWN_ERROR = -1,
	OUT_OF_RANGE = -2,
} MOTOR_ERROR_TYPE;

typedef struct h_motor_t {
	int dutyCycle;
	int period;
	int counter;
	int isReverse;
	MOTOR_STATUS_TYPE status;
	TIM_HandleTypeDef * htim_motor;
	TIM_HandleTypeDef * htim_encoder;
	uint32_t Channel_Motor_Forward;
	uint32_t Channel_Motor_Reverse;
} h_motor_t;

typedef struct h_motors_t {
	h_motor_t * motor1;
	h_motor_t * motor2;
} h_motors_t;


int motor_run_forward(h_motor_t * motor);

int motor_run_reverse(h_motor_t * motor);

int motor_stop(h_motor_t * motor);

int motor_change_dutyCycle(h_motor_t * motor, int dutyCycle);

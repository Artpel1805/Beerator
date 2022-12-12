/*
 * motor_drive.h
 *
 *  Created on: Dec 5, 2022
 *      Author: arthurpellegrin
 */
#include "main.h"
#include "tim.h"

#ifndef INC_MOTOR_DRIVE_H_
#define INC_MOTOR_DRIVE_H_

#define MOTOR1_TIM &htim16
#define MOTOR1_CHANNEL TIM_CHANNEL_1
#define MOTOR2_TIM &htim17
#define MOTOR2_CHANNEL TIM_CHANNEL_1
#define MAX_DUTY 100
#define MIN_DUTY 0

#endif /* INC_MOTOR_DRIVE_H_ */

typedef struct h_motor_t {
	TIM_HandleTypeDef * htim_motor;
	uint32_t channel_motor;
} h_motor_t;

typedef struct h_motors_t {
	h_motor_t * motor1;
	h_motor_t * motor2;
} h_motors_t;


void motor_start_forward(h_motor_t * motor);

void motor_start_reverse(h_motor_t * motor);

void motor_stop(h_motor_t * motor);

void motors_stop(h_motors_t * motors);

void motors_init(h_motors_t * motors_struc);

//static void change_speed(TIM_HandleTypeDef *htim, int dutyCycle);
//
//static int get_ccr_value(TIM_HandleTypeDef *htim, int dutyCycle);

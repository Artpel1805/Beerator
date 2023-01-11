/*
 * asserv.c
 *
 *  Created on: 9 janv. 2023
 *      Author: arthurpellegrin
 */


#include "motor_drive.h"
#include "asserv.h"
#include <stdio.h>


int pid_vitesse(h_motor_t * motor){
	int vitesse;
	if(motor->isReverse){
		vitesse = (( motor->htim_encoder->Instance->ARR - motor->htim_encoder->Instance->CNT )* 60000 * 10 ) / (NUMBER_TICK * SAMPLE_RATE_PID);
	} else {
		vitesse = (motor->htim_encoder->Instance->CNT * 60000 * 10 ) / (NUMBER_TICK * SAMPLE_RATE_PID);
	}
	int erreur = SPEED_COMMAND - (vitesse);

	motor->htim_motor->Instance->CCR1 += erreur * Kp;

	if(motor->htim_motor->Instance->CCR1 > 100) {
		motor->htim_motor->Instance->CCR1 = 100;
	}
	if(motor->htim_motor->Instance->CCR1 < 20) {
		motor->htim_motor->Instance->CCR1 = 20;
	}
	motor->htim_encoder->Instance->CNT=0;
	return 0;
}

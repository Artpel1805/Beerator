/*
 * asserv.c
 *
 *  Created on: 9 janv. 2023
 *      Author: arthurpellegrin
 */


#include "motor_drive.h"
#include "asserv.h"
#include <stdio.h>
#include "math.h"


int pid_vitesse(h_motor_t * motor){
	if(motor->isReverse){
		motor->vitesse = ((int)((motor->counter - motor->htim_encoder->Instance->CNT)) * 60000 * 10 ) / (NUMBER_TICK * SAMPLE_RATE_PID);
		motor->counter = motor->htim_encoder->Instance->CNT;

	} else {
		motor->vitesse = ((int)((motor->htim_encoder->Instance->CNT - motor->counter)) * 60000 * 10 ) / (NUMBER_TICK * SAMPLE_RATE_PID);
		motor->counter = motor->htim_encoder->Instance->CNT;
	}
	int erreur = motor->speedInstruction - (motor->vitesse);
	motor->htim_motor->Instance->CCR1 += erreur * Kp;

	if(motor->htim_motor->Instance->CCR1 > 100) {
		motor->htim_motor->Instance->CCR1 = 100;
	}
	if(motor->htim_motor->Instance->CCR1 < 0) {
		motor->htim_motor->Instance->CCR1 = 0;
	}
	return 0;
}

void update_position(pos_R *pos, h_motor_t * motor)
{
	float dR0 = pos->dR;
	float dL0 = pos->dL;

	if(motor->isReverse){
		pos->dR += Rayon*(motor->vitesse)*(2*M_PI/60)*0.05;
	}else {
		pos->dL += Rayon*(motor->vitesse)*(2*M_PI/60)*0.05;
	}
	pos->d_alpha = ((((dR0-pos->dR)-(dL0-pos->dL))/L)*180)/M_PI;
    pos->alpha = pos->alpha + pos->d_alpha;

}


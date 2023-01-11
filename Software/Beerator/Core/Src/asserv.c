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

float vr_left_sum=0;
float vr_right_sum=0;

int pid_vitesse(h_motor_t * motor){
	//int vitesse;
	if(motor->isReverse){
		motor->vitesse = (((motor->htim_encoder->Instance->ARR - motor->htim_encoder->Instance->CNT )%motor->htim_encoder->Instance->ARR) * 60000 * 10 ) / (NUMBER_TICK * SAMPLE_RATE_PID);

	} else {
		motor->vitesse = (motor->htim_encoder->Instance->CNT * 60000 * 10 ) / (NUMBER_TICK * SAMPLE_RATE_PID);
	}


	int erreur = SPEED_COMMAND - (motor->vitesse);

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

void position(pos_R *pos,h_motor_t * motorR, h_motor_t * motorL)
{

	vr_left_sum+=(motorL->vitesse)*(2*M_PI/60);
	vr_right_sum+=(motorR->vitesse)*(2*M_PI/60);

	//pos->dR=Rayon*vr_right_sum;
	//pos->dL=Rayon*vr_left_sum;

	pos->dR+=Rayon*(motorR->vitesse)*(2*M_PI/60)*0.1;
	pos->dL+=Rayon*(motorL->vitesse)*(2*M_PI/60)*0.1;


	pos->d_theta=(pos->dR+pos->dL)/2;
	pos->d_alpha =((pos->dR-pos->dL)/L);

    pos->alpha=pos->alpha + pos->d_alpha;



	//printf("\r\ntheta  en metre : %f \r\n",pos.delta_theta);
	//printf("\r\n aplha %f \r\n",pos.alpha);

}
void Turn(float Angle)
{


}

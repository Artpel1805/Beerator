/*
 * asserv.h
 *
 *  Created on: 9 janv. 2023
 *      Author: arthurpellegrin
 */

#ifndef INC_ASSERV_H_
#define INC_ASSERV_H_



#endif /* INC_ASSERV_H_ */


#define NUMBER_TICK 2244 * 4
#define SAMPLE_RATE_PID 100
#define SPEED_COMMAND 80 // between 80 and 300
#define Kp 1/10
#define Rayon 21.4 // en mm
#define L 148.8

typedef struct coordinate {
	float x;
	float y;
	float d_theta;
	float d_alpha;
	float dR;
	float dL;
	float d_Left;
	float d_Right;
	float dX;
	float dY;
	float alpha;
	float theta;

} pos_R;


int pid_vitesse(h_motor_t * motor);
void position(pos_R *pos,h_motor_t * motorR,h_motor_t * motorL);

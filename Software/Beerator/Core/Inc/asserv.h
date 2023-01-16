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
#define STOP_COMMAND 0
#define Kp 1/10
#define Rayon 42.51/2 // mm
#define L 153.84 // mm

typedef struct coordinate {
	float d_alpha;
	float dR; //mm
	float dL; //mm
	float alpha;

} pos_R;


int pid_vitesse(h_motor_t * motor);
void update_position(pos_R *pos,h_motor_t * motor);

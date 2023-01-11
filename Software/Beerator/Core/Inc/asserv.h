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
#define Kp 2/10


int pid_vitesse(h_motor_t * motor);

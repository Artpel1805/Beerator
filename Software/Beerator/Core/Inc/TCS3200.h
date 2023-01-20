/*
 * TCS3200.h
 *
 *  Created on: Nov 21, 2022
 *      Author: arthurpellegrin
 */

#include "main.h"
#include <stdio.h>
#include "cmsis_os.h"

#ifndef INC_TCS3200_H_
#define INC_TCS3200_H_

typedef struct color_struct
{

	uint32_t red;
	uint32_t green;

} TCS3200_struct;

#endif /* INC_TCS3200_H_ */




void TCS3200_Init(void);
void TCS3200_Switch_Led(int);
void TCS3200_Switch_S2(int);
void TCS3200_Switch_S3(int);

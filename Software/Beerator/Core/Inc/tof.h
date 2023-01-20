/*
 * driver_TOF.h
 *
 *  Created on: 12 déc. 2022
 *      Author: adrie
 */

#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

#include "vl53l0x_api.h"
#include <string.h>

#ifndef DRIVER_TOF_INC_DRIVER_TOF_H_
#define DRIVER_TOF_INC_DRIVER_TOF_H_

#define SIZE_BUF_MESSAGE 50

#define NO_ERROR_TOF 0
#define INIT_ERROR_TOF 1
#define MEASURE_ERROR_TOF 2
#define MEASURE_OUT_OF_RANGE_TOF 3

#define MAX_READABLE_DISTANCE 500 // Distance maximale mesuree (millimetres)
// Lignes 2485 à 2489 de "vl53lox_api.c" en rapport direct
// Une distance de 501 sigifie "out_of_range"

typedef struct {
	I2C_HandleTypeDef *I2cHandle;
	// Les autres constantes relatives à la connectique sont définies depuis la perspective IOC
} TOF_InitStruct;

typedef struct {
	int currentValue; // Last read range value
	//int lastTenValues[10]; // Last ten range values
	//int averageOnTen; // Average of the ten last range values

	int currentErrorStatus; // Current error status as defined from #20 to #22
	char errorBuf[100]; // Current error Status as a String
} TOF_UserStruct;

int TOF_init(TOF_InitStruct*); // Initialise le capteur TOF, renvoit un code d'erreur entier
int TOF_measure(void); // Lance une mesure avec le TOF, resultat en mm entre 0 et MAX_READABLE_DISTANCE, ou -1 si hors de portée
TOF_UserStruct* TOF_getTOF_UserStruct(void); // Renvoie le pointeur vers la structure TOF_UserStruct utilisée
int TOF_correctValue(int readValue); // Corrige la lecture faussée du TOF avec une courbe calculée en amont
void TOF_setErrorLiteral(int errorCode); // Renseigne l'explicitation lettrée dans la structure TOF_UserStruct utilisé

#endif /* DRIVER_TOF_INC_DRIVER_TOF_H_ */


// Utilisation/choses à rajouter dans le main :

/* USER CODE BEGIN 1 */
	//TOF_InitStruct TOF_InitStructure;
	//TOF_InitStruct * pTOF_InitStruct = &TOF_InitStructure;
	//pTOF_InitStruct->I2cHandle = &hi2c2;
/* USER CODE END 1 */

//...

/* USER CODE BEGIN 2 */

  //TOF_init(pTOF_InitStruct);

/* USER CODE END 2 */

//...

// Dans la tâche de mesure :

// TOF_measure();
//printf("%d\r\n", TOF_getTOF_UserStruct()->currentValue); (PAR EXEMPLE)

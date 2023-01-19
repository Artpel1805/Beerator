/*
 * driver_TOF.c
 *
 *  Created on: 12 déc. 2022
 *      Author: adrie
 */

#include "tof.h"

VL53L0X_RangingMeasurementData_t RangingData;
VL53L0X_Dev_t vl53l0x_c;
VL53L0X_DEV Dev = &vl53l0x_c;

// uint8_t Message[SIZE_BUF_MESSAGE];
// int8_t MessageLen;

static TOF_UserStruct TOF_UserStruct_;
static TOF_UserStruct * pTOF_UserStruct = &TOF_UserStruct_;


void TOF_init(TOF_InitStruct* pTOF_InitStruct){
	uint32_t refSpadCount;
	uint8_t isApertureSpads;
	uint8_t VhvSettings;
	uint8_t PhaseCal;

	// MessageLen = sprintf((char *)Message, "\r\n\r\n==== Début des mesures ====\r\n\r\n");
	// HAL_UART_Transmit(&huart1, Message, MessageLen, 100);

	Dev->I2cHandle = pTOF_InitStruct->I2cHandle;
	Dev->I2cDevAddr = 0x52;

	// Séquence obligatoire pour initialiser les mesures.
	HAL_GPIO_WritePin(TOF_XSHUT_GPIO_Port, TOF_XSHUT_Pin, GPIO_PIN_RESET); // Enable XSHUT
	HAL_Delay(20);// Disable XSHUT
	HAL_GPIO_WritePin(TOF_XSHUT_GPIO_Port, TOF_XSHUT_Pin, GPIO_PIN_SET); // Enable XSHUT
	HAL_Delay(20);// Disable XSHUT

	// VL53L0X initialization for Single Measurement :

	VL53L0X_WaitDeviceBooted( Dev );
	VL53L0X_DataInit( Dev );
	VL53L0X_StaticInit( Dev );
	VL53L0X_PerformRefCalibration(Dev, &VhvSettings, &PhaseCal);
	VL53L0X_PerformRefSpadManagement(Dev, &refSpadCount, &isApertureSpads);
	if(VL53L0X_SetDeviceMode(Dev, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING) != VL53L0X_ERROR_NONE){////// SINGLE->CONTINUOUS
		pTOF_UserStruct->currentErrorStatus = INIT_ERROR_TOF;
		TOF_setErrorLiteral(INIT_ERROR_TOF);
	}

	VL53L0X_StartMeasurement(Dev);//////

	// Enable/Disable Sigma and Signal check :

	VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
	VL53L0X_SetLimitCheckEnable(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
	VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1*65536));
	VL53L0X_SetLimitCheckValue(Dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60*65536));

	// On initialise à 501 la distance lue :
	pTOF_UserStruct->currentValue = 501;

	return;
}



int TOF_measure(void){ // Retourne -1 si erreur, la distance lue en mm sinon (entre ~20 et 501)

	int distanceCorrige;

	if(VL53L0X_PerformSingleRangingMeasurement(Dev, &RangingData) != VL53L0X_ERROR_NONE){
			pTOF_UserStruct->currentErrorStatus = MEASURE_ERROR_TOF;
			TOF_setErrorLiteral(MEASURE_ERROR_TOF);
			return -1;
	}

	if(RangingData.RangeMilliMeter == 501){
		pTOF_UserStruct->currentErrorStatus = MEASURE_OUT_OF_RANGE_TOF;
		TOF_setErrorLiteral(MEASURE_OUT_OF_RANGE_TOF);
	}

	distanceCorrige = TOF_correctValue(RangingData.RangeMilliMeter);

	if((distanceCorrige >= 0) && (distanceCorrige <= 500)){
		// On actualise à sa vraie valeur la distance lue :
		pTOF_UserStruct->currentValue = distanceCorrige;
		return distanceCorrige;
	}

	pTOF_UserStruct->currentErrorStatus = MEASURE_OUT_OF_RANGE_TOF;
	TOF_setErrorLiteral(MEASURE_OUT_OF_RANGE_TOF);
	return -1;
}



TOF_UserStruct* TOF_getTOF_UserStruct(){
	return pTOF_UserStruct;
}



// Static methods to ease of processes :

int TOF_correctValue(int readValue){ // Valeurs à vérifier en test
	return readValue \
			- 20*(MAX_READABLE_DISTANCE-readValue)*(MAX_READABLE_DISTANCE-readValue)*(MAX_READABLE_DISTANCE-readValue)\
			/(MAX_READABLE_DISTANCE*MAX_READABLE_DISTANCE*MAX_READABLE_DISTANCE); // x  <-  x - 20*(1 - x/500)**3
}



void TOF_setErrorLiteral(int errorCode){
	if (errorCode == NO_ERROR_TOF) {
		strcpy(pTOF_UserStruct->errorBuf, "Aucune erreur détectée.\r\n");
	} else if (errorCode == INIT_ERROR_TOF) {
		strcpy(pTOF_UserStruct->errorBuf, "Erreur lors de l'initialisation.\r\n");
	} else if (errorCode == MEASURE_ERROR_TOF) {
		strcpy(pTOF_UserStruct->errorBuf, "Erreur lors de la mesure.\r\n");
	} else if (errorCode == MEASURE_OUT_OF_RANGE_TOF) {
		strcpy(pTOF_UserStruct->errorBuf, "Erreur, cible trop lointaine ou trop proche.\r\n");
	} else {
		strcpy(pTOF_UserStruct->errorBuf, "Erreur inconnue.\r\n");
	}
}

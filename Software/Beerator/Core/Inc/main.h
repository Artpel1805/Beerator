/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IR1_OUT_Pin GPIO_PIN_11
#define IR1_OUT_GPIO_Port GPIOC
#define IR1_OUT_EXTI_IRQn EXTI4_15_IRQn
#define IR2_OUT_Pin GPIO_PIN_12
#define IR2_OUT_GPIO_Port GPIOC
#define IR2_OUT_EXTI_IRQn EXTI4_15_IRQn
#define LED_GREEN_Pin GPIO_PIN_3
#define LED_GREEN_GPIO_Port GPIOC
#define TOF_INT_Pin GPIO_PIN_2
#define TOF_INT_GPIO_Port GPIOA
#define TOF_XSHUT_Pin GPIO_PIN_3
#define TOF_XSHUT_GPIO_Port GPIOA
#define TOF2_XSHUT_Pin GPIO_PIN_5
#define TOF2_XSHUT_GPIO_Port GPIOA
#define TCS_OUT_Pin GPIO_PIN_1
#define TCS_OUT_GPIO_Port GPIOB
#define BAT_SENSE_Pin GPIO_PIN_10
#define BAT_SENSE_GPIO_Port GPIOB
#define TCS_S3_Pin GPIO_PIN_13
#define TCS_S3_GPIO_Port GPIOB
#define TCS_S2_Pin GPIO_PIN_14
#define TCS_S2_GPIO_Port GPIOB
#define TCS_LED_Pin GPIO_PIN_15
#define TCS_LED_GPIO_Port GPIOB
#define TOF_I2C_SCL_Pin GPIO_PIN_11
#define TOF_I2C_SCL_GPIO_Port GPIOA
#define TOF_I2C_SDA_Pin GPIO_PIN_12
#define TOF_I2C_SDA_GPIO_Port GPIOA
#define PHASE_A_M1_Pin GPIO_PIN_8
#define PHASE_A_M1_GPIO_Port GPIOC
#define PHASE_B_M1_Pin GPIO_PIN_9
#define PHASE_B_M1_GPIO_Port GPIOC
#define SERVO_DATA_Pin GPIO_PIN_5
#define SERVO_DATA_GPIO_Port GPIOD
#define PHASE_B_M2_Pin GPIO_PIN_4
#define PHASE_B_M2_GPIO_Port GPIOB
#define PHASE_A_M2_Pin GPIO_PIN_5
#define PHASE_A_M2_GPIO_Port GPIOB
#define NPWM_M1_Pin GPIO_PIN_6
#define NPWM_M1_GPIO_Port GPIOB
#define NPWM_M2_Pin GPIO_PIN_7
#define NPWM_M2_GPIO_Port GPIOB
#define PWM_M1_Pin GPIO_PIN_8
#define PWM_M1_GPIO_Port GPIOB
#define PWM_M2_Pin GPIO_PIN_9
#define PWM_M2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

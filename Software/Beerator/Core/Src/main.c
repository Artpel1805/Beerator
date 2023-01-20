/* USER CODE BEGIN Header */
/**
******************************************************************************
* @file           : main.c
* @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "motor_drive.h"
#include "shell.h"
#include "drv_uart1.h"
#include "XL320.h"
#include "asserv.h"
#include "tof.h"
#include "TCS3200.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STACK_SIZE 300
#define ANGLE_ERROR 5
#define TIMCLOCK   64000000
#define PRESCALAR  64
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
char * logo = "\r\n\n\n .oPYo. .oPYo. .oPYo.  .oPYo.      .oo ooooo .oPYo.  .oPYo.\r\n8   `8 8.     8.      8   `8     .P 8   8   8    8  8   `8\r\no8YooP' `boo   `boo   o8YooP'    .P  8   8   8    8 o8YooP'\r\n 8   `b .P     .P      8   `b   oPooo8   8   8    8  8   `b\r\n8    8 8      8       8    8  .P    8   8   8    8  8    8\r\n8oooP' `YooP' `YooP'  8    8 .P     8   8   `YooP'  8    8\r\n:......::.....::.....::..:::....:::::..::..:::.....::..:::..\r\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\r\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\r\n\r\n\r\n\r\n";

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//Shell
h_shell_t h_shell;

// Motors
h_motor_t motor1;
h_motor_t motor2;
h_motors_t motors;
pos_R pos;

// Servo
h_XL320_t XL320;

//TOF
TOF_InitStruct tof;

//Color Sensor
int red_freq =0;
int green_freq=0;
int flag=0;

//parametre Robot

float angle;
float distance;
int mesure ;

//TCS3200
TCS3200_struct *TCS3200_st_color;

//Handlers

TaskHandle_t xOpenXL320Handle = NULL;
TaskHandle_t xCloseXL320Handle = NULL;
TaskHandle_t xBorderDetectionHandle = NULL;
TaskHandle_t xMotorSpeedControl = NULL;
TaskHandle_t xTOFMesure = NULL;
TaskHandle_t xTurnHandle = NULL;
TaskHandle_t xTurnReverseRightHandle = NULL;
TaskHandle_t xTurnReverseLeftHandle = NULL;
TaskHandle_t  xGetPosHandle=NULL;
TaskHandle_t xAvanceHandle=NULL;
TaskHandle_t xBackHandle=NULL;
TaskHandle_t xSearchHandle=NULL;
TaskHandle_t xColorHandle=NULL;


//Global

float distance = 100;
int borderDetected = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
int Is_First_Captured = 0;

/* Measure Frequency */
float frequency = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
{
	if (Is_First_Captured==0) // if the first rising edge is not captured
	{
		IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // read the first value
		Is_First_Captured = 1;  // set the first captured as true
	}

	else   // If the first rising edge is captured, now we will capture the second edge
	{
		IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value

		if (IC_Val2 > IC_Val1)
		{
			Difference = IC_Val2-IC_Val1;
		}

		else if (IC_Val1 > IC_Val2)
		{
			Difference = (0xffff - IC_Val1) + IC_Val2;
		}

		float refClock = TIMCLOCK/(PRESCALAR);

		frequency = refClock/Difference;

		__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
		Is_First_Captured = 0; // set it back to false
	}
}
}



int __io_putchar(int chr)
{
HAL_UART_Transmit(&UART_DEVICE, (uint8_t*)&chr, 1, HAL_MAX_DELAY);
return chr;
}



// Task

void Start_Shell(void* pvParameters){
shell_run(&h_shell);
vTaskDelete(NULL);
}

void Motor_Speed_Control(void * PvParameters){
motor2.htim_motor->Instance->CCR1 = 20;
motor1.htim_motor->Instance->CCR1 = 20;
xTaskNotifyGive(xSearchHandle);
for(;;){
	pid_vitesse(&motor2);
	pid_vitesse(&motor1);
	//		printf("Motor Controled \r\n");
	vTaskDelay(100);
}
vTaskDelete(NULL);
}

void Get_Position(void*PvParameters)
{
for(;;)
{
	update_position(&pos, &motor1);
	update_position(&pos, &motor2);
	//		printf("Position Updated \r\n");

	vTaskDelay(50);
}
vTaskDelete(NULL);
}


void Border_Detected(void * PvParameters){
float angle=35;
for(;;){
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	motor_stop(&motor2);
	motor_stop(&motor1);
	float buff_r = pos.dR;
	float buff_l = pos.dL;

	motor_run_reverse(&motor1);
	motor_run_reverse(&motor2);

	while((abs(buff_r - pos.dR) < 100) && (abs(buff_l - pos.dL) < 100))
	{
		if(abs(buff_r - pos.dR) > 100){
			motor_stop(&motor1);

		}
		if(abs(buff_l - pos.dL) > 100){
			motor_stop(&motor2);

		}
	}
	motor_stop(&motor1);
	motor_stop(&motor2);

	float buff=pos.alpha;
	motor_run_reverse(&motor2);
	while(abs(angle-abs(buff-pos.alpha))>ANGLE_ERROR)
	{

	}
	motor_stop(&motor2);
	xTaskNotifyGive(xSearchHandle);
	borderDetected = 0 ;
}
vTaskDelete(NULL);
}

void Search_Task(void* PvParameters){
for(;;){
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	int delay = rand() % 5000;
	motor_stop(&motor2);
	motor_stop(&motor1);
	motor2.speedInstruction =  50;
	motor_run_forward(&motor2);
	vTaskDelay(delay);
	motor2.speedInstruction =  SPEED_COMMAND;

	motor_run_forward(&motor1);
}
vTaskDelete(NULL);
}

void TOF_Task(void* PvParameters){
for(;;){
	mesure = TOF_measure();
	printf("mesure TOF: %d \r\n", mesure);
	if((mesure != -1) && (mesure > 20)){
		motor_stop(&motor1);
		motor_stop(&motor2);
		xTaskNotifyGive(xOpenXL320Handle);
		xTaskNotifyGive(xAvanceHandle);
	}
	if((mesure != -1) && (mesure < 20)){
		motor_stop(&motor1);
		motor_stop(&motor2);
		xTaskNotifyGive(xCloseXL320Handle);
		xTaskNotifyGive(xColorHandle);
		xTaskNotifyGive(xColorHandle);

	}
	vTaskDelay(100);
}
}

void Avance_Task(void*PvParameters)
{
for(;;)
{
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	float buff_r = pos.dR;
	float buff_l = pos.dL;
	motor_run_forward(&motor1);
	motor_run_forward(&motor2);
	while((abs(buff_r - pos.dR) < mesure) && (abs(buff_l - pos.dL) < mesure))
	{
		if(abs(buff_r - pos.dR) > mesure){
			motor_stop(&motor1);

		}
		if(abs(buff_l - pos.dL) > mesure){
			motor_stop(&motor2);

		}
	}
	motor_stop(&motor2);
	motor_stop(&motor1);
}
vTaskDelete(NULL);
}

void TCS3200_Task(void * pvParameterts)
{

	for(;;)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		if(flag==0)
		{

			TCS3200_Switch_S2(0);
			TCS3200_Switch_S3(0);
			HAL_TIM_IC_Start_IT(&htim14, TIM_CHANNEL_1);
			red_freq=frequency;
			printf("RED ---: %d \r\n  ",red_freq);
			HAL_TIM_IC_Stop_IT(&htim14, TIM_CHANNEL_1);
			TCS3200_Switch_S2(1);
			TCS3200_Switch_S3(1);

			flag++;

		}
		else
		{
			HAL_TIM_IC_Start_IT(&htim14, TIM_CHANNEL_1);
			green_freq=frequency;
			printf("Green --- : %d \r\n  ",green_freq);
			HAL_TIM_IC_Stop_IT(&htim14, TIM_CHANNEL_1);
			TCS3200_Switch_S2(0);
			TCS3200_Switch_S3(0);
			flag=0;

		}

	}
}

void XL320_Open(void * pvParameterts){
for(;;){
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	if(XL320_set_goal_position(&XL320, XL320_OPEN_ANGLE) != XL320_OK){
		printf("Error Opening XL320 \r\n");
		Error_Handler();
	};
}
vTaskDelete(NULL);
}

void XL320_Catch_Task(void * pvParameterts){
for(;;){
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	uint16_t XL320_Response = XL320_Catch(&XL320);
	if(XL320_Response == XL320_ERROR){
		printf("Error Catching XL320 \r\n");
		Error_Handler();
	}
}
vTaskDelete(NULL);
}
// Shell Function Text

int XL320_Open_Shell(h_shell_t * h_shell, int argc, char ** argv)
{
xTaskNotifyGive(xOpenXL320Handle);
return SHELL_OK;
}

int XL320_Catch_Shell(h_shell_t * h_shell, int argc, char ** argv)
{
xTaskNotifyGive(xCloseXL320Handle);
return SHELL_OK;
}

int Avance_shell()
{
xTaskNotifyGive(xAvanceHandle);
return SHELL_OK;
}

int Back_shell()
{
xTaskNotifyGive(xBackHandle);
return SHELL_OK;
}

int Turn_shell()
{
xTaskNotifyGive(xTurnReverseLeftHandle);
return SHELL_OK;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM14_Init();
  MX_TIM16_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
printf(logo);


// Init XL320
printf("Initialisation XL320 ... \r\n");

XL320.firmware_version = 0;
XL320.model_number = 0;
XL320.uart = &huart2;
XL320.id = XL320_ID;

if(XL320_Init(&XL320) != XL320_OK){
	printf("Error Initialising XL320...\r\n");
	Error_Handler();
};

printf("Initialisation XL320 Successful \r\n\n");

printf("Initialisation Motors ... \r\n");

motor1.htim_motor = MOTOR1_TIM;
motor1.htim_encoder = ENCODER_MOTOR_1_TIM;
motor1.htim_encoder->Instance->CNT = motor1.htim_encoder->Instance->ARR / 2;
motor1.Channel_Motor_Forward = MOTOR1_CHANNEL_FORWARD;
motor1.Channel_Motor_Reverse = MOTOR1_CHANNEL_REVERSE;
motor1.counter = motor1.htim_encoder->Instance->ARR / 2;
motor1.speedInstruction = 0;
motor1.isReverse = 0;
motor1.status = MOTOR_PAUSED;

motor2.htim_motor = MOTOR2_TIM;
motor2.htim_encoder = ENCODER_MOTOR_2_TIM;
motor2.htim_encoder->Instance->CNT = motor2.htim_encoder->Instance->ARR / 2;
motor2.Channel_Motor_Forward = MOTOR2_CHANNEL_FORWARD;
motor2.Channel_Motor_Reverse = MOTOR2_CHANNEL_REVERSE;
motor2.counter = motor2.htim_encoder->Instance->ARR / 2;
motor2.speedInstruction = 0;
motor2.isReverse = 1;
motor2.status = MOTOR_PAUSED;

printf("Initialisation Motor Control ... \r\n");

HAL_TIM_Encoder_Start_IT(ENCODER_MOTOR_1_TIM, TIM_CHANNEL_ALL);
HAL_TIM_Encoder_Start_IT(ENCODER_MOTOR_2_TIM, TIM_CHANNEL_ALL);

pos.dR = 0;
pos.dL = 0;
pos.d_alpha = 0;
pos.alpha = 0;

printf("Initialisation Motors Successful \r\n\n");

//TOF
printf("Initialisation TOF ... \r\n");

tof.I2cHandle = &hi2c2;
TOF_init(&tof);

//TCS3200

TCS3200_Init();

printf("Initialisation TOF Successful \r\n\n");

//Shell Configuration
printf("Launching Shell .... \r\n");
h_shell.drv.receive = drv_uart1_receive;
h_shell.drv.transmit = drv_uart1_transmit;
h_shell.pos = 0;
h_shell.xHandleShell = NULL;

shell_init(&h_shell);
shell_add(&h_shell, 'O', XL320_Open_Shell, "Open XL320");
shell_add(&h_shell, 'C', XL320_Catch_Shell, "Catch XL320");
shell_add(&h_shell, 'a', Avance_shell, "Avance ");
shell_add(&h_shell, 't', Turn_shell, "Recule ");





// Creating Tasks

BaseType_t xReturned;

xReturned = xTaskCreate(Motor_Speed_Control, "SpeedControl", 500, NULL, tskIDLE_PRIORITY + 6, &xMotorSpeedControl);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}
xReturned = xTaskCreate(Get_Position, "Position", 200, NULL, tskIDLE_PRIORITY + 6, &xGetPosHandle);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}
xReturned = xTaskCreate(Border_Detected, "Border Detection", 1000, NULL, tskIDLE_PRIORITY + 6 ,&xBorderDetectionHandle);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}

xReturned = xTaskCreate(Avance_Task, "Avance Task", 300, NULL, tskIDLE_PRIORITY + 5, &xAvanceHandle);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}

xReturned = xTaskCreate(TOF_Task, "TOFMesure", 1000, NULL, tskIDLE_PRIORITY + 4, &xTOFMesure);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}

xReturned = xTaskCreate(Search_Task, "Search Task", 100, NULL, tskIDLE_PRIORITY + 2, &xSearchHandle);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}


xReturned = xTaskCreate(Start_Shell, "StartShell", STACK_SIZE, NULL, tskIDLE_PRIORITY, &h_shell.xHandleShell);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}


xReturned = xTaskCreate(XL320_Catch_Task, "XL320_Close", STACK_SIZE, NULL, tskIDLE_PRIORITY + 5 ,&xCloseXL320Handle);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}

xReturned = xTaskCreate(XL320_Open, "XL320_OPEN", STACK_SIZE, NULL, tskIDLE_PRIORITY + 5,&xOpenXL320Handle);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}
xReturned = xTaskCreate(TCS3200_Task, "Color Sensor", STACK_SIZE, NULL, tskIDLE_PRIORITY + 5,&xColorHandle);
if(xReturned != pdPASS){
	printf("Error Creating the task \r\n");
}




vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
while (1)
{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
if(huart == &UART_DEVICE){
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(h_shell.xHandleShell, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

}


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){
if(borderDetected != 1){
	borderDetected = 1;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if((GPIO_Pin == IR1_OUT_Pin)){
		vTaskNotifyGiveFromISR(xBorderDetectionHandle, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	if( (GPIO_Pin == IR2_OUT_Pin)){
		vTaskNotifyGiveFromISR(xBorderDetectionHandle, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
/* User can add his own implementation to report the HAL error return state */
__disable_irq();
while (1)
{
	HAL_NVIC_SystemReset();
}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
/* User can add his own implementation to report the file name and line number,
 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

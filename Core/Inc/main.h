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
#include "stm32l1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

void sleep();
void wakeUp();
void SystemClock_Config(void);
void usart3Init();
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
uint8_t NAME1[50];
uint8_t NAME2[50];
uint8_t NAME3[50];
uint8_t IMEI[20];
uint8_t PNUM[40];
uint8_t ID[50];
extern uint8_t* FW;
extern uint8_t* BW;
uint8_t ERROR_LOG1[100];
uint8_t ERROR_LOG2[100];
uint8_t ERROR_LOG3[100];
uint8_t ERROR_LOG4[100];
uint8_t UPNUM[20];
uint8_t SENSORTYPE[10];
uint8_t SETH[10];
uint8_t MINDISTANCE[10];
uint8_t MINSENDPERIOD[10];
uint8_t MINMEASUREMENT[10];
uint8_t MINMEASUREPERIOD[10];

uint8_t APNCONTYPE[10];
uint8_t APNNAME[30];
uint8_t APNUSERNAME[20];
uint8_t APNPASSWORD[20];
uint8_t APNPHONENUM[20];
uint8_t APNRATE[10];

uint8_t MQTTIP[20];
uint8_t MQTTPORT[10];
uint8_t MQTTUSERNAME[20];
uint8_t MQTTPASSWORD[20];
uint8_t MQTTQOS[10];
uint8_t MQTTCLEANASS[10];
uint8_t MQTTKEEPALIVE[10];
uint8_t MQTTTIMEOUT[10];
uint8_t MQTTTOPICS[20][10];
uint8_t FTPIP[20];
uint8_t FTPPORT[10];
uint8_t DEVICEPASSWORD[10];
uint8_t FTPUSERNAME[20];
uint8_t FTPPASSWORD[20];
uint8_t FTPINTERVAL[10];
uint8_t FTPTOPICS[20][10];
uint8_t FTPFOLDER[40];
long TABLE[1001];
extern uint8_t SATH[10];
uint8_t DIST[10];
uint8_t SARF[15];
uint8_t COR[10];
uint8_t LOCK[200];
uint8_t GSMLOCK[200];
uint8_t TEMP[10];
uint8_t BAT[10];
uint8_t SIG[10];
uint8_t MSATH[20];
uint8_t MCON[10];
uint8_t MTEMP[10];
uint8_t PG[10];
extern uint8_t MV[40];
extern uint8_t TIME[30];
extern uint8_t version;
uint8_t TIME_a[30];
uint16_t tick;
int newConfig;
int newTable;
int downloads[4];
int newFirmware;
long logPosition;
long sendPosition;
int batSum;
int batSumc;
int bPG;
int ftpinterval;
int resetDevice;
volatile int sleepCounter;
volatile int checksleepCount;
int savebattery;
int pgCounter;
long pgCounterEnd;
uint8_t POSST[10];
uint8_t STAT[5];
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ON_Pin GPIO_PIN_13
#define ON_GPIO_Port GPIOC
#define ON_EXTI_IRQn EXTI15_10_IRQn
#define ON1_Pin GPIO_PIN_0
#define ON1_GPIO_Port GPIOA
#define status_Pin GPIO_PIN_1
#define status_GPIO_Port GPIOB
#define debug_Pin GPIO_PIN_2
#define debug_GPIO_Port GPIOB
#define _1pps_Pin GPIO_PIN_12
#define _1pps_GPIO_Port GPIOB
#define _1pps_EXTI_IRQn EXTI15_10_IRQn
#define pg_Pin GPIO_PIN_15
#define pg_GPIO_Port GPIOB
#define onsol_Pin GPIO_PIN_7
#define onsol_GPIO_Port GPIOC
#define onc_Pin GPIO_PIN_8
#define onc_GPIO_Port GPIOC
#define onb_Pin GPIO_PIN_9
#define onb_GPIO_Port GPIOC
#define pwrs_Pin GPIO_PIN_10
#define pwrs_GPIO_Port GPIOC
#define gnde_Pin GPIO_PIN_11
#define gnde_GPIO_Port GPIOC
#define onv_Pin GPIO_PIN_12
#define onv_GPIO_Port GPIOC
#define RE_Pin GPIO_PIN_5
#define RE_GPIO_Port GPIOB
#define l3_Pin GPIO_PIN_6
#define l3_GPIO_Port GPIOB
#define l2_Pin GPIO_PIN_7
#define l2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/*
 * uart.c
 *
 *  Created on: Jun 14, 2021
 *      Author: pcbde
 */
#include "uart.h"
#include "bluetooth.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "stm32l1xx_hal.h"
#include "sim868.h"
#include "ftp.h"
#include "files.h"
#include "sleep.h"
#include "mqtt.h"
#include "gps.h"
#include "gsmloc.h"
#include "memory.h"
#include "clock.h"
#include "sensor.h"

uint8_t rx_channel;
uint8_t rx_data;
int rx_count;
uint8_t rx_buf[2000];
int tx_end;
int REGESTRATION = NOT_REGISTERED;
int CONNECTION = NOT_CONNECTED;
int MQTTCONNECTION = NOT_CONNECTED;
int GSMLOCATION = NOT_LOCKED;
int GPSLOCATION = NOT_LOCKED;
int BATTERYSAVER = BATTERYSAVER_OFF;

TIM_HandleTypeDef   uartT;
TIM_HandleTypeDef   callT;
TIM_HandleTypeDef   uart7T;
TIM_HandleTypeDef   buzzT;
ADC_HandleTypeDef adcB;
UART_HandleTypeDef *uartModem;
UART_HandleTypeDef *uartSensor;
UART_HandleTypeDef *uartDebug;
RTC_HandleTypeDef 	*rtc;


#define STM32_UUID ((uint32_t *)0x1FF800D0)

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART1){
		if(rx_channel == 1){
			rx_buf[rx_count++] = rx_data;
		}
		HAL_UART_Receive_IT(uartModem, &rx_data, 1);
	}
	if(huart->Instance == USART2){
		if(rx_channel == 2){
			rx_buf[rx_count++] = rx_data;
		}
		HAL_UART_Receive_IT(uartDebug, &rx_data, 1);
	}
	if(huart->Instance == USART3){
		if(rx_channel == 3){
			rx_buf[rx_count++] = rx_data;
		}
		HAL_UART_Receive_IT(uartSensor, &rx_data, 1);
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	tx_end = 1;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
}

void Initialize(UART_HandleTypeDef *myuart1,UART_HandleTypeDef *myuart2,UART_HandleTypeDef *myuart3,TIM_HandleTypeDef *mytim2,TIM_HandleTypeDef *mytim5,TIM_HandleTypeDef *mytim3,TIM_HandleTypeDef *mytim4,RTC_HandleTypeDef *myrtc,ADC_HandleTypeDef *adc){
	uartModem = myuart1;
	uartDebug = myuart2;
	uartSensor = myuart3;
	uartT = *mytim2;
	buzzT = *mytim5;
	callT = *mytim3;
	uart7T = *mytim4;
	adcB = *adc;
	rtc = myrtc;
	HAL_TIM_Base_Start(&uart7T);
	HAL_UART_Receive_IT(uartModem, &rx_data, 1);
	HAL_UART_Receive_IT(uartSensor, &rx_data, 1);
	HAL_UART_Receive_IT(uartDebug, &rx_data, 1);
	HAL_TIM_Base_Start(&uartT);
	__HAL_TIM_SET_COMPARE(&buzzT,TIM_CHANNEL_2,100);
	__HAL_TIM_SET_COUNTER(&buzzT,366);

	pgCounter = 0;
	pgCounterEnd = 0;
	savebattery = 0;
	REGESTRATION = NOT_REGISTERED;
	CONNECTION = NOT_CONNECTED;
	MQTTCONNECTION = NOT_CONNECTED;
	GSMLOCATION = NOT_LOCKED;
	GPSLOCATION = NOT_LOCKED;
	BATTERYSAVER = BATTERYSAVER_OFF;
	strcpy((char*)DEVICEPASSWORD,"hello");
	  removeBoot();
	  uint32_t d1 = STM32_UUID[0];
	  uint32_t d2 = STM32_UUID[1];
	  uint32_t d3 = STM32_UUID[2];
	  itoa(d1+d2+d3,(char*)ID,16);
	  getTime();
	  debug(TIME);
	  debug(SIMN);
	  debug(ID);
	  debug(SIMN);
	  debug(FW);
	  debug(SIMN);
	  if(HAL_GPIO_ReadPin(ON1_GPIO_Port, ON1_Pin) == GPIO_PIN_RESET){
		  resetDevice = 1;
	  }
	  HAL_Delay(2000);
	  buzzOn();
	  HAL_Delay(200);
	  buzzOff();
	  HAL_Delay(200);
	  buzzOn();
	  HAL_Delay(200);
	  buzzOff();
	  loraOff();
}
int tx(UART_HandleTypeDef *huart,uint8_t* data){
	if(strlen((char*)data) != 0){
		HAL_UART_Transmit_IT(huart, data, (uint16_t)strlen((char*)data));
		tx_end = 0;
		__HAL_TIM_SET_COUNTER(&uartT,0);
		while(tx_end == 0){
			if(__HAL_TIM_GET_COUNTER(&uartT) > 20000){
				return UART_TIMEOUT;
			}
		}
	}
	return UART_OK;
}
int rx(UART_HandleTypeDef *huart,uint8_t* term,uint16_t timeout){
	rx_buf[0]='\0';
	rx_count = 0;
	int i;
	int len = strlen((char *)term);
	int endCount=0;
	int seconCount = 0;
	if(huart->Instance == USART1){
		rx_channel = 1;
	}
	if(huart->Instance == USART2){
		rx_channel = 2;
	}
	if(huart->Instance == USART3){
		rx_channel = 3;
	}
	__HAL_TIM_SET_COUNTER(&uartT,0);
	while(1){
		if(__HAL_TIM_GET_COUNTER(&uartT) > 999){
			__HAL_TIM_SET_COUNTER(&uartT,0);
			seconCount++;
			if(seconCount == timeout){
				return UART_TIMEOUT;
			}
		}
		for(i=0;i<len;i++){
			if(rx_buf[rx_count - len + i] == term[i]){
				endCount++;
				if(endCount == len){
					rx_buf[rx_count] = '\0';
					rx_c = rx_count;
					return UART_OK;
				}
			}
			else{
				endCount = 0;
			}
		}
	}
}
void gpsWait(int w){
	int h;
	for(h=0;h<w;h++){
		HAL_Delay(1000);
	}
}
void buzzOn(){
	HAL_TIM_PWM_Start(&buzzT, TIM_CHANNEL_2);
}
void buzzOff(){
	HAL_TIM_PWM_Stop(&buzzT, TIM_CHANNEL_2);
}
void rxr(UART_HandleTypeDef *huart,uint8_t* term,uint16_t timeout){
	if(rx(huart, term, timeout) == UART_TIMEOUT){
		debug(ERROR_LOG1);
		debug(ERROR_LOG2);
		debug(ERROR_LOG3);
		debug(ERROR_LOG4);
		NVIC_SystemReset();
	}
}
void txr(UART_HandleTypeDef *huart,uint8_t* data){
	static int erl = 1;
	if(erl == 1){
		strcpy((char*)ERROR_LOG1,(char*)data);
	}
	if(erl == 2){
		strcpy((char*)ERROR_LOG2,(char*)data);
	}
	if(erl == 3){
		strcpy((char*)ERROR_LOG3,(char*)data);
	}
	if(erl == 4){
		strcpy((char*)ERROR_LOG4,(char*)data);
		erl = 0;
	}
	erl++;
	if(tx(huart, data) == UART_TIMEOUT){
		debug(ERROR_LOG1);
		debug(ERROR_LOG2);
		debug(ERROR_LOG3);
		debug(ERROR_LOG4);
		NVIC_SystemReset();
	}
}
int bootUp(){
	ledsOn();
	sim868On();
	REGESTRATION = sim868Init();
	getIMEI();
	if(REGESTRATION == MODEM_ERROR){
		//tx(uartDebug, (uint8_t*)"MODEM_ERROR\r\n");
	}
	if(REGESTRATION == NO_SIM){
		//tx(uartDebug, (uint8_t*)"NO_SIM\r\n");
	}
	if(REGESTRATION == NOT_REGISTERED){
		//tx(uartDebug, (uint8_t*)"NOT_REGISTERED\r\n");
	}
	if(REGESTRATION == REGISTERED){
		//tx(uartDebug, (uint8_t*)"REGISTERED\r\n");
		removeFirst();
	}
	readConfig(0);
	readTable(0);
	if(HAL_GPIO_ReadPin(ON1_GPIO_Port, ON1_Pin) == GPIO_PIN_RESET){
		resetDevice = 0;
	}
	if(resetDevice == 1){
		  HAL_FLASHEx_DATAEEPROM_Unlock();
		  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080200, 0xFEEDC0DD);
		  HAL_FLASHEx_DATAEEPROM_Lock();
		  HAL_FLASHEx_DATAEEPROM_Unlock();
		  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080200, 0xFEEDC0DD);
		  HAL_FLASHEx_DATAEEPROM_Lock();
		  buzzOn();
		  HAL_Delay(2000);
		  buzzOff();
		  NVIC_SystemReset();
	}
	if(TIME[0] == '0'){
		dataConnect();
		if(CONNECTION == CONNECTED){
			getgsmloc();
			updateTime();
		}
		dataDisconnect();
		bluetoothMode(180);
		BTPowerOff();
		getADC();
		getTemp();
		getSig();
		getPNUM();
		dataConnect();
		mqttConnect();
		mqttSend(2);
		mqttReceive();
		mqttDisconnect();
		dataDisconnect();
	}
	sim868Off();
	getSleepParametrs();
	getPos();
	sleepCounter = 0;
	checksleepCount=1;
	return REGESTRATION;
}
void modemOn(){
	sim868On();
	if(BATTERYSAVER == BATTERYSAVER_OFF){
		REGESTRATION = sim868Init();
	}
	if(BATTERYSAVER == BATTERYSAVER_ON){
		sim868onPrep();
	}
}

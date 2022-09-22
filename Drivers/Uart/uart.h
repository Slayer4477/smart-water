/*
 * uart.h
 *
 *  Created on: Jun 14, 2021
 *      Author: pcbde
 */

#ifndef UART_H_
#define UART_H_

#include "main.h"

#define UART_OK 50
#define UART_TIMEOUT 51

#define NO_SIM 52
#define NOT_REGISTERED 53
#define REGISTERED 54
#define MODEM_ERROR 55

#define CONNECTED 56
#define NOT_CONNECTED 57

#define LOCKED 58
#define NOT_LOCKED 59

#define MQTTSEND_OK 60
#define MQTTSEND_ERROR 61

#define BATTERYSAVER_ON 62
#define BATTERYSAVER_OFF 63

extern uint8_t rx_channel;
extern uint8_t rx_data;
extern int rx_count;
extern uint8_t rx_buf[2000];
extern int tx_end;
extern TIM_HandleTypeDef   uartT;
extern TIM_HandleTypeDef   buzzT;
extern TIM_HandleTypeDef   callT;
extern TIM_HandleTypeDef   uart7T;
extern ADC_HandleTypeDef adcB;
extern UART_HandleTypeDef *uartModem;
extern UART_HandleTypeDef *uartSensor;
extern UART_HandleTypeDef *uartDebug;
extern RTC_HandleTypeDef 	*rtc;

extern int REGESTRATION;
extern int CONNECTION;
extern int MQTTCONNECTION;
extern int GPSLOCATION;
extern int GSMLOCATION;
extern int BATTERYSAVER;

void Initialize(UART_HandleTypeDef *myuart1,UART_HandleTypeDef *myuart2,UART_HandleTypeDef *myuart3,TIM_HandleTypeDef *mytim2,TIM_HandleTypeDef *mytim5,TIM_HandleTypeDef *mytim3,TIM_HandleTypeDef *mytim4,RTC_HandleTypeDef *myrtc,ADC_HandleTypeDef *adc);
int bootUp();
void modemOn();
int tx(UART_HandleTypeDef *huart,uint8_t* data);
int rx(UART_HandleTypeDef *huart,uint8_t* term,uint16_t timeout);
void gpsWait(int w);
void buzzOn();
void buzzOff();
void rxr(UART_HandleTypeDef *huart,uint8_t* term,uint16_t timeout);
void txr(UART_HandleTypeDef *huart,uint8_t* data);
#endif /* UART_H_ */

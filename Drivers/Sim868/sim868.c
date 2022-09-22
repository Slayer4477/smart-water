/*
 * sim868.c
 *
 *  Created on: Jun 15, 2021
 *      Author: pcbde
 */
#include "sim868.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include "sensor.h"
#include "sleep.h"
#include "clock.h"
uint8_t* SIMR = (uint8_t*)"OK\r\n";
uint8_t* SIMR1 = (uint8_t*)"+CREG: 0,1\r\n";
uint8_t* SIMR2 = (uint8_t*)"READY\r\n";
uint8_t* SIM = (uint8_t*)"AT\r\n";
uint8_t* SIM01 = (uint8_t*)"ATE0\r\n";
uint8_t* SIM02 = (uint8_t*)"AT+CREG=0\r\n";
uint8_t* SIM1 = (uint8_t*)"AT+CREG?\r\n";
uint8_t* SIMN = (uint8_t*)"\r\n";
uint8_t* SIMV = (uint8_t*)",";
uint8_t* SIMVN = (uint8_t*)"\"\r\n";
uint8_t* SIM2 = (uint8_t*)"AT+GSN\r\n";
uint8_t* SIM3 = (uint8_t*)"AT+CCID\r\n";
uint8_t* SIM4 = (uint8_t*)"AT+CMTE=1\r\n";
uint8_t* SIM5 = (uint8_t*)"AT+CMTE?\r\n";
uint8_t* SIM6 = (uint8_t*)"AT+CCLK?\r\n";
uint8_t* SIM7 = (uint8_t*)"AT+CSQ\r\n";
uint8_t* SIM8 = (uint8_t*)"AT+CPIN?\r\n";

void l3On(){
	HAL_GPIO_WritePin(l3_GPIO_Port, l3_Pin, GPIO_PIN_SET);
}
void l2On(){
	HAL_GPIO_WritePin(l2_GPIO_Port, l2_Pin, GPIO_PIN_SET);
}
void l3Off(){
	HAL_GPIO_WritePin(l3_GPIO_Port, l3_Pin, GPIO_PIN_RESET);
}
void l2Off(){
	HAL_GPIO_WritePin(l2_GPIO_Port, l2_Pin, GPIO_PIN_RESET);
}

void ledsOn(){
	HAL_GPIO_WritePin(gnde_GPIO_Port, gnde_Pin, GPIO_PIN_SET);
}
void ledsOff(){
	HAL_GPIO_WritePin(gnde_GPIO_Port, gnde_Pin, GPIO_PIN_RESET);
}
void sim868On(void){
	HAL_GPIO_WritePin(pwrs_GPIO_Port, pwrs_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(l2_GPIO_Port, l2_Pin, GPIO_PIN_SET);
	__HAL_TIM_SET_COUNTER(&uartT,0);
	while(HAL_GPIO_ReadPin(status_GPIO_Port, status_Pin) == GPIO_PIN_RESET){
		if((__HAL_TIM_GET_COUNTER(&uartT) > 10000)){
			NVIC_SystemReset();
		}
	}
	HAL_GPIO_WritePin(pwrs_GPIO_Port, pwrs_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(l2_GPIO_Port, l2_Pin, GPIO_PIN_RESET);
}
void sim868Off(void){
	HAL_GPIO_WritePin(pwrs_GPIO_Port, pwrs_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(l2_GPIO_Port, l2_Pin, GPIO_PIN_SET);
	__HAL_TIM_SET_COUNTER(&uartT,0);
	while(HAL_GPIO_ReadPin(status_GPIO_Port, status_Pin) == GPIO_PIN_SET){
		if((__HAL_TIM_GET_COUNTER(&uartT) > 10000)){
			NVIC_SystemReset();
		}
	}
	HAL_GPIO_WritePin(pwrs_GPIO_Port, pwrs_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(l2_GPIO_Port, l2_Pin, GPIO_PIN_RESET);
}
void sensorOn(void){
	HAL_GPIO_WritePin(onc_GPIO_Port, onc_Pin, GPIO_PIN_SET);//SW1.10
}
void sensorOff(void){
	HAL_GPIO_WritePin(onc_GPIO_Port, onc_Pin, GPIO_PIN_RESET);//SW1.10
}
void loraOn(void){
	HAL_GPIO_WritePin(onb_GPIO_Port, onb_Pin, GPIO_PIN_SET);//SW1.10
}
void loraOff(void){
	HAL_GPIO_WritePin(onb_GPIO_Port, onb_Pin, GPIO_PIN_RESET);//SW1.10
}
int sim868Init(void){
	int s=0;
	while(1){
		tx(uartModem, SIM);
		if(rx(uartModem, SIMR, 1) == UART_OK){
			break;
		}
		if(s == 10){
			return MODEM_ERROR;
		}
		s++;
	}
	getTime();
	txr(uartModem, SIM01);
	rxr(uartModem, SIMR, 1);
	txr(uartModem, SIM02);
	rxr(uartModem, SIMR, 1);
	if(TIME[0] == '0'){
		getMV();
		getIMEI();
		debug(IMEI);
		debug(SIMN);
	}
	HAL_Delay(1000);
	txr(uartModem, SIM8);
	if(rx(uartModem, SIMR2, 2) == UART_TIMEOUT){
		return NO_SIM;
	}
	s = 0;
	while(1){
		txr(uartModem, SIM1);
		if(rx(uartModem, SIMR1, 1) == UART_OK){
			return REGISTERED;
		}
		if(s == 40){
			return NOT_REGISTERED;
		}
		s++;
	}
}
int sim868onPrep(){
	int s=0;
	while(1){
		tx(uartModem, SIM);
		if(rx(uartModem, SIMR, 1) == UART_OK){
			break;
		}
		if(s == 10){
			return MODEM_ERROR;
		}
		s++;
	}
	txr(uartModem, SIM01);
	rxr(uartModem, SIMR, 1);
	txr(uartModem, SIM02);
	rxr(uartModem, SIMR, 1);
	return NOT_REGISTERED;
}
void getIMEI(){
	txr(uartModem, SIM2);
	rxr(uartModem, SIMR, 1);
	strcpy((char*)IMEI,strtok((char*)rx_buf,"\r\n"));
}
void getPNUM(){
	if(REGESTRATION != NO_SIM){
		txr(uartModem, SIM3);
		rxr(uartModem, SIMR, 1);
		strcpy((char*)PNUM,strtok((char*)rx_buf,"\r\n"));
	}
}
void getTemp(){
	txr(uartModem, SIM4);
	rxr(uartModem, SIMR, 5);
	txr(uartModem, SIM5);
	rxr(uartModem, SIMR, 5);
	strtok((char*)rx_buf,",");
	strcpy((char*)TEMP,strtok((char*)NULL,"\r\n"));
}
void getADC(){
	int adcCount;
	long sum=0;
	int batP[11]={3400,3500,3650,3700,3710,3720,3730,3740,3750,3800,4300};
	for(adcCount=0;adcCount<1000;adcCount++){
		HAL_ADC_Start(&adcB);
		HAL_ADC_PollForConversion(&adcB, 1);
		sum+= HAL_ADC_GetValue(&adcB);
		HAL_Delay(1);
	}
	HAL_ADC_Stop(&adcB);
	int b = sum/1000.0;
	b*= 6600;
	b/=	4096;
	adcCount=0;
	while(batP[adcCount++] < b)
	;
	adcCount-=2;
	b = (adcCount*10) +	((b-batP[adcCount])*10)/(batP[adcCount+1]-batP[adcCount]);
	itoa(b,(char*)BAT,10);
}
void getSig(){
	txr(uartModem, SIM7);
	rxr(uartModem, SIMR, 5);
	strtok((char*)rx_buf," ");
	strcpy((char*)SIG,strtok((char*)NULL,"\r\n"));
}
void getPG(){
	pgCounterEnd++;
	if(pgCounterEnd == 1440/minSendPeriod){
		itoa((pgCounter*100)/(1440/minSendPeriod),(char*)PG,10);
		pgCounter = 0;
		pgCounterEnd = 0;
	}
	if(HAL_GPIO_ReadPin(pg_GPIO_Port, pg_Pin) == GPIO_PIN_SET){
		pgCounter ++;
	}
}
void getMV(){
	tx(uartModem, (uint8_t*)"AT+CGMR\r\n");
	rx(uartModem, SIMR, 1);
	strtok((char*)rx_buf,":");
	strcpy((char*)MV,strtok(NULL,"\r\n"));
	debug(MV);
	debug(SIMN);
}

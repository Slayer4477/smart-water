/*
 * sensor.c
 *
 *  Created on: Jun 17, 2021
 *      Author: pcbde
 */


#include "sensor.h"
#include "fs.h"
#include "uart.h"
#include "sleep.h"
#include <stdlib.h>
#include <string.h>
#include "clock.h"
#include "sim868.h"
#include "main.h"
#include "bluetooth.h"
void measureAll(){
	getSath();
	getPNUM();
	getSig();
	getTemp();
	getADC();
	getTime();
	getBTRSII();
}
void setHeight(){
	getSath();
	if(strcmp((char*)SENSORTYPE,"WellVer") == 0){
		sensorOn();
		HAL_Delay(2000);
		tx(uartSensor, (uint8_t*)"SetH");
		tx(uartSensor, SETH);
		rx(uartSensor, (uint8_t*)"OK", 5);
		HAL_Delay(1000);
		sensorOff();
	}
	else{
		HAL_FLASHEx_DATAEEPROM_Unlock();
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080300, distance + atoi((char*)SETH));
		HAL_FLASHEx_DATAEEPROM_Lock();
		HAL_FLASHEx_DATAEEPROM_Unlock();
		HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080300, distance + atoi((char*)SETH));
		HAL_FLASHEx_DATAEEPROM_Lock();
	}
}

void getSath(){
	int di;
	int dif;
	if(strstr((char*)SENSORTYPE,"MB7588")!=NULL){
		GPIO_InitTypeDef gp;
		gp.Pin = GPIO_PIN_10;
		gp.Mode = GPIO_MODE_OUTPUT_PP;
		gp.Pull = GPIO_PULLUP;
		gp.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &gp);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		sensorOn();
		if(rx(uartSensor, (uint8_t*)"TempI\r", 3) == UART_OK){
			rx(uartSensor, (uint8_t*)"\r", 2);
			rx(uartSensor, (uint8_t*)"\r", 2);
			rx(uartSensor, (uint8_t*)"\r", 2);
			rx(uartSensor, (uint8_t*)"\r", 2);

			distanceSum = 0;
			rx(uartSensor, (uint8_t*)"\r", 2);
			da[0] = rx_buf[1];
			da[1] = rx_buf[2];
			da[2] = rx_buf[3];
			da[3] = rx_buf[4];
			da[4] = '\0';
			distanceSum += atoi((char*)da);
			rx(uartSensor, (uint8_t*)"\r", 2);
			da[0] = rx_buf[1];
			da[1] = rx_buf[2];
			da[2] = rx_buf[3];
			da[3] = rx_buf[4];
			da[4] = '\0';
			distanceSum += atoi((char*)da);
			rx(uartSensor, (uint8_t*)"\r", 2);
			da[0] = rx_buf[1];
			da[1] = rx_buf[2];
			da[2] = rx_buf[3];
			da[3] = rx_buf[4];
			da[4] = '\0';
			distanceSum += atoi((char*)da);
			rx(uartSensor, (uint8_t*)"\r", 2);
			da[0] = rx_buf[1];
			da[1] = rx_buf[2];
			da[2] = rx_buf[3];
			da[3] = rx_buf[4];
			da[4] = '\0';
			distanceSum += atoi((char*)da);
			rx(uartSensor, (uint8_t*)"\r", 2);
			da[0] = rx_buf[1];
			da[1] = rx_buf[2];
			da[2] = rx_buf[3];
			da[3] = rx_buf[4];
			da[4] = '\0';
			distanceSum += atoi((char*)da);
			rx(uartSensor, (uint8_t*)"\r", 2);
			da[0] = rx_buf[1];
			da[1] = rx_buf[2];
			da[2] = rx_buf[3];
			da[3] = rx_buf[4];
			da[4] = '\0';
			distanceSum += atoi((char*)da);

			distance = distanceSum/6.0;
			di = *(int*)0x08080300 - distance;
			if(di<0){
				di = 0;
			}
			itoa(di,(char*)SATH,10);
			itoa(distance,(char*)DIST,10);
			di+=atoi((char*)COR)*10;
			if(di < 0){
				strcpy((char*)SARF,"-1000");
			}
			else{
				dif = TABLE[(di/10)+1]-TABLE[di/10];
				itoa(TABLE[di/10]+((di%10)*(dif/10.0)),(char*)SARF,10);
			}
		}
		else{
			itoa(-10,(char*)SATH,10);
			strcpy((char*)SARF,(char*)"-1000");
		}
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		sensorOff();
	}
	if(strstr((char*)SENSORTYPE,"WellVer")!=NULL){
		usart3Init();
		sensorOn();
		HAL_Delay(2000);
		if(strstr((char*)SENSORTYPE,"WellVer2")!=NULL){
			uint8_t ssd[8] = {0x01,0x03,0x00,0x00,0x00,0x06,0xC5,0xC8};
			HAL_UART_Transmit(uartSensor, ssd, 8, 1000);
			rx(uartSensor, (uint8_t*)"\r\n", 2);
			if(rx_buf[0] != '\0'){
				char shor[10];
				uint32_t sh;
				int tens;

				tens = rx_buf[6];
				sh = (rx_buf[3] << 8) + rx_buf[4];
				while(tens < 3){
				  sh*=10;
				  tens++;
				}
				itoa((int)sh,shor,10);
				strcpy((char*)MCON,shor);
			    sh = (rx_buf[7] << 8) + rx_buf[8];
				itoa((int)sh,shor,10);
				strcpy((char*)MTEMP,shor);

				tens = 0;
				sh = (rx_buf[11] << 8) + rx_buf[12];
				while(tens < rx_buf[14]){
				sh*=10;
				  tens++;
				}
				itoa((int)sh,shor,10);
				strcpy((char*)MSATH,shor);
				long int a= atol((char*)MSATH);
				if(a>2000000000){
				  tx(uartDebug, (uint8_t*)"QUDIQQA_TIQING\r\n");
				}
				distance = atoi((char*)MSATH);
				di = *(int*)0x08080300 - atoi((char*)MSATH);
				itoa(di,(char*)MSATH,10);
				if(di < 0){
					strcpy((char*)MSATH,"-1");
				}
			}
			else{
				strcpy((char*)MSATH,"-10");
				strcpy((char*)MCON,"-1000");
				strcpy((char*)MTEMP,"-10");
			}
		}
		if(strstr((char*)SENSORTYPE,"WellVer3")!=NULL){
			//HAL_Delay(1000);//2000
			uint8_t ssd[8] = {0x01,0x03,0x00,0x00,0x00,0x06,0xC5,0xC8};
			HAL_GPIO_WritePin(RE_GPIO_Port, RE_Pin, GPIO_PIN_SET);//10
			HAL_Delay(10);
			HAL_UART_Transmit(uartDebug, ssd, 8, 100);//3
			HAL_Delay(3);
			HAL_GPIO_WritePin(RE_GPIO_Port, RE_Pin, GPIO_PIN_RESET);
			rx_buf[0] = '\0';
			rx(uartDebug, (uint8_t*)"\r\n", 2);//6

			if(rx_buf[0] != '\0'){
				char shor[10];
				uint32_t sh;
				int tens;

				tens = rx_buf[6];
				sh = (rx_buf[3] << 8) + rx_buf[4];
				while(tens < 3){
				  sh*=10;
				  tens++;
				}
				itoa((int)sh,shor,10);
				strcpy((char*)MCON,shor);
			    sh = (rx_buf[7] << 8) + rx_buf[8];
				itoa((int)sh,shor,10);
				strcpy((char*)MTEMP,shor);

				tens = 0;
				sh = (rx_buf[11] << 8) + rx_buf[12];
				while(tens < rx_buf[14]){
				sh*=10;
				  tens++;
				}
				itoa((int)sh,shor,10);
				strcpy((char*)MSATH,shor);
				long int a= atol((char*)MSATH);
				if(a>2000000000){
				  tx(uartDebug, (uint8_t*)"QUDIQQA_TIQING\r\n");
				}
				distance = atoi((char*)MSATH);
				di = *(int*)0x08080300 - atoi((char*)MSATH);
				itoa(di,(char*)MSATH,10);
				if(di < 0){
					strcpy((char*)MSATH,"-1");
				}
			}else{
				strcpy((char*)MSATH,"-10");
				strcpy((char*)MCON,"-1000");
				strcpy((char*)MTEMP,"-10");
			}
		}
		else{
			tx(uartSensor, (uint8_t*)"ALL");
			rx(uartSensor, (uint8_t*)"%$&*", 6);
			if(rx_buf[0] != '\0'){
				strcpy((char*)MSATH,strtok((char*)rx_buf,"-"));
				strcpy((char*)MTEMP,strtok((char*)NULL,"-"));
				strcpy((char*)MCON,strtok((char*)NULL,"OK"));
				long int a= atol((char*)MSATH);
				if(a>2000000000){
					tx(uartDebug, (uint8_t*)"QUDIQQA_TIQING\r\n");
				}
				distance = atoi((char*)MSATH);
				di = atoi((char*)MSATH);
				itoa(di,(char*)MSATH,10);
				if(di < 0){
					strcpy((char*)MSATH,"-1");
				}
			}
			else{
				strcpy((char*)MSATH,"-10");
				strcpy((char*)MCON,"-1000");
				strcpy((char*)MTEMP,"-10");
			}
		}
		sensorOff();
	}
	if(strstr((char*)SENSORTYPE,"A011617")){
		uint8_t ssd2[8] = {0x01,0x03,0x01,0x01,0x00,0x01,0xD4,0x36};
		sensorOn();
		HAL_Delay(3000);
		HAL_GPIO_WritePin(RE_GPIO_Port, RE_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
		HAL_UART_Transmit(uartDebug, ssd2, 8, 100);
		HAL_Delay(10);
		HAL_GPIO_WritePin(RE_GPIO_Port, RE_Pin, GPIO_PIN_RESET);
		rx_buf[0] = '\0';
		rx(uartDebug, (uint8_t*)"&$*&", 3);
		if(rx_buf[0] != '\0'){
			distance = (rx_buf[3] << 8) + rx_buf[4];
			di = *(int*)0x08080300 - distance;
			if(di<0){
				di = 0;
			}
			itoa(di,(char*)SATH,10);
			itoa(distance,(char*)DIST,10);
			di+=atoi((char*)COR)*10;
			if(di < 0){
				strcpy((char*)SARF,"-1000");
			}
			else{
				dif = TABLE[(di/10)+1]-TABLE[di/10];
				itoa(TABLE[di/10]+((di%10)*(dif/10.0)),(char*)SARF,10);
			}
		}
		else{
			itoa(-10,(char*)SATH,10);
			strcpy((char*)SARF,(char*)"-1000");
		}
		sensorOff();
	}
}

void debug(uint8_t* data){
	int f;
	uint8_t bit;
	while(*data != '\0'){
		bit = *data;
		for(f=0;f<9;f++){
			if(f == 0){
				HAL_GPIO_WritePin(debug_GPIO_Port, debug_Pin, GPIO_PIN_RESET);
			}
			else if(f > 7){
				HAL_GPIO_WritePin(debug_GPIO_Port, debug_Pin, GPIO_PIN_SET);
			}
			else{
				if(bit & (1 << (f-1))){
					HAL_GPIO_WritePin(debug_GPIO_Port, debug_Pin, GPIO_PIN_SET);
				}
				else{
					HAL_GPIO_WritePin(debug_GPIO_Port, debug_Pin, GPIO_PIN_RESET);
				}
			}
			delay(97);
		}
		data ++;
	}
}
void delay(int timeout){
	__HAL_TIM_SET_COUNTER(&uart7T,0);
	while(__HAL_TIM_GET_COUNTER(&uart7T) < timeout)
		;
}

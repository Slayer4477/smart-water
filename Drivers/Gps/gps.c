/*
 * gpc.c
 *
 *  Created on: Jun 20, 2021
 *      Author: pcbde
 */

#include "gps.h"
#include "uart.h"
#include "sim868.h"
#include "files.h"
#include "mqtt.h"
#include "fs.h"
#include "memory.h"
#include "clock.h"
#include "gsmloc.h"
#include <string.h>
#include <stdio.h>

uint8_t* GPSR3 = (uint8_t*)"0,200";
uint8_t* GPSR4 = (uint8_t*)"+CGNSAID:";
uint8_t* GPSR5 = (uint8_t*)"+CGNSPWR: 1";
uint8_t* GPS11 = (uint8_t*)"AT+CGNSPWR=1\r\n";
uint8_t* GPS13 = (uint8_t*)"AT+CGNSINF\r\n";

void updateTime(){
	if(GPSLOCATION == LOCKED){
		TIME[0] = LOCK[6];
		TIME[1] = LOCK[7];
		TIME[2] = (uint8_t)'/';
		TIME[3] = LOCK[8];
		TIME[4] = LOCK[9];
		TIME[5] = (uint8_t)'/';
		TIME[6] = LOCK[10];
		TIME[7] = LOCK[11];
		TIME[8] = (uint8_t)',';
		TIME[9] = LOCK[12];
		TIME[10] = LOCK[13];
		TIME[11] = (uint8_t)':';
		TIME[12] = LOCK[14];
		TIME[13] = LOCK[15];
		TIME[14] = (uint8_t)':';
		TIME[15] = LOCK[16];
		TIME[16] = LOCK[17];
		TIME[17] = (uint8_t)'+';
		TIME[18] = (uint8_t)'0';
		TIME[19] = (uint8_t)'0';
		TIME[20] = '\0';
		setTime();
	}
	else if(GSMLOCATION == LOCKED){
		strcpy((char*)rx_buf,(char*)GSMLOCK);
		strtok((char*)rx_buf,",");
		strtok(NULL,",");
		strtok(NULL,",");
		strtok(NULL,",");
		strcpy((char*)TIME,strtok(NULL,"\r\n"));
		setTime();
	}
}
void gpsPowerOn(){
	txr(uartModem, GPS11);
	rx(uartModem, GPSR5, 5);
	tx(uartModem, GPS13);
	HAL_Delay(1000);
}
void NMEA(int times){
	int t=0;
	GPSLOCATION = NOT_LOCKED;
	gpsPowerOn();
	while(t<times){
		tx(uartModem, GPS13);
		rx(uartModem, (uint8_t*)"+CGNSINF: ", 2);
		rx(uartModem, SIMN, 1);
		strcpy((char*)LOCK,(char*)rx_buf);
		uint8_t* p = (uint8_t*)strstr((char*)LOCK,"\r\n");
		LOCK[p - LOCK] = '\0';
		if(LOCK[4] == '2'){
			GPSLOCATION = LOCKED;
		}
		updateTime();
		dataConnect();
		buzzOn();
		getgsmloc();
		buzzOff();
		mqttConnect();
		mqttSend(2);
		t++;
	}
}

/*
 * memory.c
 *
 *  Created on: Jul 14, 2021
 *      Author: pcbde
 */


#include "memory.h"
#include "fs.h"
#include "files.h"
#include "uart.h"
#include "sim868.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void addToLog(){
	if(strstr((char*)SENSORTYPE,"MB7588")!=NULL || strstr((char*)SENSORTYPE,"A011617")!=NULL){
		sprintf((char*)fs_buf,"{\"D\":{\"T\":\"%20s\",\"H\":\"%05d\",\"Q\":\"%09d\",\"C\":\"%03d\",\"S\":\"%1s\"}}\r\n",(char*)TIME,atoi((char*)SATH),atoi((char*)SARF),atoi((char*)COR),STAT);
	}
	if(strstr((char*)SENSORTYPE,"WellVer")!=NULL){
		sprintf((char*)fs_buf,"{\"D\":{\"T\":\"%20s\",\"H\":\"%05d\",\"Q\":\"%09d\",\"C\":\"%03d\",\"S\":\"%1s\"}}\r\n",(char*)TIME,atoi((char*)MSATH),atoi((char*)MCON),atoi((char*)MTEMP),STAT);
	}
	fs_buf[82]='\0';
	if(fsAdd(PATHL, fs_buf, 82) == 0){
		NVIC_SystemReset();
	}
	txr(uartModem, (uint8_t*)"AT+FSFLSIZE=D:\\log.txt\r\n");
	rxr(uartModem, SIMR, 2);
	strtok((char*)rx_buf,": ");
	logPosition = atoi(strtok(NULL,"\r\n"))/82;
	uint8_t sds[10];
	itoa((int)logPosition-1,(char*)sds,10);
}
int readFromLog(long pos){
	itoa(pos*82,(char*)POSST,10);
	tx(uartModem, (uint8_t*)"AT+FSREAD=");
	tx(uartModem, PATHL);
	tx(uartModem, (uint8_t*)",1,82,");
	tx(uartModem, POSST);
	tx(uartModem, (uint8_t*)"\r\n");
	rxr(uartModem, SIMR, 10);
	strcpy((char*)rx_buf,(char*)&rx_buf[2]);
	if(strstr((char*)SENSORTYPE,"MB7588")!=NULL || strstr((char*)SENSORTYPE,"A011617")!=NULL){
		sscanf((char*)rx_buf,"{\"D\":{\"T\":\"%20s\",\"H\":\"%05s\",\"Q\":\"%09s\",\"C\":\"%03s\",\"S\":\"%1s\"}}\r\n",(char*)&TIME,(char*)&SATH,(char*)&SARF,(char*)&COR,(char*)&STAT);
	}
	if(strstr((char*)SENSORTYPE,"WellVer")!=NULL){
		sscanf((char*)rx_buf,"{\"D\":{\"T\":\"%20s\",\"H\":\"%05s\",\"Q\":\"%09s\",\"C\":\"%03s\",\"S\":\"%1s\"}}\r\n",(char*)&TIME,(char*)&MSATH,(char*)&MCON,(char*)&MTEMP,(char*)&STAT);
	}
	return 0;
}

void getPos(){
	sendPosition = *(uint32_t*)0x08080110;
}
void setPos(long position){
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080110, position);
	HAL_FLASHEx_DATAEEPROM_Lock();
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080110, position);
	HAL_FLASHEx_DATAEEPROM_Lock();
	uint8_t sds[10];
	itoa((int)position,(char*)sds,10);
}
void defaulLog(){
	fsCreate(PATHL);
	setPos(0);
}

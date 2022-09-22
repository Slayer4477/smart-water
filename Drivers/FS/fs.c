/*
 * fs.c
 *
 *  Created on: Jun 16, 2021
 *      Author: pcbde
 */
#include "fs.h"
#include "uart.h"
#include "sim868.h"
#include <stdlib.h>
#include <string.h>

uint8_t* FSN = (uint8_t*)">";
uint8_t* FS1 = (uint8_t*)"AT+FSCREATE=";
uint8_t* FS2 = (uint8_t*)"AT+FSREAD=";
uint8_t* FS3 = (uint8_t*)"AT+FSWRITE=";
uint8_t* FSMODE = (uint8_t*)"0";
uint8_t* FSSIZE = (uint8_t*)"10240";
uint8_t* FSPOSITION = (uint8_t*)"0";
uint8_t* FSINPUTTIME = (uint8_t*)"10";

void fsCreate(uint8_t* path){
	txr(uartModem, (uint8_t*)"AT+FSCREATE=");
	txr(uartModem, path);
	txr(uartModem, (uint8_t*)"\r\n");
	rxr(uartModem, SIMR, 5);
}
int fsRead(uint8_t* path){
	int fp = 0;
	uint8_t fp_p[10];
	uint8_t* rxb_p;
	itoa(fp,(char*)fp_p,10);
	strcpy((char*)fs_buf,"");
	txr(uartModem, (uint8_t*)"AT+FSREAD=");
	txr(uartModem, path);
	txr(uartModem, (uint8_t*)",1,50,");
	txr(uartModem, fp_p);
	txr(uartModem, SIMN);
	if(rx(uartModem, SIMR, 1) == UART_TIMEOUT){
		return 0;
	}
	while(strstr((char*)rx_buf,"ERROR") == NULL){
		rxb_p = &rx_buf[2];
		rxb_p[strlen((char*)rxb_p)-6]='\0';
		strcat((char*)fs_buf,(char*)rxb_p);
		fp+=50;
		itoa(fp,(char*)fp_p,10);
		txr(uartModem, (uint8_t*)"AT+FSREAD=");
		txr(uartModem, path);
		txr(uartModem, (uint8_t*)",1,50,");
		txr(uartModem, fp_p);
		txr(uartModem, SIMN);
		rx(uartModem, SIMR, 1);
	}
	return 1;
}
void fsWrite(uint8_t* path){
	fsCreate(path);
	uint8_t fileSizeBuf[5];
	itoa(strlen((char*)fs_buf),(char*)fileSizeBuf,10);
	txr(uartModem, FS3);
	txr(uartModem, path);
	txr(uartModem, SIMV);
	txr(uartModem, FSMODE);
	txr(uartModem, SIMV);
	txr(uartModem, fileSizeBuf);
	txr(uartModem, SIMV);
	txr(uartModem, FSINPUTTIME);
	txr(uartModem, SIMN);
	rxr(uartModem, FSN, 5);
	tx(uartModem, fs_buf);
	rxr(uartModem, SIMR, 5);
}
int fsAdd(uint8_t* path,uint8_t* data,int filesize){
	uint8_t fileSizeBuf[5];
	itoa(filesize,(char*)fileSizeBuf,10);
	txr(uartModem, FS3);
	txr(uartModem, path);
	txr(uartModem, SIMV);
	txr(uartModem, (uint8_t*)"1");
	txr(uartModem, SIMV);
	txr(uartModem, fileSizeBuf);
	txr(uartModem, SIMV);
	txr(uartModem, FSINPUTTIME);
	txr(uartModem, SIMN);
	if(rx(uartModem, FSN, 5) == UART_TIMEOUT){
		return 0;
	}
	tx(uartModem, data);
	if(rx(uartModem, SIMR, 11) == UART_TIMEOUT){
		return 0;
	}
	return 1;
}
long fsFileSize(uint8_t *path){
	uint8_t Size[10];
	txr(uartModem, (uint8_t*)"AT+FSFLSIZE=");
	txr(uartModem, path);
	txr(uartModem, (uint8_t*)"\r\n");
	rxr(uartModem, SIMR, 1);
	strtok((char*)rx_buf," ");
	strcpy((char*)Size,strtok(NULL,"\r\n"));
	return atoi((char*)Size);
}

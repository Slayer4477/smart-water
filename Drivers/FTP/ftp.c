/*
 * ftp.c
 *
 *  Created on: Jun 20, 2021
 *      Author: pcbde
 */

#include "ftp.h"
#include "uart.h"
#include "sim868.h"
#include "fs.h"
#include "files.h"
#include <string.h>
#include <stdlib.h>
#include "memory.h"

uint8_t* FTPR1 = (uint8_t*)"+FTPLIST: 1,1";
uint8_t* FTPR2 = (uint8_t*)"+FTPLIST: 1,0";
uint8_t* FTPR3 = (uint8_t*)"+FTPGETTOFS: 0,";
uint8_t* FTP1 = (uint8_t*)"AT+FTPCID=1\r\n";
uint8_t* FTP2 = (uint8_t*)"AT+FTPSERV=\"";
uint8_t* FTP3 = (uint8_t*)"AT+FTPPORT=\"";
uint8_t* FTP4 = (uint8_t*)"AT+FTPUN=\"";
uint8_t* FTP5 = (uint8_t*)"AT+FTPPW=\"";
uint8_t* FTP6 = (uint8_t*)"\"\r\n";
uint8_t* FTP7 = (uint8_t*)"AT+FTPGETNAME=1K.txt\r\n";
uint8_t* FTP8 = (uint8_t*)"AT+FTPGETPATH=\"";
uint8_t* FTP9 = (uint8_t*)"AT+FTPLIST=1\r\n";
uint8_t* FTP10 = (uint8_t*)"AT+FTPLIST=2,1460\r\n";
uint8_t* FTP11 = (uint8_t*)"AT+FTPGETNAME=\"";
uint8_t* FTP13 = (uint8_t*)"AT+FTPGETTOFS=1,\"config.txt\",3,59\r\n";
uint8_t* FTP14 = (uint8_t*)"AT+FTPGETTOFS=1,\"table.txt\",5,59\r\n";
uint8_t* FTP15 = (uint8_t*)"AT+FTPGETTOFS=1,\"firmware.hex\",30,59\r\n";

int ftpList(){
	if(CONNECTION == CONNECTED){
		txr(uartModem, FTP8);
		txr(uartModem, (uint8_t*)"/SSS2/");
		txr(uartModem, IMEI);
		txr(uartModem, (uint8_t*)"/\"\r\n");
		rxr(uartModem, SIMR, 1);

		txr(uartModem, FTP7);
		rxr(uartModem, SIMR, 1);

		txr(uartModem, FTP9);
		if(rx(uartModem, FTPR1, 50)==UART_TIMEOUT){
			return 0;
		}
		HAL_Delay(1000);
		txr(uartModem, FTP10);
		rxr(uartModem, SIMR, 5);
		strcpy((char*)fs_buf,(char*)rx_buf);
		rx(uartModem, SIMN, 60);
		newConfig = 0;
		newTable = 0;
		newFirmware = 0;
		//tx(uartDebug, (uint8_t*)"FTPLISTED\r\n");
		if(strstr((char*)fs_buf,"config.txt") != NULL){
			//tx(uartDebug, (uint8_t*)"NEW_CONFIG\r\n");
			newConfig=1;
		}
		if(strstr((char*)fs_buf,"table.txt") != NULL){
			//tx(uartDebug, (uint8_t*)"NEW_TABLE\r\n");
			newTable = 1;
		}
		if(strstr((char*)fs_buf,"firmware.hex") != NULL){
			//tx(uartDebug, (uint8_t*)"NEW_FIRMWARE\r\n");
			newFirmware = 1;
		}
		return 1;
	}
	return 0;
}
void ftpSetParam(){
	txr(uartModem, FTP1);
	rxr(uartModem, SIMR, 1);

	txr(uartModem, FTP2);
	txr(uartModem, FTPIP);
	txr(uartModem, FTP6);
	rxr(uartModem, SIMR, 1);

	txr(uartModem, FTP3);
	txr(uartModem, FTPPORT);
	txr(uartModem, FTP6);
	rxr(uartModem, SIMR, 1);

	txr(uartModem, FTP4);
	txr(uartModem, FTPUSERNAME);
	txr(uartModem, FTP6);
	rxr(uartModem, SIMR, 1);

	txr(uartModem, FTP5);
	txr(uartModem, FTPPASSWORD);
	txr(uartModem, FTP6);
	rxr(uartModem, SIMR, 1);
}
void ftpGet(){
	ftpList();
	if(newConfig == 1){
		txr(uartModem, (uint8_t*)"AT+FTPGETNAME=\"config.txt\"\r\n");
		rxr(uartModem, SIMR, 1);
		txr(uartModem, FTP13);
		rxr(uartModem, SIMR, 1);
		if(rx(uartModem, FTPR3, 300) == UART_OK){
			txr(uartModem, (uint8_t*)"AT+FTPDELE\r\n");
			rxr(uartModem, SIMR, 1);
			rx(uartModem, (uint8_t*)"+FTPDELE: 1,0\r\n", 30);
			fsRead((uint8_t*)"D:\\FTP\\config.txt");
			fsWrite(PATHC);
			//tx(uartDebug, (uint8_t*)"CONFIG_UPDATE\r\n");
			updateConfig(1);
		}
	}
	if(newTable == 1){
		txr(uartModem, (uint8_t*)"AT+FTPGETNAME=\"table.txt\"\r\n");
		rxr(uartModem, SIMR, 1);
		txr(uartModem, FTP14);
		rxr(uartModem, SIMR, 1);
		if(rx(uartModem, FTPR3, 400) == UART_OK){
			txr(uartModem, (uint8_t*)"AT+FTPDELE\r\n");
			rxr(uartModem, SIMR, 1);
			rx(uartModem, (uint8_t*)"+FTPDELE: 1,0\r\n", 30);
			fsRead((uint8_t*)"D:\\FTP\\config.txt");
			fsWrite(PATHC);
			//tx(uartDebug, (uint8_t*)"TABLE_UPDATE\r\n");
			updateTable(1);
		}
	}
	if(newFirmware == 1){
		txr(uartModem, (uint8_t*)"AT+FTPGETNAME=\"firmware.hex\"\r\n");
		rxr(uartModem, SIMR, 1);
		txr(uartModem, FTP15);
		rxr(uartModem, SIMR, 1);
		if(rx(uartModem, FTPR3, 2000) == UART_OK){
			txr(uartModem, (uint8_t*)"AT+FTPDELE\r\n");
			rxr(uartModem, SIMR, 1);
			rx(uartModem, (uint8_t*)"+FTPDELE: 1,0\r\n", 30);
			//tx(uartDebug, (uint8_t*)"FIRMWARE_UPDATE\r\n");
			strcpy((char*)fs_buf,"D:\\FTP\\firmware.hex#");
			fsWrite((uint8_t*)"C:\\fw.cfg");
			//tx(uartDebug, (uint8_t*)"JUMPING_BOOTLOADER\r\n");
			sim868Off();
			HAL_FLASHEx_DATAEEPROM_Unlock();
			HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080100, 0xFEEDC0DE);
			HAL_FLASHEx_DATAEEPROM_Lock();
			HAL_Delay(1000);
			HAL_UART_AbortReceive_IT(uartModem);
			HAL_UART_AbortReceive_IT(uartDebug);
			HAL_UART_AbortReceive_IT(uartSensor);
			HAL_UART_DeInit(uartModem);
			HAL_UART_DeInit(uartDebug);
			HAL_UART_DeInit(uartSensor);
			HAL_RCC_DeInit();
			HAL_DeInit();
			HAL_Delay(100);
			NVIC_SystemReset();
		}
	}
}
void ftpDel(){
	tx(uartModem, (uint8_t*)"AT+FTPDELE\r\n");
	rx(uartModem, (uint8_t*)"+FTPDELE: 1,0", 50000);
}
int checkFile(){
	uint8_t tx_num[10];
	tx(uartModem, (uint8_t*)"AT+FSFLSIZE=D:\\FTP\\firmware.hex\r\n");
	rx(uartModem, SIMR, 1000);
	strtok((char*)rx_buf," ");
	itoa(atoi(strtok(NULL,"\r\n"))-18,(char*)tx_num,10);
	tx(uartModem, (uint8_t*)"AT+FSREAD=D:\\FTP\\firmware.hex,1,20,");
	tx(uartModem, tx_num);
	tx(uartModem, (uint8_t*)"\r\n");
	rx(uartModem, SIMR, 1000);
	if(strstr((char*)rx_buf,":00000001FF") != NULL){
		return 1;
	}
	return 0;
}
void removeBoot(){
	if(*(uint32_t*)0x08080100 == 0xFEEDC0DE){
		  HAL_FLASHEx_DATAEEPROM_Unlock();
		  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080100, 0xFEEDC0DD);
		  HAL_FLASHEx_DATAEEPROM_Lock();
		  HAL_FLASHEx_DATAEEPROM_Unlock();
		  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080100, 0xFEEDC0DD);
		  HAL_FLASHEx_DATAEEPROM_Lock();
		  //tx(uartDebug, (uint8_t*)"BOOTLOADER_REMOVED\n");
		  NVIC_SystemReset();
	}
}
void removeFirst(){
	if(*(uint32_t*)0x08080200 != 0xFEEDC0DE){
		  //tx(uartDebug, (uint8_t*)"FIRST_REMOVED\n");
		  tx(uartModem, (uint8_t*)"AT+FSDRIVE=1;&W\r\n");
		  rx(uartModem, SIMR, 2);
		  tx(uartModem, (uint8_t*)"AT+FSMKDIR=D:\\BtReceived\r\n");
		  rx(uartModem, SIMR, 2);
		  defaultParam();
		  writeConfig();
		  defaultTable();
		  writeTable();
		  defaulLog();

          HAL_FLASHEx_DATAEEPROM_Unlock();
		  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080200, 0xFEEDC0DE);
		  HAL_FLASHEx_DATAEEPROM_Lock();
		  HAL_FLASHEx_DATAEEPROM_Unlock();
		  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080200, 0xFEEDC0DE);
		  HAL_FLASHEx_DATAEEPROM_Lock();
	}
}
void addFirst(){
	  HAL_FLASHEx_DATAEEPROM_Unlock();
	  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080200, 0xFEEDC0DD);
	  HAL_FLASHEx_DATAEEPROM_Lock();
}

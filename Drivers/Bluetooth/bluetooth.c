/*
 * bluetooth.c
 *
 *  Created on: Jun 16, 2021
 *      Author: pcbde
 */
#include "bluetooth.h"
#include "sensor.h"
#include "uart.h"
#include "sim868.h"
#include "files.h"
#include "gps.h"
#include "string.h"
#include "fs.h"
#include "stdlib.h"
#include "clock.h"
#include "mqtt.h"
#include "sleep.h"

uint8_t* BLR0 = (uint8_t*)":";
uint8_t* BLR1 = (uint8_t*)"+BTPAIRING:";
uint8_t* BLR2 = (uint8_t*)"+BTCONNECTING:";
uint8_t* BLR3 = (uint8_t*)"+BTSPPDATA:";
uint8_t* BLR4 = (uint8_t*)"\r\n\x1A";
uint8_t* BLR5 = (uint8_t*)"HOME";
uint8_t* BLR6 = (uint8_t*)"SEND OK";
uint8_t* BLR7 = (uint8_t*)"PASSWORD";
uint8_t* BLR8 = (uint8_t*)"+BTOPPPUSH:";
uint8_t* BLR9 = (uint8_t*)"SETTING";
uint8_t* BLR10 = (uint8_t*)"TABLE";
uint8_t* BLR11 = (uint8_t*)"HISTORY";
uint8_t* BLR12 = (uint8_t*)"+BTOPPPUSHING:";
uint8_t* BLR13 = (uint8_t*)"+BTOPPPUSH:";
uint8_t* BLR14 = (uint8_t*)"+BTDISCONN:";
uint8_t* BL0 = (uint8_t*)"AT+BTHOST=";
uint8_t* BL1 = (uint8_t*)"AT+BTPOWER=1\r\n";
uint8_t* BL2 = (uint8_t*)"AT+BTPAIR=1,1\r\n";
uint8_t* BL3 = (uint8_t*)"AT+BTACPT=1\r\n";
uint8_t* BL4 = (uint8_t*)"AT+BTSPPCFG=\"MC\",0\r\n";
uint8_t* BL5 = (uint8_t*)"AT+BTSPPSEND\r\n";
uint8_t* BL6 = (uint8_t*)"AT+BTSTATUS?\r\n";
uint8_t* BL7 = (uint8_t*)"AT+BTOPPPUSH=";
uint8_t* BL8 = (uint8_t*)"AT+BTOPPACPT=1,1\r\n";
uint8_t* BL9 = (uint8_t*)"END#\r\n";
uint8_t* BL10 = (uint8_t*)"AT+BTSCAN=1,10\r\n";

int rx_c = 0;

void BTPowerOn(){
	getIMEI();
	txr(uartModem, BL0);
	txr(uartModem, IMEI);
	txr(uartModem, (uint8_t*)"\r\n");
	rxr(uartModem, SIMR, 1);
	txr(uartModem, BL1);
	rxr(uartModem, SIMR, 5);
	//tx(uartDebug, (uint8_t*)"BT_POWER_ON\r\n");
}

void BTPowerOff(){
	txr(uartModem, (uint8_t*)"AT+BTPOWER=0\r\n");
	rxr(uartModem, SIMR, 5);
	//tx(uartDebug, (uint8_t*)"BT_POWER_OFF\r\n");
}

void bluetoothMode(int timeout){
	int bt_timeout=0;
	int connected = 0;
	uint8_t* oldFileName;
	uint8_t oldFileNameFirmware[100];
	sim868Off();
	HAL_Delay(1000);
	sim868On();
	REGESTRATION = sim868Init();
	BTPowerOn();
	while(bt_timeout < timeout || connected == 1){
		tx(uartModem, (uint8_t*)"AT+BTSTATUS?\r\n");
		rx(uartModem, SIMR, 2);
		if(strstr((char*)rx_buf,"SPP\"\r\n") == NULL){
			//tx(uartDebug, (uint8_t*)"BT_DISCONNECTED\r\n");
			if(connected == 1){
				connected = 0;
				bt_timeout = 0;
			}
		}
		else{
			connected = 1;
		}
		if(bt_timeout++%6 == 0 && connected == 0){
			buzzOn();
		}
		rx(uartModem, (uint8_t*)"#$%", 1);
		buzzOff();
		if(strstr((char*)rx_buf,(char*)BLR1) != NULL){
			tx(uartModem, BL2);
			rx(uartModem, SIMR, 10);
			//tx(uartDebug, (uint8_t*)"BT_PAIRED\r\n");
		}

		if(strstr((char*)rx_buf,(char*)BLR2) != NULL){
			tx(uartModem, BL3);
			rx(uartModem, SIMR, 10);
		}

		if(strstr((char*)rx_buf,(char*)BLR12) != NULL && strstr((char*)rx_buf,"config") != NULL){
			strtok((char*)rx_buf,"\"");
			strtok((char*)NULL,"\"");
			strtok((char*)NULL,"\"");
			oldFileName = (uint8_t*)strtok(NULL,"\"");
			tx(uartModem, BL8);
			rx(uartModem, BLR13, 160);
			if(strcmp((char*)oldFileName,(char*)"config.txt") != 0){
				txr(uartModem, (uint8_t*)"AT+FSDEL=D:\\BtReceived\\config.txt");
				txr(uartModem, SIMN);
				rxr(uartModem, SIMR, 5);
				txr(uartModem, (uint8_t*)"AT+FSRENAME=D:\\BtReceived\\");
				txr(uartModem, oldFileName);
				txr(uartModem, (uint8_t*)",D:\\BtReceived\\config.txt");
				txr(uartModem, SIMN);
				rxr(uartModem, SIMR, 1);
			}
			updateConfig(0);
			getSleepParametrs();
		}

		if(strstr((char*)rx_buf,(char*)BLR12) != NULL && strstr((char*)rx_buf,"table") != NULL){
			strtok((char*)rx_buf,"\"");
			strtok((char*)NULL,"\"");
			strtok((char*)NULL,"\"");
			oldFileName = (uint8_t*)strtok(NULL,"\"");
			tx(uartModem, BL8);
			rx(uartModem, BLR13, 160);
			if(strcmp((char*)oldFileName,(char*)"table.txt") != 0){
				txr(uartModem, (uint8_t*)"AT+FSDEL=D:\\BtReceived\\table.txt");
				txr(uartModem, SIMN);
				rxr(uartModem, SIMR, 5);
				txr(uartModem, (uint8_t*)"AT+FSRENAME=D:\\BtReceived\\");
				txr(uartModem, oldFileName);
				txr(uartModem, (uint8_t*)",D:\\BtReceived\\table.txt");
				txr(uartModem, SIMN);
				rxr(uartModem, SIMR, 5);
			}
			updateTable(0);
		}

		if(strstr((char*)rx_buf,(char*)BLR12) != NULL && strstr((char*)rx_buf,"log") != NULL){
			strtok((char*)rx_buf,"\"");
			strtok((char*)NULL,"\"");
			strtok((char*)NULL,"\"");
			oldFileName = (uint8_t*)strtok(NULL,"\"");
			tx(uartModem, BL8);
			rx(uartModem, BLR13, 160);
		}

		if(strstr((char*)rx_buf,(char*)BLR12) != NULL && strstr((char*)rx_buf,".hex") != NULL){
			strtok((char*)rx_buf,"\"");
			strtok((char*)NULL,"\"");
			strtok((char*)NULL,"\"");
			oldFileName = (uint8_t*)strtok(NULL,"\"");
			tx(uartModem, BL8);
			rx(uartModem, BLR13, 160);

			//tx(uartDebug, (uint8_t*)"FIRMWARE_UPDATE\r\n");
			strcpy((char*)oldFileNameFirmware,"D:\\BtReceived\\");
			strcat((char*)oldFileNameFirmware,(char*)oldFileName);
			strcat((char*)oldFileNameFirmware,"#");
			strcpy((char*)fs_buf,(char*)oldFileNameFirmware);
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


		if(strstr((char*)rx_buf,(char*)BLR3) != NULL){
			if(strstr((char*)rx_buf,(char*)BLR7) != NULL){
				if(1){//strstr((char*)rx_buf,(char*)DEVICEPASSWORD) != NULL
					txr(uartModem, BL5);
					rx(uartModem, (uint8_t*)"> ", 1);
					txr(uartModem, (uint8_t*)"AOK#");
					txr(uartModem, BLR4);
					rx(uartModem, SIMR, 2);
				}
				else{
					txr(uartModem, BL5);
					rx(uartModem, (uint8_t*)"> ", 1);
					txr(uartModem, (uint8_t*)"AERROR#");
					txr(uartModem, BLR4);
					rx(uartModem, SIMR, 2);
				}
			}
			if(strstr((char*)rx_buf,(char*)BLR5) != NULL){
				HAL_Delay(2000);
				measureAll();
				sendSpp();
			}
			if(strstr((char*)rx_buf,(char*)BLR9) != NULL){//setting
				updateConnectedDeviceID();
				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);

				txr(uartModem, BL7);
				txr(uartModem, PDID);
				txr(uartModem, (uint8_t*)",");
				txr(uartModem, PATHC);
				txr(uartModem, (uint8_t*)"\r\n");
				rx(uartModem, BLR8, 160);

				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);
			}

			if(strstr((char*)rx_buf,(char*)BLR10) != NULL){//table
				updateConnectedDeviceID();
				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);

				txr(uartModem, BL7);
				txr(uartModem, PDID);
				txr(uartModem, (uint8_t*)",");
				txr(uartModem, PATHT);
				txr(uartModem, (uint8_t*)"\r\n");
				rx(uartModem, BLR8, 160);

				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);
			}

			if(strstr((char*)rx_buf,(char*)BLR11) != NULL){//log
				updateConnectedDeviceID();
				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);

				txr(uartModem, BL7);
				txr(uartModem, PDID);
				txr(uartModem, (uint8_t*)",");
				txr(uartModem, PATHL);
				txr(uartModem, (uint8_t*)"\r\n");
				rx(uartModem, BLR8, 300);

				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);
			}
			if(strstr((char*)rx_buf,"CALL=") != NULL){
				uint8_t callNum[15];
				strtok((char*)rx_buf,"=");
				strcpy((char*)callNum,strtok(NULL,"#"));
				call(callNum);

				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);
			}
			if(strstr((char*)rx_buf,"SETH=") != NULL){
				strtok((char*)rx_buf,"=");
				strcpy((char*)SETH,strtok(NULL,"#"));
				setHeight();

				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);
			}
			if(strstr((char*)rx_buf,"INFO=") != NULL){
				uint8_t BNMEA[10];
				strtok((char*)rx_buf,"=");
				strcpy((char*)BNMEA,strtok(NULL,"#"));
				NMEA(atoi((char*)BNMEA));
				mqttDisconnect();
				dataDisconnect();

				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, LOCK);
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);

				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);
			}
			if(strstr((char*)rx_buf,"SENDDATA") != NULL){
				measureAll();
				dataConnect();
				mqttConnect();
				mqttSend(1);
				mqttDisconnect();
				dataDisconnect();

				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);
			}
			if(strstr((char*)rx_buf,"SENSORON") != NULL){
				sensorOn();
				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);
			}
			if(strstr((char*)rx_buf,"SENSOROFF") != NULL){
				sensorOff();
				txr(uartModem, BL5);
				rx(uartModem, (uint8_t*)"> ", 1);
				txr(uartModem, (uint8_t*)"AOK#");
				txr(uartModem, BLR4);
				rx(uartModem, SIMR, 2);
			}
		}
	}
}

void sendSpp(){
	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ASATH=");
	txr(uartModem, SATH);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ADIST=");
	txr(uartModem, DIST);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ASARF=");
	txr(uartModem, SARF);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ACORRECTION=");
	txr(uartModem, COR);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ALOCK=");
	txr(uartModem, LOCK);
	txr(uartModem, (uint8_t*)"GLOCK=");
	txr(uartModem, GSMLOCK);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ATEMP=");
	txr(uartModem, TEMP);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ABAT=");
	txr(uartModem, BAT);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ASIG=");
	txr(uartModem, SIG);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"AMSATH=");
	txr(uartModem, MSATH);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"AMCON=");
	txr(uartModem, MCON);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"AMTEMP=");
	txr(uartModem, MTEMP);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"APG=");
	txr(uartModem, PG);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ANAME1=");
	txr(uartModem, NAME1);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ANAME2=");
	txr(uartModem, NAME2);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ANAME3=");
	txr(uartModem, NAME3);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ATIME=");
	txr(uartModem, TIME);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"APNUM=");
	txr(uartModem, PNUM);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"AIMEI=");
	txr(uartModem, IMEI);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"AID=");
	txr(uartModem, ID);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"AFV=");
	txr(uartModem, FW);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ABV=");
	txr(uartModem, BW);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"AMV=");
	MV[0] = '1';
	txr(uartModem, MV);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ABTRSSI=");
	txr(uartModem, BTRSSI);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"ASESNORTYPE=");
	txr(uartModem, SENSORTYPE);
	txr(uartModem, (uint8_t*)"#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);

	txr(uartModem, BL5);
	rx(uartModem, (uint8_t*)"> ", 1);
	txr(uartModem, (uint8_t*)"END#");
	txr(uartModem, BLR4);
	rx(uartModem, SIMR, 4);
	HAL_Delay(50);
}

int updateConnectedDeviceID(){
	txr(uartModem, BL6);
	rxr(uartModem, SIMR, 1);
	uint8_t btdeviceAdress[30];
	uint8_t* btConnections = (uint8_t*)strtok((char*)rx_buf,"\r\n");
	while(btConnections != NULL){
		if(strstr((char*)btConnections,"SPP") != NULL){
			strtok((char*)btConnections," ");
			strcpy((char*)CDID,strtok(NULL,","));
			strtok(NULL,"\",");
			strcpy((char*)btdeviceAdress,strtok(NULL,","));
			txr(uartModem, BL6);
			rxr(uartModem, SIMR, 1);
			btConnections = (uint8_t*)strtok((char*)rx_buf,"\r\n");
			while(btConnections != NULL){
				if(strstr((char*)btConnections,(char*)btdeviceAdress) !=  NULL && strstr((char*)btConnections,"P: ") !=  NULL){
					strtok((char*)btConnections," ");
					strcpy((char*)PDID,strtok(NULL,","));
					strtok(NULL,"\",");
					return 1;
				}
				btConnections = (uint8_t*)strtok(NULL,"\r\n");
			}
		}
		btConnections = (uint8_t*)strtok(NULL,"\r\n");
	}
	return 0;
}

void getBTRSII(){
	updateConnectedDeviceID();
	txr(uartModem, (uint8_t*)"AT+BTRSSI=");
	txr(uartModem, CDID);
	txr(uartModem, SIMN);
	rx(uartModem, SIMR, 2);
	strtok((char*)rx_buf," ");
	strcpy((char*)BTRSSI,strtok(NULL,"\r\n"));
}

void call(uint8_t* num){
	txr(uartModem, (uint8_t*)"ATD");
	txr(uartModem, num);
	txr(uartModem, (uint8_t*)";");
	txr(uartModem, SIMN);
	rx(uartModem, SIMR, 5);
	rx(uartModem, SIMN, 20);
}

/*
 * mqtt.c
 *
 *  Created on: Jun 19, 2021
 *      Author: pcbde
 */

#include "mqtt.h"
#include "uart.h"
#include "sim868.h"
#include <string.h>
#include <stdio.h>
#include "files.h"
#include "clock.h"
#include "bluetooth.h"
#include "ftp.h"
#include <stdlib.h>
#include "gps.h"
#include "sleep.h"
#include "memory.h"
#include "sensor.h"
uint8_t* SMR1 = (uint8_t*)",0";
uint8_t* SM1 = (uint8_t*)"AT+SAPBR=3,1,\"Contype\",\"";
uint8_t* SM2 = (uint8_t*)"AT+SAPBR=3,1,\"APN\",\"";
uint8_t* SM21 = (uint8_t*)"AT+SAPBR=3,1,\"USER\",\"";
uint8_t* SM22 = (uint8_t*)"AT+SAPBR=3,1,\"PWD\",\"";
uint8_t* SM23 = (uint8_t*)"AT+SAPBR=3,1,\"PHONENUM\",\"";
uint8_t* SM24 = (uint8_t*)"AT+SAPBR=3,1,\"RATE\",\"";
uint8_t* SM3 = (uint8_t*)"AT+SAPBR=1,1\r\n";
uint8_t* SM4 = (uint8_t*)"AT+SAPBR=2,1\r\n";
uint8_t* SM6 = (uint8_t*)"AT+SAPBR=0,1\r\n";
uint8_t* SM7 = (uint8_t*)"AT+SMCONF=\"URL\",\"";
uint8_t* SM8 = (uint8_t*)"\"\r\n";
uint8_t* SM9 = (uint8_t*)"AT+SMCONF=\"CLIENTID\",\"";
uint8_t* SM10 = (uint8_t*)"AT+SMCONF=\"USERNAME\",\"";
uint8_t* SM11 = (uint8_t*)"AT+SMCONF=\"PASSWORD\",\"";
uint8_t* SM111 = (uint8_t*)"AT+SMCONF=\"KEEPALIVE\",\"";
uint8_t* SM112 = (uint8_t*)"AT+SMCONF=\"CLEANSS\",\"";
uint8_t* SM113 = (uint8_t*)"AT+SMCONF=\"TIMEOUT\",\"";
uint8_t* SM12 = (uint8_t*)"AT+SMCONN\r\n";
uint8_t* SM13 = (uint8_t*)"AT+SMPUB=\"";
uint8_t* SM14 = (uint8_t*)"AT+SMPUB=\"";
uint8_t* SM15 = (uint8_t*)"\"";
uint8_t* SM16 = (uint8_t*)",";


void dataConnect(){
	if(REGESTRATION == REGISTERED){
		tx(uartModem, SM4);
		rx(uartModem, SIMR, 1);
		if(strstr((char*)rx_buf,"0.0.0.0") == NULL){
			CONNECTION = CONNECTED;
		}
		else{
			CONNECTION = NOT_CONNECTED;
		}
		if(REGESTRATION == REGISTERED){
			if(CONNECTION == NOT_CONNECTED && BATTERYSAVER == BATTERYSAVER_OFF){
				txr(uartModem, SM1);
				txr(uartModem, APNCONTYPE);
				txr(uartModem, SIMVN);
				rxr(uartModem, SIMR, 1);

				txr(uartModem, SM2);
				txr(uartModem, APNNAME);
				txr(uartModem, SIMVN);
				rxr(uartModem, SIMR, 1);

				txr(uartModem, SM21);
				txr(uartModem, APNUSERNAME);
				txr(uartModem, SIMVN);
				rxr(uartModem, SIMR, 1);

				txr(uartModem, SM22);
				txr(uartModem, APNPASSWORD);
				txr(uartModem, SIMVN);
				rxr(uartModem, SIMR, 1);

				txr(uartModem, SM23);
				txr(uartModem, APNPHONENUM);
				txr(uartModem, SIMVN);
				rxr(uartModem, SIMR, 1);

				txr(uartModem, SM24);
				txr(uartModem, (uint8_t*)"2");
				txr(uartModem, SIMVN);
				rxr(uartModem, SIMR, 1);

				txr(uartModem, SM3);
				rx(uartModem, SIMR, 20);
				tx(uartModem, SM4);
				rx(uartModem, SIMR, 1);
				if(strstr((char*)rx_buf,"0.0.0.0") == NULL){
					CONNECTION = CONNECTED;
				}
				else{
					CONNECTION = NOT_CONNECTED;
					if(savebattery < 1440/minSendPeriod){
						savebattery ++;
					}
					else{
						BATTERYSAVER = BATTERYSAVER_ON;
					}
				}
			}
			HAL_Delay(100);
		}
	}
}

void dataDisconnect(){
	if(REGESTRATION == REGISTERED){
		tx(uartModem, SM4);
		rx(uartModem, SIMR, 1);
		if(strstr((char*)rx_buf,"0.0.0.0") == NULL){
			CONNECTION = CONNECTED;
			//tx(uartDebug, (uint8_t*)"CONNECTED\r\n");
		}
		else{
			CONNECTION = NOT_CONNECTED;
		}
		if(CONNECTION == CONNECTED){
			txr(uartModem, SM6);
			if(rx(uartModem, SIMN, 40) == UART_OK){
				CONNECTION = NOT_CONNECTED;
			}
			HAL_Delay(100);
		}
	}
}

void mqttConnect(){
	if(REGESTRATION == REGISTERED){
		tx(uartModem, (uint8_t*)"AT+SMSTATE?\r\n");
		rx(uartModem, SIMR, 1);
		if(strstr((char*)rx_buf,"+SMSTATE: 1") != NULL){
			MQTTCONNECTION = CONNECTED;
		}
		else{
			MQTTCONNECTION = NOT_CONNECTED;
		}
		if(CONNECTION == CONNECTED){
			if(MQTTCONNECTION == NOT_CONNECTED && BATTERYSAVER == BATTERYSAVER_OFF){
				txr(uartModem, SM7);
				txr(uartModem, MQTTIP);
				txr(uartModem, (uint8_t*)":");
				txr(uartModem, MQTTPORT);
				txr(uartModem, SM8);
				rxr(uartModem, SIMR, 1);


				txr(uartModem, SM9);
				txr(uartModem, IMEI);
				txr(uartModem, TIME);
				txr(uartModem, SM8);
				rxr(uartModem, SIMR, 1);


				txr(uartModem, SM10);
				txr(uartModem, MQTTUSERNAME);
				txr(uartModem, SM8);
				rxr(uartModem, SIMR, 1);


				txr(uartModem, SM11);
				txr(uartModem, MQTTPASSWORD);
				txr(uartModem, SM8);
				rxr(uartModem, SIMR, 1);


				txr(uartModem, SM111);
				txr(uartModem, (uint8_t*)"100");
				txr(uartModem, SM8);
				rxr(uartModem, SIMR, 1);


				txr(uartModem, SM112);
				txr(uartModem, (uint8_t*)"0");
				txr(uartModem, SM8);
				rxr(uartModem, SIMR, 1);

				txr(uartModem, SM12);
				rx(uartModem, SIMN, 20);
				tx(uartModem, (uint8_t*)"AT+SMSTATE?\r\n");
				rx(uartModem, SIMR, 1);
				if(strstr((char*)rx_buf,"+SMSTATE: 1") != NULL){
					MQTTCONNECTION = CONNECTED;
				}
				else{
					MQTTCONNECTION = NOT_CONNECTED;
					if(savebattery < 1440/minSendPeriod){
						savebattery ++;
					}
					else{
						BATTERYSAVER = BATTERYSAVER_ON;
					}
				}
			}
			HAL_Delay(100);
		}
	}
}

void mqttDisconnect(){
	if(REGESTRATION == REGISTERED){
		tx(uartModem, (uint8_t*)"AT+SMSTATE?\r\n");
		rx(uartModem, SIMR, 1);
		if(strstr((char*)rx_buf,"+SMSTATE: 1") != NULL){
			MQTTCONNECTION = CONNECTED;
		}
		else{
			MQTTCONNECTION = NOT_CONNECTED;
		}
	if(MQTTCONNECTION == CONNECTED){
		txr(uartModem, (uint8_t*)"AT+SMDISC\r\n");
		rx(uartModem, SIMN, 40);

		txr(uartModem, (uint8_t*)"AT+SMSTATE?\r\n");
		rx(uartModem, SIMR, 2);
		if(strstr((char*)rx_buf,"+SMSTATE: 1") != NULL){
			MQTTCONNECTION = CONNECTED;
		}
		else{
			MQTTCONNECTION = NOT_CONNECTED;
		}
		HAL_Delay(100);
		}
	}
}

int mqttSend(int type){
	if(MQTTCONNECTION == CONNECTED){
		if(type == 1){
			if(strstr((char*)SENSORTYPE,"MB7588") != NULL || strstr((char*)SENSORTYPE,"A011617")!=NULL){
				txr(uartModem, SM13);
				txr(uartModem, (uint8_t*)"W/");
				txr(uartModem, NAME1);
				tx(uartModem, (uint8_t*)"/");
				txr(uartModem, NAME2);
				txr(uartModem, (uint8_t*)"/");
				txr(uartModem, IMEI);
				txr(uartModem, (uint8_t*)"/data\",1,1,\"{\\\"i\\\":\\\"");
				txr(uartModem, IMEI);
				txr(uartModem, (uint8_t*)"\\\",\\\"t\\\":\\\"");
				txr(uartModem, TIME);
				txr(uartModem, (uint8_t*)"\\\",\\\"d\\\":\\\"");
				txr(uartModem, SATH);
				txr(uartModem, (uint8_t*)"\\\",\\\"v\\\":\\\"");
				txr(uartModem, SARF);
				txr(uartModem, (uint8_t*)"\\\",\\\"c\\\":\\\"");
				txr(uartModem, COR);
				txr(uartModem, (uint8_t*)"\\\"}\"\r\n");
			}
			else if(strstr((char*)SENSORTYPE,"SWellVer") != NULL){
				txr(uartModem, SM13);
				txr(uartModem, (uint8_t*)"S/");
				txr(uartModem, NAME1);
				txr(uartModem, (uint8_t*)"/");
				txr(uartModem, NAME2);
				txr(uartModem, (uint8_t*)"/");
				txr(uartModem, IMEI);
				txr(uartModem, (uint8_t*)"/data\",1,1,\"{\\\"i\\\":\\\"");
				txr(uartModem, IMEI);
				txr(uartModem, (uint8_t*)"\\\",\\\"t\\\":\\\"");
				txr(uartModem, TIME);
				txr(uartModem, (uint8_t*)"\\\",\\\"d\\\":\\\"");
				txr(uartModem, MSATH);
				txr(uartModem, (uint8_t*)"\\\",\\\"r\\\":\\\"");
				txr(uartModem, MCON);
				txr(uartModem, (uint8_t*)"\\\",\\\"q\\\":\\\"");
				txr(uartModem, MTEMP);
				txr(uartModem, (uint8_t*)"\\\"}\"\r\n");
			}
			else if(strstr((char*)SENSORTYPE,"WellVer") != NULL){
				txr(uartModem, SM13);
				txr(uartModem, (uint8_t*)"M/");
				txr(uartModem, NAME1);
				txr(uartModem, (uint8_t*)"/");
				txr(uartModem, NAME2);
				txr(uartModem, (uint8_t*)"/");
				txr(uartModem, IMEI);
				txr(uartModem, (uint8_t*)"/data\",1,1,\"{\\\"i\\\":\\\"");
				txr(uartModem, IMEI);
				txr(uartModem, (uint8_t*)"\\\",\\\"t\\\":\\\"");
				txr(uartModem, TIME);
				txr(uartModem, (uint8_t*)"\\\",\\\"d\\\":\\\"");
				txr(uartModem, MSATH);
				txr(uartModem, (uint8_t*)"\\\",\\\"r\\\":\\\"");
				txr(uartModem, MCON);
				txr(uartModem, (uint8_t*)"\\\",\\\"q\\\":\\\"");
				txr(uartModem, MTEMP);
				txr(uartModem, (uint8_t*)"\\\"}\"\r\n");
			}
			rx(uartModem, SIMR, 40);
			rx(uartModem, SIMN, 1);
			rx(uartModem, SIMN, 1);
			if(strstr((char*)rx_buf,",0") != NULL){
				sendPosition++;
				savebattery = 0;
				return MQTTSEND_OK;
			}
			else{
				return MQTTSEND_ERROR;
			}
		}
		if(type == 2){
			tx(uartModem, SM13);
			if(strstr((char*)SENSORTYPE,"MB7588") != NULL || strstr((char*)SENSORTYPE,"A011617")!=NULL){
				tx(uartModem, (uint8_t*)"W/");
			}
			else if(strstr((char*)SENSORTYPE,"SWellVer") != NULL){
				tx(uartModem, (uint8_t*)"S/");
			}
			else if(strstr((char*)SENSORTYPE,"WellVer") != NULL){
				tx(uartModem, (uint8_t*)"M/");
			}
			tx(uartModem, NAME1);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, NAME2);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, IMEI);
			tx(uartModem, (uint8_t*)"/info\",1,1,\"{\\\"i\\\":\\\"");
			tx(uartModem, IMEI);
			tx(uartModem, (uint8_t*)"\\\",\\\"t\\\":\\\"");
			tx(uartModem, TIME);
			tx(uartModem, (uint8_t*)"\\\",\\\"p1\\\":\\\"");
			tx(uartModem, NAME1);
			tx(uartModem, (uint8_t*)"\\\",\\\"p2\\\":\\\"");
			tx(uartModem, NAME2);
			tx(uartModem, (uint8_t*)"\\\",\\\"p3\\\":\\\"");
			tx(uartModem, NAME3);
			tx(uartModem, (uint8_t*)"\\\",\\\"p4\\\":\\\"");
			tx(uartModem, PNUM);
			tx(uartModem, (uint8_t*)"\\\",\\\"p5\\\":\\\"");
			tx(uartModem, UPNUM);
			tx(uartModem, (uint8_t*)"\\\",\\\"p6\\\":\\\"");
			tx(uartModem, LOCK);
			tx(uartModem, (uint8_t*)"GLOCK=");
			tx(uartModem, GSMLOCK);
			tx(uartModem, (uint8_t*)"\\\",\\\"p7\\\":\\\"");
			tx(uartModem, TEMP);
			tx(uartModem, (uint8_t*)"\\\",\\\"p8\\\":\\\"");
			tx(uartModem, BAT);
			tx(uartModem, (uint8_t*)"\\\",\\\"p9\\\":\\\"");
			tx(uartModem, SIG);
			tx(uartModem, (uint8_t*)"\\\",\\\"p10\\\":\\\"");
			tx(uartModem, FW);
			tx(uartModem, (uint8_t*)"\\\",\\\"p11\\\":\\\"");
			tx(uartModem, BW);
			tx(uartModem, (uint8_t*)"\\\",\\\"p12\\\":\\\"");
			tx(uartModem, MINDISTANCE);
			tx(uartModem, (uint8_t*)"\\\",\\\"p13\\\":\\\"");
			tx(uartModem, MINMEASUREPERIOD);
			tx(uartModem, (uint8_t*)"\\\",\\\"p14\\\":\\\"");
			tx(uartModem, MINSENDPERIOD);
			tx(uartModem, (uint8_t*)"\\\",\\\"p15\\\":\\\"");
			tx(uartModem, FTPINTERVAL);
			tx(uartModem, (uint8_t*)"\\\",\\\"p16\\\":\\\"");
			tx(uartModem, ID);
			tx(uartModem, (uint8_t*)"\\\",\\\"p17\\\":\\\"");
			tx(uartModem, SENSORTYPE);
			tx(uartModem, (uint8_t*)"\\\"}\"\r\n");
			rx(uartModem, SIMR, 40);
			rx(uartModem, SIMN, 1);
			rx(uartModem, SIMN, 1);
			if(strstr((char*)rx_buf,",0") != NULL){
				savebattery = 0;
				BATTERYSAVER = BATTERYSAVER_OFF;
				return MQTTSEND_OK;
			}
			else{
				return MQTTSEND_ERROR;
			}
		}
		if(type == 3){//move first
			tx(uartModem, SM13);
			if(strstr((char*)SENSORTYPE,"MB7588") != NULL || strstr((char*)SENSORTYPE,"A011617")!=NULL){
				tx(uartModem, (uint8_t*)"W/");
			}
			else if(strstr((char*)SENSORTYPE,"SWellVer") != NULL){
				tx(uartModem, (uint8_t*)"S/");
			}
			else if(strstr((char*)SENSORTYPE,"WellVer") != NULL){
				tx(uartModem, (uint8_t*)"M/");
			}
			tx(uartModem, NAME1);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, NAME2);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, IMEI);
			tx(uartModem, (uint8_t*)"/alert1\",1,1,\"{\\\"i\\\":\\\"");
			tx(uartModem, IMEI);
			tx(uartModem, (uint8_t*)"\\\",\\\"t\\\":\\\"");
			tx(uartModem, TIME);
			tx(uartModem, (uint8_t*)"\\\",\\\"p1\\\":\\\"");
			tx(uartModem, NAME1);
			tx(uartModem, (uint8_t*)"\\\",\\\"p2\\\":\\\"");
			tx(uartModem, NAME2);
			tx(uartModem, (uint8_t*)"\\\",\\\"p3\\\":\\\"");
			tx(uartModem, NAME3);
			tx(uartModem, (uint8_t*)"\\\",\\\"p4\\\":\\\"");
			tx(uartModem, PNUM);
			tx(uartModem, (uint8_t*)"\\\",\\\"p5\\\":\\\"");
			tx(uartModem, UPNUM);
			tx(uartModem, (uint8_t*)"\\\",\\\"p6\\\":\\\"");
			tx(uartModem, BAT);
			tx(uartModem, (uint8_t*)"\\\",\\\"p7\\\":\\\"");
			tx(uartModem, (uint8_t*)"MoveFirst");
			tx(uartModem, (uint8_t*)"\\\"}\"\r\n");
			rx(uartModem, SIMR, 40);
			rx(uartModem, SIMN, 1);
			rx(uartModem, SIMN, 1);
			if(strstr((char*)rx_buf,",0") != NULL){
				return MQTTSEND_OK;
			}
			else{
				return MQTTSEND_ERROR;
			}
		}
		if(type == 4){//move second
			tx(uartModem, SM13);
			if(strstr((char*)SENSORTYPE,"MB7588") != NULL || strstr((char*)SENSORTYPE,"A011617")!=NULL){
				tx(uartModem, (uint8_t*)"W/");
			}
			else if(strstr((char*)SENSORTYPE,"SWellVer") != NULL){
				tx(uartModem, (uint8_t*)"S/");
			}
			else if(strstr((char*)SENSORTYPE,"WellVer") != NULL){
				tx(uartModem, (uint8_t*)"M/");
			}
			tx(uartModem, NAME1);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, NAME2);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, IMEI);
			tx(uartModem, (uint8_t*)"/alert2\",1,1,\"{\\\"i\\\":\\\"");
			tx(uartModem, IMEI);
			tx(uartModem, (uint8_t*)"\\\",\\\"t\\\":\\\"");
			tx(uartModem, TIME);
			tx(uartModem, (uint8_t*)"\\\",\\\"p1\\\":\\\"");
			tx(uartModem, NAME1);
			tx(uartModem, (uint8_t*)"\\\",\\\"p2\\\":\\\"");
			tx(uartModem, NAME2);
			tx(uartModem, (uint8_t*)"\\\",\\\"p3\\\":\\\"");
			tx(uartModem, NAME3);
			tx(uartModem, (uint8_t*)"\\\",\\\"p4\\\":\\\"");
			tx(uartModem, PNUM);
			tx(uartModem, (uint8_t*)"\\\",\\\"p5\\\":\\\"");
			tx(uartModem, UPNUM);
			tx(uartModem, (uint8_t*)"\\\",\\\"p6\\\":\\\"");
			tx(uartModem, LOCK);
			tx(uartModem, (uint8_t*)"#");
			tx(uartModem, GSMLOCK);
			tx(uartModem, (uint8_t*)"\\\",\\\"p7\\\":\\\"");
			tx(uartModem, (uint8_t*)"MoveSecond");
			tx(uartModem, (uint8_t*)"\\\"}\"\r\n");
			rx(uartModem, SIMR, 40);
			rx(uartModem, SIMN, 1);
			rx(uartModem, SIMN, 1);
			if(strstr((char*)rx_buf,",0") != NULL){
				return MQTTSEND_OK;
			}
			else{
				return MQTTSEND_ERROR;
			}
		}
		if(type == 7){
			tx(uartModem, SM13);
			if(strstr((char*)SENSORTYPE,"MB7588") != NULL || strstr((char*)SENSORTYPE,"A011617")!=NULL){
				tx(uartModem, (uint8_t*)"W/");
			}
			else if(strstr((char*)SENSORTYPE,"SWellVer") != NULL){
				tx(uartModem, (uint8_t*)"S/");
			}
			else if(strstr((char*)SENSORTYPE,"WellVer") != NULL){
				tx(uartModem, (uint8_t*)"M/");
			}
			tx(uartModem, NAME1);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, NAME2);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, IMEI);
			tx(uartModem, (uint8_t*)"/warning\",1,1,\"{\\\"i\\\":\\\"");
			tx(uartModem, IMEI);
			tx(uartModem, (uint8_t*)"\\\",\\\"t\\\":\\\"");
			tx(uartModem, TIME);
			tx(uartModem, (uint8_t*)"\\\",\\\"p1\\\":\\\"");
			tx(uartModem, NAME1);
			tx(uartModem, (uint8_t*)"\\\",\\\"p2\\\":\\\"");
			tx(uartModem, NAME2);
			tx(uartModem, (uint8_t*)"\\\",\\\"p3\\\":\\\"");
			tx(uartModem, NAME3);
			tx(uartModem, (uint8_t*)"\\\",\\\"p4\\\":\\\"");
			tx(uartModem, PNUM);
			tx(uartModem, (uint8_t*)"\\\",\\\"p5\\\":\\\"");
			tx(uartModem, UPNUM);
			tx(uartModem, (uint8_t*)"\\\",\\\"p6\\\":\\\"");
			tx(uartModem, COR);
			tx(uartModem, (uint8_t*)"\\\",\\\"p7\\\":\\\"");
			tx(uartModem, (uint8_t*)"CorrectionChange");
			tx(uartModem, (uint8_t*)"\\\"}\"\r\n");
			rx(uartModem, SIMR, 40);
			rx(uartModem, SIMN, 1);
			rx(uartModem, SIMN, 1);
			if(strstr((char*)rx_buf,",0") != NULL){
				return MQTTSEND_OK;
			}
			else{
				return MQTTSEND_ERROR;
			}
		}
		if(type == 8){
			tx(uartModem, SM13);
			tx(uartModem, (uint8_t*)"R/");
			tx(uartModem, NAME1);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, NAME2);
			tx(uartModem, (uint8_t*)"/");
			tx(uartModem, IMEI);
			tx(uartModem, (uint8_t*)"/receive\",1,1,\"#\"");
			tx(uartModem, (uint8_t*)"\r\n");
			rx(uartModem, SIMR, 40);
			rx(uartModem, SIMN, 1);
			rx(uartModem, SIMN, 1);
			if(strstr((char*)rx_buf,",0") != NULL){
				return MQTTSEND_OK;
			}
			else{
				return MQTTSEND_ERROR;
			}
		}
	}
	return MQTTSEND_ERROR;
}

void mqttReceive(){
	uint8_t* REC_p;
	if(MQTTCONNECTION == CONNECTED){
		uint8_t NMEAT[10];
		uint8_t REC[500];
		uint8_t RECS[100];
		int newUp=0;
		txr(uartModem, (uint8_t*)"AT+SMSUB=\"");
		tx(uartModem, (uint8_t*)"R/");
		txr(uartModem, NAME1);
		txr(uartModem, (uint8_t*)"/");
		txr(uartModem, NAME2);
		txr(uartModem, (uint8_t*)"/");
		txr(uartModem, IMEI);
		txr(uartModem, (uint8_t*)"/");
		txr(uartModem, (uint8_t*)"receive\",1\r\n");
		rx(uartModem, (uint8_t*)"%@$#$@", 10);
		REC_p = (uint8_t*)strtok((char*)rx_buf,"\"");

		txr(uartModem, (uint8_t*)"AT+SMUNSUB=\"");
		if(strstr((char*)SENSORTYPE,"MB7588") != NULL || strstr((char*)SENSORTYPE,"A011617")!=NULL){
			txr(uartModem, (uint8_t*)"W/");
		}
		else if(strstr((char*)SENSORTYPE,"SWellVer") != NULL){
			txr(uartModem, (uint8_t*)"S/");
		}
		else if(strstr((char*)SENSORTYPE,"WellVer") != NULL){
			txr(uartModem, (uint8_t*)"M/");
		}
		txr(uartModem, NAME1);
		txr(uartModem, (uint8_t*)"/");
		txr(uartModem, NAME2);
		txr(uartModem, (uint8_t*)"/");
		tx(uartModem, IMEI);
		txr(uartModem, (uint8_t*)"/");
		txr(uartModem, (uint8_t*)"receive\"\r\n");
		rx(uartModem, SIMR, 5);

		while(REC_p != NULL){
			if(strlen((char*)REC_p) > 2 && strstr((char*)REC_p,"#") != NULL){
				strcpy((char*)REC,strtok((char*)REC_p,"#"));
				newUp = 1;
				break;
			}
			REC_p = (uint8_t*)strtok(NULL,"\"");
		}
		if(newUp == 1){
			strcpy((char*)RECS,strtok((char*)REC,","));
			while(RECS[0] != '\0'){
				if(strstr((char*)RECS,"UPNUM") != NULL){
					sscanf((char*)RECS,"UPNUM=%s",(char*)UPNUM);
				}
				if(strstr((char*)RECS,"COR") != NULL){
					sscanf((char*)RECS,"COR=%s",(char*)COR);
				}
				if(strstr((char*)RECS,"MINSENDPERIOD") != NULL){
					sscanf((char*)RECS,"MINSENDPERIOD=%s",(char*)MINSENDPERIOD);
				}
				if(strstr((char*)RECS,"MINMEASUREPERIOD") != NULL){
					sscanf((char*)RECS,"MINMEASUREPERIOD=%s",(char*)MINMEASUREPERIOD);
				}
				if(strstr((char*)RECS,"MINMEASUREMENT") != NULL){
					sscanf((char*)RECS,"MINMEASUREMENT=%s",(char*)MINMEASUREMENT);
				}
				if(strstr((char*)RECS,"MINDISTANCE") != NULL){
					sscanf((char*)RECS,"MINDISTANCE=%s",(char*)MINDISTANCE);
				}
				if(strstr((char*)RECS,"FTPINTERVAL") != NULL){
					sscanf((char*)RECS,"FTPINTERVAL=%s",(char*)FTPINTERVAL);
				}
				if(strstr((char*)RECS,"FTPIP") != NULL){
					sscanf((char*)RECS,"FTPIP=%s",(char*)FTPIP);
				}
				if(strstr((char*)RECS,"FTPPORT") != NULL){
					sscanf((char*)RECS,"FTPPORT=%s",(char*)FTPPORT);
				}
				if(strstr((char*)RECS,"FTPPASSWORD") != NULL){
					sscanf((char*)RECS,"FTPPASSWORD=%s",(char*)FTPPASSWORD);
				}
				if(strstr((char*)RECS,"FTPUSERNAME") != NULL){
					sscanf((char*)RECS,"FTPUSERNAME=%s",(char*)FTPUSERNAME);
				}
				if(strstr((char*)RECS,"NAME1=") != NULL){
					sscanf((char*)RECS,"NAME1=%s",(char*)NAME1);
				}
				if(strstr((char*)RECS,"NAME2=") != NULL){
					sscanf((char*)RECS,"NAME2=%s",(char*)NAME2);
				}
				if(strstr((char*)RECS,"NAME3=") != NULL){
					sscanf((char*)RECS,"NAME3=%s",(char*)NAME3);
				}
				if(strstr((char*)RECS,"RESETLOG") != NULL){
					defaulLog();
				}
				if(strstr((char*)RECS,"SETH=") != NULL){
					sscanf((char*)RECS,"SETH=%s",(char*)SETH);
					setHeight();
				}
				if(strstr((char*)RECS,"INFO") != NULL){
					dataConnect();
					mqttConnect();
					mqttSend(8);
					sscanf((char*)RECS,"INFO=%s",(char*)NMEAT);
					NMEA(atoi((char*)NMEAT));
				}
				if(strstr((char*)RECS,"BT") != NULL){
					dataConnect();
					mqttConnect();
					mqttSend(8);
					mqttDisconnect();
					dataDisconnect();
					bluetoothMode(180);
				}
				if(strstr((char*)RECS,"RESET") != NULL){
					dataConnect();
					mqttConnect();
					mqttSend(8);
					HAL_FLASHEx_DATAEEPROM_Unlock();
					HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080200, 0xFEEDC0DD);
					HAL_FLASHEx_DATAEEPROM_Lock();
					HAL_FLASHEx_DATAEEPROM_Unlock();
					HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, 0x08080200, 0xFEEDC0DD);
					HAL_FLASHEx_DATAEEPROM_Lock();
					NVIC_SystemReset();
				}
				if(strstr((char*)RECS,"FTPCHECK") != NULL){
					dataConnect();
					mqttConnect();
					mqttSend(8);
					dataConnect();
					ftpSetParam();
					ftpGet(1);
				}
				if(strstr((char*)RECS,"SENSORTYPE") != NULL){
					dataConnect();
					mqttConnect();
					mqttSend(8);
					sscanf((char*)RECS,"SENSORTYPE=%s",(char*)SENSORTYPE);
				}
				strcpy((char*)RECS,strtok((char*)NULL,","));
			}
			dataConnect();
			mqttConnect();
			mqttSend(8);
			writeConfig();
			getSleepParametrs();
		}
		else{
			dataConnect();
			mqttConnect();
			mqttSend(8);
		}
	}
}

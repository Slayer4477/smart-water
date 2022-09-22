/*
 * files.c
 *
 *  Created on: Jun 16, 2021
 *      Author: pcbde
 */


#include "files.h"
#include "uart.h"
#include "sim868.h"
#include "fs.h"
#include "sleep.h"
#include "ftp.h"
#include "mqtt.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t* PATHC = (uint8_t*)"D:\\BtReceived\\config.txt";//Config path
uint8_t* PATHT = (uint8_t*)"D:\\BtReceived\\table.txt";//TablePath
uint8_t* PATHCF = (uint8_t*)"D:\\FTP\\config.txt";//Config path
uint8_t* PATHTF = (uint8_t*)"D:\\FTP\\table.txt";//TablePath
uint8_t* PATHL = (uint8_t*)"D:\\log.txt";//LogPath
uint8_t* PATHP = (uint8_t*)"D:\\pos.txt";//PosPath

void readConfig(int type){
	uint8_t* config_p;
	uint8_t config_a[1000];
	if(type == 0){
		fsRead(PATHC);
	}
	else{
		fsRead(PATHCF);
	}
	config_p = (uint8_t*)strstr((char*)fs_buf,"UPNUM");
	if(config_p != NULL){
		strcpy((char*)config_a,(char*)config_p+6);
		config_a[strcspn((char*)config_a,"#")] = '\0';
		sscanf((char*)config_a,"%s",(char*)UPNUM);
	}
	config_p = (uint8_t*)strstr((char*)fs_buf,"SENSOR");
	if(config_p != NULL){
		strcpy((char*)config_a,(char*)config_p+7);
		config_a[strcspn((char*)config_a,"#")] = '\0';
		sscanf((char*)config_a,"%[^,],%[^,],%s",(char*)SENSORTYPE,(char*)SETH,(char*)COR);
	}
	config_p = (uint8_t*)strstr((char*)fs_buf,"APN");
	if(config_p != NULL){
		strcpy((char*)config_a,(char*)config_p+4);
		config_a[strcspn((char*)config_a,"#")] = '\0';
		sscanf((char*)config_a,"%[^,],%[^,],%[^,],%[^,],%[^,],%s",(char*)APNCONTYPE,(char*)APNNAME,(char*)APNUSERNAME,(char*)APNPASSWORD,(char*)APNPHONENUM,(char*)APNRATE);
	}
	config_p = (uint8_t*)strstr((char*)fs_buf,"DAQ");
	if(config_p != NULL){
		strcpy((char*)config_a,(char*)config_p+4);
		config_a[strcspn((char*)config_a,"#")] = '\0';
		sscanf((char*)config_a,"%[^,],%[^,],%[^,],%s",(char*)MINDISTANCE,(char*)MINSENDPERIOD,(char*)MINMEASUREMENT,(char*)MINMEASUREPERIOD);
	}
	config_p = (uint8_t*)strstr((char*)fs_buf,"MQTT");
	if(config_p != NULL){
		strcpy((char*)config_a,(char*)config_p+5);
		config_a[strcspn((char*)config_a,"#")] = '\0';
		sscanf((char*)config_a,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",(char*)MQTTIP,(char*)MQTTPORT,(char*)MQTTUSERNAME,(char*)MQTTPASSWORD,(char*)MQTTQOS,(char*)MQTTCLEANASS,(char*)MQTTKEEPALIVE,(char*)MQTTTIMEOUT,(char*)MQTTTOPICS[0],(char*)MQTTTOPICS[1]);
	}
	config_p = (uint8_t*)strstr((char*)fs_buf,"FTP");
	if(config_p != NULL){
		strcpy((char*)config_a,(char*)config_p+4);
		config_a[strcspn((char*)config_a,"#")] = '\0';
		sscanf((char*)config_a,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]",(char*)FTPIP,(char*)FTPPORT,(char*)FTPUSERNAME,(char*)FTPPASSWORD,(char*)FTPINTERVAL,(char*)FTPTOPICS[0],(char*)FTPTOPICS[1]);
	}
	config_p = (uint8_t*)strstr((char*)fs_buf,"NAME1");
	if(config_p != NULL){
		strcpy((char*)config_a,(char*)config_p+6);
		config_a[strcspn((char*)config_a,"#")] = '\0';
		sscanf((char*)config_a,"%s",(char*)NAME1);
	}
	config_p = (uint8_t*)strstr((char*)fs_buf,"NAME2");
	if(config_p != NULL){
		strcpy((char*)config_a,(char*)config_p+6);
		config_a[strcspn((char*)config_a,"#")] = '\0';
		sscanf((char*)config_a,"%s",(char*)NAME2);
	}
	config_p = (uint8_t*)strstr((char*)fs_buf,"NAME3");
	if(config_p != NULL){
		strcpy((char*)config_a,(char*)config_p+6);
		config_a[strcspn((char*)config_a,"#")] = '\0';
		sscanf((char*)config_a,"%s",(char*)NAME3);
	}
}
void writeConfig(void){
	int i;
	strcpy((char*)fs_buf,"UPNUM=");
	strcat((char*)fs_buf,(char*)UPNUM);
	strcat((char*)fs_buf,"#\r\n");


	strcat((char*)fs_buf,"SENSOR=");
	strcat((char*)fs_buf,(char*)SENSORTYPE);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)SETH);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)COR);
	strcat((char*)fs_buf,"#\r\n");


	strcat((char*)fs_buf,"DAQ=");
	strcat((char*)fs_buf,(char*)MINDISTANCE);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MINSENDPERIOD);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MINMEASUREMENT);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MINMEASUREPERIOD);
	strcat((char*)fs_buf,"#\r\n");

	strcat((char*)fs_buf,"APN=");
	strcat((char*)fs_buf,(char*)APNCONTYPE);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)APNNAME);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)APNUSERNAME);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)APNPASSWORD);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)APNPHONENUM);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)APNRATE);
	strcat((char*)fs_buf,"#\r\n");

	strcat((char*)fs_buf,"MQTT=");
	strcat((char*)fs_buf,(char*)MQTTIP);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MQTTPORT);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MQTTUSERNAME);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MQTTPASSWORD);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MQTTQOS);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MQTTCLEANASS);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MQTTKEEPALIVE);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)MQTTTIMEOUT);
	i = 0;
	while(MQTTTOPICS[i][0] != '\0'){
		strcat((char*)fs_buf,",");
		strcat((char*)fs_buf,(char*)MQTTTOPICS[i++]);
	}
	strcat((char*)fs_buf,"#\r\n");


	strcat((char*)fs_buf,"FTP=");
	strcat((char*)fs_buf,(char*)FTPIP);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)FTPPORT);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)FTPUSERNAME);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)FTPPASSWORD);
	strcat((char*)fs_buf,",");
	strcat((char*)fs_buf,(char*)FTPINTERVAL);
	i = 0;
	while(FTPTOPICS[i][0] != '\0'){
		strcat((char*)fs_buf,",");
		strcat((char*)fs_buf,(char*)FTPTOPICS[i++]);
	}
	strcat((char*)fs_buf,"#\r\n");
	strcat((char*)fs_buf,"NAME1=");
	strcat((char*)fs_buf,(char*)NAME1);
	strcat((char*)fs_buf,"#\r\n");

	strcat((char*)fs_buf,"NAME2=");
	strcat((char*)fs_buf,(char*)NAME2);
	strcat((char*)fs_buf,"#\r\n");

	strcat((char*)fs_buf,"NAME3=");
	strcat((char*)fs_buf,(char*)NAME3);
	strcat((char*)fs_buf,"#\r\n");

	fsWrite(PATHC);
}
void readTable(int type){
	int i=0;
	uint8_t* TABLE_P;
	if(type == 0){
		fsRead(PATHT);
	}
	else{
		fsRead(PATHTF);
	}
	TABLE_P = (uint8_t*)strtok((char*)fs_buf,",");
	while(TABLE_P != NULL){
		TABLE[i++] = atol((char*)TABLE_P);
		TABLE_P = (uint8_t*)strtok((char*)NULL,",");
	}
	TABLE[i] = -1;
	while(i++<999){
		TABLE[i] = -1000;
	}
}
void writeTable(void){
	int i=0;
	char TABLE_A[10];
	strcpy((char*)fs_buf,"");
	itoa((int)TABLE[i++],TABLE_A,10);
	strcat((char*)fs_buf,TABLE_A);
	while(TABLE[i] != -1){
		itoa((int)TABLE[i++],TABLE_A,10);
		strcat((char*)fs_buf,",");
		strcat((char*)fs_buf,TABLE_A);
	}
	strcat((char*)fs_buf,"#");
	fsWrite(PATHT);
}
void defaultParam(void){
	strcpy((char*)UPNUM,"+998");

	strcpy((char*)SENSORTYPE,"MB7588");
	strcpy((char*)SETH,"0");
	strcpy((char*)COR,"0");

	strcpy((char*)MINDISTANCE,"10");
	strcpy((char*)MINSENDPERIOD,"2");
	strcpy((char*)MINMEASUREMENT,"2");
	strcpy((char*)MINMEASUREPERIOD,"5");

	strcpy((char*)MQTTIP,"185.196.214.190");
	strcpy((char*)MQTTPORT,"1883");
	strcpy((char*)MQTTUSERNAME,"emqx");
	strcpy((char*)MQTTPASSWORD,"12345");
	strcpy((char*)MQTTQOS,"1");
	strcpy((char*)MQTTCLEANASS,"0");
	strcpy((char*)MQTTKEEPALIVE,"60");
	strcpy((char*)MQTTTIMEOUT,"60");
	strcpy((char*)MQTTTOPICS[0],"SARF");
	strcpy((char*)MQTTTOPICS[1],"SATH");
	strcpy((char*)MQTTTOPICS[2],"BAT");
	strcpy((char*)MQTTTOPICS[3],"COR");

	strcpy((char*)FTPIP,"185.196.214.63");
	strcpy((char*)FTPPORT,"21");
	strcpy((char*)FTPUSERNAME,"admin_smart_ftp");
	strcpy((char*)FTPPASSWORD,"12345678");
	strcpy((char*)FTPINTERVAL,"2");
	strcpy((char*)FTPTOPICS[0],"BAT");
	strcpy((char*)FTPTOPICS[1],"FW");

	strcpy((char*)NAME1,"0");
	strcpy((char*)NAME2,"0");
	strcpy((char*)NAME3,"0");

	strcpy((char*)APNCONTYPE,"GPRS");
	strcpy((char*)APNNAME,"internet");
	strcpy((char*)APNUSERNAME,"");
	strcpy((char*)APNPASSWORD,"");
	strcpy((char*)APNPHONENUM,"");
	strcpy((char*)APNRATE,"2");

}
void defaultTable(void){
	TABLE[0]=0;
	TABLE[1]=0;
	TABLE[2]=0;
	TABLE[3]=0;
	TABLE[4]=0;
	TABLE[5]=0;
	TABLE[6]=0;
	TABLE[7]=0;
	TABLE[8]=0;
	TABLE[9]=0;
	TABLE[10]=0;
	TABLE[11]=-1;
}
void updateConfig(int type){
	readConfig(type);
	writeConfig();
}
void updateTable(int type){
	readTable(type);
	writeTable();
}

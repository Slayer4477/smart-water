/*
 * working.c
 *
 *  Created on: May 22, 2022
 *      Author: pcbde
 */

#include "working.h"


void robbery(){
	  if(HAL_GPIO_ReadPin(ON_GPIO_Port, ON_Pin) == GPIO_PIN_RESET){
		  ledsOn();
		  int robcounter;
		  HAL_GPIO_WritePin(l2_GPIO_Port, l2_Pin, GPIO_PIN_SET);
		  HAL_Delay(1000);
		  for(robcounter=0;robcounter<100;robcounter++){
			  SystemClock_Config();
			  HAL_ResumeTick();
			  HAL_Delay(20);
			  if(HAL_GPIO_ReadPin(ON_GPIO_Port, ON_Pin) == GPIO_PIN_SET){
				  break;
			  }
		  }
		  HAL_GPIO_WritePin(l2_GPIO_Port, l2_Pin, GPIO_PIN_RESET);
		  if(robcounter == 100){
			  BATTERYSAVER = BATTERYSAVER_OFF;
			  savebattery = 0;
			  sim868On();
			  REGESTRATION = sim868Init();
			  dataConnect();
			  mqttConnect();
			  mqttSend(3);
			  NMEA(10);
			  mqttSend(4);
			  mqttReceive();
			  mqttDisconnect();
			  dataDisconnect();
			  sim868Off();
		  }
		  ledsOff();
	  }
}
void working(){
	  if(sleepCounter%minSendPeriod == 0 && checksleepCount == 1){
		  getPG();
		  ledsOff();
		  getSath();
		  modemOn();
		  getTime();
		  if(TIME[0] != '0'){
			  addToLog();
		  }
		  while(sendPosition < logPosition){
			  readFromLog(sendPosition);
			  dataConnect();
			  mqttConnect();
			  if(mqttSend(1) == MQTTSEND_ERROR){
				  break;
			  }
		  }
		  checksleepCount = 0;
		  if(sleepCounter>ftpinterval){
			  BATTERYSAVER = BATTERYSAVER_OFF;
			  modemOn();
			  getADC();
			  getTemp();
			  getSig();
			  getPNUM();
			  dataConnect();
			  getgsmloc();
			  mqttConnect();
			  mqttSend(2);
			  mqttDisconnect();
			  mqttConnect();
			  mqttReceive();
			  sleepCounter = 0;
			  setPos(sendPosition);
			  if(CONNECTION == CONNECTED && MQTTCONNECTION == CONNECTED){
				  savebattery = 0;
			  }
			  else{
				  BATTERYSAVER = BATTERYSAVER_ON;
			  }
		  }
		  mqttDisconnect();
		  dataDisconnect();
		  sim868Off();
	  }
}
void sleeping(){
	sleep();
	wakeUp();
}

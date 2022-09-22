/*
 * mqtt.h
 *
 *  Created on: Jun 19, 2021
 *      Author: pcbde
 */

#ifndef MQTT_H_
#define MQTT_H_

#include "main.h"


extern uint8_t* SMR1;
extern uint8_t* SM1;
extern uint8_t* SM2;
extern uint8_t* SM21;
extern uint8_t* SM22;
extern uint8_t* SM23;
extern uint8_t* SM24;
extern uint8_t* SM3;
extern uint8_t* SM4;
extern uint8_t* SM6;
extern uint8_t* SM7;
extern uint8_t* SM8;
extern uint8_t* SM9;
extern uint8_t* SM10;
extern uint8_t* SM11;
extern uint8_t* SM111;
extern uint8_t* SM112;
extern uint8_t* SM113;
extern uint8_t* SM12;
extern uint8_t* SM13;
extern uint8_t* SM14;
extern uint8_t* SM15;
extern uint8_t* SM16;


void dataConnect();
void dataDisconnect();
void mqttConnect();
void mqttDisconnect();
int mqttSend(int type);
void mqttReceive();
#endif /* MQTT_H_ */

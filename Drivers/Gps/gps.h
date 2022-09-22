/*
 * gpc.h
 *
 *  Created on: Jun 20, 2021
 *      Author: pcbde
 */

#ifndef GPS_H_
#define GPS_H_

#include "main.h"

extern uint8_t* GPSR3;
extern uint8_t* GPSR4;
extern uint8_t* GPSR5;
extern uint8_t* GPS11;
extern uint8_t* GPS13;


void updateTime();
void gpsPowerOn();
void NMEA(int times);
#endif /* GPS_H_ */

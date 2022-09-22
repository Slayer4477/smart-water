/*
 * sensor.h
 *
 *  Created on: Jun 17, 2021
 *      Author: pcbde
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include "main.h"

int distanceSum;
int distance;
uint8_t da[5];
int lastDistance;
void getSath();
void setHeight();
void debug(uint8_t* data);
void delay(int timeout);
void changeWellVer();
void measureAll();
#endif /* SENSOR_H_ */

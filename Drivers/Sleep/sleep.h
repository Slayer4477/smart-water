/*
 * sleep.h
 *
 *  Created on: Jun 22, 2021
 *      Author: pcbde
 */

#ifndef SLEEP_H_
#define SLEEP_H_

#include "main.h"

extern int minDistance;
extern int minSendPeriod;
extern int minMeasurement;
extern int minMeasurePeriod;

void sleepInit();
void getSleepParametrs();
#endif /* SLEEP_H_ */

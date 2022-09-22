/*
 * clock.h
 *
 *  Created on: Jun 28, 2021
 *      Author: pcbde
 */

#ifndef CLOCK_H_
#define CLOCK_H_
#include "main.h"

RTC_TimeTypeDef t;
RTC_DateTypeDef d;

void getTime();
void setTime();

#endif /* CLOCK_H_ */

/*
 * sleep.c
 *
 *  Created on: Jun 22, 2021
 *      Author: pcbde
 */

#include "sleep.h"
#include "uart.h"
#include "files.h"
#include <stdlib.h>
#include "clock.h"
#include "main.h"
#include "sim868.h"
int minDistance = 0;
int minSendPeriod = 0;
int minMeasurement = 0;
int minMeasurePeriod = 0;
void getSleepParametrs(){
	minDistance = atoi((char*)MINDISTANCE);
	minMeasurement = atoi((char*)MINMEASUREMENT);
	minMeasurePeriod = atoi((char*)MINMEASUREPERIOD);
	minSendPeriod = atoi((char*)MINSENDPERIOD);
	ftpinterval = atoi((char*)FTPINTERVAL);
	if(minSendPeriod > ftpinterval){
		itoa(ftpinterval,(char*)MINSENDPERIOD,10);
		getSleepParametrs();
	}
}


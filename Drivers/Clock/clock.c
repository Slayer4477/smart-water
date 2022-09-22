/*
 * clock.c
 *
 *  Created on: Jun 28, 2021
 *      Author: pcbde
 */

#include "clock.h"
#include "uart.h"
#include <stdio.h>

void getTime(){
	HAL_RTC_GetTime(rtc, &t, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(rtc, &d, RTC_FORMAT_BIN);
	sprintf((char*)TIME, "%d/%02d/%02d,%02d:%02d:%02d+00", d.Year,d.Month,d.Date,t.Hours,t.Minutes,t.Seconds);
}
void setTime(){
	//21/06/28,20:04:21+00
	int a1,a2,a3,a4,a5,a6;
	sscanf((char*)TIME,"%d/%d/%d,%d:%d:%d",&a1,&a2,&a3,&a4,&a5,&a6);
	d.Year = (uint8_t)a1;
	d.Month = (uint8_t)a2;
	d.Date = (uint8_t)a3;
	t.Hours = (uint8_t)a4;
	t.Minutes = (uint8_t)a5;
	t.Seconds = (uint8_t)a6;
	HAL_RTC_SetTime(rtc, &t, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(rtc, &d, RTC_FORMAT_BIN);
}

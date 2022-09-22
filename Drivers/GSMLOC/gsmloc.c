/*
 * gsmloc.c
 *
 *  Created on: May 14, 2022
 *      Author: pcbde
 */

#include "gsmloc.h"
#include "uart.h"
#include "mqtt.h"
#include "sim868.h"
#include <string.h>
void getgsmloc(){
	if(CONNECTION == CONNECTED){
		GSMLOCATION = NOT_LOCKED;
		txr(uartModem, (uint8_t*)"AT+CLBS=4,1\r\n");
		rx(uartModem, SIMN, 25);
		rx(uartModem, (uint8_t*)"+CLBS: ", 1);
		rx(uartModem, SIMR, 1);
		if(rx_buf[0] == '0'){
			GSMLOCATION = LOCKED;
			strcpy((char*)GSMLOCK,(char*)rx_buf);
			GSMLOCK[strlen((char*)GSMLOCK)-8]='\0';
		}
	}
}

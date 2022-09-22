/*
 * bluetooth.h
 *
 *  Created on: Jun 16, 2021
 *      Author: pcbde
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "main.h"

#define BT_CONNECT_OK 10
#define BT_CONNECT_TIMEOUT 11



extern uint8_t* BLR0;
extern uint8_t* BLR1;
extern uint8_t* BLR2;
extern uint8_t* BLR3;
extern uint8_t* BLR4;
extern uint8_t* BLR5;
extern uint8_t* BLR6;
extern uint8_t* BLR7;
extern uint8_t* BLR8;
extern uint8_t* BLR9;
extern uint8_t* BLR10;
extern uint8_t* BLR11;
extern uint8_t* BLR12;
extern uint8_t* BLR13;
extern uint8_t* BLR14;
extern uint8_t* BL0;
extern uint8_t* BL1;
extern uint8_t* BL2;
extern uint8_t* BL3;
extern uint8_t* BL4;
extern uint8_t* BL5;
extern uint8_t* BL6;
extern uint8_t* BL7;
extern uint8_t* BL8;
extern uint8_t* BL9;
extern uint8_t* BL10;

uint8_t CDID[3];
uint8_t PDID[3];
uint8_t BTRSSI[5];

extern int rx_c;

void BTPowerOn();
void BTPowerOff();
int BTConnect();
void bluetoothMode(int timeout);
void sendSpp();
int updateConnectedDeviceID();
void getBTRSII();
void call(uint8_t* num);

//void BT
#endif /* BLUETOOTH_H_ */

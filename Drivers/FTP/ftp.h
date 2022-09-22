/*
 * ftp.h
 *
 *  Created on: Jun 20, 2021
 *      Author: pcbde
 */

#ifndef FTP_H_
#define FTP_H_

#include "main.h"

extern uint8_t* FTPR1;
extern uint8_t* FTPR2;
extern uint8_t* FTPR3;
extern uint8_t* FTP1;
extern uint8_t* FTP2;
extern uint8_t* FTP3;
extern uint8_t* FTP4;
extern uint8_t* FTP5;
extern uint8_t* FTP6;
extern uint8_t* FTP7;
extern uint8_t* FTP8;
extern uint8_t* FTP9;
extern uint8_t* FTP10;
extern uint8_t* FTP11;
extern uint8_t* FTP13;
extern uint8_t* FTP14;
extern uint8_t* FTP15;


int ftpList();
void ftpSetParam();
void ftpGet();
void ftpDel();
int checkFile();
void removeBoot();
void removeFirst();
void addFirst();
#endif /* FTP_H_ */


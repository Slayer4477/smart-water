/*
 * fs.h
 *
 *  Created on: Jun 16, 2021
 *      Author: pcbde
 */

#ifndef FS_H_
#define FS_H_

#include "main.h"

extern uint8_t* FSN;
extern uint8_t* FS1;
extern uint8_t* FS2;
extern uint8_t* FS3;
extern uint8_t* FSMODE;
extern uint8_t* FSSIZE;
extern uint8_t* FSPOSITION;
extern uint8_t* FSINPUTTIME;



uint8_t fs_buf[10000];
uint8_t* fs_buff_p;

#endif /* FS_H_ */

void fsCreate(uint8_t *path);
int fsRead(uint8_t *path);
void fsWrite(uint8_t *path);
int fsAdd(uint8_t *path,uint8_t *data,int filesize);
long fsFileSize(uint8_t *path);

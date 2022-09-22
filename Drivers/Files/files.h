/*
 * files.h
 *
 *  Created on: Jun 16, 2021
 *      Author: pcbde
 */

#ifndef FILES_H_
#define FILES_H_

#include "main.h"

extern uint8_t* PATHC;//Config path
extern uint8_t* PATHT;//TablePath
extern uint8_t* PATHCF;//Config path
extern uint8_t* PATHTF;//TablePath
extern uint8_t* PATHL;//LogPath
extern uint8_t* PATHP;//PosPath

void readID(int type);
void writeID(void);
void readConfig(int type);
void writeConfig(void);
void readTable(int type);
void writeTable(void);
void defaultParam(void);
void defaultTable(void);
void updateConfig(int type);
void updateTable(int type);
#endif /* FILES_H_ */

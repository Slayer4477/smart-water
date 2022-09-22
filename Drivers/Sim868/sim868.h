/*
 * sim868.h
 *
 *  Created on: Jun 15, 2021
 *      Author: pcbde
 */

#ifndef SIM868_H_
#define SIM868_H_

#include "main.h"

#define SIM868_INIT_ERROR 0
#define SIM868_INIT_OK 1

ADC_HandleTypeDef *madc;
GPIO_InitTypeDef gp;

extern uint8_t* SIMR;
extern uint8_t* SIMR1;
extern uint8_t* SIMR2;
extern uint8_t* SIM;
extern uint8_t* SIM01;
extern uint8_t* SIM02;
extern uint8_t* SIM1;
extern uint8_t* SIMN;
extern uint8_t* SIMV;
extern uint8_t* SIMVN;
extern uint8_t* SIM2;
extern uint8_t* SIM3;
extern uint8_t* SIM4;
extern uint8_t* SIM5;
extern uint8_t* SIM6;
extern uint8_t* SIM7;
extern uint8_t* SIM8;


void ledsOn();
void ledsOff();
void l3On();
void l2On();
void l3Off();
void l2Off();
void sim868On(void);
void sim868Off(void);
void sensorOn(void);
void sensorOff(void);
void loraOn(void);
void loraOff(void);
int sim868Init(void);
void getIMEI();
void getPNUM();
void getTemp();
void getADC();
void getSig();
void getPG();
void getMV();
int sim868onPrep();
#endif /* SIM868_H_ */

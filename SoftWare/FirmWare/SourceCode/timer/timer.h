#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx.h"

extern  unsigned int Systemtime;

void TIM5_Init(void);
void TIM4_Init(void);
unsigned long GetSystemTime(void);
#endif



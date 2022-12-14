#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx.h"

extern  unsigned int Systemtime;

void TIM5_Init(void);
unsigned long GetSystemTime(void);
#endif



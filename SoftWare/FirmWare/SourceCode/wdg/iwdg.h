#ifndef __IWDG_H__
#define __IWDG_H__

#include "stm32f4xx.h"

#define  RELOAD_VALUE   4000

void IWDG_Init(void);
void IWDG_Feed(void);


#endif




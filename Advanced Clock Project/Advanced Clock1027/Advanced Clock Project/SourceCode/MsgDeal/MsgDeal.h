#ifndef __MSGDEAL_H__
#define __MSGDEAL_H__

#include "stm32f4xx.h"

extern struct SystemVar system_var; 
void Uart_AT_Deal(void);
void QueueInfoPrint(void);

#endif


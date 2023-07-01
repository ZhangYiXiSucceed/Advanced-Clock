#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx.h"

typedef void (*timer_callback)(void* para);
typedef struct timer_interval_func_struct
{
  int interval;
  int target_time;
  timer_callback cb;
  void* para;
}timer_interval_func_t;
extern  unsigned int Systemtime;

void TIM5_Init(void);
void TIM4_Init(void);
unsigned long GetSystemTime(void);
s8 timer_set_func(timer_interval_func_t* para);
void timer_interval_func_task();
#endif



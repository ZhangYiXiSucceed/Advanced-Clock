#ifndef __LED_H
#define __LED_H
#include "stm32f4xx.h"

#define  LED_D1_ON        GPIO_SetBits(GPIOE,GPIO_Pin_11) 
#define  LED_D1_OFF       GPIO_ResetBits(GPIOE,GPIO_Pin_11)
 
#define  LED_D2_ON        GPIO_SetBits(GPIOE,GPIO_Pin_12)
#define  LED_D2_OFF       GPIO_ResetBits(GPIOE,GPIO_Pin_12)

#define  LED_D3_ON        GPIO_SetBits(GPIOE,GPIO_Pin_13)
#define  LED_D3_OFF       GPIO_ResetBits(GPIOE,GPIO_Pin_13)

#define  LED_D4_ON        GPIO_SetBits(GPIOE,GPIO_Pin_14)
#define  LED_D4_OFF       GPIO_ResetBits(GPIOE,GPIO_Pin_14)


void LED_Init(void);//≥ı ºªØ		
void LedRun(void);
void LedBlink(void);
void PWM_LEDInit(void);
void LED_BreathingLight(void);
#endif



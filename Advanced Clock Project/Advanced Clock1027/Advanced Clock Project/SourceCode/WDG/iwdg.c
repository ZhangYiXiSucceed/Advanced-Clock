#include "main.h"


void IWDG_Init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对IWDG->PR IWDG->RLR的写
	
  IWDG_SetPrescaler(IWDG_Prescaler_128); //设置IWDG分频系数

  IWDG_SetReload(4000);   //设置IWDG装载值

  IWDG_ReloadCounter(); //reload
  
  IWDG_Enable();       //使能看门狗
}


void IWDG_Feed(void)
{
  IWDG_ReloadCounter();//reload
}





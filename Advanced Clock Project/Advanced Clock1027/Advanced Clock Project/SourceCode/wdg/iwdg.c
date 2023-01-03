#include "main.h"


void IWDG_Init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //enable IWDG->PR IWDG->RLR
	
  IWDG_SetPrescaler(IWDG_Prescaler_128); //set IWDG pre fre

  IWDG_SetReload(RELOAD_VALUE);   //set IWDG reload    tout = (4*2^pre)/40*reload_vale = 128/40*4000 = 12800ms

  IWDG_ReloadCounter(); //reload
  
  IWDG_Enable();       //enable iwdg
}


void IWDG_Feed(void)
{
  IWDG_ReloadCounter();//reload
}






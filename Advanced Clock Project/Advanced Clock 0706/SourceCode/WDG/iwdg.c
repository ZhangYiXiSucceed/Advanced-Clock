#include "main.h"


void IWDG_Init(void)
{
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶ�IWDG->PR IWDG->RLR��д
	
  IWDG_SetPrescaler(IWDG_Prescaler_128); //����IWDG��Ƶϵ��

  IWDG_SetReload(4000);   //����IWDGװ��ֵ

  IWDG_ReloadCounter(); //reload
  
  IWDG_Enable();       //ʹ�ܿ��Ź�
}


void IWDG_Feed(void)
{
  IWDG_ReloadCounter();//reload
}





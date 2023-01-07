#include "main.h" 



void KEY_Init(void)
{    	 
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOF时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);                        // SYSCFG
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource4);                 // PB3??????3
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5); 

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);  
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1); 
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;                                    // LINE3
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                           // ????
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                       // ????? 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                     // ??LINE3
  EXTI_Init(&EXTI_InitStructure);                                               // ??
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_Init(&EXTI_InitStructure);   
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_Init(&EXTI_InitStructure);   
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_Init(&EXTI_InitStructure);   
	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;                              // ????8
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;                  // ?????1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;                         // ????2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                               // ????????
  NVIC_Init(&NVIC_InitStructure);  
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Init(&NVIC_InitStructure); 
	 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
}



void EXTI4_IRQHandler()                //KEY3
{
	    if(EXTI_GetITStatus(EXTI_Line4))
    {
      if(system_var.Key3Flag == 0)
			{
				system_var.Key3Flag =1;
			}
      EXTI_ClearITPendingBit(EXTI_Line4);
    }
}
void EXTI9_5_IRQHandler()               //KEY2
{
	    if(EXTI_GetITStatus(EXTI_Line5))
    {
     if(system_var.Key2Flag == 0)
			{
				system_var.Key2Flag =1;
			}
      EXTI_ClearITPendingBit(EXTI_Line5);
    }
	
}
unsigned int temp_key4_systemtime;
void EXTI0_IRQHandler()      //KEY4
{
	    if(EXTI_GetITStatus(EXTI_Line0))
    {
      if(system_var.Key4Flag == 0)
			{
				system_var.Key4Flag =1;
				temp_key4_systemtime = Systemtime; 
			}
      EXTI_ClearITPendingBit(EXTI_Line0);
    }
	
}
unsigned int temp_key5_systemtime;
void EXTI1_IRQHandler()        //KEY5
{ 
	    if(EXTI_GetITStatus(EXTI_Line1))
    {
      if(system_var.Key5Flag == 0)
			{
				system_var.Key5Flag =1;
				temp_key5_systemtime = Systemtime; 
			}
      EXTI_ClearITPendingBit(EXTI_Line1);
    }
}


void KEYService()
{
	
	if(system_var.Key4Flag == 1)
	{
		if(Systemtime > temp_key4_systemtime + 20)
		{
		  system_var.NRFRxFlag =1;
		  system_var.Key4Flag = 0;
		}
	}

	
	if(system_var.Key5Flag == 1)
	{
		if(Systemtime > temp_key5_systemtime + 20)
		{
			 system_var.NRFTxFlag =1;
		   system_var.Key5Flag = 0;
		}
	}	
	
}

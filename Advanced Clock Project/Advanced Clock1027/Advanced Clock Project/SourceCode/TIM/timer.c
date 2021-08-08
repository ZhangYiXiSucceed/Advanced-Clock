#include "main.h"


unsigned int Systemtime;

void TIM5_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);                          // TIM3 clock enable
    // Enable the TIM3 gloabal Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Time base configuration 
    TIM_TimeBaseStructure.TIM_Period = 1000;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    // Prescaler configuration
    TIM_PrescalerConfig(TIM5, 83, TIM_PSCReloadMode_Immediate);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);                                    // TIM Interrupts enable 
    TIM_Cmd(TIM5, ENABLE);                                                        // TIM3 enable counter 
}

void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        Systemtime++;
        
		if(Systemtime % 1000 == 1)
		{
			system_data.SystemGMTTime++;
		}
        if(Systemtime % 120000 == 0)
        {
          system_var.TwoMinuteFlag = 1;
        }
        
        if(Systemtime % 3000 == 0)
        {
          system_var.LedBlinkFlag = 1;
        }
        
        if(Systemtime % 2000 == 0)
        {
          system_var.HTDataFlag = 1;
        }
        
        if(Systemtime % 1000 == 0)
        {
          system_var.CurrentTimeFlag = 1;
        }
        UART1Poll();
        UART2Poll();
        UART3Poll();
        UART4Poll();
        LED_BreathingLight();
    }
}


unsigned long GetSystemTime(void)
{

   return system_data.SystemGMTTime;
	
}


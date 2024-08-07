#include "main.h"
#define TIMER_FUNC_MAX_NUM  8
typedef struct timer_func_list_struct
{
  timer_interval_func_t timer_func[TIMER_FUNC_MAX_NUM];
  uint8_t timer_func_header_index;
  uint8_t timer_func_tail_index;
  uint8_t timer_func_num;
}timer_func_list_t;

timer_func_list_t timer_func_list_g;

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
    TIM_Cmd(TIM5, ENABLE);                                      // TIM3 enable counter 
}

void TIM4_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);                          // TIM3 clock enable
    // Enable the TIM3 gloabal Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Time base configuration 
    TIM_TimeBaseStructure.TIM_Period = 1000;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // Prescaler configuration
    TIM_PrescalerConfig(TIM4, 8400-1, TIM_PSCReloadMode_Immediate);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);                                    // TIM Interrupts enable 
                                                          // TIM3 enable counter 
}

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
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
        if(Systemtime % 30000 == 0)
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
		if(Systemtime % 50 == 0)
		{
			system_var.Time_20_ms_flag=1;
		}
		
        UART1Poll();
		    UART4Poll();
#ifndef BOOT
        UART2Poll();
        UART3Poll();
#endif
    }
}


unsigned long GetSystemTime(void)
{

   return system_data.SystemGMTTime;
	
}


s8 timer_set_func(timer_interval_func_t* para)
{
  if(TIMER_FUNC_MAX_NUM > timer_func_list_g.timer_func_num)
  {
    timer_func_list_g.timer_func[timer_func_list_g.timer_func_header_index] = *para;
    timer_func_list_g.timer_func_num++;
    timer_func_list_g.timer_func_header_index++;
    timer_func_list_g.timer_func_header_index = (timer_func_list_g.timer_func_header_index) % TIMER_FUNC_MAX_NUM;
    rt_kprintf(" target = %d num=%d index=%d\r\n",para->target_time,timer_func_list_g.timer_func_num,timer_func_list_g.timer_func_header_index);
    return timer_func_list_g.timer_func_num;
  }
  return -1;
}


void timer_interval_func_task()
{
    if(0 == timer_func_list_g.timer_func_num)
     return;

    if(GetSystemTime() < timer_func_list_g.timer_func[timer_func_list_g.timer_func_tail_index].target_time)
      return;

    rt_kprintf("exec fun\r\n");
    if(NULL != timer_func_list_g.timer_func[timer_func_list_g.timer_func_tail_index].cb)
    {
       
        (*timer_func_list_g.timer_func[timer_func_list_g.timer_func_tail_index].cb)(timer_func_list_g.timer_func[timer_func_list_g.timer_func_tail_index].para);
        timer_func_list_g.timer_func_num --;
        timer_func_list_g.timer_func_tail_index++;
        timer_func_list_g.timer_func_tail_index = (timer_func_list_g.timer_func_tail_index) % TIMER_FUNC_MAX_NUM;
    }
}



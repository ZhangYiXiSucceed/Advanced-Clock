#include "main.h"


int main()
{
  u8 Data[20]="go to while!\r\n";
  SysTick_Config(SystemCoreClock/1000000);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  UART1Init();
  UART2Init();
  
  LED_Init();
  TIM5_Init();
  
  HTSensor_Init();
  HTSensorMode();
  
  UART1_SentMsgL(Data,20);
  UART2_SentMsgL(Data,20);
  
  OLED_Init();
  
  IWDG_Init();
  My_RTC_Init();
  while(1)
  {
    IWDG_Feed();
    LedBlink();
    QueueInfoPrint();
    Uart_AT_Deal();
    PrintHTInfo();
    ShowCurrentTime();
    //Show16X32();
  }
}



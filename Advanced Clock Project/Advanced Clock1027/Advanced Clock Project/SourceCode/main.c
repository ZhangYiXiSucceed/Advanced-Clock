#include "main.h"


int main()
{
  u8 Data[32]="go to while!\r\n";
  
  SysTick_Config(SystemCoreClock/1000000);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  UART1Init();       //���Դ���
  UART2Init();       //��������
  UART3Init();       //NB����
  UART4Init();       //WIFI����
	
  //LED_Init();
  PWM_LEDInit();
  KEY_Init();
  TIM5_Init();
  
  HTSensor_Init();
  HTSensorMode();
	
  OLED_Init();
 
  My_RTC_Init();
	
  //NBModuleInit();

  QueueInit(&MyQueue);
  rt_kprintf((char*)Data);
  rt_kprintf2((char*)Data);
	
  //NRF24L01_Init();

  IWDG_Init();
  while(1)
  {
    IWDG_Feed();
    //LedBlink();
    KEYService();
    //NRFCommunicationService();
    QueueInfoPrint();
    Uart_AT_Deal();
    PrintHTInfo();
    ShowCurrentTime();
    //Show16X32();
  }
}



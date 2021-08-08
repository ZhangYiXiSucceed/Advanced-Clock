#include "main.h"


int main()
{
  u8 Data[32]="go to while!\r\n";
  
  SysTick_Config(SystemCoreClock/1000000);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  UART1Init();       /*common uart,can receive uart device(ch340) data and execute cmd*/

  
  UART2Init();       /*BlueTooth uart*/
  UART3Init();       /*NB-IoT uart*/
  UART4Init();       /*WIFI uart */

  
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

  QueueInit(&wifi_queue);
  QueueInit(&nb_queue);
  QueueInit(&ble_queue);

  
  rt_kprintf((char*)Data);
  rt_kprintf2((char*)Data);
	
  //NRF24L01_Init();

  FifoInit(&sOperCmdUnionFifo);

  init_wifi_network();
  
  IWDG_Init();

  
  while(1)
  {

  	
    IWDG_Feed();
    //LedBlink();
    KEYService();
    //NRFCommunicationService();
    QueueInfoPrint();
    Uart_AT_Deal();

    wifi_task_deal();
    PrintHTInfo();
    ShowCurrentTime();

    //show_oled_char(0,0,'0',32);

	//show_oled_char(20,0,'1',32);
    //show_oled_char(36,0,'2',32);

	//show_oled_char(52,0,':',32);
	
    //show_oled_char(68,0,'3',32);
	//show_oled_char(84,0,'4',32);

	ShowBMP();
	show_line(0,5);
	OLED_ShowCHinese(0,6,0);
	OLED_ShowCHinese(18,6,1);
	OLED_ShowCHinese(36,6,2);
	OLED_ShowCHinese(54,6,3);
    //Show16X32();
  
  }
}



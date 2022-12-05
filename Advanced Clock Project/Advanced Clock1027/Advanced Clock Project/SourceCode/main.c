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

  queue_init(&MyQueue);

  queue_init(&wifi_queue);
  queue_init(&nb_queue);
  queue_init(&ble_queue);

  
  rt_kprintf((char*)Data);
  rt_kprintf2((char*)Data);
	
  //NRF24L01_Init();

  FifoInit(&sOperCmdUnionFifo);

  init_wifi_network();
  
  //IWDG_Init();

  diag_cmd_start();

  while(1)
  {
  	
    //IWDG_Feed();
    //LedBlink();
    KEYService();
    //NRFCommunicationService();
    bluetooth_msg_porcess();
    Uart_AT_Deal();

    wifi_task_deal();
	show_interface_oled();
  }
}



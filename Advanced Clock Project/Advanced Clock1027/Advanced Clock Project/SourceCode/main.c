#include "main.h"


int main()
{
  u8 Data[32]="go to while!\r\n";
  
  SysTick_Config(SystemCoreClock/1000000);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  shell_init();
  
  //LED_Init();
  PWM_LEDInit();
  KEY_Init();
  TIM5_Init();
  
  HTSensor_Init();
  OLED_Init();
  My_RTC_Init();

  ble_init();
  init_wifi_network();
  NRF24L01_Init();
  
  IWDG_Init();

  rt_kprintf((char*)Data);
  rt_kprintf2((char*)Data);

  while(1)
  {
    IWDG_Feed();
    //LedBlink();
    KEYService();
    nrf_communication_service_msg_process();
    bluetooth_msg_porcess();
    shell_process();
		periodic_task_process();
    wifi_msg_process();
	show_interface_oled();
  }
}



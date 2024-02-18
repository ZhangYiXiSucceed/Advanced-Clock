#include "main.h"


int main()
{
  u8 Data[32]="go to app!\r\n";

  NVIC_SetVectorTable(NVIC_VectTab_FLASH,APP_ADDR_OFFSET);
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

  rt_kprintf("*******************************************\r\n");
  rt_kprintf("%s, time=%s date=%s\r\n",(char*)Data, __TIME__,__DATE__);
  rt_kprintf("*******************************************\r\n");

  rt_kprintf2((char*)Data);
  region_header_init(&region_header);


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
    timer_interval_func_task();
	  show_interface_oled();
  }
}



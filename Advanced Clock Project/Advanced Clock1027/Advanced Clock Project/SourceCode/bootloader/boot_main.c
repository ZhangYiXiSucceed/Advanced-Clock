#include "boot_main.h"
int main()
{
  u8 Data[32]="go to while!\r\n";
  
  SysTick_Config(SystemCoreClock/1000000);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  shell_init();
  
  TIM5_Init();
  
  init_wifi_network();
  IWDG_Init();

  rt_kprintf((char*)Data);

  while(1)
  {
    IWDG_Feed();
    shell_process();
    wifi_msg_process();
  }
}


#include "boot_main.h"

/****  BootFlash 0x08000000 - 0x08008000   0   - 32k    ****/
/****  DataFlash 0x08008000 - 0x08010000   32k - 64k    ****/
/****  APPFlash  0x08010000 - 0x08040000   64k - 256K   ****/

int main()
{
  u8 Data[32]="go to bootloader!\r\n";
  
  SysTick_Config(SystemCoreClock/1000000);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  shell_init();
  
  TIM5_Init();
  
  init_wifi_network();
  IWDG_Init();


  rt_kprintf("*******************************************\r\n");
  rt_kprintf("%s, time=%s date=%s\r\n",(char*)Data, __TIME__,__DATE__);
  rt_kprintf("*******************************************\r\n");

  region_header_init(&region_header);
  while(1)
  {
    IWDG_Feed();
    shell_process();
	periodic_task_process();
    wifi_msg_process();
  }
}


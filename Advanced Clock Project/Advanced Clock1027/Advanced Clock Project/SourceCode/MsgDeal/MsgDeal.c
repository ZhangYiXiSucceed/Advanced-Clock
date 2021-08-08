#include "main.h"

struct SystemVar system_var;
struct SystemData system_data;
void Uart_AT_Deal()
{
    if( UART1FramInFlag == 1 )  // ´òÓ¡¿Ú
    {
        UART1FramInFlag = 0;
        FrameInlen 	    = Uart1Read(FrameInBuff);
        //rt_kprintf((char*)FrameInBuff);
        PrintfIOTPort4(FrameInBuff,FrameInlen);
        //UART1_SentMsgL(FrameInBuff,FrameInlen);
        
    }
    if(system_var.TwoMinuteFlag ==1)
    {
        system_var.TwoMinuteFlag = 0;
		get_network_time_cmds();
        rt_kprintf("*: two minute\r\n"); 
    }
}

void QueueInfoPrint()
{
  if(QueueOut(&MyQueue, FrameInBuff, &FrameInlen) == QueueOperateOk) 
  {
    UART1_SentMsgL(FrameInBuff,FrameInlen);
    memset(FrameInBuff,0x00,sizeof(FrameInBuff));
  }
}



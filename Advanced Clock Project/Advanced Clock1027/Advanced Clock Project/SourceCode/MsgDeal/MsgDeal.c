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
    }
    if(system_var.TwoMinuteFlag ==1)
    {
        system_var.TwoMinuteFlag = 0;
        rt_kprintf("*: two minute\r\n"); 
    }
}


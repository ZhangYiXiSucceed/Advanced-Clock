#include "main.h"



__attribute__((section("BANK_SEC"),used))  static  void bank_handler(u8 bank_id, u8 func_id)
{
    switch(func_id)
    {
        case 2:
        {
            rt_kprintf("this is bank func,bank id=%d func id=%d\r\n", bank_id, func_id);
        }break;
        default:
        rt_kprintf("this is bank func,func id=%d,err\r\n", func_id);
        break;
    }
}


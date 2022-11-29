#include "main.h"

void bluetooth_msg_porcess()
{
  if(queue_out(&ble_queue, FrameInBuff, &FrameInlen) == QUEUE_OPER_OK) 
  {
    UART2_SentMsgL(FrameInBuff,FrameInlen);
    memset(FrameInBuff,0x00,sizeof(FrameInBuff));
  }
}


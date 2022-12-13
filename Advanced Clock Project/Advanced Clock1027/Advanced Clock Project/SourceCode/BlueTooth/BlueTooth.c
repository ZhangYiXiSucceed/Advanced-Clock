#include "main.h"

void BleStateCheck(char *data)
{
	unsigned char *point;
    point = strstr(data, "GetWifi");               // IP_CONNECT 
    if(point != NULL)
    {
      return RESP_BLE_GET_WIFI_INFO;
    }
}

void bluetooth_msg_porcess()
{
  unsigned char mrtn;
  if(queue_out(&ble_queue, FrameInBuff, &FrameInlen) == QUEUE_OPER_OK) 
  {
	  memset(&FrameInBuff[FrameInlen],0x00,RX_BUFFER_SIZE - FrameInlen); 
	  UART2_SentMsgL(FrameInBuff,FrameInlen);
	  switch(sOperCmdBuff.tid)
	  {
	  		 case AT_CMD:                                                                 // 2�����CESQ
		      mrtn = BleStateCheck((char*)FrameInBuff);
		      if(mrtn == RESP_WIFI_OK)
		      {                                                                     // recv ok
		          sOperCmdBuff.tid = 0xff;

		      }
		      break;
			  default:
			  	break;
	  }
  }
}


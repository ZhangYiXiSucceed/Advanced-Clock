#include "main.h"

int BleStateCheck(char *data)
{
	unsigned char *point;
    point = strstr(data, "getinfo");               // IP_CONNECT 
    if(point != NULL)
    {
      return RESP_BLE_GET_WIFI_TIME_INFO;
    }
	point = strstr(data, "rst"); 
	if(point != NULL)
    {
      return RESP_BLE_RESET;
    }
	return -1;
}



void bluetooth_msg_porcess()
{
  unsigned char mrtn;
  if(queue_out(&ble_queue, FrameInBuff, &FrameInlen) == QUEUE_OPER_OK) 
  {
	  memset(&FrameInBuff[FrameInlen],0x00,RX_BUFFER_SIZE - FrameInlen); 
	  UART2_SentMsgL(FrameInBuff,FrameInlen);
	  rt_kprintf2("\r\n");
	  mrtn = BleStateCheck((char*)FrameInBuff);
	  switch(mrtn)
	  {
	    case RESP_BLE_GET_WIFI_TIME_INFO:
      	{
      		print_wifi_weather_time_info();
      	}break;
		case RESP_BLE_RESET:
		{
			NVIC_SystemReset();
		}break;
		default:
			break;
	  }
  }
}


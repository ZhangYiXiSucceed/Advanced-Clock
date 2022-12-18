#include "main.h"



void ble_init()
{
	
}

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
	point = strstr(data, "close"); 
	if(point != NULL)
    {
      return RESP_BLE_CLOSE_INT;
    }
	point = strstr(data, "open"); 
	if(point != NULL)
    {
      return RESP_BLE_OPEN_INT;
    }
	point = strstr(data, "nrf24"); 
	if(point != NULL)
    {
      return RESP_BLE_SEND_NRF24L01;
    }
	point = strstr(data, "nrftx"); 
	if(point != NULL)
    {
      return RESP_BLE_SET_TX;
    }
	point = strstr(data, "nrfrx"); 
	if(point != NULL)
    {
      return RESP_BLE_SET_RX;
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
	  UART1_SentMsgL(FrameInBuff,FrameInlen);
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
		case RESP_BLE_CLOSE_INT:
		{
			IntsStorage;
			StoreDisableInts;
		}break;
		case RESP_BLE_OPEN_INT:
		{
			IntsStorage = 0;
			RestoreInts;
		}break;
		case RESP_BLE_SEND_NRF24L01:
		{
			queue_in(&nrf24l01_queue,(char*)FrameInBuff,FrameInlen);
			rt_kprintf2("str=%s \r\n",FrameInBuff);
		}break;
		case RESP_BLE_SET_TX:
		{
			system_var.NRFTxFlag = 1;
			rt_kprintf2("set NRF24 TX \r\n");
		}break;
		case RESP_BLE_SET_RX:
		{
			system_var.NRFRxFlag = 1;
			rt_kprintf2("set NRF24 RX \r\n");
		}break;
		default:
			break;
	  }
  }
}


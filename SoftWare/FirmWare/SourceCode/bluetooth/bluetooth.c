#include "main.h"

unsigned char  ble_cmd[][32]=
{
  {"AT"},
  {"AT+NAME=Clock"},
  {"AT+BAUD=9600"},
};


void ble_init()
{
	queue_init(&ble_queue);
	FifoInit(&sOperCmdUnionFifo_ble);
	UART2Init();       /*BlueTooth uart*/
	
	sOperCmdBuff_ble.tid = 0xFF;

	
	sOperCmdUnion_ble.tid = BLE_AT_CMD;
	sOperCmdUnion_ble.cmd = BLE_AT_CMD;
	sOperCmdUnion_ble.len = strlen(ble_cmd[BLE_AT_CMD]);
	sOperCmdUnion_ble.trycnt = 3;
	memcpy(sOperCmdUnion_ble.buffer, &ble_cmd[BLE_AT_CMD], sOperCmdUnion_ble.len);
	FifoIn(&sOperCmdUnionFifo_ble,&sOperCmdUnion_ble);

	sOperCmdUnion_ble.tid = BLE_AT_SET_NAME_CMD;
	sOperCmdUnion_ble.cmd = BLE_AT_SET_NAME_CMD;
	sOperCmdUnion_ble.len = strlen(ble_cmd[BLE_AT_SET_NAME_CMD]);
	sOperCmdUnion_ble.trycnt = 3;
	memcpy(sOperCmdUnion_ble.buffer, &ble_cmd[BLE_AT_SET_NAME_CMD], sOperCmdUnion_ble.len);
	FifoIn(&sOperCmdUnionFifo_ble,&sOperCmdUnion_ble);

	sOperCmdUnion_ble.tid = BLE_AT_SET_BAUD_CMD;
	sOperCmdUnion_ble.cmd = BLE_AT_SET_BAUD_CMD;
	sOperCmdUnion_ble.len = strlen(ble_cmd[BLE_AT_SET_BAUD_CMD]);
	sOperCmdUnion_ble.trycnt = 3;
	memcpy(sOperCmdUnion_ble.buffer, &ble_cmd[BLE_AT_SET_BAUD_CMD], sOperCmdUnion_ble.len);
	FifoIn(&sOperCmdUnionFifo_ble,&sOperCmdUnion_ble);

	
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
	point = strstr(data, "OK"); 
	if(point != NULL)
    {
      return RESP_BLE_OK;
    }
	return -1;
}



void bluetooth_msg_porcess()
{
  unsigned char mrtn;
  if(sOperCmdBuff_ble.tid == 0xff)
  {
    if(FifoOut(&sOperCmdUnionFifo_ble, &sOperCmdBuff_ble) == ValFifoOperateOk)        
    {
        sOperCmdBuff_ble.time   = GetSystemTime();
        rt_kprintf("BLE-CMD:%d===>%s\r\n",sOperCmdBuff_ble.cmd,sOperCmdBuff_ble.buffer);
        PrintfIOTPort2(sOperCmdBuff_ble.buffer,sOperCmdBuff_ble.len);
    } 	
  }
  else
  {
      if(GetSystemTime()> sOperCmdBuff_ble.time + 10)                          
      {
          sOperCmdBuff_ble.trycnt = sOperCmdBuff_ble.trycnt -1;
          if(sOperCmdBuff_ble.trycnt)
          {
              sOperCmdBuff_ble.time   = GetSystemTime();
              rt_kprintf("*: rty:%d\r\n", sOperCmdBuff_ble.trycnt);
              PrintfIOTPort2(sOperCmdBuff_ble.buffer,sOperCmdBuff_ble.len);
          }
          else
          {                                   
            sOperCmdBuff_ble.tid = 0xff;
          }
       }
   }
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
			 
			 
		}break;
		case RESP_BLE_OPEN_INT:
		{
			 
			 
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
		case RESP_BLE_OK:
		{
			sOperCmdBuff_ble.tid = 0xff;
			rt_kprintf("ble ok \r\n");
		}break;
		default:
			break;
	  }
  }
}


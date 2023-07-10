#include "main.h"


unsigned char  WiFiNetCmd[][64]=
{
  {"AT\x0d\x0a"},
  {"AT+RST\x0d\x0a"},                                                              
  {"ATE0\x0d\x0a"},                                                         
  {"AT+CWMODE=1\x0d\x0a"},                                                        
  {"AT+CWJAP=\"ZhangYixiSucceed\",\"qazwsx123\"\x0d\x0a"}, 
#ifdef BOOT
  {"AT+CIPSTART=\"TCP\",\"192.168.0.141\",51230\x0d\x0a"},
#else
  {"AT+CIPSTART=\"TCP\",\"api.k780.com\",80\x0d\x0a"},	   
#endif 
  {"AT+CIPMODE=1\x0d\x0a"},            
  {"AT+CIPSEND\x0d\x0a"},                                                   
  {"+++"},                                         
  {"AT+CWQAP\x0d\x0a"},                                     
  {"AT+CEDRXS=0,5\x0d\x0a"},    
  {"AT+CWLAP=\"ZhangYixiSucceed\"\x0d\x0a"},
  {"AT+CIPCLOSE\x0d\x0a"},
  {"AT+CIPSTA?\x0d\x0a"},
  {"AT+CIPAPMAC?\x0d\x0a"},
  {"AT+CIPSTART=\"TCP\",\"192.168.0.141\",51230\x0d\x0a"},
};


char get_network_time_str[]="GET http://api.k780.com:88/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json\r\n";
char get_network_weather_str[]="GET http://api.k780.com/?app=weather.today&weaId=40&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json\r\n";
/*{"success":"1","result":{"timestamp":"1627798664","datetime_1":"2021-08-01 14:17:44","datetime_2":"2021年08月01日 14时17分44秒","week_1":"0","week_2":"星期日","week_3":"周日","week_4":"Sunday"}}*/
/*{"success":"1","result":{"weaid":"40","days":"2021-08-04","week":"星期三","cityno":"pudongxinqu","citynm":"浦东新区","cityid":"101020600","temperature":"30℃/26℃","temperature_curr":"28℃","humidity":"93%","aqi":"20","weather":"多云","weather_curr":"多云","weather_icon":"http://api.k780.com/upload/weather/d/1.gif","weather_icon1":"","wind":"东风","winp":"2级","temp_high":"30","temp_low":"26","temp_curr":"28","humi_high":"0","humi_low":"0","weatid":"2","weatid1":"","windid":"2","winpid":"2","weather_iconid":"1"}}*/
wifi_info_t wifi_info_g;

unsigned char get_wifi_info_data(unsigned char* frame_buffer,unsigned char frame_buffer_length, wifi_info_t *wifi_info);
unsigned char get_data_str_index(unsigned char* frame_buffer, int comma_num, int *index,char symbol);
unsigned char parsing_wifi_mac_data(unsigned char* frame_buffer,unsigned char frame_buffer_length,wifi_info_t *wifi_info);

void init_wifi_network()
{
	queue_init(&wifi_queue);
	FifoInit(&sOperCmdUnionFifo_wifi);
	UART4Init();       /*WIFI uart */
	
	sOperCmdBuff.tid = 0xFF;
	
	sOperCmdUnion_wifi.tid = AT_CMD;
	sOperCmdUnion_wifi.cmd = 0x00;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CMD]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CMD], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);

	sOperCmdUnion_wifi.tid = AT_RST;
	sOperCmdUnion_wifi.cmd = 0x01;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_RST]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_RST], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);

	sOperCmdUnion_wifi.tid = ATE0;
	sOperCmdUnion_wifi.cmd = 0x02;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[ATE0]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[ATE0], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);

	sOperCmdUnion_wifi.tid = AT_CWMODE;
	sOperCmdUnion_wifi.cmd = 0x03;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CWMODE]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CWMODE], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);

	quiry_wifi();

	connect_wifi_network();
	
#ifndef BOOT
	get_network_time_cmds();
#else
	connect_host();
#endif
}

void connect_wifi_network()
{
	/*
		WIFI CONNECTED
		WIFI GOT IP
	*/
	sOperCmdUnion_wifi.tid = AT_CWJAP;
	sOperCmdUnion_wifi.cmd = 0x04;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CWJAP]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CWJAP], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);

	/*  CMD:13===>AT+CIPSTA?
		OK
		
		+CIPSTA:     ip:"192.168.0.156"
		+CIPSTA:gateway:"192.168.0.1"
		+CIPSTA:netmask:"255.255.255.0" 
	*/
	sOperCmdUnion_wifi.tid = AT_QUIRY_IP;
	sOperCmdUnion_wifi.cmd = 13;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_QUIRY_IP]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_QUIRY_IP], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);

	/*
	
	*/
	sOperCmdUnion_wifi.tid = AT_QUIRY_MAC;
	sOperCmdUnion_wifi.cmd = 14;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_QUIRY_MAC]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_QUIRY_MAC], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
	
}

void stop_wifi_network()
{
	sOperCmdUnion_wifi.tid = AT_CWQAP;
	sOperCmdUnion_wifi.cmd = 0x09;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CWQAP]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CWQAP], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}

void connect_server()
{
	sOperCmdUnion_wifi.tid = AT_CIPSTART;
	sOperCmdUnion_wifi.cmd = 0x05;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CIPSTART]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CIPSTART], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}

void connect_host_ip()
{
	sOperCmdUnion_wifi.tid = AT_HOST_IP_CONNECT;
	sOperCmdUnion_wifi.cmd = 0x0F;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_HOST_IP_CONNECT]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_HOST_IP_CONNECT], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}


void set_send_mode(unsigned char mode)
{
	if(0 == mode)
		WiFiNetCmd[AT_CIPMODE][11] = '0';
	else
		WiFiNetCmd[AT_CIPMODE][11] = '1';
	sOperCmdUnion_wifi.tid = AT_CIPMODE;
	sOperCmdUnion_wifi.cmd = 0x06;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CIPMODE]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CIPMODE], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}
void entry_send_state()
{
	sOperCmdUnion_wifi.tid = AT_CIPSEND;
	sOperCmdUnion_wifi.cmd = 0x07;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CIPSEND]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CIPSEND], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}

void send_data(char* demo_data,unsigned char data_len)
{
	
	sOperCmdUnion_wifi.tid = AT_SEND_DATA;
	sOperCmdUnion_wifi.cmd = 0x07;
	sOperCmdUnion_wifi.len = data_len;
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, demo_data, sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}

void send_weather_data(char* demo_data,unsigned char data_len)
{
	
	sOperCmdUnion_wifi.tid = AT_QUIRY_WEATHER_DATA;
	sOperCmdUnion_wifi.cmd = 0x07;
	sOperCmdUnion_wifi.len = data_len;
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, demo_data, sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}


void get_network_time_cmds()
{
	connect_server();

	set_send_mode(1);
	entry_send_state();

	send_data(get_network_time_str,sizeof(get_network_time_str));

	send_weather_data(get_network_weather_str,sizeof(get_network_weather_str));

	quit_send_data_mode_cmd();

	set_send_mode(0);

	quit_network_connect_cmd();
}

void connect_host()
{
	connect_host_ip();
	set_send_mode(1);
	entry_send_state();
}

void leave_host()
{
	quit_send_data_mode_cmd();
	set_send_mode(0);
	quit_network_connect_cmd();
}

void quit_send_data_mode_cmd()
{
	sOperCmdUnion_wifi.tid = AT_STOP_SEND;
	sOperCmdUnion_wifi.cmd = 0x08;
	sOperCmdUnion_wifi.len = 3;
	sOperCmdUnion_wifi.trycnt = 1;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_STOP_SEND], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}

void quit_network_connect_cmd()
{
	sOperCmdUnion_wifi.tid = AT_QUIT_TCP_CONNECT;
	sOperCmdUnion_wifi.cmd = 0x0C;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_QUIT_TCP_CONNECT]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_QUIT_TCP_CONNECT], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}

void clear_wifi_op_req()
{
	u8 req_num = GetTsakFiFoCount(&sOperCmdUnionFifo_wifi);
	if(req_num >= WIFI_OP_REQ_NUM_MAX)
	{
		int i;
		struct OperCmdUnion sdat;
		for(i=0;i<req_num;i++)
		{
			FifoOut(&sOperCmdUnionFifo_wifi,&sdat);
		}
		rt_kprintf("wifi op err too many,num=%d\r\n",req_num);
	}
}

void quiry_wifi()
{
	
	sOperCmdUnion_wifi.tid = AT_QUIRY_WIFI;
	sOperCmdUnion_wifi.cmd = 0x0B;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_QUIRY_WIFI]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_QUIRY_WIFI], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo_wifi,&sOperCmdUnion_wifi);
}


int8_t WifiStateCheck(char *data)
{   
    char *point;
    // rtn:1
    
    point = strstr(data, "CONNECT");               // IP_CONNECT 
    if(point != NULL)
    {
      return RESP_IP_CONNECT;
    }
    
    // rtn:3
    point = strstr(data, ">");                       // >
    if(point != NULL)
    {
      return RESP_SEND_MODE;
    }
    
    // rtn:4
    point = strstr(data, "WIFI");             // WIFI CONNECTED   WIFI GOT IP
    if(point != NULL)
    {
      return RESP_WIFI_CONNECT;
    }
    // rtn:5
    point = strstr(data, "+CWLAP:(");         // +CWLAP:(4,"ZhangYixiSucceed",-38,"50:2b:73:f6:92:d1",2,10,0,5,3,7,1)
    if(point != NULL)
    {
      return RESP_LIST_WIFI_INFO;
    }  
	
	point = strstr(data, "+CIPSTA");      // +CIPSTA:ip:"192.168.0.156" + CIPSTA:gateway:"192.168.0.1" + CIPSTA:netmask:"255.255.255.0"

    if(point != NULL)
    {
      return RESP_GET_WIFI_IP;
    }

	point = strstr(data, "+CIPAPMAC");      // +CIPAPMAC:"42:f5:20:05:19:44"
    if(point != NULL)
    {
      return RESP_GET_WIFI_MAC;
    }
    
    point = strstr(data, "ERR");      // ERR
    if(point != NULL)
    {
      return RESP_ERR;
    }

	point = strstr(data, "OK");      			   // OK
    if(point != NULL)
    {
      return RESP_WIFI_OK;
    }
    return -1;
}

void wifi_msg_process()
{
  int mrtn;
  if(sOperCmdBuff.tid == AT_IDLE_CMD)
  {
    if(FifoOut(&sOperCmdUnionFifo_wifi, &sOperCmdBuff) == ValFifoOperateOk)          //������������Ƿ�������
    {
                                                          //������ݽ���
        sOperCmdBuff.time   = GetSystemTime();
        //PrintCmd(sOperCmdBuff.cmd);
        rt_kprintf("CMD:%d===>%s\r\n",sOperCmdBuff.cmd,sOperCmdBuff.buffer);
        PrintfIOTPort4(sOperCmdBuff.buffer,sOperCmdBuff.len);
    } 	
  }
  else
  {
      if(GetSystemTime()> sOperCmdBuff.time + 10)                            //���������ش�
      {
          sOperCmdBuff.trycnt = sOperCmdBuff.trycnt -1;
          if(sOperCmdBuff.trycnt)
          {
              sOperCmdBuff.time   = GetSystemTime();
              rt_kprintf("*: rty:%d\r\n", sOperCmdBuff.trycnt);
              PrintfIOTPort4(sOperCmdBuff.buffer,sOperCmdBuff.len);
          }
          else
          {                                   
            sOperCmdBuff.tid = 0xff;
          }
       }
   	}
     if(queue_out(&wifi_queue, FrameInBuff, &FrameInlen) == QUEUE_OPER_OK)         // ����IOT�˿�����        
  	{
		  memset(&FrameInBuff[FrameInlen],0x00,RX_BUFFER_SIZE - FrameInlen); 
		  switch(sOperCmdBuff.tid)
		  {
		  		 case AT_CMD:                                                                 // 2�����CESQ
			      mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_WIFI_OK)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
			      break;
				 case AT_RST:                                                                 // 2�����CESQ
			      mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_WIFI_OK)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
					  delay_ms(1000);

			      }
			      break;
			    case ATE0:                                                                 // 2�����CESQ
				{	mrtn = WifiStateCheck((char*)FrameInBuff);
					if(mrtn == RESP_WIFI_OK)
					{                                                                     // recv ok
					  sOperCmdBuff.tid = 0xff;
					}
			    }
			    break;
			    case AT_CWMODE:
				{
					mrtn = WifiStateCheck((char*)FrameInBuff);
					if(mrtn == RESP_WIFI_OK)
					{                                                                     // recv ok
					sOperCmdBuff.tid = 0xff;

					}
				}
				break;
				case AT_CWJAP:
				{
				 mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_WIFI_CONNECT)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
					  system_var.WIFIConnectFlag = 1;
					  delay_ms(1000);

			      }
				  else if(mrtn == RESP_ERR)
				  {
					  connect_wifi_network();
					  system_var.WIFIConnectFlag = 0;
				  }
				}
				break;
				case AT_CIPSTART:
				{
				 mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_IP_CONNECT)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
						
			      }
				}
				break;
				case AT_CIPMODE:
				{
				 mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_WIFI_OK)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
			      }
				  else
				  {
					  clear_wifi_op_req();
					  sOperCmdBuff.tid = 0xff;
				  }
				}
				break;
				case AT_CIPSEND:
				{
				 mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_SEND_MODE)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
				}
				break;
				case AT_CWQAP:
				{
				 mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_WIFI_OK)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
				}
				break;
#ifndef BOOT
				case AT_SEND_DATA:
				{
				 	//rt_kprintf("%s\r\n",FrameInBuff);
					parsing_time_json_info(FrameInBuff,FrameInlen);
					if(0 == system_var.TimeGetFlag)
					{
						system_var.TimeGetFlag = 1;
					}
				 	sOperCmdBuff.tid = 0xff;
				}
				break;
				case AT_QUIRY_WEATHER_DATA:
				{
				 	//rt_kprintf("%s\r\n",FrameInBuff);
					parsing_weather_json_info(FrameInBuff,FrameInlen);
				 	sOperCmdBuff.tid = 0xff;
				}
				break;
#endif
				case AT_QUIRY_WIFI:
				{
				  mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_LIST_WIFI_INFO)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
					  int signal_numer = parsing_wifi_signal_info(FrameInBuff,FrameInlen);
					  wifi_info_g.wifi_signal_num = signal_numer;
					  rt_kprintf("signal_numer = %d\r\n", signal_numer);
			      }
				}
				break;
				case AT_QUIRY_IP:
				{
				  mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_GET_WIFI_IP)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
					  get_wifi_info_data(FrameInBuff,FrameInlen,&wifi_info_g);
			      }				  
				}
				break;
				case AT_QUIRY_MAC:
				{
				  mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_GET_WIFI_MAC)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
					  parsing_wifi_mac_data(FrameInBuff,FrameInlen,&wifi_info_g);
			      }				  
				}
				break;
				case AT_QUIT_TCP_CONNECT:
				{
				  mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_WIFI_OK)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
			      }
				  else
				  {
					clear_wifi_op_req();
					sOperCmdBuff.tid = 0xff;
				  }
				}
				break;
				case AT_HOST_IP_CONNECT:
				{
				  mrtn = WifiStateCheck((char*)FrameInBuff);
			      if(mrtn == RESP_IP_CONNECT)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
			      }
				  else
				  {
					clear_wifi_op_req();
					sOperCmdBuff.tid = 0xff;
				  }
				}
				break;
				case AT_RECIVE_CMD:
				{
					server_msg_process(FrameInBuff,FrameInlen);
				}break;
				default:
				{
					server_msg_process(FrameInBuff,FrameInlen);
				}
				break;
		 }
    }
}

unsigned char parsing_wifi_mac_data(unsigned char* frame_buffer,unsigned char frame_buffer_length,wifi_info_t *wifi_info)
{
	unsigned char get_info_flag = Z_TRUE;
	int index[2];
	if(strstr(frame_buffer,"MAC"))
	{
		unsigned char is_find_flag = get_data_str_index(frame_buffer,1,index,QUOTATION);
		if(is_find_flag)
		{
			memcpy(wifi_info->mac,&frame_buffer[index[0]+1],(index[1] - index[0] - 1));
		}
		else
		{
			rt_kprintf("MAC:find err\r\n");
		}
	}
	else
	{
		get_info_flag = Z_FALSE;
		rt_kprintf("parsing err\n");
	}	
	return get_info_flag;
}
unsigned char get_wifi_info_data(unsigned char* frame_buffer,unsigned char frame_buffer_length,wifi_info_t *wifi_info)
{
	/*  
		CMD:13===>AT+CIPSTA?
		OK
		
		+CIPSTA:     ip:"192.168.0.156"
		+CIPSTA:gateway:"192.168.0.1"
		+CIPSTA:netmask:"255.255.255.0" 
	*/
	unsigned char get_info_flag = Z_TRUE;
	int index[2];
	if(strstr(frame_buffer,"ip"))
	{
		unsigned char is_find_flag = get_data_str_index(frame_buffer,1,index,QUOTATION);
		if(is_find_flag)
		{
			memcpy(wifi_info->ip,&frame_buffer[index[0]+1],(index[1] - index[0] - 1));
		}
		else
		{
			rt_kprintf("IP:find err\r\n");
		}
	}
	else
	{
		get_info_flag = Z_FALSE;
		rt_kprintf("parsing err\n");
	}	
	if(strstr(frame_buffer,"gateway"))
	{
		unsigned char is_find_flag = get_data_str_index(frame_buffer,3,index,QUOTATION);
		if(is_find_flag)
		{	
			memcpy(wifi_info->gateway,&frame_buffer[index[0]+1],(index[1] - index[0] - 1));
		}
		else
		{
			rt_kprintf("gateway:find err\r\n");
		}
	}
	else
	{
		get_info_flag = Z_FALSE;
		rt_kprintf("parsing err\n");
	}
	if(strstr(frame_buffer,"netmask"))
	{
		unsigned char is_find_flag = get_data_str_index(frame_buffer,5,index,QUOTATION);
		if(is_find_flag)
		{	
			memcpy(wifi_info->netmask,&frame_buffer[index[0]+1],(index[1] - index[0] - 1));
		}
		else
		{
			rt_kprintf("netmark:find err\r\n");
		}
	}
	else
	{
		get_info_flag = Z_FALSE;
		rt_kprintf("parsing err\n");
	}
	return get_info_flag;
}

unsigned char get_data_str_index(unsigned char* frame_buffer, int comma_num, int *index,char symbol)
{
	int i=0;
	int comma_count = 0;
	unsigned char first_flag = Z_FALSE;
	unsigned char is_find_flag = Z_FALSE;
	while('\0' != frame_buffer[i])
	{                
		if(symbol == frame_buffer[i])
			comma_count++;

		if((comma_num == comma_count) &&  (Z_FALSE == first_flag))
		{
			index[0]=i;
			first_flag =Z_TRUE;
		}
		if((comma_num + 1) == comma_count)
		{
			index[1]=i;
			is_find_flag = Z_TRUE;
			break;
		}
		i++;
	}
	return is_find_flag;
}
int parsing_wifi_signal_info(unsigned char* frame_buffer,unsigned char frame_buffer_length)
{
	int signal_numer = 0;
	int i=0;
	int symbol_num = 0;

	int index[2]={0,0};

	/*+CWLAP:(4,"ZhangYixiSucceed",-41,"50:2b:73:f6:92:d1",2,11,0,5,3,7,1)*/
	unsigned char symbol_flag = 0;

	unsigned char is_find_flag = get_data_str_index(frame_buffer,2,index,COMMA);
    if(Z_FALSE ==  is_find_flag)
	{
		rt_kprintf("dont find the wifi str\n");
		signal_numer = 0;
		return signal_numer;
	}
	for(i=index[0]+1;i<(index[1]);i++)
	{
		if(MINUS == frame_buffer[i])
		{
			symbol_flag=1;
		}
		else
		{
			signal_numer = signal_numer*10+(frame_buffer[i]-'0');
		}
	}
	signal_numer = (1 == symbol_flag)?(-signal_numer):(signal_numer);
	return signal_numer;
}

#ifndef BOOT
time_and_weather_t  time_and_weather_g;

void parsing_weather_json_info(unsigned char* frame_buffer,unsigned short frame_buffer_length)
{
	cJSON *root = cJSON_Parse(frame_buffer);
	if (0 == root)
	{
		rt_kprintf("error\n");
		return;
	}
	//rt_kprintf("%s\n\n", cJSON_Print(root));

	cJSON *success = cJSON_GetObjectItem(root, "success");
	if (0 == success)
		return;
	rt_kprintf("success name:%s success value:%s\r\n", success->string, success->valuestring);
	
	cJSON *result = cJSON_GetObjectItem(root, "result");
	if (0 == result)
		return;
	
	cJSON *weaid = cJSON_GetObjectItem(result, "weaid");
	rt_kprintf("weaid value:%s\r\n", weaid->valuestring);

	cJSON *days = cJSON_GetObjectItem(result, "days");
	rt_kprintf("days value:%s\r\n", days->valuestring);

	cJSON *cityno = cJSON_GetObjectItem(result, "cityno");
	rt_kprintf("cityno value:%s\r\n", cityno->valuestring);

	cJSON *temperature = cJSON_GetObjectItem(result, "temperature");
	rt_kprintf("temperture value:%s\r\n", temperature->valuestring);
	
	cJSON *temperature_curr = cJSON_GetObjectItem(result, "temperature_curr");
	rt_kprintf("temperture_curr value:%s\r\n", temperature_curr->valuestring);
	
	cJSON *humidity = cJSON_GetObjectItem(result, "humidity");
	rt_kprintf("humidity value:%s\r\n", humidity->valuestring);
	
	cJSON *aqi = cJSON_GetObjectItem(result, "aqi");
	rt_kprintf("aqi value:%s\r\n", aqi->valuestring);

	cJSON *weather = cJSON_GetObjectItem(result, "weather");

	time_and_weather_g.tempeture =  parsing_the_str(temperature_curr->valuestring);
	time_and_weather_g.api       =  parsing_the_str(aqi->valuestring);
	time_and_weather_g.humidty   =  parsing_the_str(humidity->valuestring);
	time_and_weather_g.city_id   =  parsing_the_str(weaid->valuestring);
	
	memcpy(time_and_weather_g.city,cityno->valuestring,strlen(cityno->valuestring));
	memcpy(time_and_weather_g.weather,weather->valuestring,strlen(weather->valuestring));
	cJSON_Delete(root);
}

void print_wifi_weather_time_info()
{
	rt_kprintf2("**********weather and time**********\r\n");
	
	rt_kprintf2("date:%d-%d-%d,week=%d\r\n",time_and_weather_g.year,time_and_weather_g.month,\
											time_and_weather_g.day,time_and_weather_g.week);
	rt_kprintf2("time:%d-%d-%d\r\n",time_and_weather_g.hour,time_and_weather_g.minute,\
											time_and_weather_g.second);
	
	rt_kprintf2("weather:temp=%d C humidty=%d %% api=%d\r\n",time_and_weather_g.tempeture,
											time_and_weather_g.humidty,\
											time_and_weather_g.api);
	rt_kprintf2("city_id=%d,city=%s\r\n",time_and_weather_g.city_id,\
											time_and_weather_g.city);

	rt_kprintf2("**********wifi**********\r\n");
	
	rt_kprintf2("ip=%s\r\n",wifi_info_g.ip);
	rt_kprintf2("gateway=%s\r\n",wifi_info_g.gateway);
	rt_kprintf2("netmark=%s\r\n",wifi_info_g.netmask);
	rt_kprintf2("mac = %s\r\n",wifi_info_g.mac);
	rt_kprintf2("signal=%d\r\n",wifi_info_g.wifi_signal_num);
}
void parsing_time_json_info(unsigned char* frame_buffer,unsigned char frame_buffer_length)
{
	cJSON *root = cJSON_Parse(frame_buffer);
	if (0 == root)
	{
		rt_kprintf("error\n");
		return;
	}
	//
	//rt_kprintf("%s\n\n", cJSON_Print(root));

	cJSON *success = cJSON_GetObjectItem(root, "success");
	if (0 == success)
		return;
	rt_kprintf("success name:%s success value:%s\r\n", success->string, success->valuestring);
	
	cJSON *result = cJSON_GetObjectItem(root, "result");
	if (0 == result)
		return;
	
	cJSON *timestamp = cJSON_GetObjectItem(result, "timestamp");
	rt_kprintf("timestamp value:%s\r\n", timestamp->valuestring);

	cJSON *datetime_1 = cJSON_GetObjectItem(result, "datetime_1");
	rt_kprintf("datetime_1 value:%s\r\n", datetime_1->valuestring);

	cJSON *datetime_2 = cJSON_GetObjectItem(result, "datetime_2");
	rt_kprintf("datetime_1 value:%s\r\n", datetime_2->valuestring);

	cJSON *week_1 = cJSON_GetObjectItem(result, "week_1");
	rt_kprintf("timestamp value:%s\r\n", week_1->valuestring);
	cJSON *week_2 = cJSON_GetObjectItem(result, "week_2");
	rt_kprintf("timestamp value:%s\r\n", week_2->valuestring);
	cJSON *week_3 = cJSON_GetObjectItem(result, "week_3");
	rt_kprintf("timestamp value:%s\r\n", week_3->valuestring);
	cJSON *week_4 = cJSON_GetObjectItem(result, "week_4");
	rt_kprintf("timestamp value:%s\r\n", week_4->valuestring);
	
	paraing_time_string(datetime_1->valuestring,week_1->valuestring);
	
	cJSON_Delete(root);
}


void paraing_time_string(char* temp_time_date_str,char* temp_week)
{
	unsigned char year,month,day;
	unsigned char hour,min,sec,week;

	year = (temp_time_date_str[2]-'0')*10+(temp_time_date_str[3]-'0');
	month = (temp_time_date_str[5]-'0')*10+(temp_time_date_str[6]-'0');
	day = (temp_time_date_str[8]-'0')*10+(temp_time_date_str[9]-'0');

	hour = (temp_time_date_str[11]-'0')*10+(temp_time_date_str[12]-'0');
	min = (temp_time_date_str[14]-'0')*10+(temp_time_date_str[15]-'0');
	sec = (temp_time_date_str[17]-'0')*10+(temp_time_date_str[18]-'0');	

	
	if('0' == temp_week[0])
		week = RTC_Weekday_Sunday;
	else
		week = temp_week[0]-'0';
	
	RTC_Set_Date(year,month,day,week);
	RTC_Set_Time(hour,min,sec,RTC_H12_PM);	
	
	time_and_weather_g.year   = year;
	time_and_weather_g.month  = month;
	time_and_weather_g.day    = day;

	time_and_weather_g.hour   = hour;
	time_and_weather_g.minute = min;
	time_and_weather_g.second = sec;

	time_and_weather_g.week   = week;
}

int parsing_the_str(char* str)
{
	char* temp_str = str;
	if(NULL == temp_str)
	{
		return -1;
	}

	int num = 0;
	while((*temp_str >= '0' ) && (*temp_str <= '9'))
	{		
		num = num*10 + (*temp_str - '0');
		temp_str++;		
	}
	return num;
}

#endif











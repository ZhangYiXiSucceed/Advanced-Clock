#include "main.h"


unsigned char  WiFiNetCmd[][100]=
{
  {"AT\x0d\x0a"},
  {"AT+RST\x0d\x0a"},                                                              
  {"ATE0\x0d\x0a"},                                                         
  {"AT+CWMODE=1\x0d\x0a"},                                                        
  {"AT+CWJAP=\"ZhangYixiSucceed\",\"qazwsx123\"\x0d\x0a"},                                                     
  {"AT+CIPSTART=\"TCP\",\"api.k780.com\",80\x0d\x0a"},            
  {"AT+CIPMODE=1\x0d\x0a"},            
  {"AT+CIPSEND\x0d\x0a"},                                                   
  {"+++"},                                         
  {"AT+CWQAP\x0d\x0a"},                                     
  {"AT+CEDRXS=0,5\x0d\x0a"},    
  {"AT+CWLAP=\"ZhangYixiSucceed\"\x0d\x0a"},
  {"AT+CIPCLOSE\x0d\x0a"}
};


char get_network_time_str[]="GET http://api.k780.com:88/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json\r\n";

char get_network_weather_str[]="GET http://api.k780.com/?app=weather.today&weaId=40&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json\r\n";
/*{"success":"1","result":{"timestamp":"1627798664","datetime_1":"2021-08-01 14:17:44","datetime_2":"2021年08月01日 14时17分44秒","week_1":"0","week_2":"星期日","week_3":"周日","week_4":"Sunday"}}*/
/*{"success":"1","result":{"weaid":"40","days":"2021-08-04","week":"星期三","cityno":"pudongxinqu","citynm":"浦东新区","cityid":"101020600","temperature":"30℃/26℃","temperature_curr":"28℃","humidity":"93%","aqi":"20","weather":"多云","weather_curr":"多云","weather_icon":"http://api.k780.com/upload/weather/d/1.gif","weather_icon1":"","wind":"东风","winp":"2级","temp_high":"30","temp_low":"26","temp_curr":"28","humi_high":"0","humi_low":"0","weatid":"2","weatid1":"","windid":"2","winpid":"2","weather_iconid":"1"}}*/


void init_wifi_network()
{
	sOperCmdBuff.tid = 0xFF;
	
	sOperCmdUnion_wifi.tid = AT_CMD;
	sOperCmdUnion_wifi.cmd = 0x00;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CMD]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CMD], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);

	sOperCmdUnion_wifi.tid = AT_RST;
	sOperCmdUnion_wifi.cmd = 0x01;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_RST]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_RST], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);

	sOperCmdUnion_wifi.tid = ATE0;
	sOperCmdUnion_wifi.cmd = 0x02;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[ATE0]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[ATE0], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);

	sOperCmdUnion_wifi.tid = AT_CWMODE;
	sOperCmdUnion_wifi.cmd = 0x03;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CWMODE]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CWMODE], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);

	quiry_wifi();
	
	connect_wifi_network();
	
	get_network_time_cmds();
	
	sOperCmdUnion_wifi.tid = AT_CMD;
	sOperCmdUnion_wifi.cmd = 0x00;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CMD]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CMD], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
	
}

void connect_wifi_network()
{
	sOperCmdUnion_wifi.tid = AT_CWJAP;
	sOperCmdUnion_wifi.cmd = 0x04;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CWJAP]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CWJAP], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
}

void stop_wifi_network()
{
	sOperCmdUnion_wifi.tid = AT_CWQAP;
	sOperCmdUnion_wifi.cmd = 0x09;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CWQAP]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CWQAP], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
}

void connect_server()
{
	
	sOperCmdUnion_wifi.tid = AT_CIPSTART;
	sOperCmdUnion_wifi.cmd = 0x05;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CIPSTART]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CIPSTART], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
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
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
}
void entry_send_state()
{
	
	sOperCmdUnion_wifi.tid = AT_CIPSEND;
	sOperCmdUnion_wifi.cmd = 0x07;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_CIPSEND]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_CIPSEND], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
}

void send_data(char* demo_data,unsigned char data_len)
{
	
	sOperCmdUnion_wifi.tid = AT_SEND_DATA;
	sOperCmdUnion_wifi.cmd = 0x07;
	sOperCmdUnion_wifi.len = data_len;
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, demo_data, sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
}

void send_weather_data(char* demo_data,unsigned char data_len)
{
	
	sOperCmdUnion_wifi.tid = AT_QUIRY_WEATHER_DATA;
	sOperCmdUnion_wifi.cmd = 0x07;
	sOperCmdUnion_wifi.len = data_len;
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, demo_data, sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
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

void quit_send_data_mode_cmd()
{
	sOperCmdUnion_wifi.tid = AT_STOP_SEND;
	sOperCmdUnion_wifi.cmd = 0x08;
	sOperCmdUnion_wifi.len = 3;
	sOperCmdUnion_wifi.trycnt = 1;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_STOP_SEND], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
}

void quit_network_connect_cmd()
{
	sOperCmdUnion_wifi.tid = AT_QUIT_TCP_CONNECT;
	sOperCmdUnion_wifi.cmd = 0x0C;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_QUIT_TCP_CONNECT]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_QUIT_TCP_CONNECT], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
}


void quiry_wifi()
{
	
	sOperCmdUnion_wifi.tid = AT_QUIRY_WIFI;
	sOperCmdUnion_wifi.cmd = 0x0B;
	sOperCmdUnion_wifi.len = sizeof(WiFiNetCmd[AT_QUIRY_WIFI]);
	sOperCmdUnion_wifi.trycnt = 3;
	memcpy(sOperCmdUnion_wifi.buffer, &WiFiNetCmd[AT_QUIRY_WIFI], sOperCmdUnion_wifi.len);
	FifoIn(&sOperCmdUnionFifo,&sOperCmdUnion_wifi);
}
void wifi_task_deal()
{
  int mrtn;
  if(sOperCmdBuff.tid == 0xff)
  {
    if(FifoOut(&sOperCmdUnionFifo, &sOperCmdBuff) == ValFifoOperateOk)          //������������Ƿ�������
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
		  rt_kprintf("%s\r\n",FrameInBuff);
		  switch(sOperCmdBuff.tid)
		  {
		  		 case AT_CMD:                                                                 // 2�����CESQ
			      mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 1)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
			      break;
				 case AT_RST:                                                                 // 2�����CESQ
			      mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 1)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
					  delay_ms(1000);

			      }
			      break;
			    case ATE0:                                                                 // 2�����CESQ
			      mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 1)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
			      break;
			    case AT_CWMODE:
				{
				 mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 1)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
				}
				break;
				case AT_CWJAP:
				{
				 mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 4)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
					  system_var.WIFIConnectFlag =1;

			      }
				  else if(mrtn == 0)
			  	{
			  		connect_wifi_network();
					system_var.WIFIConnectFlag =0;
			  	}
				}
				break;
				case AT_CIPSTART:
				{
				 mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 2)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
				}
				break;
				case AT_CIPMODE:
				{
				 mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 1)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
				}
				break;
				case AT_CIPSEND:
				{
				 mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 3)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
				}
				break;
				case AT_CWQAP:
				{
				 mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 1)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;

			      }
				}
				break;
				

				case AT_SEND_DATA:
				{
				 	//rt_kprintf("%s\r\n",FrameInBuff);
					parsing_time_json_info(FrameInBuff,FrameInlen);
					system_var.TimeGetFlag = 1;
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
				case AT_QUIRY_WIFI:
				{
				 mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 5)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
					  parsing_wifi_signal_info(FrameInBuff,FrameInlen);
			      }
				  
				}
				break;
				case AT_QUIT_TCP_CONNECT:
				{
				 mrtn = StateCheck((char*)FrameInBuff);
			      if(mrtn == 1)
			      {                                                                     // recv ok
			          sOperCmdBuff.tid = 0xff;
					 
			      }
				  
				}
				break;
				default:
				{
				
				}
				break;
		 }
    }
}


void parsing_wifi_signal_info(unsigned char* frame_buffer,unsigned char frame_buffer_length)
{
	int signal_numer;
	int i=0;
	int symbol_num = 0;

	int index[2]={0,0};

	unsigned char symbol_flag = 0;
	while('\0' != frame_buffer[i])
	{
        //rt_kprintf("%c\r\n",frame_buffer[i]);
                
		if(',' == frame_buffer[i])
			symbol_num++;

		if((2 == symbol_num) &&  (0 == symbol_flag))
		{
			index[0]=i;
			symbol_flag =1;
		}
		if(3 == symbol_num)
		{
			index[1]=i;
			break;
		}
		i++;
	}
	symbol_flag = 0;
	for(i=index[0]+1;i<(index[1]);i++)
	{
			if('-' == frame_buffer[i])
			{
				symbol_flag=1;
			}
			else
			{
				signal_numer = signal_numer*10+(frame_buffer[i]-'0');
			}
	}
	signal_numer = (1 == symbol_flag)?(-signal_numer):signal_numer;

	rt_kprintf("signal_numer = %d\r\n", signal_numer);
}


void parsing_weather_json_info(unsigned char* frame_buffer,unsigned short frame_buffer_length)
{
	cJSON *root = cJSON_Parse(frame_buffer);
	if (0 == root)
	{
		rt_kprintf("error\n");
		return;
	}
	//rt_kprintf("%s\n", "有格式的方式打印Json:");
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

	
	
	cJSON_Delete(root);
}


void parsing_time_json_info(unsigned char* frame_buffer,unsigned char frame_buffer_length)
{

	rt_kprintf("%s\r\n", frame_buffer);
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


	
	
	RTC_Set_Time(hour,min,sec,RTC_H12_PM);	//�1�7�1�7�1�7�1�7�0�2�1�7�1�7
	if('0' == temp_week[0])
    	RTC_Set_Date(year,month,day,RTC_Weekday_Sunday);
	else
		RTC_Set_Date(year,month,day,temp_week[0]-'0');
	
}


void wifi_frame_deal(unsigned char* frame_buffer,unsigned char frame_buffer_length)
{
	
}















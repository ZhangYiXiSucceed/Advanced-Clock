#include "main.h"


show_state_t  oled_show_state_g = init_show;
u8 picture_frame_buf_g[2][PICTURE_FRMAE_SIZE];
u8 buf_control_flag_g;

extern time_and_weather_t time_and_weather_g;
extern wifi_info_t wifi_info_g;

extern unsigned char connect[];
extern unsigned char f_city[][16];
extern unsigned char battery_bmp1[];
extern unsigned char battery_bmp2[];
extern unsigned char battery_bmp3[];
extern unsigned char battery_bmp4[];

extern unsigned char signal_bmp1[];
extern unsigned char signal_bmp2[];
extern unsigned char signal_bmp3[];
extern unsigned char signal_idle[];

extern unsigned char advanced_clock[];
extern unsigned char eight_diagrams_bmp[];
extern unsigned char weather_icon[][84];

void wifi_bmp_clear()
{
	uint8_t x,y;
	
	for(y=1;y<3;y++)
	{	
		OLED_Set_Pos(0,y);
		for(x=0;x<128;x++)
		{
			OLED_WR_Byte(0x00,OLED_DATA);
		}
			
	}
}

void set_show_state_change(show_state_t state)
{
	oled_show_state_g = state;
}


void show_interface_oled()
{
	static unsigned int temp_sys_time = 0;
	static unsigned char index = 0;
	static unsigned char fresh_once_flag[10] = {0};
	static unsigned char fresh_once_flag_last[10]={0};
	switch(oled_show_state_g)
	{
		case init_show:
		{
			OLED_DrawBMP(0,0,64,64,advanced_clock);
			show_common_string(72,0,"clock",font_size8X16);
			show_common_string(64,2,"zyixi.com ",font_size6X8);
			show_common_string(72,4,"V1.0.0",font_size6X8);
			if(0 == temp_sys_time)
			{
				temp_sys_time = system_data.SystemGMTTime;
			}
			else if(system_data.SystemGMTTime > (temp_sys_time+3))
			{
				set_show_state_change(eight_diagrams);
				OLED_Clear();
			}
		}break;
		case eight_diagrams:
		{
			OLED_DrawBMP(0,0,64,128,eight_diagrams_bmp);
			if(0 == temp_sys_time)
			{
				temp_sys_time = system_data.SystemGMTTime;
			}
			else if(system_data.SystemGMTTime > (temp_sys_time+3))
			{
				set_show_state_change(wifi_connect);
				OLED_Clear();
			}
		}break;
		case wifi_connect:
		{
			if(system_var.TimeGetFlag == 0)
			{	
				if(system_var.Time_20_ms_flag)
				{
					system_var.Time_20_ms_flag=0;
					wifi_bmp_clear();
					OLED_DrawBMP(index,1,16,16,connect);
					index+=8;;
					if(index>=128)
						index=0;
				}
				show_line(0,0);
				show_line(0,3);
				show_common_string(0,4,"wifi connect ..",font_size8X16);
				temp_sys_time = system_data.SystemGMTTime;
			}
			else if(system_var.TimeGetFlag == 1)
			{
				set_show_state_change(wifi_ok);
				OLED_Clear();
			}
		}break;
		case wifi_ok:
		{
			u8 TempBuff[40];
			
			if(system_var.TimeGetFlag == 1)
			{
				sprintf((char*)TempBuff,"ip: %s ",wifi_info_g.ip);
				show_common_string(0,0,TempBuff,font_size6X8);
				
				show_common_string(0,4,"wifi connect ok",font_size8X16);
				if(system_data.SystemGMTTime > (temp_sys_time+3))
				{
					system_var.TimeGetFlag = 2;
					set_show_state_change(city);
					OLED_Clear();
				}
			}
		}break;
		case wifi_failed:
		{
			
		}break;
		case city:
		{
			if((system_var.TimeGetFlag == 2))
			{
				if(system_var.WIFIConnectFlag)
				{
					fresh_once_flag[0]=1;
					if (fresh_once_flag[0] != fresh_once_flag_last[0])
						OLED_Clear();
					OLED_DrawBMP(0,0,11,13,signal_bmp1);

				}
				else
				{
					fresh_once_flag[0]=2;
					if (fresh_once_flag[0] != fresh_once_flag_last[0])
						OLED_Clear();
					OLED_DrawBMP(0,0,16,16,signal_idle);

				}
				fresh_once_flag_last[0] = fresh_once_flag[0];

				//OLED_DrawBMP(0,0,8,9,signal_bmp2);

				//OLED_DrawBMP(32,0,5,6,signal_bmp3);

				OLED_DrawBMP(100,0,16,8,battery_bmp2);

				show_line(0,5);
				OLED_ShowCHinese(0,6,0,f_city);
				OLED_ShowCHinese(18,6,1,f_city);
				OLED_ShowCHinese(36,6,2,f_city);
				OLED_ShowCHinese(54,6,3,f_city);
			}
			set_show_state_change(temp_and_humi);
			temp_sys_time = system_data.SystemGMTTime;
		}break;
		case temp_and_humi:
		{
			u8 TempBuff[40];
			if((system_var.CurrentTimeFlag == 1) && (system_var.TimeGetFlag == 2))
			{
				sprintf((char*)TempBuff,"%dC",(int)time_and_weather_g.tempeture);
				show_t_rh_string(72,6,TempBuff,font_size8X16);

				sprintf((char*)TempBuff,"%d%%",(int)time_and_weather_g.humidty);
				show_t_rh_string(104,6,TempBuff,font_size8X16);
			}
			set_show_state_change(time_and_date);
			temp_sys_time = system_data.SystemGMTTime;
		}break;
		case time_and_date:
		{
			  u8 tbuf[40];
			  RTC_TimeTypeDef RTC_TimeStruct;
			  RTC_DateTypeDef RTC_DateStruct;
			  
			  if((system_var.CurrentTimeFlag == 1) && (system_var.TimeGetFlag == 2))
			  {
			    system_var.CurrentTimeFlag = 0;
				RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
			    //rt_kprintf("Date:20%02d-%02d-%02d  ",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
			    sprintf((char*)tbuf,"20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date);
			    show_date_str(16,0,tbuf);

				sprintf((char*)tbuf,"0%d",RTC_DateStruct.RTC_WeekDay);
				show_common_string(0,3,tbuf,font_size8X16);
				
			    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
				sprintf((char*)tbuf,"%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes);

				show_time_str(20,1,tbuf);

				sprintf((char*)tbuf,"%02d",RTC_TimeStruct.RTC_Seconds);
				show_common_string(104,3,tbuf,font_size8X16);
			  }
			  if(system_data.SystemGMTTime > (temp_sys_time + 5))
			  {
				set_show_state_change(weather);
				temp_sys_time = system_data.SystemGMTTime;
				OLED_Clear();
			  }
		}break;
		case weather:
		{
			u8 TempBuff[40];
			if((system_var.TimeGetFlag == 2))
			{
				sprintf((char*)TempBuff,"%02d",time_and_weather_g.day);
				OLED_DrawBMP(1,1,28,20,weather_icon[0]);
				show_common_string(7,4,TempBuff,font_size8X16);

				sprintf((char*)TempBuff,"%02d",time_and_weather_g.day + 1);
				OLED_DrawBMP(40,1,28,20,weather_icon[1]);
				show_common_string(50,4,TempBuff,font_size8X16);

				sprintf((char*)TempBuff,"%02d",time_and_weather_g.day + 2);
				OLED_DrawBMP(80,1,28,20,weather_icon[2]);
				show_common_string(90,4,TempBuff,font_size8X16);				
			}
			if(system_data.SystemGMTTime > (temp_sys_time + 5))
			{
				set_show_state_change(city);
				temp_sys_time = system_data.SystemGMTTime;
				OLED_Clear();
			}
		}break;
		case picture:
		{
			if(buf_control_flag_g & BUF_FULL_0_MARK)
			{
				buf_control_flag_g |= BUF_USE_0_MARK;
				OLED_DrawBMP(0,0,128,64,picture_frame_buf_g[0]);
				buf_control_flag_g &= !BUF_USE_0_MARK;
			}
			else if(buf_control_flag_g & BUF_FULL_1_MARK)
			{
				buf_control_flag_g |= BUF_USE_1_MARK;
				OLED_DrawBMP(0,0,128,64,picture_frame_buf_g[1]);
				buf_control_flag_g &= !BUF_USE_1_MARK;
			}
		}
		break;
		default:
		{
			rt_kprintf("oled show state err\r\n");
		}
	}		
}





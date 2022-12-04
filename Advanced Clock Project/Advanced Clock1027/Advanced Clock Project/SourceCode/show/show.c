#include "main.h"


show_state_t  oled_show_state;

extern time_and_weather_t time_and_weather_g;

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

void show_weather_info()
{
  u8 TempBuff[40];
  if((system_var.HTDataFlag == 1) && (system_var.TimeGetFlag == 2))
  {
    system_var.HTDataFlag = 0;
    
    sprintf((char*)TempBuff,"%dC",(int)time_and_weather_g.tempeture);
    show_t_rh_string(72,6,TempBuff,font_size8X16);
    
    sprintf((char*)TempBuff,"%d%%",(int)time_and_weather_g.humidty);
    show_t_rh_string(104,6,TempBuff,font_size8X16);
  }
}

void ShowBMP()
{
	//OLED_DrawBMP(0,0,127,7,BMP2);


	static unsigned char fresh_once_flag[10] = {0};
	static unsigned char fresh_once_flag_last[10]={0};

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
}

void ShowWifiConnect()
{
	static unsigned char index = 0;
	static  unsigned int temp_sys_time;
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
	if(system_var.TimeGetFlag == 1)
	{
		show_common_string(0,4,"wifi connect ok",font_size8X16);
		OLED_Clear();
		if(system_data.SystemGMTTime > (temp_sys_time+3))
		{
			system_var.TimeGetFlag = 2;
		}
	}

}

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


void show_interface_oled()
{
	switch(oled_show_state)
	{
		case init_show:
		{
			
		}break;
		case wifi_connect:
		{
			
		}break;
		default:
		{
			
		}
	}		
}



#ifndef  _SHOW_H__
#define _SHOW_H__



typedef enum show_state_struct
{
	init_show      = 0x00,

	wifi_connect   ,
	wifi_ok        ,
	wifi_failed    ,

	time_and_date  ,
	city           ,
	temp_and_humi  ,
	weather        ,
	max            ,
}show_state_t;



void ShowWifiConnect();
void wifi_bmp_clear();
void show_weather_info();

#endif

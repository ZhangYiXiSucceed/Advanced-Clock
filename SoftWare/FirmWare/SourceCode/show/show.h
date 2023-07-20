#ifndef  _SHOW_H__
#define _SHOW_H__

#define PICTURE_FRMAE_SIZE  1024
enum 
{
	BUF_USE_0_MARK = 0x01,
	BUF_USE_1_MARK,
	BUF_FULL_0_MARK,
	BUF_FULL_1_MARK,
};

typedef enum show_state_struct
{
	init_show      = 0x00,
	eight_diagrams,

	wifi_connect   ,
	wifi_ok        ,
	wifi_failed    ,

	time_and_date  ,
	city           ,
	temp_and_humi  ,
	weather        ,

	picture,
	max            ,
}show_state_t;

extern u8 picture_frame_buf_g[2][PICTURE_FRMAE_SIZE];
extern u8 buf_control_flag_g;

void ShowCurrentTime(void);
void ShowWifiConnect(void);
void wifi_bmp_clear(void);
void show_weather_info(void);
void show_interface_oled(void);
void set_show_state_change(show_state_t state);
#endif

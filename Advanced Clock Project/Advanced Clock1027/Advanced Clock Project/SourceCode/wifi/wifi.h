#ifndef __WIFI_H__
#define __WIFI_H__



#define  AT_CMD             0
#define  AT_RST             1
#define  ATE0               2
#define  AT_CWMODE          3
#define  AT_CWJAP           4
#define  AT_CIPSTART        5
#define  AT_CIPMODE         6
#define  AT_CIPSEND         7
#define  AT_STOP_SEND       8
#define  AT_CWQAP           9


#define AT_QUIRY_WIFI          	   11
#define AT_QUIT_TCP_CONNECT        12
#define AT_QUIRY_IP                13
#define AT_QUIRY_MAC               14

#define  AT_SEND_DATA       	 20
#define  AT_QUIRY_WEATHER_DATA   21


enum 
{
	RESP_WIFI_OK          = 0x00,
	RESP_WIFI_CONNECT     = 0x01,
	RESP_IP_CONNECT       = 0x02,
	RESP_LIST_WIFI_INFO   = 0x03,
	RESP_SEND_MODE        = 0x04,
	RESP_GET_WIFI_IP      = 0x05,
	RESP_GET_WIFI_MAC     = 0x06,
	RESP_ERR,
};

void init_wifi_network(void);
void connect_wifi_network(void);
void wifi_msg_process(void);
void connect_server(void);
void set_send_mode(unsigned char mode);
void entry_send_state(void);
void send_data(char* demo_data,unsigned char data_len);
void send_weather_data(char* demo_data,unsigned char data_len);

void quiry_wifi(void);
int parsing_wifi_signal_info(unsigned char* frame_buffer,unsigned char frame_buffer_length);
void get_network_time_cmds();
void parsing_time_json_info(unsigned char* frame_buffer,unsigned char frame_buffer_length);
void paraing_time_string(char* temp_str,char* temp_week);
void quit_network_connect_cmd(void);
void quit_send_data_mode_cmd(void);
void parsing_weather_json_info(unsigned char* frame_buffer,unsigned short frame_buffer_length);
int parsing_the_str(char* str);
void print_wifi_weather_time_info();

int8_t WifiStateCheck(char *data);

#endif



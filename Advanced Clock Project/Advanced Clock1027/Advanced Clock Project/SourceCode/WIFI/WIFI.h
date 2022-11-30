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


#define AT_QUIRY_WIFI          11
#define AT_QUIT_TCP_CONNECT        12



#define  AT_SEND_DATA       20
#define  AT_QUIRY_WEATHER_DATA   21



void init_wifi_network(void);
void connect_wifi_network(void);
void wifi_task_deal(void);
void connect_server(void);
void set_send_mode(unsigned char mode);
void entry_send_state(void);
void send_data(char* demo_data,unsigned char data_len);
void send_weather_data(char* demo_data,unsigned char data_len);

void quiry_wifi(void);
void parsing_wifi_signal_info(unsigned char* frame_buffer,unsigned char frame_buffer_length);
void get_network_time_cmds();
void parsing_time_json_info(unsigned char* frame_buffer,unsigned char frame_buffer_length);
void paraing_time_string(char* temp_str,char* temp_week);
void quit_network_connect_cmd(void);
void quit_send_data_mode_cmd(void);
void parsing_weather_json_info(unsigned char* frame_buffer,unsigned short frame_buffer_length);
int parsing_the_str(char* str);



#endif



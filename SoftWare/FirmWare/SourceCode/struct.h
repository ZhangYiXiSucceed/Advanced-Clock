#ifndef   __STRUCT_H__
#define   __STRUCT_H__

#include "stm32f4xx.h"


#define Z_TRUE     1
#define Z_FALSE    0
#define COMMA      ','
#define MINUS      '-'  
#define COLON      ':'
#define QUOTATION  '"'

struct SystemData
{
  unsigned int SystemGMTTime;
  int tempture;
  int humidty;
};



struct SystemVar
{
	unsigned char TwoMinuteFlag;
	unsigned char LedBlinkFlag;
	unsigned char HTDataFlag;
	unsigned char CurrentTimeFlag;
	
	unsigned char Key2Flag;
	unsigned char Key3Flag;
	unsigned char Key4Flag;
	unsigned char Key5Flag;

	unsigned char NRFRxFlag;
	unsigned char NRFTxFlag;
	unsigned char WIFIConnectFlag;
	unsigned char TimeGetFlag;

	unsigned char   Time_20_ms_flag;
	unsigned char   uart1_mode;
	unsigned char   host_cmd_flag;
};

typedef struct STR_FrameHead  
{
    unsigned int   sign;		//帧头	   		        
    unsigned int   addr; 		//帧地址			        
    unsigned short   seq;         //帧序列号	                                
    unsigned char  len;    		//帧长度	                        
    unsigned char   cmd;         //ָ帧指令码
    unsigned char data[]; 		//帧数据                    
}STR_FrameHead_t;    



typedef struct time_and_weather_struct{
	
	unsigned short year;
	unsigned char  month;
	unsigned char  day;
	
	unsigned char  hour;	
	unsigned char  minute;
	unsigned char  second;
	unsigned char  week;

	char tempeture;
	char humidty;
	char api;
	char city_id;

	char weather[16];
	char city[16];
	
}time_and_weather_t;


typedef struct wifi_info_struct
{
 	char ip[32];
	char gateway[32];
	char netmask[32];
	char mac[32];
	int  wifi_signal_num;
}wifi_info_t;
#endif


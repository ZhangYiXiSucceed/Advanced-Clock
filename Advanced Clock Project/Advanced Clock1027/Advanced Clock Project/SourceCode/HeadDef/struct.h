#ifndef   __STRUCT_H__
#define   __STRUCT_H__

#include "stm32f4xx.h"


struct SystemData
{
  unsigned int SystemGMTTime;
  float Tempture;
  float Humidty;
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

/*
typedef struct 
{
	
};

*/

#endif


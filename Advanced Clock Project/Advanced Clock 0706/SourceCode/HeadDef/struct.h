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
};



#endif


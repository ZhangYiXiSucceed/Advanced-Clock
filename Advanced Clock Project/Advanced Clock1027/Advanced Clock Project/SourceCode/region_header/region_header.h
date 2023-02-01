#ifndef __REGION_HEADER_H__
#define __REGION_HEADER_H__

#include "stm32f4xx.h"
#define STM32_MARKER    0x51230
#define APP_SAVE_ADDR   0x08010000
#define APP_LOAD_ADDR   0x08010000
#define APP_VERSION     0x12345678                

typedef struct boot_region_header_struct
{
	u32 marker;
	u32 crc;
	u32 load_address;
	u32 save_address;
	u32 entry;
	u32 attr;
	u32 version;
}boot_region_header_t;

extern boot_region_header_t region_header;
#endif







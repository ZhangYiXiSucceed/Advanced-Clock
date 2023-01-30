#ifndef __REGION_HEADER_H__
#define __REGION_HEADER_H__

#include "boot_main.h"

#define STM32_MARKER 0x51230

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


#endif







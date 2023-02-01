#include "region_header.h"



#ifndef BOOT
#include "main.h"
extern void Reset_Handler();
__attribute__((section("region_header")))   boot_region_header_t region_header =
{
	.marker = STM32_MARKER,
	.crc    = 0,
	.load_address  = APP_LOAD_ADDR,
	.save_address  = APP_SAVE_ADDR,
	.entry = (u32)Reset_Handler,
	.attr = 0,
	.version = APP_VERSION 
};

#else
#include "boot_main.h"
boot_region_header_t region_header;
#endif




#include "region_header.h"



#ifndef BOOT
#include "main.h"

extern void* Image$$ER_IROM1$$Base;
extern void* Image$$ER_IROM1$$Length;
extern void* Image$$ER_IROM1$$Limit;

extern void* Image$$ER_IROM2$$Base;
extern void* Image$$ER_IROM2$$Length;
extern void* Image$$ER_IROM2$$Limit;

extern void* Image$$RW_IRAM1$$Base;
extern void* Image$$RW_IRAM1$$Length;
extern void* Image$$RW_IRAM1$$Limit;

extern void* Image$$RW_IRAM1$$ZI$$Base;
extern void* Image$$RW_IRAM1$$ZI$$Length;
extern void* Image$$RW_IRAM1$$ZI$$Limit;

extern void Reset_Handler();
__attribute__((section("region_header")))   boot_region_header_t region_header =
{
	.marker = STM32_MARKER,
	.crc    = 0,
	.len    = 0,
	.load_address  = APP_LOAD_ADDR,
	.save_address  = APP_SAVE_ADDR,
	.entry = (u32)Reset_Handler,
	.attr = 0,
	.version = APP_VERSION 
};
void region_header_init(boot_region_header_t* temp_region_header)
{
	u32 IROM1_len = (u32)((u32)&Image$$ER_IROM1$$Limit - (u32)&Image$$ER_IROM1$$Base);
	u32 IROM2_len = (u32)((u32)&Image$$ER_IROM2$$Limit - (u32)&Image$$ER_IROM2$$Base);
	u32 IRAM1_RW_len = (u32)((u32)&Image$$RW_IRAM1$$Limit - (u32)&Image$$RW_IRAM1$$Base);
	u32 IRAM1_ZI_len = (u32)((u32)&Image$$RW_IRAM1$$ZI$$Limit - (u32)&Image$$RW_IRAM1$$ZI$$Base);
	
	temp_region_header->len = ((u32)&Image$$ER_IROM1$$Length + (u32)&Image$$ER_IROM2$$Length \
							   + (u32)&Image$$RW_IRAM1$$Length + (u32)&Image$$RW_IRAM1$$ZI$$Length);

	rt_kprintf("app size=%dB %dkb\r\n", temp_region_header->len,\
		temp_region_header->len>>10);
							   
	rt_kprintf("IROM1_len=%d IROM2_len=%d IRAM1_RW_len=%d IRAM1_ZI_len=%d\r\n", IROM1_len,\
		IROM2_len,IRAM1_RW_len,IRAM1_ZI_len);
}
#else
#include "boot_main.h"

extern void* Image$$ER_IROM1$$Base;
extern void* Image$$ER_IROM1$$Length;
extern void* Image$$ER_IROM1$$Limit;

extern void* Image$$RW_IRAM1$$Base;
extern void* Image$$RW_IRAM1$$Length;
extern void* Image$$RW_IRAM1$$Limit;

extern void* Image$$RW_IRAM1$$ZI$$Base;
extern void* Image$$RW_IRAM1$$ZI$$Length;
extern void* Image$$RW_IRAM1$$ZI$$Limit;

boot_region_header_t region_header;
void region_header_init(boot_region_header_t* temp_region_header)
{
	memcpy(temp_region_header,(u8*)(APP_LOAD_ADDR + REGION_OFFSET),sizeof(boot_region_header_t));
	rt_kprintf("marker=%x load_address=%x \r\n", temp_region_header->marker,\
	temp_region_header->load_address);

	u32 IROM1_len = (u32)((u32)&Image$$ER_IROM1$$Limit - (u32)&Image$$ER_IROM1$$Base);
	u32 IRAM1_RW_len = (u32)((u32)&Image$$RW_IRAM1$$Limit - (u32)&Image$$RW_IRAM1$$Base);
	u32 IRAM1_ZI_len = (u32)((u32)&Image$$RW_IRAM1$$ZI$$Limit - (u32)&Image$$RW_IRAM1$$ZI$$Base);

	u32 total_size = IROM1_len + IRAM1_RW_len;

	rt_kprintf("bootloader size=%d b %d kb\r\n", total_size,\
		total_size>>10);

	rt_kprintf("IROM1_len=%d  IRAM1_RW_len=%d IRAM1_ZI_len=%d\r\n", IROM1_len,\
		IRAM1_RW_len,IRAM1_ZI_len);
}

#endif




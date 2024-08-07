#ifndef __BOOT_H__
#define __BOOT_H__

#include "region_header.h"
void jump_exec(boot_region_header_t* region_header);
u8 check_crc(boot_region_header_t* region_header);

__INLINE u8 check_header(boot_region_header_t* region_header)
{
	return (STM32_MARKER == region_header->marker);
}

#endif


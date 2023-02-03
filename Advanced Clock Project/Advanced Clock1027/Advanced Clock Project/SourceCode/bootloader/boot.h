#ifndef __BOOT_H__
#define __BOOT_H__

#include "region_header.h"
void jump_exec(boot_region_header_t* region_header);
void check_crc(boot_region_header_t* region_header);

#endif
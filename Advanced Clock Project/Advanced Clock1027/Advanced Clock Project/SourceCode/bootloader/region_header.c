#include "region_header.h"
#include "boot.h"

__attribute__((section("region_header")))   boot_region_header_t region_header =
{
	.marker = STM32_MARKER,
	.crc    = 0,
	.load_address  = 0,
	.save_address  = 0,
	.entry = 0,
	.attr = 0,
	.version = 0 
};



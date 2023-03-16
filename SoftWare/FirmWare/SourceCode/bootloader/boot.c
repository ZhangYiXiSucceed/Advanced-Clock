#include "uart.h"
#include "boot.h"
#include "crc32.h"


typedef void (*pFunction)();
void jump_exec(boot_region_header_t* region_header)
{
	enum
	{
		ARMV7M_VECTOR_SP = 0,
		ARMV7M_VECTOR_RESET_HANDLER,
	};
	u32 *vector_addr = (u32*)region_header->load_address;
	u32 sp = (u32)vector_addr[ARMV7M_VECTOR_SP];

   	if ((sp >= SRAM_MIN_ADDR) && (sp <= SRMA_MAX_ADDR))
   	{ 	
		__disable_fault_irq();
        u32 jump_address = (u32)vector_addr[ARMV7M_VECTOR_RESET_HANDLER];
        pFunction jump_to_application = (pFunction) jump_address;
        __set_MSP(sp);
        __enable_fault_irq();
        jump_to_application();
   	}
	else
	{
		rt_kprintf("sp=0x%x\r\n", (sp));
	}
}

u8 check_crc(boot_region_header_t* region_header)
{
	u8 check_status = 0;
    crc32_data_t crc32_data;
	crc32_init(&crc32_data);

	crc32_update(&crc32_data,(u8*)region_header->save_address, region_header->len);

	if(crc32_data.currentcrc == region_header->crc)
	{
		check_status = 0;
	}
	else
	{
		check_status = 1;
	}
	rt_kprintf("cal crc=0x%x crc=0x%x \r\n",crc32_data.currentcrc,region_header->crc);
	return check_status;
}





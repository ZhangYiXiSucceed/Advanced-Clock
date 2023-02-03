#include "boot.h"



typedef (*pFunction)();
void jump_exec(boot_region_header_t* region_header)
{
    
   	if (((*(__IO uint32_t*)region_header->load_address) >= SRAM_MIN_ADDR) && ((*(__IO uint32_t*)region_header->load_address) <= SRMA_MAX_ADDR))
   	{ 	
        __disable_fault_irq();
        u32 JumpAddress = *(__IO uint32_t*) (region_header->load_address +4);
        pFunction Jump_To_Application = (pFunction) JumpAddress;
        __set_MSP(*(__IO uint32_t*) region_header->load_address);
        __enable_fault_irq();

        Jump_To_Application();
   	}
}


void check_crc(boot_region_header_t* region_header)
{
    
}





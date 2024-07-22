#include "main.h"


#define BANK_EXEC_ADDR 0x2001E000


#define BANK0_SAVE_ADDR  0x08020000
#define BANK1_SAVE_ADDR  0x08020400
#define BANK2_SAVE_ADDR  0x08020800
#define BANK_FLASH_BASE  BANK0_SAVE_ADDR


typedef void (*bank_handler_fun)(u8 bank_id,u8 func_id);

u8 current_bank_id = 0;
void bank_init()
{
    u32 current_bank_flash_addr = BANK_FLASH_BASE + current_bank_id*0x400;
    STMFLASH_Read(current_bank_flash_addr, (u32*)BANK_EXEC_ADDR, 0x400);
    bank_handler_fun bank_handler_func = (bank_handler_fun)(BANK_EXEC_ADDR);
    (*bank_handler_func)(current_bank_id, 0);
}


void bank_process()
{

}




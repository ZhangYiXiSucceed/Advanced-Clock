#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f4xx.h"

#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH start
 
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//sector 0 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//sector 1 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//sector 2 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//sector 3 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//sector 4 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//sector 5 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//sector 6 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//sector 7 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//sector 8 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//sector 9 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//sector 10ַ 128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//sector 11 128 Kbytes  

u32 STMFLASH_ReadWord(u32 faddr);		  	
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		
						   
#endif


















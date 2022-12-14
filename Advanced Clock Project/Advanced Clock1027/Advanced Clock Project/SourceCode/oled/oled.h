#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"

#define OLED_SPI_PORT               GPIOB
#define OLED_SPI_RST_PIN            GPIO_Pin_11
#define OLED_SPI_CS_PIN             GPIO_Pin_12
#define OLED_SPI_SCK_PIN            GPIO_Pin_13
#define OLED_SPI_MISO_PIN           GPIO_Pin_14
#define OLED_SPI_MOSI_PIN           GPIO_Pin_15

#define OLED_CS_Set()          GPIO_SetBits(OLED_SPI_PORT, OLED_SPI_CS_PIN)
#define OLED_CS_Clr() 	        GPIO_ResetBits(OLED_SPI_PORT, OLED_SPI_CS_PIN)

#define OLED_RST_Clr()              GPIO_ResetBits(OLED_SPI_PORT,OLED_SPI_RST_PIN)//RES
#define OLED_RST_Set()              GPIO_SetBits(OLED_SPI_PORT,OLED_SPI_RST_PIN)

#define OLED_DC_Clr()               GPIO_ResetBits(OLED_SPI_PORT,OLED_SPI_MISO_PIN)//DC
#define OLED_DC_Set()               GPIO_SetBits(OLED_SPI_PORT,OLED_SPI_MISO_PIN)


#define OLED_SCLK_Clr()             GPIO_ResetBits(OLED_SPI_PORT,OLED_SPI_SCK_PIN)//CLK
#define OLED_SCLK_Set()             GPIO_SetBits(OLED_SPI_PORT,OLED_SPI_SCK_PIN)

#define OLED_SDIN_Clr()             GPIO_ResetBits(OLED_SPI_PORT,OLED_SPI_MOSI_PIN)//DIN
#define OLED_SDIN_Set()             GPIO_SetBits(OLED_SPI_PORT,OLED_SPI_MOSI_PIN)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define OLED_InitMode 0    //0 软件SPI  1 硬件SPI

#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	 


#define   font_size6X8     0
#define   font_size8X16    16
#define   font_size8X16_encode_offset_data   8

#define   font_size16X32   32
#define   font_size16X32_encode_offset_data   8


#define   special_character_offset   10



void OLED_Init(void);
void SPI2Init(void);
void OLED_WR_Byte(u8 data,u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no,u8 city[][16]);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

void ShowStartDisplay(void);
void OLED_IOInit(void);
void ShowBMP(void);

void show_oled_char(int font_x,int font_y, char character,int font_size);
void show_time_str(u8 x,u8 y,u8 *chr);
void show_line(int x,int y);
void show_t_rh_string(u8 x,u8 y,u8 *chr,int font_size);
void show_common_string(u8 x,u8 y,u8 *chr,int font_size);
void show_date_str(u8 x,u8 y,u8 *chr);



#endif




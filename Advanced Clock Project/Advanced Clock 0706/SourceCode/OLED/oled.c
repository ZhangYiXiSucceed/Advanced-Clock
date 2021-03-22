#include "main.h"
#include "oledfont.h"
#include "bmp.h"

void Spi2IOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Enable GPIOC clocks 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  // Connect SPI2 pins to AF5
  GPIO_PinAFConfig(OLED_SPI_PORT, GPIO_PinSource13, GPIO_AF_SPI2);  // SCK MISO
  GPIO_PinAFConfig(OLED_SPI_PORT, GPIO_PinSource14, GPIO_AF_SPI2);
  GPIO_PinAFConfig(OLED_SPI_PORT, GPIO_PinSource15, GPIO_AF_SPI2);  // MOSI
  
  // SPI SCK、MOSI pin configuration
  GPIO_InitStructure.GPIO_Pin = OLED_SPI_SCK_PIN|OLED_SPI_MOSI_PIN|OLED_SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(OLED_SPI_PORT, &GPIO_InitStructure);
  
  
  //SPI2 NSS pin in output pushpull mode 
  GPIO_InitStructure.GPIO_Pin = OLED_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  //GPIO_PuPd_NOPULL;
  GPIO_Init(OLED_SPI_PORT, &GPIO_InitStructure);
	
}

void SPI2Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

  SPI_I2S_DeInit(SPI2);
  //Enable the SPI periph 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  Spi2IOInit();
  
  // SPI configuration
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  // SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  // Enable the SPI2
  SPI_Cmd(SPI2, ENABLE);
}


void OLED_IOInit()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	 //使能PC,D,G端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 //PD3,PD6推挽输出  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	//PD3,PD6 输出高
}

void OLED_Init()
{
  #if OLED_InitMode==1
   SPI2Init();
  #else
   OLED_IOInit();
  #endif

  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
  delay_ms(500);
  
	OLED_WR_Byte(0xFD,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x12,OLED_CMD);//--turn off oled panel	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0xD5,OLED_CMD);//---set low column address
	OLED_WR_Byte(0xA0,OLED_CMD);//---set high column address
	OLED_WR_Byte(0xA8,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x3F,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xD3,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0x00,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0x40,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA1,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xC8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0xDA,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0x2,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0x81,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0xBF,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0xD9,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x25,OLED_CMD);//
	OLED_WR_Byte(0xDB,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x34,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 

  OLED_Display_On();
  
}
//void OLED_WR_Byte(u8 data,u8 cmd)
//{
//  OLED_SPI_CS_LOW(); 
//	
//  if(cmd)
//     OLED_DC_Set();
//  else
//     OLED_DC_Clr();
//  
//  while((SPI2->SR &0x0002)==0);  
//  SPI2->DR = (data);
//  while((SPI2->SR &0x0001)==0);
//  SPI_I2S_ReceiveData(SPI2); 

//  OLED_DC_Set();
//  OLED_SPI_CS_HIGH();
//}


void OLED_WR_Byte(u8 data,u8 cmd)
{	
  
  #if OLED_InitMode==1
    OLED_CS_Clr();
    if(cmd)
       OLED_DC_Set();
    else
       OLED_DC_Clr();
    
    while((SPI2->SR &0x0002)==0);  
    SPI2->DR = (data);
    while((SPI2->SR &0x0001)==0);
    SPI_I2S_ReceiveData(SPI2); 

    OLED_DC_Set();
    OLED_CS_Set();
  #else
    u8 i;			  
    if(cmd)
      OLED_DC_Set();
    else 
      OLED_DC_Clr();		  
    OLED_CS_Clr();
    for(i=0;i<8;i++)
    {			  
      OLED_SCLK_Clr();
      if(data&0x80)
         OLED_SDIN_Set();
      else 
         OLED_SDIN_Clr();
      OLED_SCLK_Set();
      data<<=1;   
    }				 		  
    OLED_CS_Set();
    OLED_DC_Set();   	
  #endif
} 

	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}
      else 
        enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
    }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
    {	
      OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
      adder+=1;
    }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


void ShowStartDisplay()
{
  	OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		OLED_ShowString(0,3,"1.54' OLED TEST");
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");
}


void ShowBMP()
{
  OLED_DrawBMP(0,0,127,7,BMP2);
}


void Show16X32()
{
  unsigned char i,x,y;
  x=0;y=0;
  OLED_Set_Pos(x,y);	
  for(i=0;i<8;i++)
  OLED_WR_Byte(F16X32[i],OLED_DATA);
  OLED_Set_Pos(x,y+1);
  for(i=0;i<8;i++)
  OLED_WR_Byte(F16X32[i+8],OLED_DATA);
  
  OLED_Set_Pos(x,y+2);	
  for(i=0;i<8;i++)
  OLED_WR_Byte(F16X32[16+i],OLED_DATA);
  OLED_Set_Pos(x,y+3);
  for(i=0;i<8;i++)
  OLED_WR_Byte(F16X32[16+i+8],OLED_DATA);
  
  x=16;y=0;
  OLED_Set_Pos(x,y);	
  for(i=0;i<8;i++)
  OLED_WR_Byte(F16X32[32+i],OLED_DATA);
  OLED_Set_Pos(x,y+1);
  for(i=0;i<8;i++)
  OLED_WR_Byte(F16X32[32+i+8],OLED_DATA);
  
  OLED_Set_Pos(x,y+2);	
  for(i=0;i<8;i++)
  OLED_WR_Byte(F16X32[48+i],OLED_DATA);
  OLED_Set_Pos(x,y+3);
  for(i=0;i<8;i++)
  OLED_WR_Byte(F16X32[48+i+8],OLED_DATA);
}









#include "main.h"


extern const unsigned char F6x8[][6];
extern const unsigned char F8X16[];
extern const unsigned char F16X32[];

extern unsigned char f_city[][16];
extern unsigned char number_font[][16];
extern unsigned char line_num[];


void Spi2IOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Enable GPIOC clocks 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  // Connect SPI2 pins to AF5
  GPIO_PinAFConfig(OLED_SPI_PORT, GPIO_PinSource13, GPIO_AF_SPI2);  // SCK MISO
  GPIO_PinAFConfig(OLED_SPI_PORT, GPIO_PinSource14, GPIO_AF_SPI2);
  GPIO_PinAFConfig(OLED_SPI_PORT, GPIO_PinSource15, GPIO_AF_SPI2);  // MOSI
  
  // SPI SCK��MOSI pin configuration
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
 	
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 //PD3,PD6�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //�������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	//PD3,PD6 �����
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
	OLED_WR_Byte(0x00,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0x40,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' '; /* get offset value*/	
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
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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
//��ʾһ���ַ��Ŵ�
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
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no,u8 city[][16])
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(city[2*no][t],OLED_DATA);
		adder+=1;
    }	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
    {	
		OLED_WR_Byte(city[2*no+1][t],OLED_DATA);
		adder+=1;
    }					
}
/***********Show BMP Picture*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
 unsigned char y_line_num;
  
	if(y1%8==0) 
		y_line_num=y1/8;      
	else 
		y_line_num=y1/8+1;
	for(y=y0;y<(y_line_num + ((y0%8)?(y0/8+1):(y0/8)));y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<(x0+x1);x++)
		{      
			OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
		}
	}
} 

/***********Show BMP Picture*****************/
void OLED_DrawBMP_test(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[],unsigned char state1,unsigned char state2)
{ 	
 unsigned int j=0;
 unsigned char x,y;
 unsigned char y_line_num;
  
	if(y1%8==0) 
		y_line_num=y1/8;      
	else 
		y_line_num=y1/8+1;
	for(y=y0;y<(y_line_num + ((y0%8)?(y0/8+1):(y0/8)));y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<(x0+x1);x++)
		{      
			OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
		}
	}
} 


void show_oled_char(int font_x,int font_y, char character,int font_size)
{
	unsigned char temp_character_position=0,character_offset=0;	
	temp_character_position=character-' '; /* get offset value*/		
	if(font_x>Max_Column-1)    /*character is over right side,then new line*/
	{
		font_x=0;
		font_y=font_y+2;
	}
	if(font_size16X32 == font_size)
	{

	    if(':' == character)
    	{
    		OLED_Set_Pos(font_x,font_y);	
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(F16X32[special_character_offset*font_size16X32+character_offset],OLED_DATA);
			}
				
			OLED_Set_Pos(font_x,font_y+1);
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(F16X32[special_character_offset*font_size16X32+character_offset + font_size16X32_encode_offset_data],OLED_DATA);
			}

			OLED_Set_Pos(font_x,font_y+2);
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(F16X32[special_character_offset*font_size16X32+character_offset + font_size16X32_encode_offset_data*2],OLED_DATA);
			}
			
			OLED_Set_Pos(font_x,font_y+3);
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(F16X32[special_character_offset*font_size16X32+character_offset + font_size16X32_encode_offset_data*3],OLED_DATA);
			}
    	}
		else
		{
			temp_character_position-= 16;
		
			OLED_Set_Pos(font_x,font_y);	
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(F16X32[temp_character_position*font_size16X32+character_offset],OLED_DATA);
			}
				
			OLED_Set_Pos(font_x,font_y+1);
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(F16X32[temp_character_position*font_size16X32+character_offset + font_size16X32_encode_offset_data],OLED_DATA);
			}

			OLED_Set_Pos(font_x,font_y+2);
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(F16X32[temp_character_position*font_size16X32+character_offset + font_size16X32_encode_offset_data*2],OLED_DATA);
			}
			
			OLED_Set_Pos(font_x,font_y+3);
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(F16X32[temp_character_position*font_size16X32+character_offset + font_size16X32_encode_offset_data*3],OLED_DATA);
			}
		}
		
	}
	else if(font_size8X16 == font_size)
	{
		OLED_Set_Pos(font_x,font_y);	
		for(character_offset=0;character_offset<8;character_offset++)
		{
			OLED_WR_Byte(F8X16[temp_character_position*font_size8X16+character_offset],OLED_DATA);
		}
			
		OLED_Set_Pos(font_x,font_y+1);
		for(character_offset=0;character_offset<8;character_offset++)
		{
			OLED_WR_Byte(F8X16[temp_character_position*font_size8X16+character_offset + font_size8X16_encode_offset_data],OLED_DATA);
		}
	}
	else if(font_size6X8 == font_size)
	{	
		OLED_Set_Pos(font_x,font_y+1);
		for(character_offset=0;character_offset<6;character_offset++)
		{
			OLED_WR_Byte(F6x8[temp_character_position][character_offset],OLED_DATA);
		}
		
	}
	
}



void show_common_string(u8 x,u8 y,u8 *chr,int font_size)
{
	unsigned char j=0;

	unsigned char font_select ;

	if(font_size6X8 == font_size)
		font_select=6;
	if(font_size8X16 == font_size)
		font_select=8;
	if(font_size16X32 == font_size)
		font_select=16;
	
	while (chr[j]!='\0')
	{	
		show_oled_char(x,y,chr[j],font_size);
		x+=font_select;
		if(x>120){x=0;y+=2;}
			j++;
	}
}


void show_time_str(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	
	while (chr[j]!='\0')
	{		show_oled_char(x,y,chr[j],font_size16X32);
			x+=16;
		if(x>120){x=0;y+=2;}
			j++;
	}
	
}
void show_date_str(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	int font_x =x,font_y=y;
	int character_offset;
	int temp_character_position;
	while (chr[j]!='\0')
	{	

		if(chr[j]>='0')
		{
			temp_character_position = chr[j]-'0';
			OLED_Set_Pos(font_x,font_y);
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(number_font[temp_character_position][character_offset],OLED_DATA);
			}

			OLED_Set_Pos(font_x,font_y+1);
			for(character_offset=8;character_offset<16;character_offset++)
			{
				OLED_WR_Byte(number_font[temp_character_position][character_offset],OLED_DATA);
			}
		}
		else
		{
			temp_character_position = 10;
			OLED_Set_Pos(font_x,font_y);
			for(character_offset=0;character_offset<8;character_offset++)
			{
				OLED_WR_Byte(number_font[temp_character_position][character_offset],OLED_DATA);
			}
			OLED_Set_Pos(font_x,font_y+1);
			for(character_offset=8;character_offset<16;character_offset++)
			{
				OLED_WR_Byte(number_font[temp_character_position][character_offset],OLED_DATA);
			}
			
		}
		font_x+=8;
		j++;

	}
	
}



void show_t_rh_string(u8 x,u8 y,u8 *chr,int font_size)
{
	unsigned char j=0;

	unsigned char font_select ;

	if(font_size6X8 == font_size)
		font_select=6;
	if(font_size8X16 == font_size)
		font_select=8;
	if(font_size16X32 == font_size)
		font_select=16;
	
	while (chr[j]!='\0')
	{		
		show_oled_char(x,y,chr[j],font_size);
		x+=font_select;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

void show_line(int x,int y)
{
	int row_offset;
	int i;
	int x_var = x;
	for(row_offset=0;row_offset<((128-x)/8);row_offset++)
	{
		OLED_Set_Pos(x_var,y);

		for(i=0;i<8;i++)
		{
			OLED_WR_Byte(line_num[i],OLED_DATA);
		}
		
		x_var+=8;
	}
}






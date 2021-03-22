#include "main.h"

struct SystemData  system_data;

void HTSensor_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOB时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOB时钟
  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
  
	IIC_SCL=1;
	IIC_SDA=1;
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		    IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


void HTSensorMode()
{
  IIC_Start();
  
  IIC_Send_Byte(0x88);   //0x44 + 0（write）
  IIC_Wait_Ack();
  
  IIC_Send_Byte(0x23);   //0x2C06   
  IIC_Wait_Ack();
  IIC_Send_Byte(0x22);   //
  IIC_Wait_Ack();
  
  IIC_Stop();
}

void ReadHTSensorData()
{
  u8 CRCSum;
  u16 Temperture;
  u16 Humidity;
  
  IIC_Start();
  
  IIC_Send_Byte(0x88);   //0x44 + 0（write）
  IIC_Wait_Ack();
  
  IIC_Send_Byte(0xE0);   //0xE000 fetch data  
  IIC_Wait_Ack();
  IIC_Send_Byte(0x00);   //
  IIC_Wait_Ack();
  
  
  IIC_Start();  
  IIC_Send_Byte(0x89);   //0x44 + 1（read）
  IIC_Wait_Ack();
  
  (Temperture) = IIC_Read_Byte(1);
  (Temperture)<<=8;
  (Temperture)|=IIC_Read_Byte(1);
  CRCSum = IIC_Read_Byte(1);
  
  //rt_kprintf("%d C %d\r\n",Temperture,CRCSum);
  
  
  (Humidity) = IIC_Read_Byte(1);
  (Humidity)<<=8;
  (Humidity)|=IIC_Read_Byte(1);
  CRCSum = IIC_Read_Byte(0);
  
  IIC_Stop();
  
  //rt_kprintf("%d %% %d\r\n",Humidity,CRCSum);
  
  
  system_data.Humidty = ((float)((float)Humidity/65535))*100;
  system_data.Tempture = ((float)((float)Temperture/65535))*175-45;
}



void PrintHTInfo()
{
  u8 TempBuff[40];
  if(system_var.HTDataFlag == 1)
  {
    system_var.HTDataFlag = 0;
    ReadHTSensorData();
    rt_kprintf("%2.2f C %2.2f %%\r\n",system_data.Tempture,system_data.Humidty);
    
    sprintf((char*)TempBuff,"T   :%2.2f C",system_data.Tempture);
    OLED_ShowString(0,4,TempBuff);
    
    sprintf((char*)TempBuff,"RH  :%2.2f %%",system_data.Humidty);
    OLED_ShowString(0,6,TempBuff);
  }
}


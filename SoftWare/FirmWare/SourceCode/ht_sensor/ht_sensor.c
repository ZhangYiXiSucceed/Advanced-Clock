#include "main.h"

void HTSensor_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOBʱ��
	//GPIOB8,B9��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
  
	IIC_SCL=1;
	IIC_SDA=1;

	HTSensorMode();
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}


void HTSensorMode()
{
  IIC_Start();
  
  IIC_Send_Byte(0x88);   //0x44 + 0��write��
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
  
  IIC_Send_Byte(0x88);   //0x44 + 0��write��
  IIC_Wait_Ack();
  
  IIC_Send_Byte(0xE0);   //0xE000 fetch data  
  IIC_Wait_Ack();
  IIC_Send_Byte(0x00);   //
  IIC_Wait_Ack();
  
  
  IIC_Start();  
  IIC_Send_Byte(0x89);   //0x44 + 1��read��
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
  
  system_data.humidty = ((float)((float)Humidity/65535))*100;
  system_data.tempture = ((float)((float)Temperture/65535))*175-45;
}



void PrintHTInfo()
{
	ReadHTSensorData();
	rt_kprintf2("board:temp=%d C humi=%d %%\r\n",system_data.tempture,system_data.humidty);
	rt_kprintf("board:temp=%d C humi=%d %%\r\n",system_data.tempture,system_data.humidty);
}






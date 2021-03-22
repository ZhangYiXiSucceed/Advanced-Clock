#include "main.h" 



void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOFʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 |GPIO_Pin_13| GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
}
void LedOnD1()   //������������D1��D2��D3��D4��PCB���棩
{
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
}

void LedOnD2()
{
  GPIO_SetBits(GPIOE,GPIO_Pin_12);
}

void LedOnD3()   //������������D1��D2��D3��D4��PCB���棩
{
	GPIO_SetBits(GPIOE,GPIO_Pin_13);
}

void LedOnD4()
{
  GPIO_SetBits(GPIOE,GPIO_Pin_14);
}

void LedBlink()
{
  static unsigned char LEDCount = 0;
  if(system_var.LedBlinkFlag == 1)
  {
    system_var.LedBlinkFlag = 0;
    
    if(LEDCount == 255)
      LEDCount =0;
    else
      LEDCount++;
    if(LEDCount % 2 == 0)
    {
        LED_D1_ON;
        LED_D4_ON;
        
        LED_D2_OFF;
        LED_D3_OFF;
    }
    else
    {
        LED_D1_OFF;
        LED_D4_OFF;
        
        LED_D2_ON;
        LED_D3_ON;
    }
  }
}

#include "main.h" 



void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOF时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 |GPIO_Pin_13| GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
}
void LedOnD1()   //从左到右依次是D1，D2，D3，D4（PCB上面）
{
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
}

void LedOnD2()
{
  GPIO_SetBits(GPIOE,GPIO_Pin_12);
}

void LedOnD3()   //从左到右依次是D1，D2，D3，D4（PCB上面）
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

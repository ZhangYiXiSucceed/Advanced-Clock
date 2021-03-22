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

void PWM_LEDInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOE,&GPIO_InitStructure); 
	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
	
	TIM_TimeBaseStructure.TIM_Prescaler=168-1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=1000-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = 500;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 500;
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
  TIM_OCInitStructure.TIM_Pulse = 500;	
  TIM_OC4Init(TIM1, &TIM_OCInitStructure); 
	
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);

	TIM_Cmd(TIM1, ENABLE); 
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
}

void LED_BreathingLight()
{
	static unsigned short int LEDCount = 0;
	static unsigned char DirFlag = 0;
  if(LEDCount >= 1000)
		DirFlag = 0;
	if(LEDCount == 0)
		DirFlag = 1;
	if(DirFlag)
		LEDCount++;
	else
		LEDCount--;
	TIM_SetCompare2(TIM1,LEDCount);
	TIM_SetCompare3(TIM1,LEDCount);
	TIM_SetCompare4(TIM1,LEDCount);
}

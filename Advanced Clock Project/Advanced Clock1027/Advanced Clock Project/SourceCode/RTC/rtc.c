#include "main.h"


//NVIC_InitTypeDef   NVIC_InitStructure;

//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	RTC_TimeTypeInitStructure.RTC_H12=ampm;
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	
}
//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:SUCEE(1),�ɹ�
//       ERROR(0),�����ʼ��ģʽʧ�� 
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

//RTC��ʼ��
//����ֵ:0,��ʼ���ɹ�;
//       1,LSE����ʧ��;
//       2,�����ʼ��ģʽʧ��;
u8 My_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		//�Ƿ��һ������?
	{
		//RCC_LSEConfig(RCC_LSE_ON);//LSE ���� 
    	RCC_LSICmd(ENABLE);    
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
			{
			retry++;
			delay_ms(10);
			}
		if(retry==0)return 1;		//LSI ����ʧ��. 
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ�� 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�첽��Ƶϵ��(1~0X7F)
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTCͬ����Ƶϵ��(0~7FFF)
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC����Ϊ,24Сʱ��ʽ
    RTC_Init(&RTC_InitStructure);
 
    RTC_Set_Time(21,47,00,RTC_H12_PM);	//����ʱ��
    RTC_Set_Date(20,8,2,7);		//�������� 
 
	RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//����Ѿ���ʼ������
	} 

	return 0;
}

////��������ʱ��(����������,24Сʱ��)
////week:���ڼ�(1~7) @ref  RTC_Alarm_Definitions
////hour,min,sec:Сʱ,����,����
//void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
//{ 
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
//	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
//	
//	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//�ر�����A 
//	
//  RTC_TimeTypeInitStructure.RTC_Hours=hour;//Сʱ
//	RTC_TimeTypeInitStructure.RTC_Minutes=min;//����
//	RTC_TimeTypeInitStructure.RTC_Seconds=sec;//��
//	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
//  
//	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=week;//����
//	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//��������
//	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;//��ȷƥ�����ڣ�ʱ����
//	RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
//  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
// 
//	
//	RTC_ClearITPendingBit(RTC_IT_ALRA);//���RTC����A�ı�־
//  EXTI_ClearITPendingBit(EXTI_Line17);//���LINE17�ϵ��жϱ�־λ 
//	
//	RTC_ITConfig(RTC_IT_ALRA,ENABLE);//��������A�ж�
//	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//��������A 
//	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE17
//  EXTI_Init(&EXTI_InitStructure);//����

//	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
//  NVIC_Init(&NVIC_InitStructure);//����
//}

////�����Ի��Ѷ�ʱ������  
///*wksel:  @ref RTC_Wakeup_Timer_Definitions
//#define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
//#define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
//#define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
//#define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
//#define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
//#define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
//*/
////cnt:�Զ���װ��ֵ.����0,�����ж�.
//void RTC_Set_WakeUp(u32 wksel,u16 cnt)
//{ 
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	
//	RTC_WakeUpCmd(DISABLE);//�ر�WAKE UP
//	
//	RTC_WakeUpClockConfig(wksel);//����ʱ��ѡ��
//	
//	RTC_SetWakeUpCounter(cnt);//����WAKE UP�Զ���װ�ؼĴ���
//	
//	
//	RTC_ClearITPendingBit(RTC_IT_WUT); //���RTC WAKE UP�ı�־
//  EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ 
//	 
//	RTC_ITConfig(RTC_IT_WUT,ENABLE);//����WAKE UP ��ʱ���ж�
//	RTC_WakeUpCmd( ENABLE);//����WAKE UP ��ʱ����
//	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE22
//  EXTI_Init(&EXTI_InitStructure);//����
// 
// 
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
//  NVIC_Init(&NVIC_InitStructure);//����
//}

////RTC�����жϷ�����
//void RTC_Alarm_IRQHandler(void)
//{    
//	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A�ж�?
//	{
//		RTC_ClearFlag(RTC_FLAG_ALRAF);//����жϱ�־
//		printf("ALARM A!\r\n");
//	}   
//	EXTI_ClearITPendingBit(EXTI_Line17);	//����ж���17���жϱ�־ 											 
//}

////RTC WAKE UP�жϷ�����
//void RTC_WKUP_IRQHandler(void)
//{    
//	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP�ж�?
//	{ 
//		RTC_ClearFlag(RTC_FLAG_WUTF);	//����жϱ�־
//	}   
//	EXTI_ClearITPendingBit(EXTI_Line22);//����ж���22���жϱ�־ 								
//}


RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;
u8 tbuf[40];

void ShowCurrentTime()
{

  if(system_var.CurrentTimeFlag == 1)
  {
    system_var.CurrentTimeFlag = 0;

	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
    //rt_kprintf("Date:20%02d-%02d-%02d  ",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date); 
    sprintf((char*)tbuf,"20%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date);
    show_date_str(16,0,tbuf);

	sprintf((char*)tbuf,"0%d",RTC_DateStruct.RTC_WeekDay);
	show_common_string(0,3,tbuf,font_size8X16);
	
    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
    //rt_kprintf("Time:%02d:%02d:%02d\r\n",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
    //sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
    //OLED_ShowString(0,0,tbuf);
	sprintf((char*)tbuf,"%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes);

	show_time_str(20,1,tbuf);

	sprintf((char*)tbuf,"%02d",RTC_TimeStruct.RTC_Seconds);
	show_common_string(104,3,tbuf,font_size8X16);
	
    
  }
}












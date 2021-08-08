#include "main.h"


//NVIC_InitTypeDef   NVIC_InitStructure;

//RTC�0�2�1�7�1�7�1�7�1�7�1�7�1�7
//hour,min,sec:���0�2,�1�7�1�7�1�7�1�7,�1�7�1�7�1�7�1�7
//ampm:@RTC_AM_PM_Definitions  :RTC_H12_AM/RTC_H12_PM
//�1�7�1�7�1�7�1�7�0�5:SUCEE(1),�1�7�0�6�1�7
//       ERROR(0),�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�0�0�0�4�0�2�1�7�1�7 
ErrorStatus RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm)
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	RTC_TimeTypeInitStructure.RTC_H12=ampm;
	
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
	
}
//RTC�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
//year,month,date:�1�7�1�7(0~99),�1�7�1�7(1~12),�1�7�1�7(0~31)
//week:�1�7�1�7�1�7�1�7(1~7,0,�1�7�0�9�1�7!)
//�1�7�1�7�1�7�1�7�0�5:SUCEE(1),�1�7�0�6�1�7
//       ERROR(0),�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�0�0�0�4�0�2�1�7�1�7 
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date,u8 week)
{
	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

//RTC�1�7�1�7�0�3�1�7�1�7
//�1�7�1�7�1�7�1�7�0�5:0,�1�7�1�7�0�3�1�7�1�7�1�7�0�6�1�7;
//       1,LSE�1�7�1�7�1�7�1�7�0�2�1�7�1�7;
//       2,�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�0�0�0�4�0�2�1�7�1�7;
u8 My_RTC_Init(void)
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF; 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//�0�0�1�7�1�7PWR�0�2�1�7�1�7
	PWR_BackupAccessCmd(ENABLE);	//�0�0�1�7�1�4�4�4�0�6�1�7�1�7�1�7�1�7�1�7�1�7�1�7 
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		//�1�7�0�9�1�7�1�7�0�5�1�7�1�7�1�7�1�7�1�7�1�7?
	{
		//RCC_LSEConfig(RCC_LSE_ON);//LSE �1�7�1�7�1�7�1�7 
    	RCC_LSICmd(ENABLE);    
		while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)	//�1�7�1�7�1�7�0�8�1�7�1�7�1�7�1�7RCC�1�7�1�7�0�4�˄1�7�1�7�1�7�1�7�1�7�1�7�1�7,�1�7�0�9�1�7�1�7�1�7�1�7�1�6�1�7�1�7�1�7�1�7�1�7�1�7
			{
			retry++;
			delay_ms(10);
			}
		if(retry==0)return 1;		//LSI �1�7�1�7�1�7�1�7�0�2�1�7�1�7. 
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//�1�7�1�7�1�7�1�7RTC�0�2�1�7�1�7(RTCCLK),�0�5�1�7�1�7LSE�1�7�1�7�0�2RTC�0�2�1�7�1�7    
		RCC_RTCCLKCmd(ENABLE);	//�0�0�1�7�1�7RTC�0�2�1�7�1�7 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�1�7�4�6�1�7�1�7�0�1�0�3�1�7�1�7(1~0X7F)
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTC�0�4�1�7�1�7�1�7�1�7�0�1�0�3�1�7�1�7(0~7FFF)
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC�1�7�1�7�1�7�1�7�0�2,24���0�2�1�7�1�7�0�4
    RTC_Init(&RTC_InitStructure);
 
    RTC_Set_Time(21,47,00,RTC_H12_PM);	//�1�7�1�7�1�7�1�7�0�2�1�7�1�7
    RTC_Set_Date(20,8,2,7);		//�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7 
 
	RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//�1�7�1�7�1�7�1�7�0�4�1�7�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7
	} 

	return 0;
}

////�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�2�1�7�1�7(�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7,24���0�2�1�7�1�7)
////week:�1�7�1�7�1�7�1�8�1�7(1~7) @ref  RTC_Alarm_Definitions
////hour,min,sec:���0�2,�1�7�1�7�1�7�1�7,�1�7�1�7�1�7�1�7
//void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec)
//{ 
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
//	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
//	
//	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//�1�7�1�9�1�7�1�7�1�7�1�7�1�7A 
//	
//  RTC_TimeTypeInitStructure.RTC_Hours=hour;//���0�2
//	RTC_TimeTypeInitStructure.RTC_Minutes=min;//�1�7�1�7�1�7�1�7
//	RTC_TimeTypeInitStructure.RTC_Seconds=sec;//�1�7�1�7
//	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
//  
//	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=week;//�1�7�1�7�1�7�1�7
//	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
//	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;//�1�7�1�7�0�2�0�5�1�7�1�7�1�7�1�7�1�7�1�3�1�7�0�2�1�7�1�7�1�7�1�7
//	RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
//  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
// 
//	
//	RTC_ClearITPendingBit(RTC_IT_ALRA);//�1�7�1�7�1�7RTC�1�7�1�7�1�7�1�7A�1�7�0�3�1�7�0�4
//  EXTI_ClearITPendingBit(EXTI_Line17);//�1�7�1�7�1�7LINE17�1�7�0�3�1�7�1�7�؁0�9�1�7�0�4�� 
//	
//	RTC_ITConfig(RTC_IT_ALRA,ENABLE);//�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7A�1�7�؄1�7
//	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7A 
//	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�1�7�؄1�7�1�7�0�4�1�7
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�1�7�1�7�1�7�1�7�1�7�1�2�1�7�1�7�1�7 
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�0�0�1�7�1�7LINE17
//  EXTI_Init(&EXTI_InitStructure);//�1�7�1�7�1�7�1�7

//	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//�1�7�1�7�0�8�1�7�1�7�1�7�0�7�1�71
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�1�7�1�7�1�7�1�7�1�7�0�7�1�72
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�0�0�1�7�1�7�1�7�8�9�1�7�؄1�7�0�0�1�7�1�7
//  NVIC_Init(&NVIC_InitStructure);//�1�7�1�7�1�7�1�7
//}

////�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�0�6�1�7�0�2�1�7�1�7�1�7�1�7�1�7�1�7  
///*wksel:  @ref RTC_Wakeup_Timer_Definitions
//#define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
//#define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
//#define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
//#define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
//#define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
//#define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
//*/
////cnt:�1�7�0�8�1�7�1�7�1�7�0�4�1�7�1�7�0�5.�1�7�1�7�1�7�1�70,�1�7�1�7�1�7�1�7�1�7�؄1�7.
//void RTC_Set_WakeUp(u32 wksel,u16 cnt)
//{ 
//	EXTI_InitTypeDef   EXTI_InitStructure;
//	
//	RTC_WakeUpCmd(DISABLE);//�1�7�1�9�1�7WAKE UP
//	
//	RTC_WakeUpClockConfig(wksel);//�1�7�1�7�1�7�1�7�0�2�1�7�1�7�0�5�1�7�1�7
//	
//	RTC_SetWakeUpCounter(cnt);//�1�7�1�7�1�7�1�7WAKE UP�1�7�0�8�1�7�1�7�1�7�0�4�1�7�1�0�0�6�1�7�1�7�1�7
//	
//	
//	RTC_ClearITPendingBit(RTC_IT_WUT); //�1�7�1�7�1�7RTC WAKE UP�1�7�0�3�1�7�0�4
//  EXTI_ClearITPendingBit(EXTI_Line22);//�1�7�1�7�1�7LINE22�1�7�0�3�1�7�1�7�؁0�9�1�7�0�4�� 
//	 
//	RTC_ITConfig(RTC_IT_WUT,ENABLE);//�1�7�1�7�1�7�1�7WAKE UP �1�7�1�7�0�2�1�7�1�7�1�7�؄1�7
//	RTC_WakeUpCmd( ENABLE);//�1�7�1�7�1�7�1�7WAKE UP �1�7�1�7�0�2�1�7�1�7�1�7�1�7
//	
//	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�1�7�؄1�7�1�7�0�4�1�7
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�1�7�1�7�1�7�1�7�1�7�1�2�1�7�1�7�1�7 
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�0�0�1�7�1�7LINE22
//  EXTI_Init(&EXTI_InitStructure);//�1�7�1�7�1�7�1�7
// 
// 
//	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//�1�7�1�7�0�8�1�7�1�7�1�7�0�7�1�71
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�1�7�1�7�1�7�1�7�1�7�0�7�1�72
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�0�0�1�7�1�7�1�7�8�9�1�7�؄1�7�0�0�1�7�1�7
//  NVIC_Init(&NVIC_InitStructure);//�1�7�1�7�1�7�1�7
//}

////RTC�1�7�1�7�1�7�1�7�1�7�؁0�5�1�7�1�7�1�7�1�7�1�7
//void RTC_Alarm_IRQHandler(void)
//{    
//	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A�1�7�؄1�7?
//	{
//		RTC_ClearFlag(RTC_FLAG_ALRAF);//�1�7�1�7�1�7�1�7�؁0�9�1�7�0�4
//		printf("ALARM A!\r\n");
//	}   
//	EXTI_ClearITPendingBit(EXTI_Line17);	//�1�7�1�7�1�7�1�7�؄1�7�1�7�1�717�1�7�1�7�1�7�؁0�9�1�7�0�4 											 
//}

////RTC WAKE UP�1�7�؁0�5�1�7�1�7�1�7�1�7�1�7
//void RTC_WKUP_IRQHandler(void)
//{    
//	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP�1�7�؄1�7?
//	{ 
//		RTC_ClearFlag(RTC_FLAG_WUTF);	//�1�7�1�7�1�7�1�7�؁0�9�1�7�0�4
//	}   
//	EXTI_ClearITPendingBit(EXTI_Line22);//�1�7�1�7�1�7�1�7�؄1�7�1�7�1�722�1�7�1�7�1�7�؁0�9�1�7�0�4 								
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












#ifndef BOOT
#include "main.h"
#else
#include "boot_main.h"
#endif

struct SystemVar system_var;
struct SystemData system_data;

uint8_t cmd_process_buff[CMD_BUFF_MAX_SIZE];
uint8_t cmd_process_buff_index=0;

void diag_help(void);
void print_weather_and_time_info(void);
void jump_app();
void quit_send_mode();
void timer_test(u8 test_action);
void flash_test(u32 addr, u32 len);
void shell_reset_cmd_handler();
int32_t diag_cmd_para_parse(char* user_para_str,diag_cmd_para_t diag_cmd_para_array[],int32_t max_para_num);

typedef int32_t cmd_func_t(uint64_t arg1,uint64_t arg2,uint64_t arg3,uint64_t arg4);
diag_cmd_descriptor_t* find_cmd(char* user_cmd_name);
static __INLINE int  to_lower_character(int c);
uint8_t diag_cmd_compare(char* user_cmd,char* diag_cmd);


diag_cmd_descriptor_t diag_base_cmd[]=
{
	{
		"help",
		"help or help cmd\r\n",
		"help cmd can print all cmd\r\n",
		diag_help,
		0,
	},
	{
		"ls",
		"list cmd\r\n",
		"list  cmd can print all cmd\r\n",
		diag_help,
		0,
	},
	{
		"quit",
		"quit_send_mode \r\n",
		"quit_send_mode cmd\r\n",
		quit_send_mode,
		0,
	},
	{
		"tim",
		"timer test \r\n",
		"timer test cmd\r\n",
		timer_test,
		1,
	},
	{
		"flash",
		"flash test \r\n",
		"flash test cmd\r\n",
		flash_test,
		2,
	},
	{
		"reset",
		"reset test \r\n",
		"reset test cmd\r\n",
		shell_reset_cmd_handler,
		1,
	},
#ifndef BOOT
	{
		"wea",
		"wea cmd\r\n",
		"print weather and time info\r\n",
		print_weather_and_time_info,
		0,
	},
	{
		"OledClear",
		"OledClear\r\n",
		"OledClear clear the oled\r\n",
		OLED_Clear,
		0,
	},
#else
	{
		"jump",
		"jump_app \r\n",
		"jump_app cmd \r\n",
		jump_app,
		0,
	},
#endif
	{
		NULL,
		NULL,
		NULL,
		NULL,
		0,	
	}
};


void diag_help()
{
	uint8_t cmd_index = 0;
	while(NULL != diag_base_cmd[cmd_index].cmd_func)
	{
		rt_kprintf(diag_base_cmd[cmd_index].cmd_short_help);
		cmd_index++;
	}
}

#ifndef BOOT

extern time_and_weather_t time_and_weather_g;

void print_weather_and_time_info()
{
	rt_kprintf("aqi=%d,temp=%d C,humi=%d %%\r\n",time_and_weather_g.api,time_and_weather_g.tempeture,time_and_weather_g.humidty);
	rt_kprintf("id=%d city=%s \r\n",time_and_weather_g.city_id,time_and_weather_g.city);
}
#else
void jump_app()
{
	jump_exec(&region_header);
}
#endif

void quit_send_mode()
{
	quit_send_data_mode_cmd();
	set_send_mode(0);
	quit_network_connect_cmd();
}


void timer_test(u8 test_action)
{
	if(0 == test_action)
	{
		TIM_SetAutoreload(TIM4,(10000-1));
		TIM_ARRPreloadConfig(TIM4,ENABLE);
		TIM_Cmd(TIM4, ENABLE);  
		rt_kprintf("cnt=%d\r\n", TIM_GetCounter(TIM4));
	}
	else if(1 == test_action)
	{
		TIM_SetAutoreload(TIM4,(10000-1));
		TIM_Cmd(TIM4, ENABLE);  
		rt_kprintf("cnt=%d\r\n", TIM_GetCounter(TIM4));
	}
	else if(2 == test_action)
	{
		TIM_Cmd(TIM4, ENABLE);  
		rt_kprintf("cnt=%d\r\n", TIM_GetCounter(TIM4));
	}
	else if(3 == test_action)
	{
		TIM4_Init();  
		rt_kprintf("cnt=%d\r\n", TIM_GetCounter(TIM4));
	}
	else if(4 == test_action)
	{
		TIM_SetAutoreload(TIM4,(10000-1));
		TIM_ARRPreloadConfig(TIM4,DISABLE);
		TIM_Cmd(TIM4, ENABLE);  
		rt_kprintf("cnt=%d\r\n", TIM_GetCounter(TIM4));
	}
	else if(5 == test_action)
	{
		rt_kprintf("cnt=%d\r\n", TIM_GetCounter(TIM4));
	}
}


void flash_test(u32 addr, u32 len)
{
	u32 i;
	u8 buf[1024];
	STMFLASH_Read(addr,(u32*)buf, len/4);
	rt_kprintf("addr=0x%x len=%d\r\n", addr, len);
	for(i=0;i<len;i++)
	{
		if((1!=0) && (i%16 == 0))
		{
			shell_printf("\r\n");
		}
		shell_printf("0x%x ",buf[i]);
	}
}


void shell_reset_cmd_handler()
{
	__disable_fiq();
	NVIC_SystemReset();
}


void shell_process()
{
	int8_t status = -1;
	if(UART_SHELL_MODE == system_var.uart1_mode)
	{
		if(UART1FramInFlag == 1 )  // rec uart data
	    {
	        UART1FramInFlag = 0;
	        FrameInlen 	    = Uart1Read(FrameInBuff);
	        status = diag_cmd_input(FrameInBuff,FrameInlen);
			if(status>=0)
			{
				diag_cmd_complete(status);
				diag_cmd_start();
			}
	    }  
	}
    else if(UART_BLE_COM_MODE == system_var.uart1_mode)
	{
		if(UART1FramInFlag == 1 )  
	    {
	        UART1FramInFlag = 0;
	        FrameInlen 	    = Uart1Read(FrameInBuff);
	        PrintfIOTPort2(FrameInBuff,FrameInlen);  //BLE communication test
	    }  
	}
	else if(UART_WIFI_COM_MODE == system_var.uart1_mode)
	{
		if(UART1FramInFlag == 1 )  
		{
	        UART1FramInFlag = 0;
	        FrameInlen 	    = Uart1Read(FrameInBuff);
	        PrintfIOTPort4(FrameInBuff,FrameInlen);  //wifi communication test
	    }  
	}
	else
	{
		/*nothing*/
	}
}

#ifndef  BOOT
void send_heart_data()
{
	u8 buff[sizeof(cmd_msg_frame_t) + sizeof(heart_data_t) + sizeof(u32)];

	cmd_msg_frame_t* msg=(cmd_msg_frame_t*)buff;
	msg->header = MSG_FRAME_HEADER;
	msg->cmd = 	HEART_CMD;
	msg->device_addr = 0x00;
	msg->seq = 0x00;
	msg->data_len = sizeof(heart_data_t);

	heart_data_t* heart_data = (heart_data_t*)(msg + 1);
	heart_data->year = time_and_weather_g.year;
	heart_data->month = time_and_weather_g.month;
	heart_data->day = time_and_weather_g.day;
	heart_data->week = time_and_weather_g.week;

	heart_data->hour = time_and_weather_g.hour;
	heart_data->minute = time_and_weather_g.minute;
	heart_data->second = time_and_weather_g.second;

	heart_data->tempture = time_and_weather_g.tempeture;
	heart_data->humidty = time_and_weather_g.humidty;

	heart_data->city_id = time_and_weather_g.city_id;
	heart_data->weather_id = 0;

	u32* check_sum = (u32*)(heart_data+1);
	*check_sum = CalCheckSum(buff,sizeof(cmd_msg_frame_t) + sizeof(heart_data_t));

	PrintfIOTPort4(buff,sizeof(cmd_msg_frame_t) + sizeof(heart_data_t) + sizeof(u32));
}
#endif

void periodic_task_process()
{
	static int last_systime = 0;
	static u8 connect_state_change = 0;
#ifndef  BOOT
	if(system_var.TwoMinuteFlag ==1)
    {
		connect_state_change = (connect_state_change + 1) % 2;
		system_var.TwoMinuteFlag = 0;
		rt_kprintf("*: two minute\r\n"); 
		if(0 == connect_state_change)
		{
			get_network_time_cmds();
			PrintHTInfo();
			print_wifi_weather_time_info();
			OLED_Clear();
		}
		else 
		{
			timer_interval_func_t para;
			para.interval = 0 ;
			para.target_time = para.interval + GetSystemTime();
			para.cb = (timer_callback)connect_host;
			para.para = NULL;
			timer_set_func(&para);

			para.interval = 15 ;
			para.target_time = para.interval + GetSystemTime();
			para.cb = (timer_callback)send_heart_data;
			para.para = NULL;
			timer_set_func(&para);

			para.interval = 20 ;
			para.target_time = para.interval + GetSystemTime();
			para.cb = (timer_callback)leave_host;
			para.para = NULL;
			timer_set_func(&para);
		}
    }
	
	if(last_systime != Systemtime)
 	{
 		LED_BreathingLight();
		last_systime = Systemtime;
	}
#else
	static int cnt_count = 0;
	if(last_systime == system_data.SystemGMTTime)
		return;
	//rt_kprintf("last_systime=%d,Systemtime=%d\r\n", last_systime, system_data.SystemGMTTime);
	last_systime = system_data.SystemGMTTime;
	cnt_count++;
	if(cnt_count<= 30)
		return;
	cnt_count = 0;
	
	if(system_var.host_cmd_flag)
		return;
	if(0 == check_header(&region_header))
	{
		rt_kprintf(" header err\r\n");
		return ;
	}
	rt_kprintf("timeout,jump app\r\n");
	timer_interval_func_t para;
	para.interval = 10 ;
	para.target_time = para.interval + GetSystemTime();
	para.cb = (timer_callback)quit_send_data_mode_cmd;
	para.para = NULL;
	timer_set_func(&para);

	para.interval = 15 ;
	para.target_time = para.interval + GetSystemTime();
	para.cb = (timer_callback)set_send_mode;
	para.para = NULL;
	timer_set_func(&para);

	para.interval = 20 ;
	para.target_time = para.interval + GetSystemTime();
	para.cb = (timer_callback)jump_exec;
	para.para = &region_header;
	timer_set_func(&para);
#endif
	

}


int8_t diag_cmd_input(uint8_t *cmd_buff,uint16_t cmd_buff_len)
{
	int8_t status=0;	
	if(0 == cmd_buff_len)
		return status;
	else if(cmd_process_buff_index >= (CMD_BUFF_MAX_SIZE-1))
	{
		status = diag_cmd_process(cmd_process_buff,cmd_process_buff_index);
		cmd_process_buff_index=0;
		return status;
	}
	else if(1 == cmd_buff_len)
	{
		if((TERMINAL_NEWLINE == cmd_buff[cmd_buff_len-1]) || \
			(RETURN == cmd_buff[cmd_buff_len-1]))
		{
			shell_printf("\r\n");
			
			cmd_process_buff[cmd_process_buff_index]=0;
			cmd_process_buff_index++;
			status = diag_cmd_process(cmd_process_buff,cmd_process_buff_index);
			cmd_process_buff_index=0;
			return status;
		}
		else 
		{
			cmd_buff[1]=0;
			shell_printf(cmd_buff);
			cmd_process_buff[cmd_process_buff_index]=cmd_buff[0];
			cmd_process_buff_index++;
			if(cmd_process_buff_index >= (CMD_BUFF_MAX_SIZE-1))
				status = diag_cmd_process(cmd_process_buff,cmd_process_buff_index);
			else
				status=-1;
			return status;
		}
	}
	else
	{
		if((RETURN == cmd_buff[cmd_buff_len-1]) && (TERMINAL_NEWLINE != cmd_buff[cmd_buff_len-2]))
		{
			cmd_buff[cmd_buff_len-1] = 0;
			shell_printf(cmd_buff);
			shell_printf("\r\n");

			if((cmd_process_buff_index +cmd_buff_len)  <= (CMD_BUFF_MAX_SIZE-1))
			{
				memcpy(&cmd_process_buff[cmd_process_buff_index],cmd_buff,cmd_buff_len);
				cmd_process_buff_index+=(cmd_buff_len);
			}
			status = diag_cmd_process(cmd_process_buff,cmd_process_buff_index);
			cmd_process_buff_index=0;
			return status;
		}
		else if((RETURN == cmd_buff[cmd_buff_len-1]) && (TERMINAL_NEWLINE == cmd_buff[cmd_buff_len-2]))
		{
			cmd_buff[cmd_buff_len-2] = 0;
			cmd_buff[cmd_buff_len-1] = 0;
			shell_printf(cmd_buff);
			shell_printf("\r\n");

			if((cmd_process_buff_index +cmd_buff_len-1)  <= (CMD_BUFF_MAX_SIZE-1))
			{
				memcpy(&cmd_process_buff[cmd_process_buff_index],cmd_buff,cmd_buff_len - 1);
				cmd_process_buff_index+=(cmd_buff_len-1);
			}
						
			status = diag_cmd_process(cmd_process_buff,cmd_process_buff_index);
			cmd_process_buff_index=0;
			return status;
		}
		else if((TERMINAL_NEWLINE == cmd_buff[cmd_buff_len-1]))
		{
			cmd_buff[cmd_buff_len-1] = 0;
			shell_printf(cmd_buff);
			shell_printf("\r\n");

			if((cmd_process_buff_index +cmd_buff_len)  <= (CMD_BUFF_MAX_SIZE-1))
			{
				memcpy(&cmd_process_buff[cmd_process_buff_index],cmd_buff,cmd_buff_len);
				cmd_process_buff_index+=(cmd_buff_len);
			}
			
			status = diag_cmd_process(cmd_process_buff,cmd_process_buff_index);
			cmd_process_buff_index=0;
			return status;
		}
		cmd_buff[cmd_buff_len]=0;
		shell_printf(cmd_buff);
		if((cmd_process_buff_index +cmd_buff_len)  <= (CMD_BUFF_MAX_SIZE-1))
		{
			memcpy(&cmd_process_buff[cmd_process_buff_index],cmd_buff,cmd_buff_len);
			cmd_process_buff_index+=(cmd_buff_len);
			status=-1;
		}
		else
		{
			status = diag_cmd_process(cmd_process_buff,cmd_process_buff_index);
			cmd_process_buff_index=0;
		}
	}
	return status;
}



int8_t diag_cmd_process(uint8_t *cmd_buff,uint16_t cmd_buff_len)
{
	char* cmd_name;
	char* cmd_para;

	uint8_t cur_index=0;
	diag_cmd_descriptor_t *curr_cmd;

	diag_cmd_para_t diag_cmd_para_array[MAX_CMD_PARA];
	int8_t diag_cmd_para_num;

	uint8_t i;
	
	while(SPACE == cmd_buff[cur_index])
	{
		cur_index++;
	}
	cmd_name = &cmd_buff[cur_index];

	while((SPACE != cmd_buff[cur_index]) && (0 != cmd_buff[cur_index]))
	{
		cur_index++;
	}
	if(SPACE == cmd_buff[cur_index])
	{
		cmd_buff[cur_index]=0;
		cur_index++;
	}

	
	while(SPACE == cmd_buff[cur_index])
	{
		cur_index++;
	}

	cmd_para= &cmd_buff[cur_index];

	if((0 == cmd_name[0]) || (SPACE == cmd_name[0]) || ('#' == cmd_name[0]))
		return 0;
	curr_cmd = find_cmd(cmd_name);

	if(NULL == curr_cmd)
		return 1;
	if(0 == curr_cmd->num_args)
	{
		curr_cmd->cmd_func();
		return 0;
	}
	if(FUNC_TYPE_PARA_STRING == curr_cmd->num_args)
	{
		typedef int32_t cmd_func_t(char* para);
		cmd_func_t *diag_cmd_func;
		
		diag_cmd_func = (cmd_func_t *)curr_cmd->cmd_func;
		diag_cmd_func(cmd_para);
		return 0;
	}
	
	diag_cmd_para_num = diag_cmd_para_parse(cmd_para,diag_cmd_para_array,MAX_CMD_PARA);

	shell_printf("%d %d %d %d\r\n", diag_cmd_para_array[0].value,diag_cmd_para_array[1].value,diag_cmd_para_array[2].value,diag_cmd_para_array[3].value);
	if(diag_cmd_para_num < 0)
	{
		rt_kprintf("Error: too many parameters\r\n");
		return 1;
	}

	if(FUNC_TYPE_PARA_CHANGE == curr_cmd->num_args)
	{
		typedef int32_t cmd_func_t(diag_cmd_para_t *para_array, int32_t para_num);
		cmd_func_t *diag_cmd_func;

		diag_cmd_func = (cmd_func_t *)curr_cmd->cmd_func;
		diag_cmd_func(diag_cmd_para_array,diag_cmd_para_num);
		return 0;
	}

	if(diag_cmd_para_num != curr_cmd->num_args)
	{
		rt_kprintf("Error: this cmd(%s) expect %d parameters\r\n",curr_cmd->cmd_name,curr_cmd->num_args);
		return 2;
	}

	for(i=0;i<diag_cmd_para_num;i++)
	{
		if(diag_cmd_para_array[i].para_type != diag_cmd_para_number)
		{
			
			rt_kprintf("Error: all the parameters msut be numbers\r\n");
			return 3;
		}
	}

	cmd_func_t* diag_cmd_func;
	diag_cmd_func = (cmd_func_t*)curr_cmd->cmd_func;
	
	diag_cmd_func(diag_cmd_para_array[0].value,
				  diag_cmd_para_array[1].value,
				  diag_cmd_para_array[2].value,
				  diag_cmd_para_array[3].value);

	
	
	return 0;
}

diag_cmd_descriptor_t* find_cmd(char* user_cmd_name)
{
	uint8_t cmd_index = 0;
	int8_t cmp_res = 0;
	diag_cmd_descriptor_t *potential_match = NULL;
	while(NULL != diag_base_cmd[cmd_index].cmd_func)
	{
		cmp_res = diag_cmd_compare(user_cmd_name,diag_base_cmd[cmd_index].cmd_name);
		switch(cmp_res)
		{
			case 2:
			{
				return &diag_base_cmd[cmd_index];
			}
			case 1:
			{
				if(potential_match)
				{
					rt_kprintf("ERROR:ambious cmd: %s %s\r\n",potential_match->cmd_name,diag_base_cmd[cmd_index].cmd_name);
					return NULL;
				}
				potential_match = &diag_base_cmd[cmd_index];
			}
			break;
			default:break;
		}
		cmd_index++;
	}
	if(NULL == potential_match)
	{
		rt_kprintf("ERROR:cmd %s unknow or unavaliable\r\n",user_cmd_name);
	}
	return potential_match;
}

uint8_t diag_cmd_compare(char* user_cmd,char* diag_cmd)
{
	while(1)
	{
		if(0 == *user_cmd )
		{
			if(0 == *diag_cmd)
			{
				/* exact match */
				
				return 2;
			}
			return 1;
		}
		if(0 == *diag_cmd)
		{
			/* not at all match because user_cmd is more long */
			
			return 0;
		}
		if(to_lower_character(*user_cmd++) != to_lower_character(*diag_cmd++))
		{
			return 0;
		}
	}
	
}

static __INLINE int  to_lower_character(int c)
{
	if((c > 'Z') || (c < 'A'))
	{
		
return c;
	}

	return (c + ('a'-'A'));
}


int32_t diag_cmd_para_parse(char* user_para_str,diag_cmd_para_t diag_cmd_para_array[],int32_t max_para_num)
{
	int32_t para_num=0;
	int32_t curr_index=0;
	int32_t start_index;
	data_type_e  temp_para_type;

	while(1)
	{
		while(SPACE == user_para_str[curr_index])
		{
			curr_index++;
		}
		start_index=curr_index;

		if(0 == user_para_str[curr_index])
		{
			return para_num;
		}

		if(para_num > max_para_num)
		{
			return -1;
		}
		diag_cmd_para_array[para_num].str = &user_para_str[start_index];
		diag_cmd_para_array[para_num].value = 0;

		if('0' == user_para_str[curr_index])
		{
			diag_cmd_para_array[para_num].para_type = diag_cmd_para_number;

			if(('x' == user_para_str[curr_index + 1]) || ('X' == user_para_str[curr_index + 1]))
			{
				temp_para_type = data_hex;
				curr_index+=2;
			}
			else
			{
				temp_para_type=data_num;
			}
		}
		else if(('0' < user_para_str[curr_index]) && ('9' > user_para_str[curr_index]))
		{
			
			temp_para_type=data_num;
			diag_cmd_para_array[para_num].para_type = diag_cmd_para_number;
		}
		else
		{
			
			temp_para_type=data_str;
			diag_cmd_para_array[para_num].para_type = diag_cmd_para_string;
		}


		while((user_para_str[curr_index]!=0) &&( user_para_str[curr_index]!=' '))
		{
 
			switch(temp_para_type)
			{
				case data_num:
				{
					if(('0' <= user_para_str[curr_index]) && ('9' >= user_para_str[curr_index]))
					{
						diag_cmd_para_array[para_num].value*=10;
						diag_cmd_para_array[para_num].value+=(uint32_t)(user_para_str[curr_index] - '0');
					}
					else
					{
						temp_para_type=data_str;
						diag_cmd_para_array[para_num].para_type = diag_cmd_para_string;
						diag_cmd_para_array[para_num].str = &user_para_str[start_index];
					}
				}
					break;
				case data_hex:
				{
					if(('0' <= user_para_str[curr_index]) && ('9' >= user_para_str[curr_index]))
					{
						diag_cmd_para_array[para_num].value <<=4;
						diag_cmd_para_array[para_num].value+=(uint32_t)(user_para_str[curr_index] - '0');
					}
					else if(('a' <= user_para_str[curr_index]) && ('f' >= user_para_str[curr_index]))
					{
						diag_cmd_para_array[para_num].value <<=4;
						diag_cmd_para_array[para_num].value+=(uint32_t)(user_para_str[curr_index] - 'a') + 10;
					}
					else if(('A' <= user_para_str[curr_index]) && ('F' >= user_para_str[curr_index]))
					{
						diag_cmd_para_array[para_num].value <<=4;
						diag_cmd_para_array[para_num].value+=(uint32_t)(user_para_str[curr_index] - 'A') + 10;
					}
					else 
					{
						temp_para_type=data_str;
						diag_cmd_para_array[para_num].para_type = diag_cmd_para_string;
						diag_cmd_para_array[para_num].str = &user_para_str[start_index];
					}
				}
					break;
				case data_str:

					break;
				default:
				break;
			}
			curr_index++;
		}
		para_num++;
		if(SPACE == user_para_str[curr_index])
		{
			user_para_str[curr_index]=0;
			curr_index++;
		}
	}
	return para_num;
}


void shell_init()
{
  UART1Init();       /*common uart,can receive uart device(ch340) data and execute cmd*/
  queue_init(&MyQueue);
  rt_kprintf("shell init ok\r\n");
  diag_cmd_start();
  rt_kprintf("\r\n");
}

void diag_cmd_complete(int8_t status)
{
	shell_printf("RV:%d\r\n",status);
}

void diag_cmd_start()
{
	shell_printf("%s","shell> ");
}



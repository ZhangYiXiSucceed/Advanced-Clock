#ifndef BOOT
#include "main.h"
#else
#include "boot_main.h"
ota_package_info_t ota_info_g;
#endif

u32 CalCheckSum(uint8_t* Data, uint32_t len)
{
    int CheckSum = 0;
    for(int i=0;i<len;i++)
    {
        CheckSum+=Data[i];
    }
    return CheckSum;
}

void printf_buf(u8* data, u16 len)
{
	int i;
	for(i=0;i<len;i++)
	{
		if((1!=0) && (i%16 == 0))
		{
			shell_printf("\r\n");
		}
		shell_printf("0x%2x ",data[i]);
	}
}

void msg_rsp_packet_and_send(u16 cmd, u8 err_code)
{
	u8 cmd_data[sizeof(cmd_msg_frame_t) + 1 + 4];
	cmd_msg_frame_t* msg = (cmd_msg_frame_t*)cmd_data;

	msg->header = MSG_FRAME_HEADER;
	msg->cmd = cmd;
	msg->device_addr = 0x00;
	msg->seq = 0x00;
	msg->data_len = 0x01;

	u8* temp_err_code = (u8*)&cmd_data[sizeof(cmd_msg_frame_t)];
	*temp_err_code = err_code;

	u32* check_sum = (u32*)&cmd_data[sizeof(cmd_msg_frame_t) + 1 ];
	*check_sum = CalCheckSum(cmd_data,sizeof(cmd_msg_frame_t) + 1);

	PrintfIOTPort4(cmd_data,sizeof(cmd_msg_frame_t) + 1 + 4);
}

cmd_process_errcode_e server_msg_process(u8 *packet,u16 len)
{
    cmd_msg_frame_t *cmd_msg_frame = (cmd_msg_frame_t *)packet;
	cmd_process_errcode_e res = MSG_OK;
	system_var.host_cmd_flag = 1;
	if(MSG_FRAME_HEADER  != cmd_msg_frame->header)
	{
		rt_kprintf("frame header err,0x%x\r\n", cmd_msg_frame->header);
		return MSG_HEADER_ERR;
	}

	switch(cmd_msg_frame->cmd)
	{
		case HEART_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}
			u8* msg_data = (u8*)(cmd_msg_frame + 1);
			server_heart_rsp_t *server_heart_rsp = (server_heart_rsp_t *)msg_data;
			rt_kprintf("res=%x\r\n", server_heart_rsp->rsp_res);
		}
		break;
		case CONNECT_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}
		}
		break;
		case RESET_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}

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
			para.cb = (timer_callback)NVIC_SystemReset;
			para.para = NULL;
			timer_set_func(&para);
		}break;
		case VERSION_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}

			rt_kprintf("get version=0x%x\r\n", region_header.version);

			u8 cmd_data[sizeof(cmd_msg_frame_t) + sizeof(version_info_t) + 4];
			cmd_msg_frame_t* msg = (cmd_msg_frame_t*)cmd_data;

			msg->header = MSG_FRAME_HEADER;
			msg->cmd = VERSION_CMD;
			msg->device_addr = 0x00;
			msg->seq = 0x00;
			msg->data_len = sizeof(version_info_t);

			u8* msg_data = (u8*)(msg + 1);
			version_info_t *version_rsp = (version_info_t *)msg_data;
			version_rsp->hardware_version = 0x20200808;
			version_rsp->software_version = region_header.version;
			version_rsp->ota_attr = OTA_BOOT_ATTR;
			version_rsp->rsv2 = 0x0;

			u32* check_sum = (u32*)&cmd_data[sizeof(cmd_msg_frame_t) + sizeof(version_info_t)];
			*check_sum = CalCheckSum(cmd_data,sizeof(cmd_msg_frame_t) + sizeof(version_info_t));

			PrintfIOTPort4(cmd_data,sizeof(cmd_msg_frame_t) + sizeof(version_info_t) + 4);
		}break;
#ifdef BOOT
		case JUMP_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}
			if(0 == check_header(&region_header))
			{
				rt_kprintf("update header err\r\n");
				res = UPDATE_HEADER_ERR;
				goto err;
			}
			rt_kprintf("enter jump\r\n");

			u8 cmd_data[sizeof(cmd_msg_frame_t) + 1 + 4];
			cmd_msg_frame_t* msg = (cmd_msg_frame_t*)cmd_data;

			timer_interval_func_t para;
			para.interval = 10 ;
			para.target_time = para.interval + GetSystemTime();
			para.cb = (timer_callback)quit_send_data_mode_cmd;
			para.para = NULL;
			timer_set_func(&para);

			para.interval = 12 ;
			para.target_time = para.interval + GetSystemTime();
			para.cb = (timer_callback)set_send_mode;
			para.para = NULL;
			timer_set_func(&para);

			para.interval = 14 ;
			para.target_time = para.interval + GetSystemTime();
			para.cb = (timer_callback)jump_exec;
			para.para = &region_header;
			timer_set_func(&para);
		}break;
		case START_UPDATE:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + sizeof(ota_package_info_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t) + sizeof(ota_package_info_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t) + sizeof(ota_package_info_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}
			
			u8* msg_data = (u8*)(cmd_msg_frame + 1);
			ota_info_g = *(ota_package_info_t *)msg_data;

			rt_kprintf("ota info, package num=%d, bin size=%d, check_sum=%d\r\n", ota_info_g.package_num, ota_info_g.bin_size, ota_info_g.check_sum);
			FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3);
		}break;
		case UPDATE_DATA:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE);
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE + 1));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}

			rt_kprintf("ota seq=%d\r\n", cmd_msg_frame->seq);
			STMFLASH_Write(APP_FLASH_ADDR + OTA_ONE_PACKAGE_SIZE*cmd_msg_frame->seq,(u32*)&packet[sizeof(cmd_msg_frame_t)],256);
			//printf_buf(&packet[sizeof(cmd_msg_frame_t)],1024);

		}break;
		case UPDATE_END:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}
			u32 check_sum = CalCheckSum((char*)APP_FLASH_ADDR, ota_info_g.bin_size);
			rt_kprintf("ota end,check_sum=%d, size=%d\r\n", check_sum, ota_info_g.bin_size);

			if(check_sum == ota_info_g.check_sum)
			{
				rt_kprintf("start jump\r\n");

				timer_interval_func_t para;
				para.interval = 10 ;
				para.target_time = para.interval + GetSystemTime();
				para.cb = (timer_callback)quit_send_data_mode_cmd;
				para.para = NULL;
				timer_set_func(&para);

				para.interval = 12 ;
				para.target_time = para.interval + GetSystemTime();
				para.cb = (timer_callback)set_send_mode;
				para.para = NULL;
				timer_set_func(&para);

				para.interval = 14 ;
				para.target_time = para.interval + GetSystemTime();
				para.cb = (timer_callback)jump_exec;
				para.para = &region_header;
				timer_set_func(&para);
			}
			else{
				rt_kprintf("cal_check_sum=%d, file_check_sum=%d\r\n", check_sum, ota_info_g.check_sum);
			}
		}break;
#endif
#ifndef BOOT
		case CONNECT_MODE_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + sizeof(connect_mode_set_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t) + sizeof(connect_mode_set_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t) +  sizeof(connect_mode_set_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}
			u8* msg_data = (u8*)(cmd_msg_frame + 1);
			connect_mode_set_t *connect_mode_set = (connect_mode_set_t *)msg_data;
			system_var.connect_mode = connect_mode_set->mode;

			if(WEATHER_MODE == connect_mode_set->mode)
			{
				rt_kprintf("set wearher mode\r\n");
				timer_interval_func_t para;
				para.interval = 0 ;
				para.target_time = para.interval + GetSystemTime();
				para.cb = (timer_callback)leave_host;
				para.para = NULL;
				timer_set_func(&para);

				system_var.TwoMinuteFlag = 0;
			}
			else if(HOST_MODE == connect_mode_set->mode)
			{
				rt_kprintf("set connect host mode\r\n");
			}
		}break;
		case PICTURE_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE);
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE + 1));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}

			if(0 == (buf_control_flag_g & BUF_USE_0_MARK))
			{
				buf_control_flag_g |= BUF_FULL_0_MARK;
				memcpy(picture_frame_buf_g[0],&packet[sizeof(cmd_msg_frame_t)],PICTURE_FRMAE_SIZE);
			}
			else if(0 == (buf_control_flag_g & BUF_USE_1_MARK))
			{
				buf_control_flag_g |= BUF_FULL_1_MARK;
				memcpy(picture_frame_buf_g[1],&packet[sizeof(cmd_msg_frame_t)],PICTURE_FRMAE_SIZE);
			}
		}break;

		case OLED_SHOW_MODE_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + sizeof(oled_show_mode_set_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t) + sizeof(oled_show_mode_set_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t) +  sizeof(oled_show_mode_set_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}
			u8* msg_data = (u8*)(cmd_msg_frame + 1);
			oled_show_mode_set_t *oled_show_mode_set = (oled_show_mode_set_t *)msg_data;

			if(SHOW_TIME_MODE == oled_show_mode_set->mode)
			{
				rt_kprintf("set  time mode\r\n");
				set_show_state_change(city);
			}
			else if(SHOW_PICTURE_MODE == oled_show_mode_set->mode)
			{
				rt_kprintf("set picture mode\r\n");
				set_show_state_change(picture);
			}
		}break;
		case SET_TIME_DATE:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + sizeof(time_and_date_set_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				res = MSG_LEN_ERR;
				goto err;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t) + sizeof(time_and_date_set_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t) +  sizeof(time_and_date_set_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,read_sum);
				res = MSG_CRC_ERR;
				goto err;
			}
			u8* msg_data = (u8*)(cmd_msg_frame + 1);
			time_and_date_set_t *data_set = (time_and_date_set_t *)msg_data;
			

			rt_kprintf("year=%d month=%d dat=%d week=%d\r\n", data_set->year%100,data_set->month,data_set->day,data_set->week);
			rt_kprintf("hour=%d min=%d sec=%d\r\n", data_set->hour,data_set->minute,data_set->second);

			RTC_Set_Date(data_set->year%100,data_set->month,data_set->day,data_set->week);

			if(data_set->hour >= 12)
			{
				RTC_Set_Time(data_set->hour,data_set->minute,data_set->second,RTC_H12_PM);	
			}
			else
			{
				RTC_Set_Time(data_set->hour,data_set->minute,data_set->second,RTC_H12_AM);	
			}
			time_and_weather_g.year = data_set->year;
			time_and_weather_g.month = data_set->month;
			time_and_weather_g.day = data_set->day;
			time_and_weather_g.week = data_set->week;

			time_and_weather_g.hour = data_set->hour ;
			time_and_weather_g.minute = data_set->minute;
			time_and_weather_g.second = data_set->second;
		}break;
#endif
		default:
			rt_kprintf("cmd  err,0x%x\r\n", cmd_msg_frame->cmd);
			res =  MSG_CMD_ERR;
			goto err;
	}
	if((HEART_CMD != cmd_msg_frame->cmd) && (VERSION_CMD != cmd_msg_frame->cmd) && (PICTURE_CMD != cmd_msg_frame->cmd))
	{
		msg_rsp_packet_and_send(cmd_msg_frame->cmd,res);
	}
	
	return MSG_OK;
err:
	msg_rsp_packet_and_send(cmd_msg_frame->cmd,res);
	return res;
}	




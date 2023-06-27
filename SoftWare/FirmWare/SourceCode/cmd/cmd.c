#ifndef BOOT
#include "main.h"
#else
#include "boot_main.h"
#endif

u32 CalCheckSum(uint8_t* Data, uint16_t len)
{
    int CheckSum = 0;
    for(int i=0;i<len;i++)
    {
        CheckSum+=Data[i];
    }
    return CheckSum;
}

cmd_process_errcode_e server_msg_process(u8 *packet,u16 len)
{
    cmd_msg_frame_t *cmd_msg_frame = (cmd_msg_frame_t *)packet;
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
				return MSG_LEN_ERR;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,cal_sum);
				return MSG_CRC_ERR;
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
				return MSG_LEN_ERR;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,cal_sum);
				return MSG_CRC_ERR;
			}
			u8 cmd_data[sizeof(cmd_msg_frame_t) + 1 + 4];
			cmd_msg_frame_t* msg = (cmd_msg_frame_t*)cmd_data;

			msg->header = MSG_FRAME_HEADER;
			msg->cmd = CONNECT_CMD;
			msg->device_addr = 0x00;
			msg->seq = 0x00;
			msg->data_len = 0x01;

			u8* err_code = (u8*)&cmd_data[sizeof(cmd_msg_frame_t)];
			*err_code = 0x00;

			u32* check_sum = (u32*)&cmd_data[sizeof(cmd_msg_frame_t) + 1 ];
			*check_sum = CalCheckSum(cmd_data,sizeof(cmd_msg_frame_t) + 1);


			PrintfIOTPort4(cmd_data,sizeof(cmd_msg_frame_t) + 1 + 4);
		}
		break;
		case RESET_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				return MSG_LEN_ERR;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,cal_sum);
				return MSG_CRC_ERR;
			}

			u8 cmd_data[sizeof(cmd_msg_frame_t) + 1 + 4];
			cmd_msg_frame_t* msg = (cmd_msg_frame_t*)cmd_data;

			msg->header = MSG_FRAME_HEADER;
			msg->cmd = 	RESET_CMD;
			msg->device_addr = 0x00;
			msg->seq = 0x00;
			msg->data_len = 0x01;

			u8* err_code = (u8*)&cmd_data[sizeof(cmd_msg_frame_t)];
			*err_code = 0x00;

			u32* check_sum = (u32*)&cmd_data[sizeof(cmd_msg_frame_t) + 1 ];
			*check_sum = CalCheckSum(cmd_data,sizeof(cmd_msg_frame_t) + 1);

			PrintfIOTPort4(cmd_data,sizeof(cmd_msg_frame_t) + 1 + 4);


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
#ifdef BOOT
		case JUMP_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				return MSG_LEN_ERR;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,cal_sum);
				return MSG_CRC_ERR;
			}
			if(0 == check_header(&region_header))
			{
				rt_kprintf("update header err\r\n");
				return UPDATE_HEADER_ERR;
			}
			rt_kprintf("enter jump\r\n");

			u8 cmd_data[sizeof(cmd_msg_frame_t) + 1 + 4];
			cmd_msg_frame_t* msg = (cmd_msg_frame_t*)cmd_data;

			msg->header = MSG_FRAME_HEADER;
			msg->cmd = 	JUMP_CMD;
			msg->device_addr = 0x00;
			msg->seq = 0x00;
			msg->data_len = 0x01;

			u8* err_code = (u8*)&cmd_data[sizeof(cmd_msg_frame_t)];
			*err_code = 0x00;

			u32* check_sum = (u32*)&cmd_data[sizeof(cmd_msg_frame_t) + 1 ];
			*check_sum = CalCheckSum(cmd_data,sizeof(cmd_msg_frame_t) + 1);

			PrintfIOTPort4(cmd_data,sizeof(cmd_msg_frame_t) + 1 + 4);
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
#endif
		case PICTURE_CMD:
		{
			u16 data_len = cmd_msg_frame->data_len;
			if(data_len > 0x40)
			{
				rt_kprintf("data len err,%d\r\n", data_len);
				return MSG_LEN_ERR;
			}
		}break;
		case START_UPDATE:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				return MSG_LEN_ERR;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,cal_sum);
				return MSG_CRC_ERR;
			}
			
			u8* msg_data = (u8*)(cmd_msg_frame + 1);
			ota_package_info_t *info = (ota_package_info_t *)msg_data;

			rt_kprintf("ota info, package num=%d, bin size=%d, check_sum=%d\r\n", info->package_num, info->bin_size, info->check_sum);

			u8 cmd_data[sizeof(cmd_msg_frame_t) + 1 + 4];
			cmd_msg_frame_t* msg = (cmd_msg_frame_t*)cmd_data;

			msg->header = MSG_FRAME_HEADER;
			msg->cmd = 	START_UPDATE;
			msg->device_addr = 0x00;
			msg->seq = 0x00;
			msg->data_len = 0x01;

			u8* err_code = (u8*)&cmd_data[sizeof(cmd_msg_frame_t)];
			*err_code = 0x00;

			u32* check_sum = (u32*)&cmd_data[sizeof(cmd_msg_frame_t) + 1 ];
			*check_sum = CalCheckSum(cmd_data,sizeof(cmd_msg_frame_t) + 1);

			PrintfIOTPort4(cmd_data,sizeof(cmd_msg_frame_t) + 1 + 4);
		}break;
		case UPDATE_DATA:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				return MSG_LEN_ERR;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,cal_sum);
				return MSG_CRC_ERR;
			}

			rt_kprintf("ota seq=%d\r\n", cmd_msg_frame->seq);

			u8 cmd_data[sizeof(cmd_msg_frame_t) + 1 + 4];
			cmd_msg_frame_t* msg = (cmd_msg_frame_t*)cmd_data;

			msg->header = MSG_FRAME_HEADER;
			msg->cmd = 	UPDATE_DATA;
			msg->device_addr = 0x00;
			msg->seq = 0x00;
			msg->data_len = 0x01;

			u8* err_code = (u8*)&cmd_data[sizeof(cmd_msg_frame_t)];
			*err_code = 0x00;

			u32* check_sum = (u32*)&cmd_data[sizeof(cmd_msg_frame_t) + 1 ];
			*check_sum = CalCheckSum(cmd_data,sizeof(cmd_msg_frame_t) + 1);

			PrintfIOTPort4(cmd_data,sizeof(cmd_msg_frame_t) + 1 + 4);
		}break;
		case UPDATE_END:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				return MSG_LEN_ERR;
			}
			u32 cal_sum = CalCheckSum(packet,sizeof(cmd_msg_frame_t));
			u32 read_sum = *((u32*)(packet + sizeof(cmd_msg_frame_t)));
			if(cal_sum != read_sum)
			{
				rt_kprintf("frame check err,%x %x\r\n", cal_sum,cal_sum);
				return MSG_CRC_ERR;
			}

			u8 cmd_data[sizeof(cmd_msg_frame_t) + 1 + 4];
			cmd_msg_frame_t* msg = (cmd_msg_frame_t*)cmd_data;

			msg->header = MSG_FRAME_HEADER;
			msg->cmd = 	UPDATE_END;
			msg->device_addr = 0x00;
			msg->seq = 0x00;
			msg->data_len = 0x01;

			u8* err_code = (u8*)&cmd_data[sizeof(cmd_msg_frame_t)];
			*err_code = 0x00;

			u32* check_sum = (u32*)&cmd_data[sizeof(cmd_msg_frame_t) + 1 ];
			*check_sum = CalCheckSum(cmd_data,sizeof(cmd_msg_frame_t) + 1);

			PrintfIOTPort4(cmd_data,sizeof(cmd_msg_frame_t) + 1 + 4);
		}break;
		default:
			rt_kprintf("cmd  err,0x%x\r\n", cmd_msg_frame->cmd);
			return MSG_CMD_ERR;
			break;
	}
	
	return MSG_OK;
}




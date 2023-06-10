#ifndef BOOT
#include "main.h"
#else
#include "boot_main.h"
#endif


cmd_process_errcode_e server_msg_process(u8 *packet,u16 len)
{
    cmd_msg_frame_t *cmd_msg_frame = (cmd_msg_frame_t *)packet;

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
			u32 cal_sum = 0;
			u32 read_sum = 0;
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
#ifdef BOOT
		case JUMP_CMD:
		{
			u16 cmd_len = sizeof(cmd_msg_frame_t) + 4;
			if(cmd_len != len)
			{
				rt_kprintf("frame len err,%d %d\r\n", cmd_len,len);
				return MSG_LEN_ERR;
			}
			u32 cal_sum = 0;
			u32 read_sum = 0;
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
			u16 data_len = cmd_msg_frame->data_len;
			if(data_len > 0x40)
			{
				rt_kprintf("data len err,%d\r\n", data_len);
				return MSG_LEN_ERR;
			}
		}break;
		case UPDATE_DATA:
		{
			u16 data_len = cmd_msg_frame->data_len;
			if(data_len > 0x40)
			{
				rt_kprintf("data len err,%d\r\n", data_len);
				return MSG_LEN_ERR;
				
			}
		}break;
		case UPDATE_END:
		{
			u16 data_len = cmd_msg_frame->data_len;
			if(data_len > 0x40)
			{
				rt_kprintf("data len err,%d\r\n", data_len);
				return MSG_LEN_ERR;
			}
		}break;
		default:
			rt_kprintf("cmd  err,0x%x\r\n", cmd_msg_frame->cmd);
			return MSG_CMD_ERR;
			break;
	}
	
	return MSG_OK;
}




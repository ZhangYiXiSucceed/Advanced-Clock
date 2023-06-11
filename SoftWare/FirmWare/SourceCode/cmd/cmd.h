#ifndef __CMD_H__
#define __CMD_H__

#include "stm32f4xx.h"

#define MSG_FRAME_HEADER 0x676A737A

typedef enum cmd_process_errcode_enum
{
	MSG_OK,
	MSG_HEADER_ERR,
	MSG_CMD_ERR,
	MSG_LEN_ERR,
	MSG_CRC_ERR,
	UPDATE_HEADER_ERR,
	
}cmd_process_errcode_e;


enum 
{
	IDLE_CMD,
	HEART_CMD,
	PICTURE_CMD,
	START_UPDATE,
	UPDATE_DATA,
	UPDATE_END,
	JUMP_CMD,
	CONNECT_CMD,
};
	
typedef struct cmd_msg_frame_struct
{
    u32 header;
	u16 device_addr;
	u16  cmd;
	u16  seq;
	u16 data_len;
}cmd_msg_frame_t;


typedef struct server_heart_rsp_struct
{
	u8 rsp_res;
}server_heart_rsp_t;


cmd_process_errcode_e server_msg_process(u8* packet,u16 len);

#endif


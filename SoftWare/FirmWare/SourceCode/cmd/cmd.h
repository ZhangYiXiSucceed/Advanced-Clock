#ifndef __CMD_H__
#define __CMD_H__

#include "stm32f4xx.h"

#define MSG_FRAME_HEADER 0x676A737A
#define OTA_ONE_PACKAGE_SIZE   1024
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
	RESET_CMD,
};
	
typedef struct cmd_msg_frame_struct
{
    u32 header;
	u16 device_addr;
	u16  cmd;
	u16  seq;
	u16 data_len;
}cmd_msg_frame_t;

typedef struct heart_data_struct
{
	u8 tempture;
	u8 humidty;
	
	u16 year;
	u8 month;
	u8 day;
	u8 week;
	
	u8 hour;
	u8 minute;
	u8 second;

	u8 city_id;
	u8 weather_id;

	u32 rsv1;
	u32 rsv2;
}heart_data_t;

typedef struct server_heart_rsp_struct
{
	u8 rsp_res;
}server_heart_rsp_t;

typedef struct ota_package_info_struct
{
    uint32_t bin_size;
    uint32_t package_num;
    uint32_t check_sum;
    uint32_t rsv;
}ota_package_info_t;

cmd_process_errcode_e server_msg_process(u8* packet,u16 len);
u32 CalCheckSum(uint8_t* Data, uint16_t len);
#endif


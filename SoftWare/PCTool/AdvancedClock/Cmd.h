#ifndef CMD_H
#define CMD_H
#include <stdint.h>

#define MSG_FRAME_HEADER 0x676A737A

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
    Reset_Cmd,
};


typedef struct cmd_msg_frame_struct{
    uint32_t header;
    uint16_t device_addr;
    uint16_t cmd;
    uint16_t seq;
    uint16_t data_len;
}cmd_msg_frame_t;

typedef struct heart_data_struct
{
    uint8_t tempture;
    uint8_t humidty;

    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t week;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    uint8_t city_id;
    uint8_t weather_id;

    uint32_t rsv1;
    uint32_t rsv2;
}heart_data_t;

typedef struct server_heart_rsp_struct
{
    uint8_t rsp_res;
}server_heart_rsp_t;

int CalCheckSum(uint8_t* Data, uint16_t len);
#endif // CMD_H

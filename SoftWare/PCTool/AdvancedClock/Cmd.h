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
    VERSION_CMD,
    CONNECT_MODE_CMD,
    OLED_SHOW_MODE_CMD,
};
typedef enum weather_id_struct
{
    WEATHER_SUNNY = 0x00,
    WEATHER_CLOUDY,
    WEATHER_WINDY,
    WEATHER_SUNNY_TO_RAINY,
    WEATHER_SMALL_THUNDER_RAINY,
    WEATHER_LARGE_THUNDER_RAINY,
    WEATHER_RAINY_AND_SNOWY,
    WEATHER_SMALL_RAINY,
    WEATHER_MIDDLE_RAINY,
    WEATHER_LARGE_RAINY,
    WEATHER_MORE_LARGE_RAINY,
} weather_id_t;
typedef enum
{
    WEATHER_MODE = 0x0,
    HOST_MODE,
    OTHER_MODE,
    MAX,
}connect_mode_t;

#define OTA_BOOT_ATTR  1
#define OTA_APP_ATTR   2
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

typedef struct ota_package_info_struct
{
    uint32_t bin_size;
    uint32_t package_num;
    uint32_t check_sum;
    uint32_t rsv;
}ota_package_info_t;

typedef struct version_info_struct
{
    uint32_t software_version;
    uint32_t hardware_version;
    uint32_t ota_attr;
    uint32_t rsv2;
}version_info_t;


typedef struct connect_mode_set_struct
{
    uint8_t mode;
}connect_mode_set_t;

typedef struct oled_show_mode_set_struct
{
    uint8_t mode;
}oled_show_mode_set_t;
int CalCheckSum(uint8_t* Data, uint16_t len);
#endif // CMD_H

#ifndef  __BLUETOOTH_H__
#define  __BLUETOOTH_H__

enum {
	RESP_BLE_GET_WIFI_TIME_INFO  = 0x00,
	RESP_BLE_RESET  = 0x01,
	RESP_BLE_CLOSE_INT = 0x02,
	RESP_BLE_OPEN_INT = 0x03,
};

void bluetooth_msg_porcess();


#endif


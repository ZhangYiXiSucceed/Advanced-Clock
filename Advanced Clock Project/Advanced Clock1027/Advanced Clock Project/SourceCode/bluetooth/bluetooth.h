#ifndef  __BLUETOOTH_H__
#define  __BLUETOOTH_H__

enum {
	RESP_BLE_GET_WIFI_TIME_INFO  = 0x00,
	RESP_BLE_RESET,
	RESP_BLE_CLOSE_INT,
	RESP_BLE_OPEN_INT,
	RESP_BLE_SEND_NRF24L01,
	RESP_BLE_SET_TX,
	RESP_BLE_SET_RX,
};


enum {
	BLE_AT_CMD = 0x00,

};
void ble_init();
void bluetooth_msg_porcess();


#endif


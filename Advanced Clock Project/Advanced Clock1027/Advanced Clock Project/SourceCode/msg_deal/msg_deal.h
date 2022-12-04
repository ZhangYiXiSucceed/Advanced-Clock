#ifndef __MSGDEAL_H__
#define __MSGDEAL_H__

#include "stm32f4xx.h"


#define TERMINAL_NEWLINE   '\r'
#define RETURN             '\n'
#define SPACE              ' '
#define MAX_CMD_PARA       12
#define CMD_BUFF_MAX_SIZE  64

#define FUNC_TYPE_PARA_CHANGE -1
#define FUNC_TYPE_PARA_STRING -2

typedef enum diag_cmd_para_type_enum{
	diag_cmd_para_string=0,
	diag_cmd_para_number=1
}diag_cmd_para_type_e;

typedef enum data_type_enum{
	data_num = 0,
	data_hex,
	data_str
}data_type_e;

typedef struct diag_cmd_descriptor_struct{
	char* cmd_name;
	char* cmd_short_help;
	char* cmd_long_help;
	void (*cmd_func)();
	int32_t num_args;
}diag_cmd_descriptor_t;


typedef struct diag_cmd_para_struct{
	diag_cmd_para_type_e para_type;
	uint64_t value;
	char* str;
}diag_cmd_para_t;


extern struct SystemVar system_var; 
void Uart_AT_Deal(void);
int8_t diag_cmd_process(uint8_t *cmd_buff,uint16_t cmd_buff_len);
int8_t diag_cmd_input(uint8_t *cmd_buff,uint16_t cmd_buff_len);

void diag_cmd_complete(int8_t status);
void diag_cmd_start();


#endif


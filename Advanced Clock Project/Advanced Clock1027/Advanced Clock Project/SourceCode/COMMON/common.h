#ifndef __COMMON_H
#define __COMMON_H
#include "stm32f4xx.h"

void Delaynms(unsigned short cnt);
void TransferDataToString(unsigned char *data, unsigned char *string, unsigned char len);
int8_t StateCheck(char *data);
unsigned char GetOutputData(unsigned char* output, unsigned char* data, unsigned char len);
void DataLine(int16_t *Din, uint8_t len);
int8_t GetCesqValue(char *data);
int8_t GetRecvIOTMsg(char *msg, unsigned char* len, unsigned char* data); 


#endif

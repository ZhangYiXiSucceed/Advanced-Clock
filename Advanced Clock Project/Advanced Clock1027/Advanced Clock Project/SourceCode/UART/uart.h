
#ifndef __UART_H__
#define __UART_H__

#include "stm32f4xx.h"

#define DEBUG_PORT
#define TX_BUFFER_SIZE 4096
#define RX_BUFFER_SIZE 4096

#define   BUFFER_SIZE   4096


extern uint16_t   FrameInlen;
extern uint8_t   FrameInBuff[BUFFER_SIZE];
extern unsigned char  UART1FramInFlag; 

void UART1_DMA_Init(void);
void UART1Init(void);
unsigned char Sizeofmsg(unsigned char* msg);
void UART1_SentMsgL(unsigned char *data, unsigned short cnt);


void UART1_SentChar(char* data);
void UART1Poll(void);
unsigned short Uart1Read(unsigned char *data);
void rt_kprintf(const char *fmt, ...);
void rt_hw_console_output(char *str,uint16_t output_len);



void UART2_DMA_Init(void);
void UART2Init(void);
void UART2_SentMsgL(unsigned char *data, unsigned short cnt);
void UART2_SentMsg(unsigned char *data);
void UART2_SentChar(char* data);
void UART2Poll(void);
unsigned short Uart2Read(unsigned char *data);

void PrintfIOTPort(unsigned char*msg,unsigned char len);
void rt_kprintf2(const char *fmt, ...);
void rt_hw_console_output2(const char *str);
void Printfmsg2(unsigned char* msg);

void UART3_DMA_Init(void);
void UART3Init(void);
void UART3_SentMsg(unsigned char *data);
void rt_kprintf3(const char *fmt, ...);
void rt_hw_console_output3(const char *str);
void Printfmsg3(unsigned char* msg);
void PrintfIOTPort3(unsigned char*msg,unsigned char len);
void UART3Poll(void);
unsigned short Uart3Read(unsigned char *data);


void UART4_DMA_Init(void);
void UART4Init(void);
void UART4_SentMsg(unsigned char *data);
void rt_kprintf4(const char *fmt, ...);
void rt_hw_console_output4(const char *str);
void Printfmsg4(unsigned char* msg);
void PrintfIOTPort4(unsigned char*msg,unsigned char len);
void UART4Poll(void);
unsigned short Uart4Read(unsigned char *data);
#endif



#ifndef QUEUE_H__
#define QUEUE_H__

#include "stm32f4xx.h"

#define QueueSize       32   
#define QueueFull       0   
#define QueueEmpty      1   
#define QueueOperateOk  2   
#define ElemType        unsigned char


#define    IntsStorage                unsigned long IntsByte                         // 存储中断设置参数变量
#define    EnableInts                 __enable_interrupt();                            // 中断使能 
#define    DisableInts                __disable_interrupt();                           // 中断禁止
#define    RestoreInts                __set_interrupt_state(IntsByte);    EnableInts;  // 恢复中断设置，恢复中断
#define    StoreDisableInts           IntsByte = __get_interrupt_state(); DisableInts; // 存储中断参数并禁止中断





extern ElemType MyQueueBuf;  
extern struct FifoQueue MyQueue;

#define ElemTypeS        unsigned int
extern struct FifoQueueS HeightQueue;

extern void QueueInit(struct FifoQueue *Queue); 
extern unsigned char QueueIn(struct FifoQueue *Queue,ElemType *sdat, unsigned char len); 
extern unsigned char QueueOut(struct FifoQueue *Queue,ElemType *sdat,unsigned char *len);
extern unsigned char GetFifopagnum(struct FifoQueue *Queue);


extern void QueueInitS(struct FifoQueueS *Queue); 
extern unsigned char QueueInS(struct FifoQueueS *Queue,ElemTypeS *sdat, unsigned char len); 
extern unsigned char QueueOutS(struct FifoQueueS *Queue,ElemTypeS *sdat,unsigned char *len);
extern unsigned char GetFifopagnumS(struct FifoQueueS *Queue);

void QueueInfoPrint(void);

#endif



#ifndef QUEUE_H__
#define QUEUE_H__

#include "stm32f4xx.h"

#ifdef __IAR_CC
//#error "IAR CC Compiler"
#define    IntsStorage                unsigned long IntsByte                         // �洢�ж����ò�������
#define    EnableInts                 __enable_interrupt();                            // �ж�ʹ�� 
#define    DisableInts                __disable_interrupt();                           // �жϽ�ֹ
#define    RestoreInts                __set_interrupt_state(IntsByte);    EnableInts;  // �ָ��ж����ã��ָ��ж�
#define    StoreDisableInts           IntsByte = __get_interrupt_state(); DisableInts; // �洢�жϲ�������ֹ�ж�
#else

#define    IntsStorage                unsigned long IntsByte                         // �洢�ж����ò�������
#define    EnableInts                 __enable_irq();                            // �ж�ʹ�� 
#define    DisableInts                __disable_irq();                           // �жϽ�ֹ
#define    RestoreInts                //__set_interrupt_state(IntsByte);    EnableInts;  // �ָ��ж����ã��ָ��ж�
#define    StoreDisableInts           //IntsByte = __get_interrupt_state(); DisableInts; // �洢�жϲ�������ֹ�ж�

#endif

#define    ValFifoSize               16   
#define    ValFifoFull               0   
#define    ValFifoEmpty              1   
#define    ValFifoOperateOk          2   

typedef struct OperCmdUnion                   // OsDataList is for the definition: struct OsDataList   *next;
{
  unsigned short     tid;          // �������ͣ��¶ȡ���
  unsigned short     cmd;         // �¶�ֵ��������Ϊ����Ч��
  unsigned short     len;
  unsigned short     trycnt;
  unsigned long     time;
  unsigned char     buffer[250];
} OperCmdUnion_t;

typedef struct OperCmdUnionFifo  
{   
    unsigned char  front;          //------------------------------------------ǰ��
    unsigned char  rear;           //------------------------------------------��
    unsigned char  count;          //------------------------------------------����
    unsigned char  rsvb;
    struct OperCmdUnion    data[ValFifoSize];   //-------------------------------------------buffer  
}OperCmdUnionFifo_t;

extern struct OperCmdUnion      sOperCmdUnion_wifi;
extern struct OperCmdUnion      sOperCmdBuff;
extern struct OperCmdUnionFifo  sOperCmdUnionFifo;

void FifoInit(struct OperCmdUnionFifo *Queue); 
unsigned char FifoIn(struct OperCmdUnionFifo *Queue,  struct OperCmdUnion *sdat); 
unsigned char FifoOut(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat);
unsigned char FifoBack(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat);  
unsigned char GetTsakFiFoCount(struct OperCmdUnionFifo *Queue);














#define QueueSize       2   
#define QueueFull       0   
#define QueueEmpty      1   
#define QueueOperateOk  2   
#define ElemType        unsigned char



typedef struct FifoQueue   
{   
    unsigned char  front;        //-------------------------------------------ǰ��
    unsigned char  rear;         //-------------------------------------------��
    unsigned char  count;        //-------------------------------------------����
    unsigned char  rsv1;
    ElemType dat[QueueSize][4100];   
}fifo_queue_t;



extern ElemType MyQueueBuf;  

extern struct FifoQueue MyQueue;
extern fifo_queue_t wifi_queue;
extern fifo_queue_t nb_queue;
extern fifo_queue_t ble_queue;


#define ElemTypeS        unsigned int

void QueueInit(struct FifoQueue *Queue); 
unsigned char QueueIn(struct FifoQueue *Queue,ElemType *sdat, unsigned short len); 
unsigned char QueueOut(struct FifoQueue *Queue,ElemType *sdat,unsigned short *len);
unsigned char GetFifopagnum(struct FifoQueue *Queue);



#endif



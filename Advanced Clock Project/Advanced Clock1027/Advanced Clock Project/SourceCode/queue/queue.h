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
#define    EnableInts                 __enable_fault_irq()                            // �ж�ʹ�� 
#define    DisableInts                __disable_fault_irq()                           // �жϽ�ֹ
#define    RestoreInts                 __set_FAULTMASK(IntsByte);      // �ָ��ж����ã��ָ��ж�
#define    StoreDisableInts           do {IntsByte = __get_FAULTMASK(); DisableInts;}while(0);// �洢�жϲ�������ֹ�ж�

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
extern struct OperCmdUnionFifo  sOperCmdUnionFifo_wifi;

void FifoInit(struct OperCmdUnionFifo *Queue); 
unsigned char FifoIn(struct OperCmdUnionFifo *Queue,  struct OperCmdUnion *sdat); 
unsigned char FifoOut(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat);
unsigned char FifoBack(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat);  
unsigned char GetTsakFiFoCount(struct OperCmdUnionFifo *Queue);














#define QUEUE_SIZE       2  
#define QUEUE_MAX_LEN    4096
#define QUEUE_FULL       0   
#define QUEUE_EMPTY      1   
#define QUEUE_OPER_OK  	 2   
#define ELEM_TYPE        unsigned char
#define ElemTypeS        unsigned int

typedef struct fifo_queue_struct   
{   
    unsigned char  front;        //-------------------------------------------ǰ��
    unsigned char  rear;         //-------------------------------------------��
    unsigned short count;        //-------------------------------------------����
    unsigned int   len;
    ELEM_TYPE dat[QUEUE_SIZE][QUEUE_MAX_LEN];   
}fifo_queue_t;



extern ELEM_TYPE MyQueueBuf;  
extern struct fifo_queue_struct MyQueue;
extern fifo_queue_t wifi_queue;
extern fifo_queue_t nb_queue;
extern fifo_queue_t ble_queue;
extern fifo_queue_t nrf24l01_queue;

void queue_init(fifo_queue_t *queue); 
unsigned char queue_in(fifo_queue_t *queue,ELEM_TYPE *sdat, unsigned short len); 
unsigned char queue_out(fifo_queue_t *queue,ELEM_TYPE *sdat,unsigned short *len);
unsigned char get_queue_size(fifo_queue_t *queue);

#endif



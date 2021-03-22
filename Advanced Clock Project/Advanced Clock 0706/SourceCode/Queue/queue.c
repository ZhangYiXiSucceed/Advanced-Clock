#include "main.h"


struct FifoQueue   
{   
    unsigned char  front;        //-------------------------------------------前部
    unsigned char  rear;         //-------------------------------------------后部
    unsigned char  count;        //-------------------------------------------个数
    unsigned char  rsv1;
    ElemType dat[QueueSize][256];   
};

ElemType MyQueueBuf;  
struct FifoQueue MyQueue;



void QueueInit(struct FifoQueue *Queue)   
{   
  Queue->front = 0;
  Queue->rear  = 0;   
  Queue->count = 0;
  // Queue->point = 0;
}

// Queue In   
unsigned char QueueIn(struct FifoQueue *Queue, ElemType *sdat, unsigned char len)   
{   
  
  
  //IntsStorage;
  //StoreDisableInts;
  
  if(len > 250) len = 250;
  
  if((Queue->front == Queue->rear) && (Queue->count == QueueSize))   
  {  
      // full   
      //RestoreInts;
      return QueueFull;   
  }
  else
  {   
      // in  
      Queue->dat[Queue->rear][0] = len;
      memcpy(&Queue->dat[Queue->rear][1], sdat,len);  
      Queue->rear  = (Queue->rear + 1) & (QueueSize-1);  //---------------------加满缓冲区以后就清除0，queuesize必须为2的n次方
      Queue->count = Queue->count + 1;
      
      //RestoreInts;
      return QueueOperateOk;   
  }
}

// Queue Out   
unsigned char QueueOut(struct FifoQueue *Queue, ElemType *sdat, unsigned char *len)   
{   
  
  //IntsStorage;
  //StoreDisableInts;
  
  if((Queue->front == Queue->rear) && (Queue->count == 0))   
  {   
      // empty 
      //RestoreInts;  
      
      return QueueEmpty;   
  }
  else    
  {   
      // out
      len[0] = Queue->dat[Queue->front][0];
      
      if(len[0] > 250)
        len[0] = 250;
      
      memcpy(sdat,&Queue->dat[Queue->front][1],len[0]);  
      Queue->front = (Queue->front + 1) & (QueueSize-1);
      Queue->count = Queue->count - 1;
      
      //RestoreInts;
      return QueueOperateOk;   
  }   
} 

// Queue Out   
unsigned char GetFifopagnum(struct FifoQueue *Queue)   
{   
  return Queue->count;   
} 







struct FifoQueueS   
{   
    unsigned char  front;        //-------------------------------------------前部
    unsigned char  rear;         //-------------------------------------------后部
    unsigned char  count;        //-------------------------------------------个数
    unsigned char  rsv1;
    ElemTypeS dat[QueueSize][2];   
};


struct FifoQueueS HeightQueue;

struct   FifoQueueS  fifo_height;

void QueueInitS(struct FifoQueueS *Queue)   
{   
  Queue->front = 0;
  Queue->rear  = 0;   
  Queue->count = 0;
  // Queue->point = 0;
}

// Queue In   
unsigned char QueueInS(struct FifoQueueS *Queue, ElemTypeS *sdat, unsigned char len)   
{   
  u8 i=0;
  
  //IntsStorage;
  //StoreDisableInts;
  
  if(len > 2) len = 2;
  
  if((Queue->front == Queue->rear) && (Queue->count == QueueSize))   
  {  
      // full 
      //RestoreInts;  
      
      return QueueFull;   
  }
  else
  {   
      // in  
      Queue->dat[Queue->rear][0] = len;
      
      for(i=0;i<len;i++)
      {
        Queue->dat[Queue->rear][i+1] = sdat[i];
      }
      //memcpy(&, (u8 *)sdat,len*4);  
      Queue->rear  = (Queue->rear + 1) & (QueueSize-1);                //---------------------加满缓冲区以后就清除0，queuesize必须为2的n次方
      Queue->count = Queue->count + 1;
      
      //RestoreInts;
      return QueueOperateOk;   
  }
}

// Queue Out   
unsigned char QueueOutS(struct FifoQueueS *Queue, ElemTypeS *sdat, unsigned char *len)   
{   
  u8 i = 0;
  
  //IntsStorage;
  //StoreDisableInts;
    
  if((Queue->front == Queue->rear) && (Queue->count == 0))   
  {   
      // empty   
      //RestoreInts;
      return QueueEmpty;   
  }
  else    
  {   
      // out
      
        
      len[0] = Queue->dat[Queue->front][0];
      
      if(len[0]>2)
        len[0] = 2;
      
      for(i=0;i<len[0];i++)
      {
        sdat[i] = Queue->dat[Queue->front][i+1];
      }
      
      //memcpy(sdat,&Queue->dat[Queue->front][1],Queue->dat[Queue->front][0]);  
      Queue->front = (Queue->front + 1) & (QueueSize-1);
      Queue->count = Queue->count - 1;
      
      //RestoreInts;
      return QueueOperateOk;   
  }   
} 

// Queue Out   
unsigned char GetFifopagnumS(struct FifoQueueS *Queue)   
{   
  return Queue->count;   
} 


void QueueInfoPrint()
{
  if(QueueOut(&MyQueue, FrameInBuff, &FrameInlen) == QueueOperateOk) 
  {
    UART1_SentMsgL(FrameInBuff,FrameInlen);
    memset(FrameInBuff,0x00,sizeof(FrameInBuff));
  }
}



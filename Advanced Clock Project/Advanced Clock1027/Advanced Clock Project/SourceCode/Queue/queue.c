#include "main.h"


struct OperCmdUnion      sOperCmdUnion_wifi;  
struct OperCmdUnion      sOperCmdBuff;  

struct OperCmdUnionFifo  sOperCmdUnionFifo;


void FifoInit(struct OperCmdUnionFifo *Queue)   
{   
  Queue->front = Queue->rear;   
  Queue->count = 0;
}


// Queue In   
unsigned char FifoIn(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat)   
{   
  IntsStorage;
  StoreDisableInts;
  
  if((Queue->front == Queue->rear) && (Queue->count == ValFifoSize))   
  {  
      Queue->data[Queue->rear] = sdat[0];
      Queue->rear  = (Queue->rear  + 1) & (ValFifoSize-1);  //------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ôºï¿½ï¿½ï¿½ï¿½ï¿½0ï¿½ï¿½queuesizeï¿½ï¿½ï¿½ï¿½Îª2ï¿½ï¿½nï¿½Î·ï¿½
      Queue->front = (Queue->front + 1) & (ValFifoSize-1);
      
      RestoreInts;
      return ValFifoFull;  // full    
  }
  else
  {   
      // in  
      Queue->data[Queue->rear] = sdat[0];
      Queue->rear  = (Queue->rear + 1) & (ValFifoSize-1);  //------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ôºï¿½ï¿½ï¿½ï¿½ï¿½0ï¿½ï¿½queuesizeï¿½ï¿½ï¿½ï¿½Îª2ï¿½ï¿½nï¿½Î·ï¿½
      Queue->count = Queue->count + 1;
      
      RestoreInts;
      return ValFifoOperateOk;   
  }
}


// Queue Out   
unsigned char FifoOut(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat)   
{   
  
  IntsStorage;
  StoreDisableInts;
  
  if((Queue->front == Queue->rear) && (Queue->count == 0))   
  {   
      // empty   
      RestoreInts;
      return ValFifoEmpty;   
  }
  else    
  {   
      // out
      sdat[0] = Queue->data[Queue->front];
      Queue->front = (Queue->front + 1) & (ValFifoSize-1);
      Queue->count = Queue->count - 1;
      
      RestoreInts;
      return ValFifoOperateOk;   
  }   
} 


// Queue In   
unsigned char FifoBack(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat)   
{   
  IntsStorage;
  StoreDisableInts;
  
  if((Queue->front == Queue->rear) && (Queue->count == ValFifoSize))            // ï¿½ï¿½ï¿½ï¿½Ê½ï¿½ï¿½ï¿½Ë³ï¿½ï¿½ï¿½Ô»ï¿½Ñ¹ï¿½ï¿½Ëµï¿½ï¿½ï¿½ï¿½Ö´ï¿½Ğµï¿½ï¿½ï¿½
  {  
      Queue->data[Queue->rear] = sdat[0];
      Queue->rear  = (Queue->rear  + 1) & (ValFifoSize-1);  //------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ôºï¿½ï¿½ï¿½ï¿½ï¿½0ï¿½ï¿½queuesizeï¿½ï¿½ï¿½ï¿½Îª2ï¿½ï¿½nï¿½Î·ï¿½
      Queue->front = (Queue->front + 1) & (ValFifoSize-1);
      
      RestoreInts;
      return ValFifoFull;  // full    
  }
  else
  {   
      // in  
      if(Queue->front ==0)
      {
            Queue->front = ValFifoSize - 1;
      }
      else 
      {
            Queue->front = Queue->front-1;
      }
      Queue->data[Queue->front] = sdat[0];
      // Queue->rear  = (Queue->rear + 1) & (ValFifoSize-1);  //------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ôºï¿½ï¿½ï¿½ï¿½ï¿½0ï¿½ï¿½queuesizeï¿½ï¿½ï¿½ï¿½Îª2ï¿½ï¿½nï¿½Î·ï¿½
      Queue->count = Queue->count + 1;
      
      RestoreInts;
      return ValFifoOperateOk;   
  }
}


unsigned char GetTsakFiFoCount(struct OperCmdUnionFifo *Queue)
{
   return Queue->count;
}



ElemType MyQueueBuf;  
struct FifoQueue MyQueue;


fifo_queue_t wifi_queue;
fifo_queue_t nb_queue;
fifo_queue_t ble_queue;





void QueueInit(struct FifoQueue *Queue)   
{   
  Queue->front = 0;
  Queue->rear  = 0;   
  Queue->count = 0;
  // Queue->point = 0;
}

// Queue In   
unsigned char QueueIn(struct FifoQueue *Queue, ElemType *sdat, unsigned short len)   
{   
  
  
  //IntsStorage;
  //StoreDisableInts;
  
  if(len > (RX_BUFFER_SIZE-1)) len = RX_BUFFER_SIZE-1;
  
  if((Queue->front == Queue->rear) && (Queue->count == QueueSize))   
  {  
      // full   
      //RestoreInts;
      return QueueFull;   
  }
  else
  {   
      // in  
      Queue->dat[Queue->rear][0] = len%256;
	  Queue->dat[Queue->rear][1] = len/256;
      memcpy(&Queue->dat[Queue->rear][2], sdat,len);  
      Queue->rear  = (Queue->rear + 1) & (QueueSize-1);  //---------------------¼ÓÂú»º³åÇøÒÔºó¾ÍÇå³ı0£¬queuesize±ØĞëÎª2µÄn´Î·½
      Queue->count = Queue->count + 1;
      
      //RestoreInts;
      return QueueOperateOk;   
  }
}

// Queue Out   
unsigned char QueueOut(struct FifoQueue *Queue, ElemType *sdat, unsigned short *len)   
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
      *len = Queue->dat[Queue->front][1]*256 + Queue->dat[Queue->front][0];
      
      if(*len > (RX_BUFFER_SIZE -1))
        *len = RX_BUFFER_SIZE -1;
      
      memcpy(sdat,&Queue->dat[Queue->front][2],*len);  
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














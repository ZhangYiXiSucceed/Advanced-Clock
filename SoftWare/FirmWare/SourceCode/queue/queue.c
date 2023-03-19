#include "main.h"


struct OperCmdUnion      sOperCmdUnion_wifi;  
struct OperCmdUnion      sOperCmdBuff;  
struct OperCmdUnionFifo  sOperCmdUnionFifo_wifi;

struct OperCmdUnion      sOperCmdUnion_ble;  
struct OperCmdUnion      sOperCmdBuff_ble;  
struct OperCmdUnionFifo  sOperCmdUnionFifo_ble;

void FifoInit(struct OperCmdUnionFifo *Queue)   
{   
  Queue->front = Queue->rear;   
  Queue->count = 0;
}


// Queue In   
unsigned char FifoIn(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat)   
{   
   
   
  
  if((Queue->front == Queue->rear) && (Queue->count == ValFifoSize))   
  {  
      Queue->data[Queue->rear] = sdat[0];
      Queue->rear  = (Queue->rear  + 1) & (ValFifoSize-1);  //------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ôºï¿½ï¿½ï¿½ï¿½ï¿½0ï¿½ï¿½queuesizeï¿½ï¿½ï¿½ï¿½Îª2ï¿½ï¿½nï¿½Î·ï¿½
      Queue->front = (Queue->front + 1) & (ValFifoSize-1);
      
       
      return ValFifoFull;  // full    
  }
  else
  {   
      // in  
      Queue->data[Queue->rear] = sdat[0];
      Queue->rear  = (Queue->rear + 1) & (ValFifoSize-1);  //------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ôºï¿½ï¿½ï¿½ï¿½ï¿½0ï¿½ï¿½queuesizeï¿½ï¿½ï¿½ï¿½Îª2ï¿½ï¿½nï¿½Î·ï¿½
      Queue->count = Queue->count + 1;
      
       
      return ValFifoOperateOk;   
  }
}


// Queue Out   
unsigned char FifoOut(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat)   
{   
  
   
   
  
  if((Queue->front == Queue->rear) && (Queue->count == 0))   
  {   
      // empty   
       
      return ValFifoEmpty;   
  }
  else    
  {   
      // out
      sdat[0] = Queue->data[Queue->front];
      Queue->front = (Queue->front + 1) & (ValFifoSize-1);
      Queue->count = Queue->count - 1;
      
       
      return ValFifoOperateOk;   
  }   
} 


// Queue In   
unsigned char FifoBack(struct OperCmdUnionFifo *Queue, struct OperCmdUnion *sdat)   
{   
   
   
  
  if((Queue->front == Queue->rear) && (Queue->count == ValFifoSize))            // ï¿½ï¿½ï¿½ï¿½Ê½ï¿½ï¿½ï¿½Ë³ï¿½ï¿½ï¿½Ô»ï¿½Ñ¹ï¿½ï¿½Ëµï¿½ï¿½ï¿½ï¿½Ö´ï¿½Ğµï¿½ï¿½ï¿½
  {  
      Queue->data[Queue->rear] = sdat[0];
      Queue->rear  = (Queue->rear  + 1) & (ValFifoSize-1);  //------------------ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ôºï¿½ï¿½ï¿½ï¿½ï¿½0ï¿½ï¿½queuesizeï¿½ï¿½ï¿½ï¿½Îª2ï¿½ï¿½nï¿½Î·ï¿½
      Queue->front = (Queue->front + 1) & (ValFifoSize-1);
      
       
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
      
       
      return ValFifoOperateOk;   
  }
}


unsigned char GetTsakFiFoCount(struct OperCmdUnionFifo *Queue)
{
   return Queue->count;
}



ELEM_TYPE MyQueueBuf;  
fifo_queue_t MyQueue;


fifo_queue_t wifi_queue;
fifo_queue_t nb_queue;
fifo_queue_t ble_queue;
fifo_queue_t nrf24l01_queue;


void queue_init(fifo_queue_t *queue)   
{   
  queue->front = 0;
  queue->rear  = 0;   
  queue->count = 0;
}

// Queue In   
unsigned char queue_in(fifo_queue_t *queue, ELEM_TYPE *sdat, unsigned short len)   
{   
   
   
  
  if(len > (QUEUE_MAX_LEN)) len = QUEUE_MAX_LEN;
  if((queue->front == queue->rear) && (queue->count == QUEUE_SIZE))   
  {  
      // full   
       
      return QUEUE_FULL;   
  }
  else
  {   
      // in  
      queue->len = len;
      memcpy(&queue->dat[queue->rear][0], sdat,len);  
      queue->rear  = (queue->rear + 1) & (QUEUE_SIZE-1);  //---------------------¼ÓÂú»º³åÇøÒÔºó¾ÍÇå³ı0£¬queuesize±ØĞëÎª2µÄn´Î·½
      queue->count = queue->count + 1;
      
       
      return QUEUE_OPER_OK;   
  }
}

// Queue Out   
unsigned char queue_out(fifo_queue_t *queue, ELEM_TYPE *sdat, unsigned short *len)   
{   
  
   
   
  
  if((queue->front == queue->rear) && (queue->count == 0))   
  {   
      // empty 
         
      return QUEUE_EMPTY;   
  }
  else    
  {   
      // out
      *len =queue->len;
      if(*len > (QUEUE_MAX_LEN))
        *len = QUEUE_MAX_LEN;      
      memcpy(sdat,&queue->dat[queue->front][0],*len);  
      queue->front = (queue->front + 1) & (QUEUE_SIZE-1);
      queue->count = queue->count - 1;
      
       
      return QUEUE_OPER_OK;   
  }   
} 

// Queue Out   
unsigned char get_queue_size(fifo_queue_t *queue)   
{   
  return queue->count;   
} 














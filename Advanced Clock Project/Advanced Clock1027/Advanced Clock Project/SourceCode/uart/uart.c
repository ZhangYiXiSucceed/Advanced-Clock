#include "main.h"
                        

uint16_t   FrameInlen;
uint8_t   FrameInBuff[BUFFER_SIZE]={0};
 
unsigned short Uart1Len;    
unsigned char  UART1FramInFlag = 0; 
static unsigned char  Uart1RcvStartFlag = 0;                                      
static unsigned short Uart1DmaLastCnt, Uart1DmaStartCnt;                           
static unsigned long  Uart1DataInTime, Uart1DataCurrTime;                       
unsigned char  Uart1TxBuffer[TX_BUFFER_SIZE]={"UART1 send data"};               
unsigned char  Uart1RxBuffer[RX_BUFFER_SIZE]; 
                                     
unsigned short Uart2Len;    
unsigned char  UART2FramInFlag = 0; 
static unsigned char  Uart2RcvStartFlag = 0;                                      
static unsigned short Uart2DmaLastCnt, Uart2DmaStartCnt;                           
static unsigned long  Uart2DataInTime, Uart2DataCurrTime; 
unsigned char  Uart2TxBuffer[TX_BUFFER_SIZE]={"UART2 send data"};               
unsigned char  Uart2RxBuffer[RX_BUFFER_SIZE];   
// uart1

unsigned short Uart3Len;
unsigned char  UART3TempData;
unsigned char  UART3FramInFlag = 0; 
unsigned char  Uart3RcvStartFlag = 1;                                      
static unsigned short Uart3DmaLastCnt, Uart3DmaStartCnt;                              
static unsigned long  Uart3DataInTime, Uart3DataCurrTime; 
unsigned char  Uart3TxBuffer[TX_BUFFER_SIZE]={"UART3 send data"};               
unsigned char  Uart3RxBuffer[RX_BUFFER_SIZE];   

unsigned short Uart4Len;
unsigned char  UART4TempData;
unsigned char  UART4FramInFlag = 0; 
unsigned char  Uart4RcvStartFlag = 1;                                      
static unsigned short Uart4DmaLastCnt, Uart4DmaStartCnt;                              
static unsigned long  Uart4DataInTime, Uart4DataCurrTime; 
unsigned char  Uart4TxBuffer[TX_BUFFER_SIZE]={"UART4 send data"};               
unsigned char  Uart4RxBuffer[RX_BUFFER_SIZE]; 

void UART1_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,   ENABLE);
    // Tx
    DMA_DeInit(DMA2_Stream7);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr    = (unsigned long)&(Uart1TxBuffer[0]);
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = 0;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode        = DMA_FIFOMode_Disable;      
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_HalfFull;            
    DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;           
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);
    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
    DMA_Cmd(DMA2_Stream7, DISABLE);

    // Rx
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
    DMA_DeInit(DMA2_Stream5);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(USART1->DR));
    DMA_InitStructure.DMA_Memory0BaseAddr    = (unsigned long)&Uart1RxBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = RX_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;               // DMA_Mode_Normal;   // DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode        = DMA_FIFOMode_Disable;      
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_HalfFull;            
    DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;           
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
    DMA_Init(DMA2_Stream5, &DMA_InitStructure);
    DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);
    DMA_Cmd(DMA2_Stream5, ENABLE);
}


void UART1Init(void)
{
    // uart2 port, LORA MODULE 
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  

    // GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;        
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;  
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;   
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;     
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);

    USART_DMACmd(USART1, USART_DMAReq_Tx|USART_DMAReq_Rx, ENABLE);
    USART_Cmd(USART1, ENABLE);
    UART1_DMA_Init();

    Uart2DmaStartCnt = DMA_GetCurrDataCounter(DMA2_Stream5);                       // rx
    Uart2DmaLastCnt  = DMA_GetCurrDataCounter(DMA2_Stream5);                       // rx
}


unsigned char Sizeofmsg(unsigned char* msg)
{
    unsigned char  si = 0;
    while(1)
    {
        if(msg[si++] == '\n')
            break;
        if(si>250){
            si =250; break;
        }
    }
    return si;
}

void UART1_SentMsgL(unsigned char *data, unsigned short cnt)
{
	if(0 == cnt) return;
	DMA_Cmd(DMA2_Stream7, DISABLE);
    DMA2_Stream7->M0AR = (uint32_t)&data[0];                                     // update address
    DMA_SetCurrDataCounter(DMA2_Stream7, cnt);                                   // update tx count
    
    DMA_Cmd(DMA2_Stream7, ENABLE);
    while(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7)!=1); 
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=1);                         // ??????  
    DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
    USART_ClearFlag(USART1, USART_FLAG_TC);                                       // ??UART1???????
}

void UART1_SentMsg(unsigned char *data,uint16_t data_len)
{
    DMA_Cmd(DMA2_Stream7, DISABLE);
    DMA2_Stream7->M0AR = (uint32_t)&data[0];                                     // update address
    DMA_SetCurrDataCounter(DMA2_Stream7, data_len);                                  // update tx count
    
    DMA_Cmd(DMA2_Stream7, ENABLE);
    while(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7)!=1); 
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=1);                         // ??????
    DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
    USART_ClearFlag(USART1, USART_FLAG_TC);                                       // ??UART1???????
}



void UART1_SentChar(char* data)
{
    short wait = 10000;
    while(((USART1->SR & USART_FLAG_TXE)!= SET)&&(wait--));
    USART_SendData(USART1, data[0]);
    wait = 10000;
    while((USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET)&&(wait--));
}



void UART1Poll(void)
{
  unsigned short dmacnt;
//  unsigned char  tempbuff[200];
  dmacnt = DMA_GetCurrDataCounter(DMA2_Stream5);
  if(Uart1RcvStartFlag==0)
  {
    if(dmacnt==Uart1DmaLastCnt);
    else 
    {
      Uart1RcvStartFlag = 1;
      Uart1DataInTime   = Systemtime;    // rt_tick_get();
    }
  }
  else  
  {
    if(dmacnt==Uart1DmaLastCnt)
    {
      Uart1DataCurrTime = Systemtime;     // rt_tick_get();
      if(Uart1DataCurrTime-Uart1DataInTime>2)                                   // ????????(?????dmacnt,DMACH5LastCnt)
      {
        UART1FramInFlag    = 1;
        Uart1RcvStartFlag  = 0;
        if(Uart1DmaStartCnt >dmacnt)
        {
            Uart1Len         = Uart1DmaStartCnt - dmacnt;
            Uart1DmaStartCnt = dmacnt; 
        }
        else
        {
            Uart1Len         = RX_BUFFER_SIZE + Uart1DmaStartCnt - dmacnt;
            Uart1DmaStartCnt = dmacnt; 
        }
      }    
    }
    else 
    {
      Uart1DmaLastCnt   = dmacnt;
      Uart1DataInTime = Systemtime;    // rt_tick_get();
    }   
  } 
}

unsigned short Uart1Read(unsigned char *data)
{
    unsigned short  i;
    unsigned short  len, len1,len2;
    if(Uart1Len > (RX_BUFFER_SIZE-1)) 
		return (RX_BUFFER_SIZE-1);
    len = Uart1Len;
    if(len + Uart1DmaLastCnt < RX_BUFFER_SIZE)
    {
        for(i=0;i<len;i++)
            data[i] = Uart1RxBuffer[RX_BUFFER_SIZE - Uart1DmaLastCnt - Uart1Len + i];
    }
    else
    {
        len1 = len + Uart1DmaLastCnt - RX_BUFFER_SIZE;
        len2 = len - len1;
        for(i=0;i<len1;i++)
            data[i] = Uart1RxBuffer[RX_BUFFER_SIZE - len1 + i];
        for(i=0;i<len2;i++)
            data[len1+i] = Uart1RxBuffer[i];
    }
    UART1FramInFlag = 0; Uart1RcvStartFlag = 0;
    return(len);
}

void rt_hw_console_output(char *str,uint16_t output_len)
{   
    #ifdef DEBUG_PORT
    UART1_SentMsgL((unsigned char*)str,output_len);
    #endif
}



void rt_kprintf(const char *fmt, ...)
{
      va_list args;
      int16_t length;
      static char rt_log_buf[TX_BUFFER_SIZE];

      va_start(args, fmt);
      // the return value of vsnprintf is the number of bytes that would be
      // written to buffer had if the size of the buffer been sufficiently
      // large excluding the terminating null byte. If the output string
      // would be larger than the rt_log_buf, we have to adjust the output
      // length. 
      length = vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
      if ((length > TX_BUFFER_SIZE) || (length == -1))
          length = TX_BUFFER_SIZE;

      rt_hw_console_output(rt_log_buf,length);

      va_end(args);
}


// uart2
void UART2_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,   ENABLE);
    // Tx
    DMA_DeInit(DMA1_Stream6);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr    = (unsigned long)&(Uart2TxBuffer[0]);
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = 0;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode        = DMA_FIFOMode_Disable;      
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_HalfFull;            
    DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;           
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);
    DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
    DMA_Cmd(DMA1_Stream6, DISABLE);

    // Rx
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  
    DMA_DeInit(DMA1_Stream5);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(USART2->DR));
    DMA_InitStructure.DMA_Memory0BaseAddr    = (unsigned long)&Uart2RxBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = RX_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;               // DMA_Mode_Normal;   // DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode        = DMA_FIFOMode_Disable;      
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_HalfFull;            
    DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;           
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);
    DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
    DMA_Cmd(DMA1_Stream5, ENABLE);
}


void UART2Init(void)
{
    // uart2 port, LORA MODULE 
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);  

    // GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;        
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;  
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;      
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;   
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;     
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStructure);

    USART_DMACmd(USART2, USART_DMAReq_Tx|USART_DMAReq_Rx, ENABLE);
    USART_Cmd(USART2, ENABLE);
    UART2_DMA_Init();

    Uart2DmaStartCnt = DMA_GetCurrDataCounter(DMA1_Stream5);                       // rx
    Uart2DmaLastCnt  = DMA_GetCurrDataCounter(DMA1_Stream5);                       // rx
}

void UART2_SentMsgL(unsigned char *data, unsigned short cnt)
{
    if(cnt ==0) return;
    DMA_Cmd(DMA1_Stream6, DISABLE);
    DMA1_Stream6->M0AR = (uint32_t)&data[0];                                     // update address
    DMA_SetCurrDataCounter(DMA1_Stream6, cnt);                                   // update tx count
    
    DMA_Cmd(DMA1_Stream6, ENABLE);
    while(DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6)!=1); 
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC)!=1);                         // ??????  
    DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
    USART_ClearFlag(USART2, USART_FLAG_TC);                                       // ??UART1???????
}

void UART2_SentMsg(unsigned char *data)
{
    unsigned char ucnt = Sizeofmsg(data);
    
    DMA_Cmd(DMA1_Stream6, DISABLE);
    DMA1_Stream6->M0AR = (uint32_t)&data[0];                                     // update address
    DMA_SetCurrDataCounter(DMA1_Stream6, ucnt);                                  // update tx count
    
    DMA_Cmd(DMA1_Stream6, ENABLE);
    while(DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6)!=1); 
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC)!=1);                         // ??????
    DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);
    USART_ClearFlag(USART2, USART_FLAG_TC);                                       // ??UART1???????
}

void UART2_SentChar(char* data)
{
    short wait = 10000;
    while(((USART2->SR & USART_FLAG_TXE)!= SET)&&(wait--));
    USART_SendData(USART2, data[0]);
    wait = 10000;
    while((USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET)&&(wait--));
}


void PrintfIOTPort2(unsigned char*msg,unsigned char len)
{
    #ifdef DEBUG_PORT
    if(len<250){
        UART2_SentMsgL(msg, len);
    }
    #endif
}
void rt_hw_console_output2(const char *str)
{   
    Printfmsg2((unsigned char*)str);
}

void Printfmsg2(unsigned char* msg)
{
    #ifdef DEBUG_PORT
    UART2_SentMsg(msg);
    #endif
}

void rt_kprintf2(const char *fmt, ...)
{
      va_list args;
      char length;
      static char rt_log_buf2[128];

      va_start(args, fmt);
      // the return value of vsnprintf is the number of bytes that would be
      // written to buffer had if the size of the buffer been sufficiently
      // large excluding the terminating null byte. If the output string
      // would be larger than the rt_log_buf, we have to adjust the output
      // length. 
      length = vsnprintf(rt_log_buf2, sizeof(rt_log_buf2) - 1, fmt, args);
      if (length > 120)
          length = 120;

      rt_hw_console_output2(rt_log_buf2);

      va_end(args);
}
void UART2Poll(void)
{
  unsigned short dmacnt;
  unsigned char  tempbuff[200];
  
  dmacnt = DMA_GetCurrDataCounter(DMA1_Stream5);
  if(Uart2RcvStartFlag==0)
  {
    if(dmacnt==Uart2DmaLastCnt);
    else 
    {
      Uart2RcvStartFlag = 1;
      Uart2DataInTime   = Systemtime;    // rt_tick_get();
    }
  }
  else  
  {
    if(dmacnt==Uart2DmaLastCnt)
    {
      Uart2DataCurrTime = Systemtime;     // rt_tick_get();
      if(Uart2DataCurrTime-Uart2DataInTime>2)                                   // ????????(?????dmacnt,DMACH5LastCnt)
      {
        UART2FramInFlag    = 1;
        Uart2RcvStartFlag  = 0;
        if(Uart2DmaStartCnt >dmacnt)
        {
            Uart2Len         = Uart2DmaStartCnt - dmacnt;
            Uart2DmaStartCnt = dmacnt; 
        }
        else
        {
            Uart2Len         = RX_BUFFER_SIZE + Uart2DmaStartCnt - dmacnt;
            Uart2DmaStartCnt = dmacnt; 
        }
        
        Uart2Len=Uart2Read(tempbuff);
        Uart2DmaStartCnt  = dmacnt; 
        Uart1RcvStartFlag = 0;
        if(Uart2Len<=RX_BUFFER_SIZE) 
        {
           queue_in(&ble_queue, tempbuff, Uart2Len);  
        } 
      }    
    }
    else 
    {
      Uart2DmaLastCnt   = dmacnt;
      Uart2DataInTime = Systemtime;    // rt_tick_get();
    }   
  } 
}

unsigned short Uart2Read(unsigned char *data)
{
    unsigned short  i;
    unsigned short  len, len1,len2;
    if(Uart2Len >200) return 0;
    len = Uart2Len;
    if(len + Uart2DmaLastCnt < RX_BUFFER_SIZE)
    {
        for(i=0;i<len;i++)
            data[i] = Uart2RxBuffer[RX_BUFFER_SIZE - Uart2DmaLastCnt - Uart2Len + i];
    }
    else
    {
        len1 = len + Uart2DmaLastCnt - RX_BUFFER_SIZE;
        len2 = len - len1;
        for(i=0;i<len1;i++)
            data[i] = Uart2RxBuffer[RX_BUFFER_SIZE - len1 + i];
        for(i=0;i<len2;i++)
            data[len1+i] = Uart2RxBuffer[i];
    }
    UART2FramInFlag = 0; Uart2RcvStartFlag = 0;
    return(len);
}





void UART3_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,   ENABLE);
    // Tx
    DMA_DeInit(DMA1_Stream3);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART3->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr    = (unsigned long)&(Uart3TxBuffer[0]);
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = 0;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode        = DMA_FIFOMode_Disable;      
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_HalfFull;            
    DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;           
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
    DMA_Init(DMA1_Stream3, &DMA_InitStructure);
    DMA_ClearFlag(DMA1_Stream3, DMA_FLAG_TCIF3);
    DMA_Cmd(DMA1_Stream3, DISABLE);

    // Rx
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  
    DMA_DeInit(DMA1_Stream1);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(USART3->DR));
    DMA_InitStructure.DMA_Memory0BaseAddr    = (unsigned long)&Uart3RxBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = RX_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;               // DMA_Mode_Normal;   // DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode        = DMA_FIFOMode_Disable;      
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_HalfFull;            
    DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;           
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
    DMA_Init(DMA1_Stream1, &DMA_InitStructure);
    DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);
    DMA_Cmd(DMA1_Stream1, ENABLE);
}


void UART3Init(void)
{
    // uart2 port, LORA MODULE 
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,  ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);  

    // GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;        
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;  
    GPIO_Init(GPIOC, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;      
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;   
    GPIO_Init(GPIOC, &GPIO_InitStructure); 


    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;     
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStructure);

    USART_DMACmd(USART3, USART_DMAReq_Tx|USART_DMAReq_Rx, ENABLE);
    USART_Cmd(USART3, ENABLE);
    UART3_DMA_Init();

    Uart3DmaStartCnt = DMA_GetCurrDataCounter(DMA1_Stream1);                       // rx
    Uart3DmaLastCnt  = DMA_GetCurrDataCounter(DMA1_Stream1);                       // rx
}



void UART3_SentMsg(unsigned char *data)
{
    unsigned char ucnt = Sizeofmsg(data);
    
    DMA_Cmd(DMA1_Stream3, DISABLE);
    DMA1_Stream3->M0AR = (uint32_t)&data[0];                                     // update address
    DMA_SetCurrDataCounter(DMA1_Stream3, ucnt);                                  // update tx count
    
    DMA_Cmd(DMA1_Stream3, ENABLE);
    while(DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3)!=1); 
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC)!=1);                         // ??????
    DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
    USART_ClearFlag(USART3, USART_FLAG_TC);                                       // ??UART1???????
}

void UART3_SentMsgL(unsigned char *data, unsigned short cnt)
{
    if(cnt ==0) return;
    DMA_Cmd(DMA1_Stream3, DISABLE);
    DMA1_Stream3->M0AR = (uint32_t)&data[0];                                     // update address
    DMA_SetCurrDataCounter(DMA1_Stream3, cnt);                                   // update tx count
    
    DMA_Cmd(DMA1_Stream3, ENABLE);
    while(DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3)!=1); 
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC)!=1);                         // ??????  
    DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);
    USART_ClearFlag(USART3, USART_FLAG_TC);                                       // ??UART1???????
}

void UART3Poll(void)
{
  unsigned short dmacnt;
  unsigned char  tempbuff[200];
  
  dmacnt = DMA_GetCurrDataCounter(DMA1_Stream1);
  if(Uart3RcvStartFlag==0)
  {
    if(dmacnt==Uart3DmaLastCnt);
    else 
    {
      Uart3RcvStartFlag = 1;
      Uart3DataInTime   = Systemtime;    // rt_tick_get();
    }
  }
  else  
  {
    if(dmacnt==Uart3DmaLastCnt)
    {
      Uart3DataCurrTime = Systemtime;     // rt_tick_get();
      if(Uart3DataCurrTime-Uart3DataInTime>2)                                   // ????????(?????dmacnt,DMACH5LastCnt)
      {
        UART3FramInFlag    = 1;
        Uart3RcvStartFlag  = 0;
        if(Uart3DmaStartCnt >dmacnt)
        {
            Uart3Len         = Uart3DmaStartCnt - dmacnt;
            Uart3DmaStartCnt = dmacnt; 
        }
        else
        {
            Uart3Len         = RX_BUFFER_SIZE + Uart3DmaStartCnt - dmacnt;
            Uart3DmaStartCnt = dmacnt; 
        }
        
        Uart3Len=Uart3Read(tempbuff);
        Uart3DmaStartCnt  = dmacnt; 
        Uart3RcvStartFlag = 0;
        if(Uart3Len<=200) 
        {
           queue_in(&nb_queue, tempbuff, Uart3Len);  
        } 
      }    
    }
    else 
    {
      Uart3DmaLastCnt   = dmacnt;
      Uart3DataInTime = Systemtime;    // rt_tick_get();
    }   
  } 
}

unsigned short Uart3Read(unsigned char *data)
{
    unsigned short  i;
    unsigned short  len, len1,len2;
    if(Uart3Len >200) return 0;
    len = Uart3Len;
    if(len + Uart3DmaLastCnt < RX_BUFFER_SIZE)
    {
        for(i=0;i<len;i++)
            data[i] = Uart3RxBuffer[RX_BUFFER_SIZE - Uart3DmaLastCnt - Uart3Len + i];
    }
    else
    {
        len1 = len + Uart3DmaLastCnt - RX_BUFFER_SIZE;
        len2 = len - len1;
        for(i=0;i<len1;i++)
            data[i] = Uart3RxBuffer[RX_BUFFER_SIZE - len1 + i];
        for(i=0;i<len2;i++)
            data[len1+i] = Uart3RxBuffer[i];
    }
    UART3FramInFlag = 0; Uart3RcvStartFlag = 0;
    return(len);
}


void rt_kprintf3(const char *fmt, ...)
{
    va_list args;
    char length;
    static char rt_log_buf3[128];

    va_start(args, fmt);
    // the return value of vsnprintf is the number of bytes that would be
    // written to buffer had if the size of the buffer been sufficiently
    // large excluding the terminating null byte. If the output string
    // would be larger than the rt_log_buf, we have to adjust the output
    // length. 
    length = vsnprintf(rt_log_buf3, sizeof(rt_log_buf3) - 1, fmt, args);
    if (length > 120)
        length = 120;

    rt_hw_console_output3(rt_log_buf3);

    va_end(args);
}

void rt_hw_console_output3(const char *str)
{   
    Printfmsg3((unsigned char*)str);
}


void Printfmsg3(unsigned char* msg)
{
    #ifdef DEBUG_PORT
    UART3_SentMsg(msg);
    #endif
}


void PrintfIOTPort3(unsigned char*msg,unsigned char len)
{
    #ifdef DEBUG_PORT
    if(len<250){
        UART3_SentMsgL(msg,len);
    }
    #endif
}

void UART4_DMA_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,   ENABLE);
    // Tx
    DMA_DeInit(DMA1_Stream4);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&UART4->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr    = (unsigned long)&(Uart4TxBuffer[0]);
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize         = 0;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode        = DMA_FIFOMode_Disable;      
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_HalfFull;            
    DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;           
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
    DMA_Init(DMA1_Stream4, &DMA_InitStructure);
    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
    DMA_Cmd(DMA1_Stream4, DISABLE);

    // Rx
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  
    DMA_DeInit(DMA1_Stream2);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&(UART4->DR));
    DMA_InitStructure.DMA_Memory0BaseAddr    = (unsigned long)&Uart4RxBuffer;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = RX_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;               // DMA_Mode_Normal;   // DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode        = DMA_FIFOMode_Disable;      
    DMA_InitStructure.DMA_FIFOThreshold   = DMA_FIFOThreshold_HalfFull;            
    DMA_InitStructure.DMA_MemoryBurst     = DMA_MemoryBurst_Single;           
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
    DMA_Init(DMA1_Stream2, &DMA_InitStructure);
    DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF2);
    DMA_Cmd(DMA1_Stream2, ENABLE);
}


void UART4Init(void)
{
    // uart2 port, LORA MODULE 
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);  

    // GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;        
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;      
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;  
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;      
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;   
    GPIO_Init(GPIOA, &GPIO_InitStructure); 


    USART_InitStructure.USART_BaudRate            = 115200;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;     
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART4, &USART_InitStructure);

    USART_DMACmd(UART4, USART_DMAReq_Tx|USART_DMAReq_Rx, ENABLE);
    USART_Cmd(UART4, ENABLE);
    UART4_DMA_Init();

    Uart4DmaStartCnt = DMA_GetCurrDataCounter(DMA1_Stream2);                       // rx
    Uart4DmaLastCnt  = DMA_GetCurrDataCounter(DMA1_Stream2);                       // rx
}



void UART4_SentMsg(unsigned char *data)
{
    unsigned char ucnt = Sizeofmsg(data);
    
    DMA_Cmd(DMA1_Stream4, DISABLE);
    DMA1_Stream4->M0AR = (uint32_t)&data[0];                                     // update address
    DMA_SetCurrDataCounter(DMA1_Stream4, ucnt);                                  // update tx count
    
    DMA_Cmd(DMA1_Stream4, ENABLE);
    while(DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4)!=1); 
    while(USART_GetFlagStatus(UART4, USART_FLAG_TC)!=1);                         // ??????
    DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
    USART_ClearFlag(UART4, USART_FLAG_TC);                                       // ??UART1???????
}

void UART4_SentMsgL(unsigned char *data, unsigned short cnt)
{
    if(cnt ==0) return;
    DMA_Cmd(DMA1_Stream4, DISABLE);
    DMA1_Stream4->M0AR = (uint32_t)&data[0];                                     // update address
    DMA_SetCurrDataCounter(DMA1_Stream4, cnt);                                   // update tx count
    
    DMA_Cmd(DMA1_Stream4, ENABLE);
    while(DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4)!=1); 
    while(USART_GetFlagStatus(UART4, USART_FLAG_TC)!=1);                         // ??????  
    DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
    USART_ClearFlag(UART4, USART_FLAG_TC);                                       // ??UART1???????
}

void UART4Poll(void)
{
  unsigned short dmacnt;
  unsigned char  tempbuff[RX_BUFFER_SIZE];
  
  dmacnt = DMA_GetCurrDataCounter(DMA1_Stream2);
  if(Uart4RcvStartFlag==0)
  {
    if(dmacnt==Uart4DmaLastCnt);
    else 
    {
      Uart4RcvStartFlag = 1;
      Uart4DataInTime   = Systemtime;    // rt_tick_get();
    }
  }
  else  
  {
    if(dmacnt==Uart4DmaLastCnt)
    {
      Uart4DataCurrTime = Systemtime;     // rt_tick_get();
      if(Uart4DataCurrTime-Uart4DataInTime>2)                                   // ????????(?????dmacnt,DMACH5LastCnt)
      {
        UART4FramInFlag    = 1;
        Uart4RcvStartFlag  = 0;
        if(Uart4DmaStartCnt >dmacnt)
        {
            Uart4Len         = Uart4DmaStartCnt - dmacnt;
            Uart4DmaStartCnt = dmacnt; 
        }
        else
        {
            Uart4Len         = RX_BUFFER_SIZE + Uart4DmaStartCnt - dmacnt;
            Uart4DmaStartCnt = dmacnt; 
        }
        
        Uart4Len=Uart4Read(tempbuff);
        Uart4DmaStartCnt  = dmacnt; 
        Uart4RcvStartFlag = 0;
		if(Uart4Len>0)
			queue_in(&wifi_queue, tempbuff, Uart4Len);  

      }    
    }
    else 
    {
      Uart4DmaLastCnt   = dmacnt;
      Uart4DataInTime = Systemtime;    // rt_tick_get();
    }   
  } 
}

unsigned short Uart4Read(unsigned char *data)
{
    unsigned short  i;
    unsigned short  len, len1,len2;
    if(Uart4Len > (RX_BUFFER_SIZE - 1)) return 0;
    len = Uart4Len;
    if(len + Uart4DmaLastCnt < RX_BUFFER_SIZE)
    {
        for(i=0;i<len;i++)
            data[i] = Uart4RxBuffer[RX_BUFFER_SIZE - Uart4DmaLastCnt - Uart4Len + i];
    }
    else
    {
        len1 = len + Uart4DmaLastCnt - RX_BUFFER_SIZE;
        len2 = len - len1;
        for(i=0;i<len1;i++)
            data[i] = Uart4RxBuffer[RX_BUFFER_SIZE - len1 + i];
        for(i=0;i<len2;i++)
            data[len1+i] = Uart4RxBuffer[i];
    }
    UART4FramInFlag = 0; Uart4RcvStartFlag = 0;
    return(len);
}


void rt_kprintf4(const char *fmt, ...)
{
    va_list args;
    char length;
    static char rt_log_buf4[128];

    va_start(args, fmt);
    // the return value of vsnprintf is the number of bytes that would be
    // written to buffer had if the size of the buffer been sufficiently
    // large excluding the terminating null byte. If the output string
    // would be larger than the rt_log_buf, we have to adjust the output
    // length. 
    length = vsnprintf(rt_log_buf4, sizeof(rt_log_buf4) - 1, fmt, args);
    if (length > 120)
        length = 120;

    rt_hw_console_output4(rt_log_buf4);

    va_end(args);
}

void rt_hw_console_output4(const char *str)
{   
    Printfmsg4((unsigned char*)str);
}


void Printfmsg4(unsigned char* msg)
{
    #ifdef DEBUG_PORT
    UART4_SentMsg(msg);
    #endif
}


void PrintfIOTPort4(unsigned char*msg,unsigned char len)
{
    #ifdef DEBUG_PORT
    if(len<=250){
        UART4_SentMsgL(msg,len);
    }
    #endif
}



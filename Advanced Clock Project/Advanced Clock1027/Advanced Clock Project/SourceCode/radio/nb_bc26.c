#include "main.h"
/*
TCP: 
ATI		// Display Product Identification Information
AT+CGMI		// Request Manufacturer Identification
AT+CGMM		// Request Model Identification
AT+CGMR		// Request Manufacturer Revision
AT+CESQ		// Extended Signal Quality
AT+CGSN     	// Request Product Serial Number
AT+QBAND?	// Get Mobile Operation Band
AT+QBAND=1,8	// Set Mobile Operation Band, YIDONG

TCP0: 
AT+QIOPEN=1,0,"TCP","58.246.124.18",60000,1234,0        // Open a socket service, and the context ID is 1 and socket service index is 0.
AT+QISTATE=1,0                                          // Query the connection status of socket service 0.
AT+QISEND=0,5, HEART                                    // Send char, and the data length is 10 bytes.
AT+QISENDEX=0,5,3031323334                              // Send hex, and the data length is 5 bytes.

AT+QSODIS=0                                             // Disconnect socket, 0?socket_id
AT+QSOCL=0                                              // Close socket, 0?socket_id

recv: 
+QIURC: "closed",0                                      // port closed
*/
uint32_t LastTime;

unsigned char  IoTNetCmd[10][100]=
{
  {"ATI\x0d\x0a"},                                                              
  {"AT+CESQ\x0d\x0a\x0d\x0a"},                                                         
  {"ATE0\x0d\x0a"},                                                        
  {"AT+QISTATE=1,0\x0d\x0a"},                                                     
  {"AT+QIOPEN=1,0,\"TCP\",\"152.136.23.6.42\",1002,1234,1\x0d\x0a"},            
  {"AT+QICFG=\"dataformat\",0,1\x0d\x0a"},            
  {"AT+QICLOSE=0\x0d\x0a"},                                                   
  {"AT+QBAND=1,8\x0d\x0a"},                                         
  {"AT+CPSMS=0\x0d\x0a"},                                     
  {"AT+CEDRXS=0,5\x0d\x0a"},                                                          
};

unsigned char  IOTCMDDemoMsg[10][200]=
{
  {"AT+QISEND=0,5,\"HEART\"\x0d\x0a"},                                          // char HEART
  {"AT+QISENDEX=0,18,6699AA55102700000100041187654321AF6C\"\x0d\x0a"},          // REG 
  {"AT+QISENDEX=0,18,6699AA55102700000100041787654321276C\"\x0d\x0a"},          // HEART  
  {"AT+QISENDEX=0,34,6699AA551027000001001015D4EA705E00000000000000000000000000000000591C\"\x0d\x0a"},                          // ALARM  
  {"AT+QISENDEX=0,46,6699AA5510270000010020160A000000320000000000000000000000000000000000000000000000000000006B69\"\x0d\x0a"},  // work data  
  {"AT+QICLOSE=0\x0d\x0a"},                                                     // char HEART

};


void NBModuleInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // RST ---- PB4
    GPIO_InitStructure.GPIO_Pin   = NB_RST_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(NB_RST_PORT, &GPIO_InitStructure);
    NB_RST_DIS();                                       //--------------Deselect : Chip Select high 

    // POWER KEY ---- PB5
    GPIO_InitStructure.GPIO_Pin   = NB_PWKEY_PIN;             
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(NB_PWKEY_PORT, &GPIO_InitStructure);
    NB_PWKEY_LOW();                                          //--------------Deselect : Chip Select high 

    // PSM INT ---- PB6
    GPIO_InitStructure.GPIO_Pin   = NB_PSM_PIN;             
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(NB_PSM_PORT, &GPIO_InitStructure);
    NB_PSM_HIGH();                                           //--------------Deselect : Chip Select high 
 
    rt_kprintf("*: iot init\r\n");
    
    
    NB_RST_ENA();
    delay_ms(500);
    NB_RST_DIS();
    
    NB_PWKEY_LOW();  delay_ms(600);
    NB_PWKEY_HIGH(); delay_ms(600);
    NB_PWKEY_LOW();  
    
    delay_ms(4000);
    
    int8_t rtn    = -1;
    queue_init(&nb_queue);
	UART3Init();       /*NB-IoT uart*/
	
    rt_kprintf("*: ATI\r\n");
    PrintfIOTPort3((unsigned char*)(&IoTNetCmd[0]),sizeof(IoTNetCmd[0]));

    LastTime = Systemtime;
    while(Systemtime < (LastTime + 300000))
    {
			Uart_AT_Deal();
       if(queue_out(&nb_queue, FrameInBuff, &FrameInlen) == QUEUE_OPER_OK)      // uart1接收IOT端口数据        
        {
            UART1_SentMsgL(FrameInBuff, FrameInlen);  
            rtn = StateCheck((char*)FrameInBuff);
            if(rtn == 3)
            {
              rt_kprintf("*: GET IP\r\n");
              break;
            }
        }
    }
    if(Systemtime >= (LastTime + 300000))
    {
       while(1)
      {
        u8 i;
        for(i=0;i<10;i++)
        {
         LED_D1_ON;
         delay_ms(500);
         LED_D1_OFF;
         delay_ms(500);
        }
        __set_FAULTMASK(1);                   //关闭所有中断
        NVIC_SystemReset();                   //NVIC_SystemReset()函数用来复位STM32，SYSRESETREQ 被置为有效
      }
    }
}











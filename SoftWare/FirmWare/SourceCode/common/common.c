#include "main.h"

void DataLine(int16_t *Din, uint8_t len)
{
    uint8_t i, j;
    int16_t head, part;
    for(j=0;j<=len-2;j++)
    {
        head=Din[j];
        for(i=0;i<len-j-1;i++)
        {
            if(head>Din[i+j+1])
            {
                part=head;
                head=Din[i+j+1];			
                Din[i+j+1]=part;		
            }
        }
        Din[j]=head;
    }
}

unsigned char GetOutputData(unsigned char* output, unsigned char* data, unsigned char len)
{
    unsigned char length;
    if(len <= 32)
    {
        memcpy((void*)(&output[11]), NB_NETWORK_IPPORT, sizeof(NB_NETWORK_IPPORT)-1);
        length = 11 + sizeof(NB_NETWORK_IPPORT)-1;
        
        output[length] = ',';
        length = length + 1;
        
        sprintf((char *)(&output[length]), "%d,", len);
        if(len<10)       length = length + 2;
        else if(len<100) length = length + 3;
        else             length = length + 4;
        
        TransferDataToString(data, &output[length], len);
        length = length + len*2;
        
        output[length] = 0x0d;output[length+1] = 0x0a;
        length = length + 2;
    }
    else
    {
        length = 0;
    }
    return (length);
}

void TransferDataToString(unsigned char *data, unsigned char *string, unsigned char len)
{
    unsigned char ti;
    
    for(ti=0;ti<len;ti++)
    {
        sprintf((char *)(&string[ti*2]), "%02X", data[ti]);
    } 
}


int8_t GetCesqValue(char *data)
{   
    char *point;
    int8_t rtn   = 0;
    unsigned char start = 0; 
    unsigned char end   = 0;
    // rtn: CESQ
    point = strstr(data, "+CESQ: ");  // CGATT:1
    if(point != NULL)                   // 存在CESQ
    {
      //rt_kprintf("*: %c%c%c%c%c%c%c\n", point[1], point[4], point[5],point[6],point[7],point[8],point[9]);
      for(char ii=0;ii<10;ii++)
      {
          if(point[ii] ==':') start = ii;
          if(point[ii] ==',') end   = ii;
      }
      if((start ==0)||(end==0))return (-2);
      // rt_kprintf("*: %c %c %c %c \n", point[start],point[start+1], point[end-1], point[end]); 
      // rt_kprintf("*: s=%d e=%d \n", start,end); 
      for(char ii=0;ii<end-start-1;ii++)
      {
        if((point[end-1-ii] >='0')&&(point[end-1-ii] <='9'))
        {
          rtn = (uint8_t)(rtn + pow(10, ii)*(point[end-1-ii] - '0' ));
        }
      }
      rt_kprintf("*: rtn=%d\n", rtn); 
    }
    else{
        rt_kprintf("*: no msg\n"); 
        rtn = -1;
    }
    
    return (rtn);
}

int8_t GetRecvIOTMsg(char *msg, unsigned char* len, unsigned char* data)        // 获得接收到的数据帧长度和数组
{   
    char *point;
    int8_t rtn   = 0;
    unsigned char cnt = 0; 
    unsigned char ii;
    // rtn: CESQ
    point = strstr(msg, "+QIURC: \"recv\",0,");         // recv
    if(point != NULL)                                   // 存在CESQ
    {
      rt_kprintf("*: rcv download msg\n");
      for(ii=0;ii<4;ii++)
      {
          if((point[17+ii] =='\r')&&(point[17+1+ii] =='\n')) {break;}
      }
      if((ii == 0)||(ii == 4))return (-1);
      cnt = ii;
      rt_kprintf("*: cnt:%d, %c %c %c %c \n", cnt, point[18],point[19], point[20], point[21]); 
      if(cnt==1)
      {
          len[0] = 0;
          len[0] = len[0] + (unsigned char)(pow(10, 0))*(point[17] - '0' );
      }
      else if(cnt==2)
      {
          len[0] = 0;
          len[0] = len[0] + (unsigned char)(pow(10, 1))*(point[17] - '0' );
          len[0] = len[0] + (unsigned char)(pow(10, 0))*(point[18] - '0' );
      }
      else if(cnt==3)
      {
          len[0] = 0;
          len[0] = len[0] + (unsigned char)(pow(10, 2))*(point[17] - '0' );
          len[0] = len[0] + (unsigned char)(pow(10, 1))*(point[18] - '0' );
          len[0] = len[0] + (unsigned char)(pow(10, 0))*(point[19] - '0' );
      }
      else rtn = -1;
      if(len[0]<200){
          memcpy(data, &point[17+cnt+2], len[0]);
      }
      else{
          rt_kprintf("*: rcv len err\n"); 
      }
      rt_kprintf("*: rcv len=%d\n", len[0]); 
    }
    else{
        rt_kprintf("*: no msg\n"); 
        rtn = -1;
    }
    
    return (rtn);
}






















#ifndef __NB_IOT_BC26_H
#define __NB_IOT_BC26_H
#include "stm32f4xx.h"

#define    NB_NETWORK_IPPORT          "139.196.197.182,12000"     // nBYTE

#define NB_RST_PIN                         GPIO_Pin_1
#define NB_RST_PORT                        GPIOD
#define NB_RST_DIS()                       GPIO_ResetBits(GPIOD, GPIO_Pin_1)    // rst high, not enable
#define NB_RST_ENA()                       GPIO_SetBits(GPIOD, GPIO_Pin_1)      // rst low, enable rst

#define NB_PWKEY_PIN                       GPIO_Pin_0
#define NB_PWKEY_PORT                      GPIOD
#define NB_PWKEY_LOW()                     GPIO_ResetBits(GPIOD, GPIO_Pin_0)   // pwrkey high
#define NB_PWKEY_HIGH()                    GPIO_SetBits(GPIOD, GPIO_Pin_0)     // pwrkey low

#define NB_PSM_PIN                         GPIO_Pin_2
#define NB_PSM_PORT                        GPIOD
#define NB_PSM_HIGH()                      GPIO_ResetBits(GPIOD, GPIO_Pin_2)   // psm high
#define NB_PSM_LOW()                       GPIO_SetBits(GPIOD, GPIO_Pin_2)     // psm low


extern unsigned char  IoTNetCmd[10][100];

void NBModuleInit(void);
uint8_t IOTNetRegister(void);
void SendInitCmd(u8 *data,u32 TempSystemTime);


#endif





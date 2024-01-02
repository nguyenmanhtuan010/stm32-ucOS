#ifndef __DHT112_H
#define __DHT112_H 
#include "sys.h"   

#define DHT11_IO_IN_2()  {GPIOA->CRL&=0X0FFFFFFF;GPIOA->CRL|=8<<28;}
#define DHT11_IO_OUT_2() {GPIOA->CRL&=0X0FFFFFFF;GPIOA->CRL|=3<<28;}

#define	DHT11_DQ_OUT_2 PAout(7) 
#define	DHT11_DQ_IN_2  PAin(7) 

u8 DHT11_Init_2(void);
u8 DHT11_Read_Data_2(u8 *temp,u8 *humi);
u8 DHT11_Read_Byte_2(void);
u8 DHT11_Read_Bit_2(void);
u8 DHT11_Check_2(void);
void DHT11_Rst_2(void);  

#endif

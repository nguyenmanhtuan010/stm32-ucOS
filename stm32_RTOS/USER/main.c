#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lm393.h"
#include "lm393_2.h"
#include "dht11.h"
#include "dht11_2.h"
#include "includes.h"

uint16_t SumLight, SumLight2,i;
/////////////////////////OS///////////////////////////////////

#define START_TASK_PRIO      			10 
#define START_STK_SIZE  				64
OS_STK START_TASK_STK[START_STK_SIZE];

void start_task(void *pdata);	


#define DHT_TASK_PRIO    			5 
#define DHT_STK_SIZE  		 		128
OS_STK DHT_TASK_STK[DHT_STK_SIZE];

void DHT_task(void *pdata);


#define LM393_TASK_PRIO       			4
#define LM393_STK_SIZE  					256
__align(8) static OS_STK LM393_TASK_STK[LM393_STK_SIZE];//////////////////// 

void LM393_task(void *pdata);

////////////////////////////////////////////////////////////
OS_EVENT * msg_key;	
 

 
 int main(void)
 {	
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 delay_init();	
	 LM393_2Iint();
	 LM393Iint();
	 USARTx_Init(USART1, Pins_PA9PA10, 9600);
	 OSInit();   
 	 OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );
	 OSStart();
	 
 }

 
 void start_task(void *pdata){
	 
	 OS_CPU_SR cpu_sr=0;
	 pdata = pdata; 
	 
	 msg_key=OSMboxCreate((void*)0);	
	 OS_ENTER_CRITICAL();//	
	 
	 OSTaskCreate(DHT_task,(void *)0,(OS_STK*)&DHT_TASK_STK[DHT_STK_SIZE-1],DHT_TASK_PRIO);
	 OSTaskCreate(LM393_task,(void *)0,(OS_STK*)&LM393_TASK_STK[LM393_STK_SIZE-1],LM393_TASK_PRIO);
	 
	 
	 OSTaskSuspend(START_TASK_PRIO);	
	 OS_EXIT_CRITICAL();//
	 
 }
 
 void LM393_task(void *pdata)
{	 
	char buff[20];
	  while(1)
	{
		  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
			SumLight = ADC_GetConversionValue(ADC1);
		  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
			SumLight2 = ADC_GetConversionValue(ADC2);
			delay_ms(1000);
		  sprintf(buff,"%ld %ld",SumLight,SumLight2);
		  OSMboxPost(msg_key,(void*)buff);
		}
}	

void DHT_task(void *pdata)
{
	u8 err;
	void *key=0;
  u8 temp = 0, humi = 0, temp_2 = 0, humi_2 = 0;
	while(1)
	{
		key = OSMboxPend(msg_key,10,&err);
		DHT11_Read_Data(&temp,&humi);
		DHT11_Read_Data_2(&temp_2,&humi_2);
		printf("%d %d %d %d %s",temp,humi,temp_2,humi_2,key);
		delay_ms(2000);	 
	}									 
}




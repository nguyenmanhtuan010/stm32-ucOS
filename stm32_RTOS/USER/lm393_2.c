#include "lm393_2.h"

void LM393_2Iint(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	//cau hinh cho chan GPIO va bo ADC hoat dong
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/*cau hinh chan Input cua bo ADC1 la chan PA0*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//cau hinh ADC
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);
	/* Cau hinh chanel, rank, thoi gian lay mau */
	ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5); 
	/* Cho phep bo ADC1 hoat dong */
	ADC_Cmd(ADC2, ENABLE);
	/* Bat dau chuyen doi ADC */
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);
}
	
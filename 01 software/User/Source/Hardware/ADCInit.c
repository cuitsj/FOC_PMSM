/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : ADCInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains ADC function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>




/*-------------------------------------------------------------------------------------------------
	Function Name :	void ADC_Init(void)
	Description   :	ADC硬件设备初始化配置
	Input         :	无
	Output				:	无
-------------------------------------------------------------------------------------------------*/
void ADC_Init(void)
{
/*-------------------------------------------------------------------------------------------------
	ADC基本配置
	1、使能ADC
	4、设置数据格式为12bit
	5、禁止ADC Trigger
-------------------------------------------------------------------------------------------------*/
	SetBit(ADC_STA, ADCEN, 1);		                           // 使能ADC
	SetBit(ADC_STA, ADCBS, 0);                               // 12bit数据格式，0为12，1为10位
	SetBit(ADC_CFG, ADTRIGEN, 0);                            // 禁止ADC Trigger

	
/*-------------------------------------------------------------------------------------------------
	ADC参考电压选择
	ADCREF = 00：选择VDD5作为ADC参考电压
	ADCREF = 01：选择外部VREF作为ADC参考电压
	ADCREF = 10：选择内部VREF作为ADC参考电压
	ADCREF = 11：选择内部VREF作为ADC参考电压，同时参考电压输出到VREF管脚
-------------------------------------------------------------------------------------------------*/
	SetBit(ADC_CFG, ADCREF1, 1);
	SetBit(ADC_CFG, ADCREF0, 0);
	SetBit(VREF_CR, VREFEN , 1);//使能VREF

/*-------------------------------------------------------------------------------------------------
	ADC通道配置
	1、使能相应Pin为模拟Pin，禁止数字功能
	2、使能通道MASK，连接Pin到ADC模块
-------------------------------------------------------------------------------------------------*/
//	SetBit(P3_AN, P34, 1);	
//	SetBit(ADC_MASK, CH7EN, 1);		                          //Config P3.4 as ADC7
	SetBit(P3_AN, P33, 1);	
	SetBit(ADC_MASK, CH6EN, 1);		                       		  //Config P3.3 as ADC6,VSP,模拟调速
// 	SetBit(P3_AN, P32, 1);	
// 	SetBit(ADC_MASK, CH5EN, 1);		                         	//Config P3.2 as ADC5
	SetBit(P2_AN, P27, 1);	
	SetBit(ADC_MASK, CH4EN, 1);		                            //Config P2.7 as ADC4 IBUS,总电流
 	SetBit(P2_AN, P25, 1);	
 	SetBit(ADC_MASK, CH3EN, 1);		                        	  //Config P2.5 as ADC3 
	SetBit(P2_AN, P24, 1);	
	SetBit(ADC_MASK, CH2EN, 1);		                            //Config P2.4 as ADC2 DCBUS,母线电压
 	SetBit(P2_AN, P23, 1);	
 	SetBit(ADC_MASK, CH1EN, 1);		                         	  //Config P2.3 as ADC1 IV,V相电流
 	SetBit(P2_AN, P20, 1);				
 	SetBit(ADC_MASK, CH0EN, 1);                            	  //Config P2.0 as ADC0 IU,U相电流
	
	/*ADC通道采样时钟周期设置*/
	ADC_SCYC1 = 0x33;								                        // 3个ADC Clock
	ADC_SCYC0 = 0x33;								                        // ADC2,ADC3 15 clock,3个ADC Clock
	
/*-------------------------------------------------------------------------------------------------
	ADC中断设置
  1、使能ADC中断
	2、禁止ADC Trigger中断
	3、设置ADC中断优先级别为0
-------------------------------------------------------------------------------------------------*/	
	CLR(ADC_STA, ADCIF);
	SetBit(ADC_STA, ADCIE, 0);	                            // 0，禁止ADC中断，1，使能ADC中断
	CLR(ADC_CFG, ADTRIGIF);
	SetBit(ADC_CFG, ADTRIGIE, 0);	                          // 0，禁止ADC Trigger中断，1，使能ADC Trigger中断
	
	PADC1 = 0;
	PADC0 = 0;										                          // ADC中断优先级别为0
	EA = 1;	
	
}




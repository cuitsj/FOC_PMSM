/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : TimerInit.h
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __TIMERINIT_H_
#define __TIMERINIT_H_
#define TIM0_Fre                       (24000000.0)                              // TIM0计数频率24MHz
#define TIM5_Fre                       (187500.0)                                // TIM0计数频率187.5KHz

/* Exported types -------------------------------------------------------------------------------*/
typedef struct
{
  uint8 PWMINCnt;
	uint16 PWMINHigh;
	uint16 PWMINPeriod;
	uint16 PWMINHighDuty;
	
} PWMINCtl_TypeDef;

/* Exported variables ---------------------------------------------------------------------------*/
extern PWMINCtl_TypeDef xdata PWMINCtl;

/* Exported functions ---------------------------------------------------------------------------*/
extern void TIM0_Init(void);
extern void TIM1_Init(void);
extern void TIM2_Init(void);
extern void TIM3_Init(void);
extern void TIM4_Init(void);
extern void TIM5_Init(void);
extern void TIM5_Timing_us(uint16 DR, uint16 ARR);
extern void PWMIN_Init(void);


#endif
/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : EXTIInit.h
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __EXTIINIT_H_
#define __EXTIINIT_H_


#if (HallMode == HallIC)
	#define	HA	GP14
	#define	HB	GP16
	#define	HC	GP21

#elif (HallMode == HallSensor)
	#define	HA	GetBit(CMP_SR, CMP0OUT)
	#define	HB	GetBit(CMP_SR, CMP1OUT)
	#define	HC	GetBit(CMP_SR, CMP2OUT)
	
#endif

/* Exported functions ---------------------------------------------------------------------------*/
extern void EXTI_Init(void);

#endif



/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_ADC.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_ADC_H_
#define __FU68xx_ADC_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
#define ADC_STA XBYTE[0x4037]									// ADC_STA¼Ä´æÆ÷
#define ADCEN		 	7
#define ADCBSY	  6
#define ADCBS     5
#define ADCIE	   	1
#define ADCIF			0

#define ADC_CFG XBYTE[0x4035]									// ADC_CFG¼Ä´æÆ÷		
#define ADCREF1		4
#define ADCREF0		3
#define ADTRIGEN  2
#define ADTRIGIE	1
#define ADTRIGIF	0

#define ADC_MASK XBYTE[0x4036]								// ADC_STA¼Ä´æÆ÷
#define CH7EN	 	 	7
#define CH6EN	 		6
#define CH5EN	 		5
#define CH4EN	 		4
#define CH3EN	 		3
#define CH2EN	 		2
#define CH1EN	 		1
#define CH0EN	 		0

#define ADC_SCYC0 XBYTE[0x4048]							 // ADC²ÉÑùÊ±ÖÓÖÜÆÚ¼Ä´æÆ÷
#define ADC_SCYC1 XBYTE[0x4049]

#define ADC0_DR *(volatile unsigned int xdata *)0x4038		
#define ADC0_DRH XBYTE[0x4038]
#define ADC0_DRL XBYTE[0x4039]

#define ADC1_DR *(volatile unsigned int xdata *)0x403a
#define ADC1_DRH XBYTE[0x403a]
#define ADC1_DRL XBYTE[0x403b]

#define ADC2_DR *(volatile unsigned int xdata *)0x403c
#define ADC2_DRH XBYTE[0x403c]
#define ADC2_DRL XBYTE[0x403d]

#define ADC3_DR *(volatile unsigned int xdata *)0x403e
#define ADC3_DRH XBYTE[0x403e]
#define ADC3_DRL XBYTE[0x403f]

#define ADC4_DR *(volatile unsigned int xdata *)0x4040
#define ADC4_DRH XBYTE[0x4040]
#define ADC4_DRL XBYTE[0x4041]

#define ADC5_DR *(volatile unsigned int xdata *)0x4042
#define ADC5_DRH XBYTE[0x4042]
#define ADC5_DRL XBYTE[0x4043]

#define ADC6_DR *(volatile unsigned int xdata *)0x4044
#define ADC6_DRH XBYTE[0x4044]
#define ADC6_DRL XBYTE[0x4045]

#define ADC7_DR *(volatile unsigned int xdata *)0x4046
#define ADC7_DRH XBYTE[0x4046]
#define ADC7_DRL XBYTE[0x4047]

#endif

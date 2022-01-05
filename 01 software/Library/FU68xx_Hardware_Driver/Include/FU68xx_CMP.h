/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_CMP.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_CMP_H_
#define __FU68xx_CMP_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr CMP_CR0 = 0xd5;										// ±È½ÏÆ÷¿ØÖÆ¼Ä´æÆ÷0
#define CMP3IM1			7
#define CMP3IM0			6
#define CMP2IM1			5
#define CMP2IM0			4
#define CMP1IM1			3
#define CMP1IM0			2
#define CMP0IM1			1
#define CMP0IM0			0

sfr CMP_CR1 = 0xd6;										// ±È½ÏÆ÷¿ØÖÆ¼Ä´æÆ÷1
#define BEMFREN			6
#define CMP3HYS2		5
#define CMP3HYS1		4
#define CMP3HYS0		3
#define CMP0HYS2		2
#define CMP0HYS1		1
#define CMP0HYS0		0


sfr CMP_SR = 0xd7;										// ±È½ÏÆ÷×´Ì¬¼Ä´æÆ÷
#define CMP3OUT			7
#define CMP2OUT			6
#define CMP1OUT			5
#define CMP0OUT			4
#define CMP3INTR		3
#define CMP2INTR		2
#define CMP1INTR		1
#define CMP0INTR		0

sfr CMP_CR2 = 0xda;										// ±È½ÏÆ÷¿ØÖÆ¼Ä´æÆ÷
#define CMP3EN			7
#define CMP2EN			6
#define CMP1EN			5
#define CMP0EN  		4
#define CMPSAME			3
#define CMPSEL1			2
#define CMPSEL0			1
#define CMPOE			  0



#endif

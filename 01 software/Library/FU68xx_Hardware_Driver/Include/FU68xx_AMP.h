/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_AMP.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_AMP_H_
#define __FU68xx_AMP_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
#define AMP_CR XBYTE[0x404e]
#define AMPISEL2		6
#define AMPISEL1		5
#define AMPISEL0		4
#define AMP3EN			2
#define AMP2EN			2
#define AMP1EN			1
#define AMP0EN			0

#endif

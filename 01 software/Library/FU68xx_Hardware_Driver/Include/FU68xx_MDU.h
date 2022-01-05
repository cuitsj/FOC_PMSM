/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_MDU.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_MDU_H_
#define __FU68xx_MDU_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr MD_MODE = 0xc1;                    // MDUÅäÖÃ¼Ä´æÆ÷
#define	DIVDONE			7
#define	DIVERR			6
#define ALIGN1			3
#define ALIGN0			2
#define MDSN				1
#define DIVSTART		0

sfr16 MD_MA = 0xc2;											// ³Ë·¨¼Ä´æÆ÷
sfr MD_MAL = 0xc2;
sfr MD_MAH = 0xc3;

sfr16 MD_DAL = 0xc4;										// ³ý·¨¼Ä´æÆ÷
sfr16 MD_DAH = 0xc6;
sfr MD_DA0 = 0xc4;
sfr MD_DA1 = 0xc5;
sfr MD_DA2 = 0xc6;
sfr MD_DA3 = 0xc7;

sfr16 MD_MB = 0xca;											// ³Ë·¨¼Ä´æÆ÷
sfr MD_MBL = 0xca;
sfr MD_MBH = 0xcb;

sfr16 MD_DBL = 0xcc;										// ³ý·¨¼Ä´æÆ÷
sfr16 MD_DBH = 0xce;
sfr MD_DB0 = 0xcc;
sfr MD_DB1 = 0xcd;
sfr MD_DB2 = 0xce;
sfr MD_DB3 = 0xcf;

#endif
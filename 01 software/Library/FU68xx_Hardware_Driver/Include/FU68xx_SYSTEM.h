/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_SYSTEM.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_SYSTEM_H_
#define __FU68xx_SYSTEM_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr ACC = 0xe0;					// ÀÛ¼Ó¼Ä´æÆ÷

sfr B = 0xf0;						// B¼Ä´æÆ÷

sfr PSW = 0xd0;         // ³ÌÐò×´Ì¬¼Ä´æÆ÷
sbit CY	 = PSW^7;
sbit AC	 = PSW^6;
sbit F0  = PSW^5;
sbit RS1 = PSW^4;
sbit RS0 = PSW^3;
sbit OV  = PSW^2;
sbit F1  = PSW^1;
sbit P	 = PSW^0;

sfr PCON = 0x87;
#define GF4	 		7
#define GF3	 		5
#define GF2			4
#define GF1	 		3
#define GF0	 		2
#define STOP 		1
#define IDLE 		0

sfr RSTSRC = 0xc9;			// ¸´Î»×´Ì¬¼Ä´æÆ÷
#define POR			7
#define EXTR		6
#define LVDR		5
#define EOSR		4
#define WDTR		3
#define FEDR		2
#define DBGR		1
#define RSTCLR	0

sfr SP = 0x81;					// ¶ÑÕ»Ö¸Õë¼Ä´æÆ÷

sfr16 DP = 0x82;				// Êý¾ÝÖ¸Õë¼Ä´æÆ÷
sfr DPL = 0x82;
sfr DPH = 0x83;

sfr EVT_FILT = 0xd9;		// ¹ÊÕÏ±£»¤¼Ä´æÆ÷
#define TSDEN		 	 	7		
#define TSDADJ1			6
#define TSDADJ0		  5		
#define MCLREN		  4		
#define EFSRC				3
#define EFEN				2
#define EFDIV1		  1
#define EFDIV0		  0

#endif

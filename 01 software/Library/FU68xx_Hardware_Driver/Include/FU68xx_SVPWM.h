/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_SVPWM.c
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_SVPWM_H_
#define __FU68xx_SVPWM_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr SV_CR = 0xe1;									// SVPWM¿ØÖÆ¼Ä´æÆ÷
#define SVBUSY			7
#define SPWMSEL			4
#define SPWMEN      3
#define SV5SEG			2
#define SVPWMEN     1
#define SVSTA			  0

sfr16 SV_US = 0xe2;								// SV_US¿ØÖÆ¼Ä´æÆ÷
sfr SV_USL = 0xe2;
sfr SV_USH = 0xe3;

sfr16 SV_ANGLE = 0xe4;						// SV_ANGLE¿ØÖÆ¼Ä´æÆ÷
sfr SV_ANGLEL = 0xe4;
sfr SV_ANGLEH = 0xe5;

#define SV_PWMPRD *(volatile unsigned int xdata *)0x4060
#define SV_PWMPRDH XBYTE[0x4060]
#define SV_PWMPRDL XBYTE[0x4061]

#endif

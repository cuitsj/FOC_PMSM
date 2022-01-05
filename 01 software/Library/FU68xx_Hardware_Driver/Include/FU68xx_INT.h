/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_INT.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_INT_H_
#define __FU68xx_INT_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)


/* Register Define ------------------------------------------------------------------------------*/
sfr TCON = 0x88;											// TCON寄存器
sbit MCDIF = TCON^6;
sbit TSDIF = TCON^5;
sbit IT11  = TCON^4;
sbit IT10  = TCON^3;
sbit IF0   = TCON^2;
sbit IT01  = TCON^1;
sbit IT00  = TCON^0;

sfr IE = 0xa8;												// IE寄存器
sbit EA	 	 = IE^7;
sbit RTCIE = IE^6;
sbit MCDIE = IE^5;
sbit ES0 	 = IE^4;
sbit SPIIE = IE^3;
sbit EX1 	 = IE^2;
sbit TSDIE = IE^1;
sbit EX0 	 = IE^0;

sfr IP0 = 0xb8;											  // 中断优先级别寄存器0
sbit PFOC1 = IP0^7;
sbit PFOC0 = IP0^6;
sbit PX11  = IP0^5;
sbit PX10  = IP0^4;
sbit PX01  = IP0^3;
sbit PX00  = IP0^2;
sbit PLVD1 = IP0^1;
sbit PLVD0 = IP0^0;

sfr IP1 = 0xc0;											  // 中断优先级别寄存器1							
sbit PCMP1  = IP1^7;
sbit PCMP0  = IP1^6;
sbit PADC1  = IP1^5;
sbit PADC0  = IP1^4;
sbit PTIM11 = IP1^3;
sbit PTIM10 = IP1^2;
sbit PTIM01	= IP1^1;
sbit PTIM00	= IP1^0;

sfr IP2 = 0xc8;											  // 中断优先级别寄存器2
sbit PTSD1 		= IP2^7;
sbit PTSD0 		= IP2^6;
sbit PTIM451 	= IP2^5;
sbit PTIM450 	= IP2^4;
sbit PTIM231	= IP2^3;
sbit PTIM230	= IP2^2;
sbit PRTC1 		= IP2^1;
sbit PRTC0 		= IP2^0;

sfr IP3 = 0xd8;                       // 中断优先级别寄存器3
sbit PMCD1 = IP3^7;
sbit PMCD0 = IP3^6;
sbit PSPI1 = IP3^5;
sbit PSPI0 = IP3^4;
sbit PI2C1 = IP3^3;
sbit PI2C0 = IP3^2;
sbit PUART1 = IP3^1;
sbit PUART0 = IP3^0;




#endif
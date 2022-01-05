/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_GPIO.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_GPIO_H_
#define __FU68xx_GPIO_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

#define  key_port                      GP06
/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/*GPIO¶Ë¿ÚÎ»ºÅ¶¨Òå-------------------------------------------------------------------------------*/
#define	P07			7
#define	P06			6
#define	P05			5
#define	P04			4
#define	P03			3
#define	P02			2
#define	P01			1
#define	P00			0

#define	P17			7
#define	P16			6
#define	P15			5
#define	P14			4
#define	P13			3
#define	P12			2
#define	P11			1
#define	P10			0

#define	P27			7
#define	P26			6
#define	P25			5
#define	P24			4
#define	P23			3
#define	P22			2
#define	P21			1
#define	P20			0

#define	P37			7
#define	P36			6
#define	P35			5
#define	P34			4
#define	P33			3
#define	P32			2
#define	P31			1
#define	P30			0

#define P42			2
#define P41			1
#define P40			0


/* Register Define ------------------------------------------------------------------------------*/
sfr P0_OE = 0xfc;														// P0Êä³öÊ¹ÄÜ¼Ä´æÆ÷
sfr P1_OE = 0xfd;														// P1Êä³öÊ¹ÄÜ¼Ä´æÆ÷
sfr P2_OE = 0xfe;														// P2Êä³öÊ¹ÄÜ¼Ä´æÆ÷
sfr P3_OE = 0xff;														// P3Êä³öÊ¹ÄÜ¼Ä´æÆ÷

sfr P1IE = 0xd1;														// P1ÖÐ¶ÏÊ¹ÄÜ¼Ä´æÆ÷
sfr P1IF = 0xd2;														// P1ÖÐ¶Ï±êÖ¾¼Ä´æÆ÷
sfr P2IE = 0xd3;														// P2ÖÐ¶ÏÊ¹ÄÜ¼Ä´æÆ÷
sfr P2IF = 0xd4;														// P2ÖÐ¶Ï±êÖ¾¼Ä´æÆ÷

#define P0_PU XBYTE[0x4053]                 // P0Êä³öÊ¹ÄÜ¼Ä´æÆ÷
#define P1_PU XBYTE[0x4054]									// P1Êä³öÊ¹ÄÜ¼Ä´æÆ÷
#define P2_PU XBYTE[0x4055]									// P2Êä³öÊ¹ÄÜ¼Ä´æÆ÷
#define P3_PU XBYTE[0x4056]									// P3Êä³öÊ¹ÄÜ¼Ä´æÆ÷

#define P1_AN XBYTE[0x4050]									// P1Ä£ÄâÊ¹ÄÜ¼Ä´æÆ÷
#define HBMODE    3
#define ODE1			1
#define ODE0			0

#define P2_AN XBYTE[0x4051]									// P2Ä£ÄâÊ¹ÄÜ¼Ä´æÆ÷
#define P3_AN XBYTE[0x4052]									// P3Ä£ÄâÊ¹ÄÜ¼Ä´æÆ÷

#define DRV_CTL XBYTE[0x404d]								// Çý¶¯Êä³öÑ¡Ôñ/Ê¹ÄÜ¼Ä´æÆ÷
#define OCS1			3
#define OCS0			2
#define PRDRVEN		1
#define DRVOE			0

#define PH_SEL XBYTE[0x404c]								// ¹Ü½Å¸´ÓÃ¹¦ÄÜÑ¡Ôñ¼Ä´æÆ÷
#define UT1EN			6
#define UT0EN			5
#define T4SEL			4
#define T3SEL			3
#define T2SEL			2
#define T5SEL			1
#define T0CH4SEL	0

sfr  DRV_OUT = 0xf8;													// Çý¶¯Êä³ö¿ÕÏÐ×´Ì¬¼Ä´æÆ÷
sbit MOE   = DRV_OUT^7;
sbit OIS4  = DRV_OUT^6;
sbit OISWL = DRV_OUT^5;
sbit OISWH = DRV_OUT^4;
sbit OISVL = DRV_OUT^3;
sbit OISVH = DRV_OUT^2;
sbit OISUL = DRV_OUT^1;
sbit OISUH = DRV_OUT^0;


sfr  P0 = 0x80;															// P0¶Ë¿Ú¼Ä´æÆ÷
sbit GP07 = P0^7;
sbit GP06 = P0^6;
sbit GP05 = P0^5;
sbit GP04 = P0^4;
sbit GP03 = P0^3;
sbit GP02 = P0^2;
sbit GP01 = P0^1;
sbit GP00 = P0^0;

sfr  P1 = 0x90;															// P1¶Ë¿Ú¼Ä´æÆ÷
sbit GP17 = P1^7;
sbit GP16 = P1^6;
sbit GP15 = P1^5;
sbit GP14 = P1^4;
sbit GP13 = P1^3;
sbit GP12 = P1^2;
sbit GP11 = P1^1;
sbit GP10 = P1^0;

sfr  P2 = 0xa0;															// P2¶Ë¿Ú¼Ä´æÆ÷
sbit GP27 = P2^7;
sbit GP26 = P2^6;
sbit GP25 = P2^5;
sbit GP24 = P2^4;
sbit GP23 = P2^3;
sbit GP22 = P2^2;
sbit GP21 = P2^1;
sbit GP20	= P2^0;

sfr  P3 = 0xb0;															// P3¶Ë¿Ú¼Ä´æÆ÷
sbit GP37 = P3^7;
sbit GP36 = P3^6;
sbit GP35 = P3^5;
sbit GP34 = P3^4;
sbit GP33 = P3^3;
sbit GP32 = P3^2;
sbit GP31 = P3^1;
sbit GP30 = P3^0;


#endif

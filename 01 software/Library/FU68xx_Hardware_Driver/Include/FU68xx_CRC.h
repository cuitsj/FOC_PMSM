/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_CRC.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_CRC_H_
#define __FU68xx_CRC_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
#define CRC0DIN XBYTE[0x4021]

#define CRC0STA XBYTE[0x4022]
#define CRCDONE			4
#define	CRCDINI			3
#define CRCVAL			2
#define AUTOINT			1
#define CRCPNT			0

#define CRC0DAT XBYTE[0x4023]
#define CRC0BEG XBYTE[0x4024]
#define CRC0CNT XBYTE[0x4025]



#endif

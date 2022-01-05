/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_SPI.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_SPI_H_
#define __FU68xx_SPI_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/*ÖÐ¶Ï±êÖ¾ÇåÁãÖ¸Áî-------------------------------------------------------------------------------*/
/*SPIÖÐ¶Ï±êÖ¾ÇåÁãÖ¸Áî*/
#define CLRFlag_SPIF 					{SPI_CTRL = SPI_CTRL & (~(1<<SPIF)) | 0x70;}
#define CLRFlag_WCOL   				{SPI_CTRL = SPI_CTRL & (~(1<<WCOL)) | 0xb0;}
#define CLRFlag_MODF   				{SPI_CTRL = SPI_CTRL & (~(1<<MODF)) | 0xd0;}
#define CLRFlag_RXOVRN   			{SPI_CTRL = SPI_CTRL & (~(1<<RXOVRN)) | 0xe0;}


/* Register Define ------------------------------------------------------------------------------*/
#define SPI_CFG XBYTE[0x4030]             // SPIÅäÖÃ¼Ä´æÆ÷
#define	SPIBSY		7
#define	MSTEN			6
#define	CKPHA			5
#define	CKPOL			4
#define	SLVSEL		3
#define	NSSIN			2
#define	SRMT			1
#define	RXBMT			0
							
#define SPI_CTRL XBYTE[0x4031]            // SPI¿ØÖÆ¼Ä´æÆ÷
#define	SPIF			7
#define	WCOL			6
#define	MODF			5
#define	RXOVRN		4
#define	NSSMID1		3
#define	NSSMID0		2
#define	TXBMT			1
#define	SPIEN			0
							
#define SPI_SCR XBYTE[0x4032]             // SPIÊ±ÖÓ¼Ä´æÆ÷
							
#define SPI_DAT XBYTE[0x4033]							// SPIÊý¾Ý¼Ä´æÆ÷

#endif

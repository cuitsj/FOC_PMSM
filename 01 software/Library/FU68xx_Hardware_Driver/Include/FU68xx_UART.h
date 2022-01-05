/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_UART.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_UART_H_
#define __FU68xx_UART_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr UT_CR = 0x98;												// UART¿ØÖÆ¼Ä´æÆ÷
sbit MOD1	= UT_CR^7;
sbit MOD0	= UT_CR^6;
sbit SM2	= UT_CR^5;
sbit REN	= UT_CR^4;
sbit TB8	= UT_CR^3;
sbit RB8	= UT_CR^2;
sbit TI		= UT_CR^1;
sbit RI		= UT_CR^0;

sfr UT_DR = 0x99;                       // UARTÊý¾Ý¼Ä´æÆ÷

sfr16 UT_BAUD = 0x9a;										// UART²¨ÌØÂÊ¼Ä´æÆ÷
sfr UT_BAUDL = 0x9a;
sfr UT_BAUDH = 0x9b;



#endif
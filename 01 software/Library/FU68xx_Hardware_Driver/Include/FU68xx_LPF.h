/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_LPF.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_LPF_H_
#define __FU68xx_LPF_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr16 LPF_K = 0xdc;							// LPF_K¼Ä´æÆ÷
sfr LPF_KL = 0xdc;
sfr LPF_KH = 0xdd;

sfr16 LPF_X = 0xde;							// LPF_X¼Ä´æÆ÷
sfr LPF_XL = 0xde;
sfr LPF_XH = 0xdf;

sfr16 LPF_Y = 0xe6;							// LPF_Y¼Ä´æÆ÷
sfr LPF_YL = 0xe6;
sfr LPF_YH = 0xe7;

#endif
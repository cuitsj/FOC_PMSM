/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_PI.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_PI_H_
#define __FU68xx_PI_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
sfr PL_CR = 0xf9;												// PI/LPF¿ØÖÆ¼Ä´æÆ÷
#define PIRANGE3		7
#define PIRANGE2		6
#define PIRANGE1		5
#define PIRANGE0		4
#define PISTA		    1
#define LPFSTA			0

sfr16 PI_EK = 0xea;											// PI_EK¼Ä´æÆ÷
sfr PI_EKL = 0xea;
sfr PI_EKH = 0xeb;

sfr16 PI_UK = 0xec;											// PI_UK¼Ä´æÆ÷
sfr PI_UKL = 0xec;
sfr PI_UKH = 0xed;

sfr16 PI_KP = 0xee;											// PI_KP¼Ä´æÆ÷
sfr PI_KPL = 0xee;
sfr PI_KPH = 0xef;

sfr16 PI_KI = 0xf2;											// PI_KI¼Ä´æÆ÷
sfr PI_KIL = 0xf2;
sfr PI_KIH = 0xf3;

sfr16 PI_UKMAX = 0xf4;									// PI_UKMAX¼Ä´æÆ÷
sfr PI_UKMAXL = 0xf4;
sfr PI_UKMAXH = 0xf5;

sfr16 PI_UKMIN = 0xf6;									// PI_UKMIN¼Ä´æÆ÷
sfr PI_UKMINL = 0xf6;
sfr PI_UKMINH = 0xf7;


#endif
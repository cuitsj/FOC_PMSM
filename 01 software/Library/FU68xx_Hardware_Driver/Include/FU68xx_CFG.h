/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_CFG.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_CFG_H_
#define __FU68xx_CFG_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)


#define PreDriver      0
#define GateDriver     1

/* Register Define ------------------------------------------------------------------------------*/
sfr CFGKEY = 0x86;

#define BCFG0 XBYTE[0x4000]
#define BCFG1 XBYTE[0x4001]
#define BCFG2 XBYTE[0x4002]
#define BCFG3 XBYTE[0x4003]
#define BCFG4 XBYTE[0x4004]
#define DRVID 	 	7  
#define FOCID	 		2
#define XDID	 		1
#define UCID	 		0

#define BCFG5 XBYTE[0x4005]
#define BCFG6 XBYTE[0x4006]
#define BCFG7 XBYTE[0x4007]
#define BCFG8 XBYTE[0x4008]
#define BCFG9 XBYTE[0x4009]
#define BCFG10 XBYTE[0x400a]
#define BCFG11 XBYTE[0x400b]
#define BCFG12 XBYTE[0x400c]
#define BCFG13 XBYTE[0x400d]
#define BCFG14 XBYTE[0x400e]
#define BCFG15 XBYTE[0x400f]
#define BCFG16 XBYTE[0x4010]
#define BCFG17 XBYTE[0x4011]
#define BCFG18 XBYTE[0x4012]
#define BCFG19 XBYTE[0x4013]
#define BCFG20 XBYTE[0x4014]
#define BCFG21 XBYTE[0x4015]
#define BCFG22 XBYTE[0x4016]
#define BCFG23 XBYTE[0x4017]

#define CCFG7 XBYTE[0x4018]
#define CCFG6 XBYTE[0x4019]
#define CCFG5 XBYTE[0x401a]
#define CCFG4 XBYTE[0x401b]

#define CCFG3 XBYTE[0x401c]
#define MCDEN			7
#define SPD2      6
#define XTCURCFG1	5
#define XTCURCFG0	4
#define XTGFCFG1	3
#define XTGFCFG0	2
#define ECMOD			1

#define CCFG2 XBYTE[0x401d]
#define LVRSEL1		7
#define LVRSEL0		6
#define WDTBTEN		5
#define WDTRSTEN	4
#define EOSRSTEN	3
#define EOSGATEN	2
#define LVWSEL1		1
#define LVWSEL0		0

#define CCFG1 XBYTE[0x401e]
#define LVDENB		7
#define LVWIE			6
#define WDTEN			5
#define CKMOD			0

#define CCFG0 XBYTE[0x401f]

#define TEST XBYTE[0x4020]



/*-------------------------------------------------------------------------------------------------
	Function Name :	void DriverModeConfig(bit Mode)	
	Description   :	IC驱动模式选择，FU6831驱动模式为GateDriver，FU6811驱动模式为PreDriver
	Input         :	Mode--驱动模式选择，GateDriver或PreDriver
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void DriverModeConfig(bit Mode);	

#endif

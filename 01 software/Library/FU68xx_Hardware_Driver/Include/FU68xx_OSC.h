/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_OSC.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_OSC_H_
#define __FU68xx_OSC_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr OSC_CFG = 0xf1;
#define EFCKEN 		  7
#define IFCKEN			6
#define CKFLAG			5
#define ESOSCAE			4
#define ESOSCEN			3


#define Crystal     0
#define EClock			1
#define InternalFastClock    0
#define ExternalFastClock    1	

/*-------------------------------------------------------------------------------------------------
	Function Name :	ExternalFastClockConfig(Mode, Status)
	Description   :	外部快时钟配置，选择外部快时钟输入模式，强制使能外部快时钟晶振电路，
									切换系统时钟为外部快时钟
	Input         :	Mode--外部快时钟输入模式，Crystal(无源晶体）或EClock(外部时钟脉冲)
                  Statue--使能控制，Disable或Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void ExternalFastClockConfig(bit Mode, bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	InternalFastClockConfig(Status)
	Description   :	内部快时钟配置，强制使能内部快时钟晶振电路，切换系统时钟为内部快时钟
	Input         :	Statue--使能控制，Disable或Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void InternalFastClockConfig(bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	SystemFastClockConfig(Mode)
	Description   :	系统快时钟在线切换，外部快时钟默认为无源晶体模式。
	Input         :	Mode--系统快时钟模式，InternalFastClock或ExternalFastClock
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void SystemFastClockConfig(bit Mode);


/*-------------------------------------------------------------------------------------------------
	Function Name :	ExternalSlowClockConfig(Status)
	Description   :	外部慢时钟配置，使能外部慢时钟晶振电路
	Input         :	Statue--使能控制，Disable或Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void ExternalSlowClockConfig(bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	SystemFastClockMCDConfig(Status)
	Description   :	系统快时钟丢失侦测配置
	Input         :	Statue--使能控制，Disable或Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void SystemFastClockMCDConfig(bit Status); 


/*-------------------------------------------------------------------------------------------------
	Function Name :	SystemFastClockSPD2Config(Status)
	Description   :	系统快时钟双速模式配置
	Input         :	Statue--使能控制，Disable或Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void SystemFastClockSPD2Config(bit Status);





#endif
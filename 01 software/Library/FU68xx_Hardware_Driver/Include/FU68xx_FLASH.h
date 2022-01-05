/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_FLASH.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_FLASH_H_
#define __FU68xx_FLASH_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr FLKEY = 0x84;

sfr PSCTL = 0x85;
#define FLAVIO			4
#define	FLAACT			3
#define FLAERS			1
#define FLAWEN			0


/*-------------------------------------------------------------------------------------------------
	Function Name :	uint8 Flash_Sector_Erase(uint8 xdata *FlashAddress)
	Description   :	扇区自擦除: 指定将要擦除的Flash扇区，每个扇区128Byte，共128个扇区，
									扇区0~127对应Flash地址0x0000~0x3fff，通过指定Flash地址来指定要擦除
									的Flash地址所在扇区。一次只能擦除一个扇区，自擦除数据为任意值，一定
									要在解锁后才给DPTR赋值。
	Input         :	FlashAddress--Flash自擦除扇区内任意地址
  Output				:	0--Flash自擦除成功，1--Flash自擦除失败
-------------------------------------------------------------------------------------------------*/
extern uint8 Flash_Sector_Erase(uint8 xdata *FlashAddress);

/*-------------------------------------------------------------------------------------------------
	Function Name :	uint8 Flash_Sector_Write(uint8 xdata *FlashAddress, uint8 FlashData)
	Description   :	Flash自烧写: 对扇区预编程和自擦除后，可以对扇区内的地址进行Flash烧写，
                  一次烧写一个byte,一定要在解锁后才给DPTR赋值
	Input         :	FlashAddress--Flash烧写地址
									FlashData--Flash烧写数据
  Output				:	0--Flash自烧写成功，1--Flash自烧写失败	
-------------------------------------------------------------------------------------------------*/
extern uint8 Flash_Sector_Write(uint8 xdata *FlashAddress, uint8 FlashData);

#endif

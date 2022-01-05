/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : KeyScan.h
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __KEYSCAN_H_
#define __KEYSCAN_H_

#define Key0 	          FRPin	
#define Key1 	          BrakePin

/* Exported types -------------------------------------------------------------------------------*/
typedef struct
{
	uint8 Key0Value;
	uint8 Key0TempValue;
	uint8 FlagKey0Press;
	int16 Key0PressCnt;
	int16 Key0InitCnt;
	
	uint8 Key1Value;
	uint8 Key1TempValue;
	uint8 FlagKey1Press;
	int16 Key1PressCnt;
	int16 Key1InitCnt;
	
} KeyScanParam_TypeDef;

/* Exported variables ---------------------------------------------------------------------------*/
extern KeyScanParam_TypeDef KS;

/* Exported functions ---------------------------------------------------------------------------*/
extern void KeyInit(void);
extern uint8 KeyValue(void);
extern void KeyScan(void);

#endif


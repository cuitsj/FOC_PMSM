/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : MDU.h
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __MDU_H_
#define __MDU_H_


/* Exported functions ---------------------------------------------------------------------------*/
extern void MDUAPP(void);
extern void MDUControlInit(void);
//extern uint16 MDU_MUL_U16(uint16 TData0, uint16 TData1);
//extern uint32 MDU_MUL_U32(uint16 TData0, uint16 TData1);
//extern int16 MDU_MUL_S16(int16 TData0, int16 TData1);
//extern int32 MDU_MUL_S32(int16 TData0, int16 TData1);
//extern uint16 MDU_MULA_U16(uint16 TData0, uint16 TData1, uint8 ALIGN);
//extern uint32 MDU_MULA_U32(uint16 TData0, uint16 TData1, uint8 ALIGN);
//extern int16 MDU_MULA_S16(int16 TData0, int16 TData1, uint8 ALIGN);
//extern int32 MDU_MULA_S32(int16 TData0, int16 TData1, uint8 ALIGN);
extern uint16 MDU_DIV_U16(uint16 xdata *TData0, uint16 xdata *TData1);
//extern uint32 MDU_DIV_U32(uint8 xdata *TData0, uint8 xdata *TData1);

#endif
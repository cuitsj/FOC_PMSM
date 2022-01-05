/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : GPIOInit.h
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __GPIOINIT_H_
#define __GPIOINIT_H_


//#define ONOFFPin				GP02
//#define FRPin						GP03


#define SPIncPin        GP00
#define SPDecPin        GP02


#define LEDPin			 		GP36
#define SetLEDPin				{GP36 = 1;}
#define ResetLEDPin			{GP36 = 0;}
#define LEDPinONOFF			{GP36 = ~GP36;}




#define FGPin 				  GP01
#define SetFGPin			  {GP01 = 1;}
#define ResetFGPin		  {GP01 = 0;}
#define FGPinONOFF      {GP01 = ~GP01;}
																				
/* Exported functions ---------------------------------------------------------------------------*/
extern void GPIO_Init(void);

#endif
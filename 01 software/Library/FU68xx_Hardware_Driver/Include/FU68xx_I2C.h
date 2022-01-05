/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_I2C.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_I2C_H_
#define __FU68xx_I2C_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
#define I2C_MOD XBYTE[0x4028]								// I2CÅäÖÃ¼Ä´æÆ÷
#define I2CEN				7			
#define I2CMS				6				
#define SPEED1			2		
#define SPEED0			1		
#define I2CIE			  0		

#define I2C_ID XBYTE[0x4029]	              // I2CµØÖ·¼Ä´æÆ÷							
#define ADDR6				7		
#define ADDR5				6		
#define ADDR4				5		
#define ADDR3				4		
#define	ADDR2				3		
#define ADDR1				2		
#define ADDR0				1		
#define GC					0	

#define I2C_DAT XBYTE[0x402A]               // I2CÊý¾Ý¼Ä´æÆ÷

#define I2C_STA XBYTE[0x402B]	              // I2CÅäÖÃ¼Ä´æÆ÷
#define I2CBSY			7
#define DMOD				6		
#define STA					4		
#define STP					3
#define STR					2			
#define NACK				1				
#define I2CIF 			0		

#endif

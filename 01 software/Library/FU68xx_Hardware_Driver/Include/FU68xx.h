/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_H_
#define __FU68xx_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>						//absacc.h包含直接访问8051不同区域存储器的宏
#include <intrins.h>					//intrins.h包含空操作，左右位移等内嵌代码
#include <math.h>							//math.h包含算术运算的数学函数
#include <stdlib.h>						//stdlib.h包含数据类型装换、产生随机数和存储器定位函数
// #include <string.h>						//string.h包含字符串和缓存操作函数
// #include <stdio.h>						//stdio.h包含流输入输出的函数原型
// #include <ctype.h>						//ctype.h包含ASCII字符的分类和转换函数
// #include <setjmp.h>						//setjmp.h包含setjump和longjump程序的jump_buf类型
// #include <stdarg.h>						//stdarg.h包含访问函数参数的宏

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/*常用宏定义-------------------------------------------------------------------------------------*/
#define Q12(n)		(uint32)(n*4095.0)
#define Q15(n)		(uint32)(n*32767.0)
#define Q16(n)		(uint32)(n*65535.0)
#define Enable		1
#define Disable		0
#define True			1
#define False			0

/*自定义数据类型简称-----------------------------------------------------------------------------*/
typedef		signed char 			int8;
typedef   signed int 				int16;
typedef   signed long 			int32;
typedef 	unsigned char 		uint8;
typedef 	unsigned int 			uint16;
typedef 	unsigned long 		uint32;


/*FTC6801设备寄存器------------------------------------------------------------------------------*/
#include <FU68xx_SYSTEM.h>
#include <FU68xx_POWER.h>
#include <FU68xx_CFG.h>
#include <FU68xx_INT.h>
#include <FU68xx_FLASH.h>
#include <FU68xx_OSC.h>
#include <FU68xx_WDT.h>
#include <FU68xx_CRC.h>
#include <FU68xx_RTC.h>
#include <FU68xx_GPIO.h>
#include <FU68xx_TIMER.h>
#include <FU68xx_I2C.h>
#include <FU68xx_SPI.h>
#include <FU68xx_UART.h>
#include <FU68xx_ADC.h>
#include <FU68xx_CMP.h>
#include <FU68xx_AMP.h>
#include <FU68xx_MDU.h>
#include <FU68xx_SVPWM.h>
#include <FU68xx_FOC.h>
#include <FU68xx_PI.h>
#include <FU68xx_LPF.h>
#include <FU68xx_SINCOS.h>


#endif


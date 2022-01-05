/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_POWER.h
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_POWER_H_
#define __FU68xx_POWER_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)


/* Register Define ------------------------------------------------------------------------------*/
sfr LVSR = 0xdb;
#define LVWF				1
#define LVWIF				0

#define VREF_CR XBYTE[0x404f]
#define VREFVSEL1		7
#define VREFVSEL0		6
#define VREFISEL		5
#define VREFEN			4
#define VHALFM			1
#define VHALFEN			0

#define VDD2_8      0
#define VDD3_0			1
#define VDD3_5			2
#define VDD3_8			3

#define VDD7_0      0
#define VDD8_0			1
#define VDD9_0			2
#define VDD10_0			3		

#define VREF3_0     0
#define VREF4_0			1
#define VREF4_5			2
#define VREF5_0			3	

#define VDD5_Half   0
#define VREF_Half		1		


/*-------------------------------------------------------------------------------------------------
	Function Name :	LVDPowerDownDetectConfig(Status)
	Description   :	掉电检测使能，检测到VCC电压低于设定值后触发预警或者VDD5电压低于设定
	                值后触发复位。
	Input         :	Statue--使能控制，1--Disable或0--Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void LVDPowerDownDetectConfig(bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	LVDPowerDownWarnIEConfig(Status)
	Description   :	掉电低压预警中断使能，检测到VCC电压低于设定值后触发预警，如果低压预
                  警中断则响应中断。
	Input         :	Statue--使能控制，Disable或Enable	
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void LVDPowerDownWarnIEConfig(bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	LVRPowerDownResetConfig(Value)
	Description   :	掉电复位值配置，检测到VDD5电压低于设定值后触发低电压复位
	Input         :	Value--VDD5低电压复位值
                  0: VDD2_8, VDD5低电压复位值为2.8V
									1: VDD3_0, VDD5低电压复位值为3.0V
									2: VDD3_5, VDD5低电压复位值为3.5V
									3: VDD3_8, VDD5低电压复位值为3.8V															
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void LVRPowerDownResetConfig(uint8 Value);


/*-------------------------------------------------------------------------------------------------
	Function Name :	LVWPowerDownWarnConfig(Value) 
	Description   :	掉电预警值配置，检测到VCC电压低于设定值后触发低电压预警
	Input         :	Value--VCC低电压预警值
                  0: VDD7_0, VCC低电压预警值为7.0V
									1: VDD8_0, VCC低电压预警值为8.0V
									2: VDD9_0, VCC低电压预警值为9.0V
									3: VDD10_0, VCC低电压预警值为10.0V															
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void LVWPowerDownWarnConfig(uint8 Value);


/*-------------------------------------------------------------------------------------------------
	Function Name :	VREFConfig(Value, Status) 
	Description   :	参考电压VREF配置，如果使能VREF模块，则内部产生VREF输入到ADC作为ADC
									参考电压，如果禁止VREF模块，则由外部提供VREF输入到ADC作为ADC参考电压。
	Input         :	Value--VCC低电压预警值
                  0: VREF3_0, 参考电压VREF输出为3.0V
									1: VREF4_0, 参考电压VREF输出为4.0V
									2: VREF4_5, 参考电压VREF输出为4.5V
									3: VREF5_0, 参考电压VREF输出为5.0V	
                  Statue--使能控制，Disable或Enable。 																								
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void VREFConfig(uint8 Value, bit Status);


/*-------------------------------------------------------------------------------------------------
	Function Name :	VHALFConfig(Mode, Status)
	Description   :	基准电压VHALF配置，使能VHALF工作模块，选择VREF/2或者VDD5/2作为基准
									电压VHALF输出，VHALF可用作运放基准电压。
	Input         :	Mode--VHALF输出选择，VREF_Half或者VDD5_Half。
                  Statue--使能控制，Disable或Enable。 																								
  Output				:	None
-------------------------------------------------------------------------------------------------*/
extern void VHALFConfig(bit Mode, bit Status);




#endif

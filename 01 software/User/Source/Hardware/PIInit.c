
/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : PIInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains CMP initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

void PI_Init(void)
{
	PI_KP = SKP;		// 速度环 比例系数KP							
	PI_KI = SKI;		// 速度环 积分系数KI		
	
	PI_EK = 0;			// 速度环 输入误差
	PL_CR |= 0x02;		// Start PI		启动PI模块
	PI_UKMAX = SOUTMAX; // 速度环 输出 最大限幅
	PI_UKMIN = SOUTMIN; // 速度环 输出 最小限幅
	_nop_(); _nop_(); _nop_(); _nop_();	
	PI_UK =   0;		// 输出电压清0
	PL_CR &= 0xFD;		// stop PI		停止PI模块
}
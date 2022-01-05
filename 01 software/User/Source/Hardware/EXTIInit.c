/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : EXTIInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains EXTI function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>


/*-------------------------------------------------------------------------------------------------
	Function Name :	void EXTI_Init(void)
	Description   :	外部中断配置
	Input         :	无
	Output				:	无
-------------------------------------------------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EXTERN_Init
* Description    : P10
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI_Init(void)
{
	SetBit(P0_OE, P00, 0);  // config P00 as input，配置P0.0为硬件过流FO端口
	SetBit(P0_PU, P00, 1);  // 0:disable pull up, 1:enable P10 pull up	

	IF0	 = 0;								// clear P00 interrupt flag

	IT01 = 0;
	IT00 = 1;								// 00: rising edge interrupt，01: failing edge interrupt，1x: edge-change mode interrupt

	PX01 = 1;
	PX00 = 1;								// 中断优先级别3，中断优先级最高
		
	EX0 = 1;								// 使能外部中断0, POO位外部中断输入硬件
	EA = 1;									// 使能全局中断		
	
}
void Sleepmode_Init(void)
{
	SetBit(P1_OE, P11, 0);  // config P11 as input
	SetBit(P1_PU, P11, 0);  // diasable P10 Pull Up
	SetBit(P1_AN, P11, 0);  // diasable P11 analog function
	CLR(P1IF, P11);         // clear P11 interrupt flag	
	SetBit(P1IE, P11, 0);   // config P11 as the source of EXTI1
		
	IT11 = 0;
	IT10 = 0;								// 00: posedge mode interrupt，01: negedge mode interrupt，1x: edge-change mode interrupt

	PX11 = 0;
	PX10 = 0;								// 中断优先级别0，中断优先级最高
		
	EX1 = 1;								// 使能外部中断1, P11位外部中断输入硬件
	EA = 1;									// 使能全局中断		
}

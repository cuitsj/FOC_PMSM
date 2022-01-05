/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : AMPInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains AMP initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>


/*-------------------------------------------------------------------------------------------------
	Function Name :	void AMP_VHALF_Init(void)
	Description   :	AMP VHALF电压选择设置
	Input         :	无
	Output				:	无
-------------------------------------------------------------------------------------------------*/
void AMP_VHALF_Init(void)
{
	/*AMP VHALF 电压源选择： 0: VDD/2; 1:VREF/2*/
	SetBit(VREF_CR, VHALFM, 1);	
	SetBit(VREF_CR, VHALFEN, Enable);												// 使能VHALF
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void AMP_Iint(void)
	Description   :	AMP初始化配置,配置运放的电流采样正向输入，反向输入和输出，包括I_BUS,I_U,I_V
	Input         :	无
	Output				:	无
-------------------------------------------------------------------------------------------------*/
void AMP_Iint(void)
{		
  /*AMP管脚配置*/
 	SetBit(P2_AN, P27, 1);	
	SetBit(P3_AN, P30, 1);
	SetBit(P3_AN, P31, 1);                                  // AMP0--I_BUS
	
	SetBit(P1_AN, P16, 1);	
 	SetBit(P1_AN, P17, 1);
 	SetBit(P2_AN, P20, 1);                                	// AMP1--I_U

	SetBit(P2_AN, P21, 1);	
 	SetBit(P2_AN, P22, 1);
 	SetBit(P2_AN, P23, 1);                                	// AMP2--I_V

  /*运放使能*/
	SetBit(AMP_CR, AMP0EN, 1);  
 	SetBit(AMP_CR, AMP1EN, 1);	
 	SetBit(AMP_CR, AMP2EN, 1);	
	
}
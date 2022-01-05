/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : SVPWM.c
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains SVPWM control functionused for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>


/*-------------------------------------------------------------------------------------------------
	Function Name :	void SVPWM_Init(void)
	Description   :	SVPWM模块初始化
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void SVPWM_Init(void)
{
	SV_US = 0;								                               // SV_US:uint16,Q15(0~32767)
	SV_ANGLE = 0;	
	SV_PWMPRD = PWMARR;
	SetBit(SV_CR, SVSEGMT, 0);			// 0：7 segment，1：5 segment
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void SVPWM(void)
	Description   :	SVPWM控制
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void SVPWM(void)
{	
	if(MC.FR == CW)
	{	
		MC.Angle += MC.Theta;	
		if(MC.Angle > 65535)
		{
			MC.Angle -= 65536;
		}	
	}
	else if(MC.FR == CCW)
	{
		if(MC.Angle < MC.Theta)
		{
			MC.Angle += 65535;
		}	
		MC.Angle -= MC.Theta;	
	}
	
	SV_ANGLE = (uint16)MC.Angle;
	SV_US = MC.PWMDutyCtl.VS;

	SetBit(SV_CR, SVSTA, 1);
	
}

/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : DriverInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains driver initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>


/*-------------------------------------------------------------------------------------------------
	Function Name :	void Driver_Init(void)
	Description   :	Driver初始化配置
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void Driver_Init(void)
{
	uint32 PRDRVCnt;
	
/*-------------------------------------------------------------------------------------------------
	DRV_CTL：PWM来源选择
	OCS1 = 0, OCS1 = X: TIM0 PWM
	OCS1 = 1, OCS1 = 0: TIM1 PWM
	OCS1 = 1, OCS1 = 1: FOC 6 PWM
-------------------------------------------------------------------------------------------------*/
	/*FOC 6 PWM output*/
	SetBit(DRV_CTL, OCS1, 1);			                        	// FOC 6 PWM output
	SetBit(DRV_CTL, OCS0, 1);			                        	// Predriver使能位
	
  /*Predriver & Gatedriver模式选择配置*/	
	#if (DriverMode == Driver_3P3N)
	{
		SetBit(DRV_CTL, PRDRVEN, 1);			                   	// Predriver使能位
		for(PRDRVCnt=0;PRDRVCnt<24000;PRDRVCnt++);
	}
	#elif (DriverMode == Driver_6N)
	{
		SetBit(DRV_CTL, PRDRVEN, 0);			                    // 禁止predrive driver使能位		
		#if (PWM_Level_Mode == High_Level)										
		{ 
	    DRV_OUT=0x00;			                                  // 关闭MOE，设置空闲电平为低电平
		}
	 #elif (PWM_Level_Mode == Low_Level)										
		{
			DRV_OUT=0x3F;                                       // 关闭MOE，设置空闲电平为高电平
		}
	 #endif
	}
	#endif	
	SetBit(DRV_CTL, DRVOE, 1);															// gata driver使能位
}



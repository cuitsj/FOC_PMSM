/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : MDU.c
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the MDU  used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ----------------------------------------------------------------------------*/
MDUControl_TypeDef xdata MDUControl;


/*-------------------------------------------------------------------------------------------------
	Function Name :	void MDU_Init(void)
	Description   :	MDU模块初始化，乘法结果移位，乘法模式
	Input         :	None
  Output				:	None
-------------------------------------------------------------------------------------------------*/
void MDU_Init(void)
{
/*-------------------------------------------------------------------------------------------------
	乘法器初始化
  00:乘法结果右移0位	01:乘法结果右移8位	10:乘法结果右移12位	  11:乘法结果右移15位
-------------------------------------------------------------------------------------------------*/
	SetBit(MD_MODE, ALIGN1, 1);
	SetBit(MD_MODE, ALIGN0, 1);				                       // 00:乘法结果右移0位	
	
	SetBit(MD_MODE, MDSN, 0);						                     // 选择无符号乘法
	
	MDUControlInit();
}


/*-------------------------------------------------------------------------------------------------
	Function Name :	uint16 MDU_MUL_U16(uint16 TData0, uint16 TData1)
	Description   :	16位无符号乘法，返回16位无符号值，乘积大于65535计算溢出，运行时间
									1.2us，函数在调用过程中如果被中断打断则会重复调用
	Input         :	TData0-乘数0，TData1-乘数1
  Output				:	MD_MB-乘积低16位
-------------------------------------------------------------------------------------------------*/
uint16 MDU_MUL_U16(uint16 TData0, uint16 TData1)
{
	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:乘法结果右移0位						
	SetBit(MD_MODE, MDSN, 0);						                   // 选择无符号乘法

	MD_MA = TData0;
	MD_MB = TData1;								                         // 写被乘数和乘数

	return MD_MB;	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	uint32 MDU_MUL_U32(uint16 TData0, uint16 TData1)
	Description   :	16位无符号乘法，返回32位无符号值，运行时间10us，函数在调用过程中如果
                  被中断打断则会重复调用
	Input         :	TData0-乘数0，TData1-乘数1
  Output				:	RData32-32位乘积
-------------------------------------------------------------------------------------------------*/
uint32 MDU_MUL_U32(uint16 TData0, uint16 TData1)
{
	uint32 RData32;
	uint16 *PData16; 
	
	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:乘法结果右移0位				
	SetBit(MD_MODE, MDSN, 0);							                 // 选择无符号乘法

	MD_MA = TData0;
	MD_MB = TData1;								                         // 写被乘数和乘数
	
	PData16 = &RData32;
	*PData16 = MD_MA;
	PData16 += 1;
	*PData16 = MD_MB;

	return RData32;
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 MDU_MUL_U16(int16 TData0, int16 TData1)
	Description   :	16位有符号乘法，返回16位有符号值，乘积大于32767计算溢出,运行时间
									1.2us，函数在调用过程中如果被中断打断则会重复调用
	Input         :	TData0-乘数0，TData1-乘数1
  Output				:	MD_MB-乘积低16位
-------------------------------------------------------------------------------------------------*/
int16 MDU_MUL_S16(int16 TData0, int16 TData1)
{
	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:乘法结果右移0位				
	SetBit(MD_MODE, MDSN, 1);						                   // 选择有符号乘法

	MD_MA = TData0;
	MD_MB = TData1;								                         // 写被乘数和乘数
	
	return MD_MB;
}

/*-------------------------------------------------------------------------------------------------
	Function Name : int32 MDU_MUL_S32(int16 TData0, int16 TData1)
	Description   :	32有符号乘法，返回32位有符号值，运行时间10us，函数在调用过程中如果
                  被中断打断则会重复调用
	Input         :	TData0-乘数0，TData1-乘数1
  Output				:	RData32-32位乘积
-------------------------------------------------------------------------------------------------*/
int32 MDU_MUL_S32(int16 TData0, int16 TData1)
{
	int32 RData32;
	int16 *PData16;
	
	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:乘法结果右移0位				
	SetBit(MD_MODE, MDSN, 1);						                   // 选择有符号乘法

	MD_MA = TData0;
	MD_MB = TData1;								                         // 写被乘数和乘数

	PData16 = &RData32;
	*PData16 = MD_MA;
	PData16 += 1;
	*PData16 = MD_MB;

	return RData32;
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	uint16 MDU_MULA_U16(uint16 TData0, uint16 TData1, uint8 ALIGN)
	Description   :	16位无符号乘法，返回16位无符号值，乘积大于65535计算溢出，可选择运算
									结果移位，运行时间2.5us，函数在调用过程中如果被中断打断则会重复调用
	Input         :	TData0-乘数0，TData1-乘数1
  Output				:	MD_MB-乘积低16位
-------------------------------------------------------------------------------------------------*/
uint16 MDU_MULA_U16(uint16 TData0, uint16 TData1, uint8 ALIGN)
{
	switch(ALIGN)
	{
		case 0:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 00:乘法结果右移0位		
			break;
		case 8:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 1);					               // 01:乘法结果右移8位
			break;
		case 12:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 10:乘法结果右移12位
			break;
		case 15:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 1);					               // 11:乘法结果右移15位
			break;
		default:
			break;
	}
	
	SetBit(MD_MODE, MDSN, 0);						                   // 选择无符号乘法

	MD_MA = TData0;
	MD_MB = TData1;								                         // 写被乘数和乘数

	return MD_MB;
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	uint32 MDU_MULA_U32(uint16 TData0, uint16 TData1, uint8 ALIGN)
	Description   :	16位无符号乘法，返回32位无符号值，可选择运算结果移位，运行时间10us，
									函数在调用过程中如果被中断打断则会重复调用
	Input         :	TData0-乘数0，TData1-乘数1
  Output				:	RData32-32位乘积
-------------------------------------------------------------------------------------------------*/
uint32 MDU_MULA_U32(uint16 TData0, uint16 TData1, uint8 ALIGN)
{
	uint32 RData32;
	uint16 *PData16; 
	
	switch(ALIGN)
	{
		case 0:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 00:乘法结果右移0位		
			break;
		case 8:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 1);					               // 01:乘法结果右移8位
			break;
		case 12:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 10:乘法结果右移12位
			break;
		case 15:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 1);					               // 11:乘法结果右移15位
			break;
		default:
			break;
	}

	SetBit(MD_MODE, MDSN, 0);							                 // 选择无符号乘法

	MD_MA = TData0;
	MD_MB = TData1;								                         // 写被乘数和乘数
	
	PData16 = &RData32;
	*PData16 = MD_MA;
	PData16 += 1;
	*PData16 = MD_MB;

	return RData32;
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 MDU_MUL_U16(int16 TData0, int16 TData1)
	Description   :	16位有符号乘法，返回16位有符号值，乘积大于32767计算溢出,运行时间
									2.5us，函数在调用过程中如果被中断打断则会重复调用
	Input         :	TData0-乘数0，TData1-乘数1
  Output				:	MD_MB-乘积低16位
-------------------------------------------------------------------------------------------------*/
int16 MDU_MULA_S16(int16 TData0, int16 TData1, uint8 ALIGN)
{
	switch(ALIGN)
	{
		case 0:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 00:乘法结果右移0位		
			break;
		case 8:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 1);					               // 01:乘法结果右移8位
			break;
		case 12:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 10:乘法结果右移12位
			break;
		case 15:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 1);					               // 11:乘法结果右移15位
			break;
		default:
			break;
	}
	
	SetBit(MD_MODE, MDSN, 1);						                   // 选择有符号乘法

	MD_MA = TData0;
	MD_MB = TData1;								                         // 写被乘数和乘数

	return MD_MB;		
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name : int32 MDU_MULA_S32(int16 TData0, int16 TData1, uint8 ALIGN)
	Description   :	32有符号乘法，返回32位有符号值，可选择运算结果移位，运行时间10us，
									函数在调用过程中如果被中断打断则会重复调用
	Input         :	TData0-乘数0，TData1-乘数1
  Output				:	RData32-32位乘积
-------------------------------------------------------------------------------------------------*/
int32 MDU_MULA_S32(int16 TData0, int16 TData1, uint8 ALIGN)
{
	int32 RData32;
	int16 *PData16;  	
		
	switch(ALIGN)
	{
		case 0:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 00:乘法结果右移0位		
			break;
		case 8:
			SetBit(MD_MODE, ALIGN1, 0);
			SetBit(MD_MODE, ALIGN0, 1);					               // 01:乘法结果右移8位
			break;
		case 12:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 0);				                 // 10:乘法结果右移12位
			break;
		case 15:
			SetBit(MD_MODE, ALIGN1, 1);
			SetBit(MD_MODE, ALIGN0, 1);					               // 11:乘法结果右移15位
			break;
		default:
			break;
	}
	
	SetBit(MD_MODE, MDSN, 1);						                   // 选择有符号乘法

	MD_MA = TData0;
	MD_MB = TData1;								                         // 写被乘数和乘数

	PData16 = &RData32;
	*PData16 = MD_MA;
	PData16 += 1;
	*PData16 = MD_MB;
	
	return RData32;
	
}


/*-------------------------------------------------------------------------------------------------
	Function Name : uint16 MDU_DIV_U16(uint16 TData0, uint16 TData1)
	Description   :	16位无符号乘法，返回16位无符号值，运行时间7.5us，函数在调用过程中如果
                  被中断打断则会重复调用
	Input         :	TData0-被除数，TData1-除数
  Output				:	MD_DAL-商低16位
-------------------------------------------------------------------------------------------------*/
uint16 MDU_DIV_U16(uint8 xdata *TData0, uint8 xdata *TData1)
{
	MD_DA3 = *TData0;
	MD_DA2 = *(TData0+1);
	MD_DA1 = *(TData0+2);
	MD_DA0 = *(TData0+3);

	MD_DB3 = *TData1;
	MD_DB2 = *(TData1+1);
	MD_DB1 = *(TData1+2);
	MD_DB0 = *(TData1+3);
							

	SetBit(MD_MODE,DIVSTART,1);
	while(!GetBit(MD_MODE,DIVDONE));			                 // 等待MDU计算完成			

  return MD_DAL;
					
}


/*-------------------------------------------------------------------------------------------------
	Function Name : uint32 MDU_DIV_U32(uint8 xdata *TData0, uint8 xdata *TData1)
	Description   :	32位无符号乘法，返回32位无符号值，运行时间10us，函数在调用过程中如果
                  被中断打断则会重复调用
	Input         :	TData0-被除数，TData1-除数
  Output				:	RData32-32位商
-------------------------------------------------------------------------------------------------*/
uint32 MDU_DIV_U32(uint8 xdata *TData0, uint8 xdata *TData1)
{
	uint32 RData32;
	uint16 *PData16;
	
	MD_DA3 = *TData0;
	MD_DA2 = *(TData0+1);
	MD_DA1 = *(TData0+2);
	MD_DA0 = *(TData0+3);

	MD_DB3 = *TData1;
	MD_DB2 = *(TData1+1);
	MD_DB1 = *(TData1+2);
	MD_DB0 = *(TData1+3);

	SetBit(MD_MODE,DIVSTART,1);
	while(!GetBit(MD_MODE,DIVDONE));			                 // 等待MDU计算完成			

	PData16 = &RData32;
	*PData16 = MD_DAH;
	PData16 += 1;
	*PData16 = MD_DAL;

  return RData32;
					
}


/*-------------------------------------------------------------------------------------------------
	Function Name : void MDUControlInit(void)
	Description   :	MDU控制函数标志位初始化
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void MDUControlInit(void)
{
	MDUControl.FlagTheta = 0;
	MDUControl.FlagAngleErrCW = 0;
	MDUControl.FlagAngleErrCCW = 0;
	
	MDUControl.FlagSpeed = 0;
	MDUControl.FlagPISpeed = 0;
	MDUControl.FlagTargetSpeed = 0;
	MDUControl.FlagPITargetSpeed = 0;
	
	MDUControl.FlagCurrentBUS = 0;                          
	MDUControl.FlagPICurrentBUS = 0;
	MDUControl.FlagPITargetCurrentBUS = 0;
	
	MDUControl.FlagPWMINHighDuty = 0;
}

/*-------------------------------------------------------------------------------------------------
	Function Name : void MDUAPP(void)
	Description   :	MDU控制函数，运行频率为PWMFre(16K)，最高优先级，软件系统的所有MDU运算都在此函数
                  执行，函数扫描到相应的标志位后进行相应的运算。
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void MDUAPP(void)
{
	uint32 Temp32;
	
  /*Angle Calculate*/
//	if(MDUControl.FlagAngleErrCW)
//	{		
//		MC.AngleErr = MDU_MULA_U16(MC.SpeedCtl.Speed, TempAngleErrKPCW, 12);
//		MDUControl.FlagAngleErrCW = 0;
//	}
//	if(MDUControl.FlagAngleErrCCW)
//	{		
//		MC.AngleErr = MDU_MULA_U16(MC.SpeedCtl.Speed, TempAngleErrKPCCW, 12);
//		MDUControl.FlagAngleErrCCW = 0;
//	}
//	if(MDUControl.FlagTheta)
//	{
//		Temp32 = TempTheta;
//		MC.Theta = MDU_DIV_U16(&Temp32, &MC.PeriodTime);			// 750000Hz/PWMFre/MC.PeriodTime*65535 
//		MDUControl.FlagTheta = 0;
//	}
	
	/*Speed Control*/
	#if (SpeedCloseLoopEnable)
	{	
		if(MDUControl.FlagSpeed)
		{
			Temp32 = TempSpeed;
			MC.SpeedCtl.Speed = MDU_DIV_U16(&Temp32, &MC.PeriodTime);//Speed=750KHz/(MC.PeriodTime*MC.Pole)*60, rpm	
			MDUControl.FlagSpeed = 0;
		}
		if(MDUControl.FlagPISpeed)
		{
			Temp32 = TempPISpeed;
			MC.SpeedCtl.PISpeed = MDU_DIV_U16(&Temp32, &MC.PeriodTime);//0~SPEEDBASE(rpm)-->0~32767
			MDUControl.FlagPISpeed = 0;
		}
		if(MDUControl.FlagTargetSpeed)
		{
			MC.SpeedCtl.TargetSpeed = MDU_MULA_U16((ADCData.VoltageSREF-TempSREFMin), TempTargetSpeed, 15) + SpeedMin;
			MDUControl.FlagTargetSpeed = 0;
		}
		if(MDUControl.FlagPITargetSpeed)
		{	
			MC.SpeedCtl.PITargetSpeed = MDU_MULA_U16(MC.SpeedCtl.TargetSpeed, TempPITargetSpeed, 8);	
			MDUControl.FlagPITargetSpeed = 0;
		}
	}
	#endif
	
	/*Current Control*/
//	#if (CurrentCloseLoopEnable)
//	{	
//		if(MDUControl.FlagCurrentBUS) 
//		{
//			MC.CurrentCtl.CurrentBUS = MDU_MULA_U16(MDU_MULA_U16(ADCData.CurrentBUS, MC.PWMDutyCtl.VS, 15), 23169, 15);// 32767/Sqr(2)=23169
//			MDUControl.FlagCurrentBUS = 0;
//		}
//		if(MDUControl.FlagPICurrentBUS)
//		{
//			MC.CurrentCtl.PICurrentBUS = MDU_MULA_U16(MC.CurrentCtl.CurrentBUS, TempCurrentBase, 8);	
//			MDUControl.FlagPICurrentBUS = 0;
//		}	
//		if(MDUControl.FlagPITargetCurrentBUS)
//		{
//			MC.CurrentCtl.PITargetCurrentBUS = MDU_MULA_U16(MC.CurrentCtl.TargetCurrentBUS, TempCurrentBase, 8);
//			MDUControl.FlagPITargetCurrentBUS = 0;
//		}
//	}
	#endif
	
	
	#if (SpeedPWMEnable)                                 
	{	
		if(MDUControl.FlagPWMINHighDuty)
		{
			Temp32 = MDU_MUL_U32(PWMINCtl.PWMINHigh, 32767);
			PWMINCtl.PWMINHighDuty = MDU_DIV_U16(&Temp32, &PWMINCtl.PWMINPeriod);	
			MDUControl.FlagPWMINHighDuty = 0;
		}
	}
	#endif	
}









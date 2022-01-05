/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : PID.h
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __PID_H_
#define __PID_H_

/* Exported types -------------------------------------------------------------------------------*/
typedef struct 
{
	int16 Kp;						                                     // 定义PID比例系数
	int16 Ki;						                                     // 定义PID积分系数
	int16 Err;						                                   // 定义PID当前偏差
	int16 Err_Last1;				                                 // 定义PID上次偏差
	int16 Err_Err;					                                 // 定义PID当前偏差和上次偏差之差，Err_Err=Err-Err_Last1
	int16 Out;						                                   // 定义PID输出
	int16 OutMax;					                                   // 定义PID输出最大溢出阀值
	int16 OutMin;					                                   // 定义PID输出最小溢出阀值
}PID_TypeDef;

typedef struct 
{
	int16 RampInc;
	int16 RampDec;
	int16 RampTarget;
	int16 RampStep;
}Ramp_TypeDef;

/* Exported variables ---------------------------------------------------------------------------*/
extern PID_TypeDef idata SpeedPID;
extern Ramp_TypeDef idata SpeedRamp;
extern PID_TypeDef idata CurrentPID;

/* Exported functions ---------------------------------------------------------------------------*/
extern int16 RampControl(Ramp_TypeDef *Ramp, int16 Ref);
extern int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur);
extern int16 PID_Control(PID_TypeDef *PID, int16 Ref, int16 Cur);

extern void PI_Init(void);
extern int16 PI_Control(PID_TypeDef *PID, int16 Ref, int16 Cur);

extern void SpeedRampInit(void);
extern void SpeedPIDInit(void);									
extern void CurrentPIDInit(void);									


#endif
/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : MotorControl.h
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __MOTORCONTROL_H_
#define __MOTORCONTROL_H_

#include <FU68xx.h>

/* Exported types -------------------------------------------------------------------------------*/




typedef enum
{
	None              = 0,
 	OverVoltage    		= 1,
	UnderVoltage   		= 2,
	OverCurrent	  		= 3,
	OverPCBTH					= 4,
	OverIPMTH					= 5,
	LockRotor					= 6,
	HallError					= 7,
	FO								= 8				
}FaultStatus_TypeDef;

typedef struct
{
  FaultStatus_TypeDef FaultStatus;
	
	uint8 FlagFO;		
	uint8 FlagOverVoltageBUS;
	uint8 FlagUnderVoltageBUS;
	uint8 FlagOverCurrentBUS;
	uint8 FlagLockRotor;
	uint8 FlagHallError;	
	uint8 FlagOverPCBTH;
	uint8 FlagOverIPMTH;
	
	uint8 Sector;
	uint8 HallStatus;
	int32 Angle;
	int32 Theta;	
  uint32 StepTime;
	uint32 PeriodTime;	

  uint16 Speed;
  uint16 TargetSpeed;
	uint16 PISpeed;
	uint16 PITargetSpeed;		
	
	float FVoltageU;				// U相端电压，CH0
	float FVoltageV;				// V相端电压，CH1
	float FVoltageW;				// W相端电压，CH2
	float FVoltageBUS;			// BUS端电压，CH3
	float FPCBTH;						// PCB温度，	CH4
	float FIPMTH;						// IPM温度，	CH5	
	float FVoltageSREF;			// SREF电压，	CH6	
	float FCurrentU;				// U相电流，  CH7
	float FCurrentV;				// V相电流，  CH8
	float FCurrentBUS;			// BUS电流，  CH9  
	
}FaultMessage_TypeDef;

typedef struct
{ 
	FaultStatus_TypeDef FaultStatus; 
	
	uint8 FlagFO;	
	uint8 FlagOverVoltageBUS;
	uint8 FlagUnderVoltageBUS;
	uint8 FlagOverCurrentBUS;
	uint8 FlagLockRotor;
	uint8 FlagHallError;
	uint8 FlagOverPCBTH;
	uint8 FlagOverIPMTH;	
	
	uint8 FlagRestartOV;
	uint8 FlagRestartUV;
	uint8 FlagRestartOC;
	uint8 FlagRestartHE;	
	uint8 FlagRestartLR;
	uint8 FlagRestartOT;
	
	uint8 RestartOCCnt;
	uint8 RestartLRCnt;

	int16 OverVoltageBUSCnt;
	int16 UnderVoltageBUSCnt;
	int16 OverCurrentBUSCnt;
	int16 LockRotorCnt;
	int16 OverPCBTHCnt;
	int16 OverIPMTHCnt;
	
}Fault_TypeDef;

typedef struct
{
  uint16 Speed;
  uint16 TargetSpeed;
	uint16 PISpeed;
	uint16 PITargetSpeed;		
	
}SpeedCtl_TypeDef;

typedef struct
{
  uint16 CurrentBUS;
  uint16 TargetCurrentBUS;
	uint16 PICurrentBUS;
	uint16 PITargetCurrentBUS;		
	
}CurrentCtl_TypeDef;

typedef struct
{
	uint16 VS;
	uint16 StartVS;
	uint16 TargetVS;	
	uint16 PWMDuty;
	
}PWMDutyCtl_TypeDef;

typedef struct
{
	uint8 FlagStop;
	uint8 FlagInit;
	uint8 FlagReady;
	uint8 FlagPrecharge;
  uint8 FlagAlignment;
	uint8 FlagStart;	
	uint8 FlagNormal;
	uint8 FlagFault;
	uint32 StopCnt;
	uint32 InitCnt;
	uint32 ReadyCnt;	
	uint32 PrechargeCnt;
	uint32 AlignmentCnt;
	uint32 StartCnt;		
	uint32 NormalCnt;	
	uint32 FaultCnt;	
	
}StauteCtl_TypeDef;

typedef struct
{
	uint8 FR;
	uint8 FlagFR;
	uint8 Step;	
	uint8 Sector;
	uint8 HallStatus;
	uint8 StopMode;	
	uint8 OpenLoopCnt;
	uint8	FlagFirstLedge;	
	uint8 FlagSystemFault;

	int32 Theta;
	int32 Angle;
	int32 AngleErr;
  uint32 StepTime;
	uint32 PeriodTime;

//	MotorStatus_TypeDef 			MotorStatus;
	StauteCtl_TypeDef         StauteCtl;	
	PWMDutyCtl_TypeDef 				PWMDutyCtl;	
	SpeedCtl_TypeDef 					SpeedCtl;
	CurrentCtl_TypeDef        CurrentCtl;
	Fault_TypeDef 						Fault;
	FaultMessage_TypeDef 			FaultMessage;
	
} MotorParam_TypeDef;


/* Exported variables ---------------------------------------------------------------------------*/

extern MotorParam_TypeDef xdata MC;
extern uint16 idata Hall_Angle_Arr[6];

/* Exported functions ---------------------------------------------------------------------------*/
extern void MotorMain(void);
extern void MotorStop(void);
extern void MotorInit(void);
extern void MotorReady(void);
extern void MotorPrecharge(void);
extern void MotorAlignment(void);
extern void MotorStart(void);
extern void MotorNormal(void);
extern void MotorFault(void);
extern void ParameterInit(void);
extern void FaultDetect(void);
extern void FaultMessageRecord(void);
extern void RestartControl(void);
extern void SpeedControl(void);
extern void CurrentControl(void);
extern void FRControl(void);
extern uint8 CheckHallStatus(void);
extern void ONOFFControl(void);
extern void SystemTick(void);
extern void LEDControl(void);

extern void SpeedDetect(void);
extern void LockRotorDetect(void);



#endif

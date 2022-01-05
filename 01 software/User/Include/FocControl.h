/**
  ******************************************************************************
  * @file    FocControl.h 
  * @author  Fortior Application Team
  * @version V1.0.0
  * @date    05/06/2014
  * @brief   define motor contorl parameter
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __FocControl_H_
#define __FocControl_H_


/* Exported types -------------------------------------------------------------------------------*/

typedef enum
{
		mcInit 			= 0,
		mcCharge 		= 1,
		mcTailWind 	= 2,
		mcAlign 		= 3,
		mcStart 		= 4,
		mcRun 			= 5,	
		mcStop 			= 6,
		mcFault 		= 7,
	  mcwait      = 8,
	  mcPosiCheck = 9
}MotStaType;

typedef struct
{
	uint16  TailWind_Count;
	uint16  Charge_Count;
	uint16  Break_Count;
	uint16	Count;
	int32   Stop_Count;
	
		uint16	MotorRun_Count;
	uint8 MODE_Flag;
	uint8 START_Flag;
}TimeCnt;

typedef struct
{
	uint8 	ChargeOneTime;
	uint8 	AlginOneTime;	
	uint8 	TailWindOneTime;	
	uint8   OpenOneTime;
	uint8   PosiCheckOneTime;
	uint8	  mcSecAlignFlag;
}MotStaTim;

typedef struct
{
	int16 	IaOffset;
	int32   IaOffsetSum;
	int16 	IbOffset;
	int32   IbOffsetSum;
	int16 	IbusOffset;				//母线电流采样上的偏置电压
  int32   IbusOffsetSum;
	int16   OffsetCount;
	int16   OffsetTimes;
	int16   OffsetFlag;
}CurrentOffset;

/* Exported variables ---------------------------------------------------------------------------*/
extern MotStaType mcState;
extern TimeCnt Time;	
extern MotStaTim  MotorStateTime;
extern CurrentOffset mcCurOffset;
/* Exported functions ---------------------------------------------------------------------------*/
extern void MC_Control(void);
extern void MotorcontrolInit(void);
extern void McTailWindDealwith(void);
extern void GetCurrentOffset(void);
extern void TailWindDealwith(void);

#endif

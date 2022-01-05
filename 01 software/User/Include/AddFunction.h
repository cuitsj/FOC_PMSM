/**
  ******************************************************************************
  * @file    TypeDef.h 
  * @author  Fortior Application Team
  * @version V1.0.0
  * @date    05/06/2014
  * @brief   define motor contorl parameter
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __AddFuntion_H_
#define __AddFuntion_H_

/* Exported types -------------------------------------------------------------------------------*/

typedef struct 
{
//voltage protect
	uint16 			mcOverVoltDetecFaultCount;
	uint16 			mcUnderVoltDetecFaultCount;
	uint16      mcVoltRecoverCount;	

//Current protect
	uint16	    OverCurrentValue;	
	uint8				OverCurCnt;
	uint16      currenttime;
	uint16      CurrentRecoverCnt;


//stall protect
	uint16      mcStallCount;	
	uint16      mcStallRecover;
	uint16      mcStallTimes;	
	uint16      mcStallDelaDectEs;//method 1
	uint16  		mcStallDeSpeed;//method 3
	uint16     	mcStallDelaDectSpd;//method 3	
	uint16    	mcStallDeCurrent;//method 4
	uint16      StallCurrentVale;
	uint8       StallFlag;
	uint8       StartFlag;
	uint16      StallRecoverCurrent;
	uint16 			mcEsValue;
		
//Loss Phase protect
	uint16      Abs_ia;				//absolute value of pmsm_ia;
	uint16      Abs_ib;				//absolute value of pmsm_ib;
	uint16      Abs_ic;				//absolute value of pmsm_ic;

	uint16      Max_ia;				//max value of pmsm_ia;
	uint16      Max_ib;				//max value of pmsm_ib;
	uint16      Max_ic;				//max value of pmsm_ic;

	uint16      LphaseDelayCount;
	uint16      Lphasecnt;

	uint16      LowCurrent;
	uint16      NomCurrent;

	uint16      AOpencnt ;    //A phase loss Times
	uint16      BOpencnt ;
	uint16      COpencnt ;
	uint16      mcLossPHTimes;//loss phase Times
	uint16      mcLossPHCount;//loss phase times Count

	int32       mcStartDelay;
	int32       mcStartCnt;
	int32       mcStartEsCnt;
	int16       mcStartFocmode;
	int16       mcStartSpeedCnt;
	uint16      SecondStart;
}FaultVarible;

typedef enum
{
	FaultNoSource		= 0,
	FaultSoftOVCurrent  = 1,	
	FaultHardOVCurrent	= 2,
	FaultUnderVoltage	= 4,	
	FaultOverVoltage    = 8,
	FaultLossPhase      = 10,
	FaultStall 		    = 12,
	FaultOverwind 	    = 16,	
	FaultStart       	= 18,
} FaultStateType;

typedef struct
{
	uint16      ADCSpeed;
	uint16		ADCVref;
} ADCSample;

typedef struct
{
	uint32 LedCount;
	uint8  LedTimCot;
	uint32 Counttime;
	uint32 Count;
}MCLedDisplay;
typedef struct
{
	uint16  CtrlMode;
	uint16  SpeedLoopTime;
	uint16  PowerLoopTime;
	uint16  TorqueLoopTime;
	uint16  RunCurrent;
	int16   Powerlpf;
	int16   mcSpeedFlt;
	uint16  mcDcbusFlt;                         // 母线电压滤波值
	uint16  mcIbusFlt;                         // 母线电流滤波值
	uint16  mcIqref;
	uint16  McuRef;
	uint16  mcPosCheckAngle;
	int16   McCurrentERR;
	uint16 	Vspinput;
}FOCCTRL;

typedef struct
{
	int16  TargetValue;
	int16  ActualValue;
	int16  IncValue;
	int16  DecValue;
	int16  DelayCount;
	int16  DelayPeriod;
	int8   FlagONOFF;
}MCRAMP;
typedef struct
{
	int32 PWMCount;
	int32 PWMCompare;
	int32 PWMARR;
	int32 PWMCompareOld;
	int32 PWMARROld;
	uint16 PWMFlag;
	uint16 PWMDuty;
}PWMINPUTCAL;

typedef struct
{
	uint8 FR;
	uint8 FlagFR;
	uint8 FRStatus;
	uint16 OldTargetSpeed;
} MotorFRTypeDef;

typedef struct
{
	uint8 Ibus;
	uint8 Vbus;
	uint8 VSP;
}OLEDShowData;

/* Exported variables ---------------------------------------------------------------------------*/
extern PWMINPUTCAL    xdata   mcPwmInput;
extern FOCCTRL          xdata 		mcFocCtrl;
extern FaultVarible     xdata 		mcFaultDect;
extern FaultStateType			  mcFaultSource;
extern ADCSample            AdcSampleValue;
extern MCLedDisplay         mcLedDisplay;
extern int16                segmentstate;
extern MCRAMP     xdata     mcSpeedRamp;
extern int16							  mcSpeedFdb;	
extern int16 								VSP;
extern uint32 							SpeedValue;
extern uint16               StartCurrentRef;
extern uint8                MotorSleepFlag;
extern uint16               MotorSleepDelayCout;
extern uint8                Sleep_En;
extern MotorFRTypeDef			 	mcFRState;
extern OLEDShowData			ShowData;
extern const  int16  PowerGiven[10];


extern uint16              ON_Count;
extern uint16              OFF_Count;
extern uint32              ONOFF_Times;
extern uint8               ONOFF_Flag;
extern uint16               led_t;

extern uint8 CCWstate;
extern uint16 RecEsVlue; 


extern uint16 SKI_RUN;
extern uint16 SKP_RUN;

extern uint8 NoLoad_Flag;

extern uint8 motor_FR;

extern uint8 key_trg;   //按键触发记录
extern uint8 key_cont;  //按键长按触发记录
extern uint8 keydelay_tim;   //按键长按时间记录
extern bit key_flag;    //按键按下标志位
extern uint16 beep_cnt;
extern uint8 key_data;
extern uint16 key_static;
extern uint16 keypres_static;
extern uint16 keyscanfiltertime;

/* Exported functions ---------------------------------------------------------------------------*/
extern void Fault_OverUnderVoltage(FaultVarible *h_Fault);
extern void Fault_Overcurrent(FaultVarible *h_Fault);
extern void Fault_OverCurrentRecover(FaultVarible *h_Fault);
extern void Fault_Stall(FaultVarible *h_Fault);
extern void Fault_phaseloss(FaultVarible *h_Fault);
extern void VariablesPreInit(void);
extern void Fault_Detection(void);
extern int16 KLPF_VALUE(int16 INVlaue, int16 OutLastValue);
extern void Led_OnOff(MCLedDisplay *hLedDisplay,uint8 htime);
extern void Speed_response(void);
extern void mc_ramp(MCRAMP *hSpeedramp);
extern void FGOutput(void);
extern void VSPSample(void);
extern void HW_Speed_PI(void);
extern  int16 SoftLPF(int16 Xn1, int16 Xn0, int16 K);
extern void Sleepmode(void);
extern void LED_Display(void);
extern void FaultProcess(void);
extern void SpeedRegulation(void);
extern uint16 LPF(uint16 Xn1, uint16 Xn0, uint16 K);
extern uint32 Abs_F32(int32 value);
extern uint16 Abs_F16(int16 value);

extern void ONOFF_Starttest(void);

extern void PWMInputCapture(void);

extern void led_flashing(void);
extern void key_pro(void);
extern void adddelay(unsigned int cont_t);
void key_scan(void);
/*********************顺风检测启动初始化参数******************/
typedef struct
{
	uint16 	TailWindSpeed;
	uint8		MotorDir;
	uint8		AngleState;
	uint8		MotorTailWindState;
	uint8		SpeedStoreNum;
	uint32  SpeedCountTimer;
	int16   DeltaSpeed3to2;
	int16   DeltaSpeed2to1;
	uint8   SpeedTimerClearStatus;	
	uint32	TempThailWindSpeedBase;
	uint8		SpeedOverFlowStatus;
	uint8		TimerClearTimes;
	uint8		TailWindBrakeCount;
	uint8		BrakeStartStatus;
	uint8 	AntiFlag;	
}MotorTailWindTypeDef;

#define		BaseTimerFre								(1000)			//Hz
#define		ThailWindSpeedBase					((BaseTimerFre/2)*60/Pole_Pairs)
#define		ThailWindSpeedBase_zhouqi		(BaseTimerFre*60/Pole_Pairs)
#define		TailWindStartMinSpeed		(uint16)(1000.0/TailWind_Time*60.0/Pole_Pairs)		//顺风启动的最低转速


#define	NormalTailWind 			(0)
#define	BrakeTailWind			  (1)
#define 	CW				 				(0)
#define 	CCW								(1)
#define   stop_pin          (GP06)               

extern MotorTailWindTypeDef xdata  TailWindDetect;
extern void TailWindParInit(void);
extern void TailWindSpeedDetect(void);
extern void FOCCloseLoopStart(void);


#endif
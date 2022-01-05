/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : MotorControl.c
* Author             : Fortiortech  Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the motor control function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ----------------------------------------------------------------------------*/


MotorParam_TypeDef xdata MC;
static uint32 MC_StepTime[6];
/*-------------------------------------------------------------------------------------------------
  HAA[0]:Hall_0	  		 HAA[4]:Hall_60		 			HAA[3]:Hall_120 
  HAA[5]:Hall_180 		 HAA[1]:Hall_240				HAA[2]:Hall_300
-------------------------------------------------------------------------------------------------*/
uint16 idata Hall_Angle_Arr[6] = {0,43692,54594,21846,10924,32768};		


/*-------------------------------------------------------------------------------------------------
	Function Name :	void MotorMain(void)
	Description   :	电机控制状态扫描及状态切换，包含MOTOR_STOP，MOTOR_INIT，MOTOR_READY，
                  MOTOR_PRECHARGE，MOTOR_START，MOTOR_NORMAL，MOTOR_FAULT七种状态
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/

void MotorMain(void)
{
	switch(MC.MotorStatus)
	{
		case MOTOR_STOP:						
			MotorStop();
			break;		
		case MOTOR_INIT:						
			MotorInit();
			break;
		case MOTOR_READY:
			MotorReady();
		  break;		
		case MOTOR_PRECHARGE:
			MotorPrecharge();
      break;		
		case MOTOR_START:
			MotorStart();
		  break;
		case MOTOR_NORMAL:					
			MotorNormal();
			break;
		case MOTOR_FAULT:					
			MotorFault();		
			break;
		default:
			break;
	}
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void MotorStop(void)
	Description   :	MOTOR_STOP状态控制函数，清零部分全局变量，根据StopMode控制不同类型的停机，检测
                  速度为0时转换StopMode为FreeStop
                  FreeStop--关断驱动输出自由停机
                  BrakeStop--打开下桥驱动输出刹车停机
                  SlowStop--逐步减低占空比缓慢停机
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void MotorStop(void)
{
  if(MC.StauteCtl.FlagStop == 0)
	{
		MC.Fault.RestartOCCnt = 0;
		MC.Fault.RestartLRCnt = 0;
		MC.StauteCtl.FlagStop = 1;
	}
	
  if(MC.StauteCtl.StopCnt >= 60000)
	{
		MC.StauteCtl.StopCnt = 60000;
	}
	
  if(MC.SpeedCtl.Speed == 0)
  {
		MC.StopMode = FreeStop;
  }	
  else if(MC.SpeedCtl.Speed < MotorStopSpeed)
	{						
		MC.StopMode = BrakeStop;
	}

	if(MC.StopMode == FreeStop)
	{	
		#if (DriverMode == Driver_3P3N)
		{		
			DRV_OUT = 0x15;				                               // 关断输出
		}
		#elif (DriverMode == Driver_6N)
		{
			DRV_OUT = 0x00;				                               // 关断输出
		}
		#endif
	}
	if(MC.StopMode == BrakeStop)
	{
		#if (DriverMode == Driver_3P3N)
		{		
			DRV_OUT = 0x3f;				                               // 刹车输出
		}
		#elif (DriverMode == Driver_6N)
		{
			DRV_OUT = 0x2a;				                               // 刹车输出
		}		
		#endif
	}	
	if(MC.StopMode == SlowStop)
	{
		if((MC.PWMDutyCtl.VS - MotorVSDec) >= MotorVSMin)
		{
			MC.PWMDutyCtl.VS -= MotorVSDec;
		}
		else
		{
			MC.PWMDutyCtl.VS = MotorVSMin;
		}	
	}
}
	
/*-------------------------------------------------------------------------------------------------
	Function Name : void MotorInit(void)
	Description   :	MOTOR_INIT状态控制函数，初始化变量参数，速度闭环参数初始化，电流闭环参数初始化
                  ParameterInit()--初始化电机控制变量参数
                  SpeedRampInit()--速度闭环爬坡函数变量参数初始化
                  SpeedPIDInit()--速度闭环PI函数变量参数初始化
                  CurrentPIDInit()--电流闭环PI函数变量参数初始化
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void MotorInit(void)
{
  /*电机参数初始化*/
	ParameterInit();
	
	SpeedRampInit();	
	SpeedPIDInit();
	CurrentPIDInit();
	
  MC.MotorStatus = MOTOR_READY;
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name : void MotorReady(void)
	Description   :	MOTOR_READY状态控制函数，关闭驱动输出，检测电机转速和转向，根据电机初始状态初始化
                  启动参数
                  1、电机停止
                     静态启动，状态顺序执行
                  2、电机运行，速度小于设定启动速度
                     刹车制动后再静态启动，状态顺序执行
                  3、电机正向运行，速度大于设定启动速度
                     动态启动，根据速度初始化启动占空比，直接跳转到Normal状态
                  4、电机反向运行，速度大于设定启动速度
                     发出反转控制命令，直接跳转到Normal状态进行反转控制。                
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void MotorReady(void)
{
	static uint8 MC_FR = 0;
	static uint8 FlagBrakeStart = 0;
	
  if(MC.StauteCtl.FlagReady == 0)
	{
		#if (DriverMode == Driver_3P3N)
		{		
			DRV_OUT = 0x15;				                               // 关断输出
		}
		#elif (DriverMode == Driver_6N)
		{
			DRV_OUT = 0x00;				                               // 关断输出
		}
		#endif
		MC.StauteCtl.FlagReady = 1;
	}
	
	/*转速监测*/
	if(MC.StauteCtl.ReadyCnt < MotorReadyTime)
	{
		MC_FR = CWCCWDetect();
		if(MC.SpeedCtl.Speed == 0)
		{
			if(FlagBrakeStart == 1)
			{	
				#if (DriverMode == Driver_3P3N)
				{		
					DRV_OUT = 0x15;				                           // 关断输出
				}
				#elif (DriverMode == Driver_6N)
				{
					DRV_OUT = 0x00;				                           // 关断输出
				}
				#endif
				ParameterInit();
				MC.MotorStatus = MOTOR_PRECHARGE;
				FlagBrakeStart = 0;
			}
		}	
		else if((0 < MC.SpeedCtl.Speed )&&(MC.SpeedCtl.Speed <= MotorStartSpeed))
		{	
			MC.StauteCtl.ReadyCnt = 1;
			if(FlagBrakeStart == 0)
			{
				#if (DriverMode == Driver_3P3N)
				{		
					DRV_OUT = 0x3f;				                           // 刹车输出
				}
				#elif (DriverMode == Driver_6N)
				{
					DRV_OUT = 0x2a;				                           // 刹车输出
				}		
				#endif	
				FlagBrakeStart = 1;
			}
		}				
		else 
		{
			if(FlagBrakeStart == 0)
			{		
				if(MC.FR != MC_FR)		                             // 电机初始状态反转，刹车启动
				{
					MC.FlagFR = 1;
				}
				else
				{
					MC.PWMDutyCtl.VS = MotorStartKp * MC.SpeedCtl.Speed;
					if(MC.PWMDutyCtl.VS > MotorVSMax)
					{
						MC.PWMDutyCtl.VS = MotorVSMax;
					}	
					#if (SpeedCloseLoopEnable)
					{
						SpeedPID.Out = MC.PWMDutyCtl.VS;
						SpeedRamp.RampStep = MC.SpeedCtl.PISpeed;
					}
					#elif (CurrentCloseLoopEnable)
					{
						CurrentPID.Out = MC.PWMDutyCtl.VS;
					}
					#elif ((SpeedCloseLoopEnable)&&(CurrentCloseLoopEnable))
					{	
						SpeedPID.Out = 0;
						CurrentPID.Out = MC.PWMDutyCtl.VS;						
					}
					#endif					
				}
				MOE = 1;
				MC.MotorStatus = MOTOR_NORMAL;
			}			
		}
	}
	else
	{	
		MC.MotorStatus = MOTOR_PRECHARGE;
	}
}

/*-------------------------------------------------------------------------------------------------
	Function Name : void MotorPrecharge(void)
	Description   :	MOTOR_PRECHARGE状态控制函数，驱动电路自举回路预充电，充电顺序U->V->W，充电完毕后
                  关闭驱动输出
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void MotorPrecharge(void)
{
  static uint8 FlagPrechargeU = 0;
	static uint8 FlagPrechargeV = 0;
	static uint8 FlagPrechargeW = 0;

  if(MC.StauteCtl.FlagPrecharge == 0)
	{
		#if (DriverMode == Driver_3P3N)
		{		
			DRV_OUT = 0x15;				                               // 关断输出
		}
		#elif (DriverMode == Driver_6N)
		{
			DRV_OUT = 0x00;				                               // 关断输出
		}
		#endif
    SetBit(TIM0_CCER1, CC1E, 0);                           // OC1输出使能
    SetBit(TIM0_CCER1, CC1NE, 0);                          // OC1N输出使能
    SetBit(TIM0_CCER1, CC2E, 0);                           // OC2输出使能
    SetBit(TIM0_CCER1, CC2NE, 0);                          // OC2N输出使能
    SetBit(TIM0_CCER2, CC3E, 0);                           // OC3输出使能
		SetBit(TIM0_CCER2, CC3NE, 0);                          // OC3N输出使能
		MOE = 1;
		MC.StauteCtl.FlagPrecharge = 1;
	}
	
	/*Disable UH/VH/WH, Enable UL/VL/WL*/
	if(MC.StauteCtl.PrechargeCnt < PrechargeTime)
	{
		/*Disable UH, Enable UL*/
		if(FlagPrechargeU == 0)
		{
			TIM0_CCR1 = TempPrechargeDuty;	
			SetBit(TIM0_CCER1, CC1E, 0);                  			 // OC1输出使能, disable UH
			SetBit(TIM0_CCER1, CC1NE, 1);   										 // OC1N输出使能, enable UL	
			FlagPrechargeU = 1;
		}	
  }
	else if(MC.StauteCtl.PrechargeCnt < (PrechargeTime*2))
	{
		/*Disable VH, Enable VL*/
		if(FlagPrechargeV == 0)
		{
			TIM0_CCR2 = TempPrechargeDuty;	
			SetBit(TIM0_CCER1, CC2E, 0);    										 // OC2输出使能, disable VH
			SetBit(TIM0_CCER1, CC2NE, 1);   										 // OC2N输出使能, enable VL
			FlagPrechargeV = 1;
		}	
	}
  else if(MC.StauteCtl.PrechargeCnt < (PrechargeTime*3))
	{	
		/*Disable WH, Enable WL*/
		if(FlagPrechargeW == 0)
		{
			TIM0_CCR3 = TempPrechargeDuty;	
			SetBit(TIM0_CCER2, CC3E, 0);    										 // OC3输出使能, disable WH
			SetBit(TIM0_CCER2, CC3NE, 1);   										 // OC3N输出使能, enable WL
			FlagPrechargeW = 1;
		}		
	}
	else
	{
		FlagPrechargeU = 0;
		FlagPrechargeU = 0;
		FlagPrechargeU = 0;						
		TIM0_CCR3 = PWMARR;			
		TIM0_CCR2 = PWMARR;		
		TIM0_CCR1 = PWMARR;		
    SetBit(TIM0_CCER1, CC1E, 1);                           // OC1输出使能
    SetBit(TIM0_CCER1, CC1NE, 1);                          // OC1N输出使能
    SetBit(TIM0_CCER1, CC2E, 1);                           // OC2输出使能
    SetBit(TIM0_CCER1, CC2NE, 1);                          // OC2N输出使能
    SetBit(TIM0_CCER2, CC3E, 1);                           // OC3输出使能
		SetBit(TIM0_CCER2, CC3NE, 1);                          // OC3N输出使能
		MOE = 0;

		MC.MotorStatus = MOTOR_START;		
	}	
}

/*-------------------------------------------------------------------------------------------------
	Function Name : void MotorStart(void)
	Description   :	MOTOR_START状态控制函数，检测Hall状态同步SVPWM角度，启动占空比赋值，使能驱动输出
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void MotorStart(void)
{
	MC.HallStatus = CheckHallStatus();
	if(MC.FR == CW)
	{	
		MC.Angle = (int32)Hall_Angle_Arr[MC.HallStatus - 1] + (int32)TempAngleOffsetCW;
	}
	else if(MC.FR == CCW)
	{
		MC.Angle = (int32)Hall_Angle_Arr[MC.HallStatus - 1] - (int32)TempAngleOffsetCCW;
	}
	MC.Theta = 0;	
  MOE = 1;
	MC.PWMDutyCtl.VS = TempMotorStartDuty;
	MC.MotorStatus = MOTOR_NORMAL;
}

/*-------------------------------------------------------------------------------------------------
	Function Name : void MotorNormal(void)
	Description   :	MOTOR_NORMAL状态控制函数，在设定Normal状态检测时间内电机仍然运行于Normal状态，则
                  可判断电机运行正常，此时清零部分全局变量，如堵转和过流计数器
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void MotorNormal(void)
{
  if(MC.StauteCtl.NormalCnt >= NormalTime)  // 1000ms
	{
    MC.StauteCtl.NormalCnt = NormalTime;
		MC.Fault.RestartOCCnt = 0;
		MC.Fault.RestartLRCnt = 0;
	}		
}

/*-------------------------------------------------------------------------------------------------
	Function Name : void MotorFault(void)
	Description   :	MOTOR_FAULT状态控制函数，电机故障，根据不同故障原因选择不同的关闭输出方式，记录
                  故障信息（调试），故障重启控制。                
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void MotorFault(void)
{
	switch(MC.FlagSystemFault)
	{
		case OverVoltage:
			#if (DriverMode == Driver_3P3N)
			{		
				DRV_OUT = 0x15;				// 关断输出
			}
			#elif (DriverMode == Driver_6N)
			{
				DRV_OUT = 0x00;				// 关断输出
			}
			#endif			
			break;
		case UnderVoltage:
			#if (DriverMode == Driver_3P3N)
			{		
				DRV_OUT = 0x15;				// 关断输出
			}
			#elif (DriverMode == Driver_6N)
			{
				DRV_OUT = 0x00;				// 关断输出
			}
			#endif				
			break;
		case OverCurrent:		
			if((MC.PWMDutyCtl.VS - MotorVSDec) >= MotorVSMin)
			{
				MC.PWMDutyCtl.VS -= MotorVSDec;
			}
			else
			{
				MC.PWMDutyCtl.VS = MotorVSMin;
			}				
			break;
		case OverPCBTH:
			break;
		case OverIPMTH:
			break;
		case LockRotor:
			#if (DriverMode == Driver_3P3N)
			{		
				DRV_OUT = 0x15;				// 关断输出
			}
			#elif (DriverMode == Driver_6N)
			{
				DRV_OUT = 0x00;				// 关断输出
			}			
			#endif	
      MC.Fault.FlagLockRotor = 0; 			
			if(MC.Fault.RestartLRCnt < RestartLRCntTime)
			{					
				MC.FlagSystemFault = None;					
				MC.StauteCtl.FaultCnt = 0;			
				MC.Fault.FlagRestartLR = 1;  // 堵转重启
			}					
			break;
		case HallError:
			#if (DriverMode == Driver_3P3N)
			{		
				DRV_OUT = 0x15;				// 关断输出
			}
			#elif (DriverMode == Driver_6N)
			{
				DRV_OUT = 0x00;				// 关断输出
			}
			#endif					
			break;	
		case FO:
			#if (DriverMode == Driver_3P3N)
			{		
				DRV_OUT = 0x15;				// 关断输出
			}
			#elif (DriverMode == Driver_6N)
			{
				DRV_OUT = 0x00;				// 关断输出
			}
			#endif			
			break;			
		default:
			#if (DriverMode == Driver_3P3N)
			{		
				DRV_OUT = 0x15;				// 关断输出
			}
			#elif (DriverMode == Driver_6N)
			{
				DRV_OUT = 0x00;				// 关断输出
			}
			#endif				
			break;
	}

	
	/*记录Fault信息*/
//   FaultMessageRecord();
	
	/*Restart Control*/
  RestartControl();
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void ParameterInit(void)
	Description   :	功能函数，初始化电机控制参数变量
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void ParameterInit(void)
{
	MC.Step = 0;
	MC.Sector = 0;
	MC.StopMode = 0;
	MC.OpenLoopCnt = 0;	
	MC.FlagFirstLedge = 0;
	MC.FlagSystemFault = 0;

	MC.Theta = 0;	
	MC.Angle = 0;	
	MC.AngleErr = 0;
	
	MC.StepTime = 0;
	MC.PeriodTime = 0;

	MC.StauteCtl.FlagStop = 0;
	MC.StauteCtl.FlagInit = 0;
	MC.StauteCtl.FlagReady = 0;
	MC.StauteCtl.FlagPrecharge = 0;
  MC.StauteCtl.FlagAlignment = 0;		
	MC.StauteCtl.FlagStart = 0;
	MC.StauteCtl.FlagNormal = 0;
  MC.StauteCtl.FlagFault = 0;			
	MC.StauteCtl.StopCnt = 0;
	MC.StauteCtl.InitCnt = 0;
	MC.StauteCtl.ReadyCnt = 0;	
	MC.StauteCtl.PrechargeCnt = 0;
	MC.StauteCtl.AlignmentCnt = 0;
	MC.StauteCtl.StartCnt = 0;			
	MC.StauteCtl.NormalCnt = 0;	
	MC.StauteCtl.FaultCnt = 0;		
	
	MC.PWMDutyCtl.VS = 0;
	MC.PWMDutyCtl.StartVS = 0;
  MC.PWMDutyCtl.TargetVS = 0;		
	MC.PWMDutyCtl.PWMDuty = 0;	
	
	MC.SpeedCtl.Speed = 0;
	MC.SpeedCtl.TargetSpeed = 0;
	MC.SpeedCtl.PISpeed = 0;
	MC.SpeedCtl.PITargetSpeed = 0;
	
	MC.CurrentCtl.CurrentBUS = 0;
	MC.CurrentCtl.TargetCurrentBUS = 0;
	MC.CurrentCtl.PICurrentBUS = 0;
	MC.CurrentCtl.PITargetCurrentBUS = 0;
	
	MC.Fault.FaultStatus = None;
	MC.Fault.FlagFO = 0;
	MC.Fault.FlagOverVoltageBUS = 0;
	MC.Fault.FlagUnderVoltageBUS = 0;
	MC.Fault.FlagOverCurrentBUS = 0;
	MC.Fault.FlagLockRotor = 0;
	MC.Fault.FlagHallError = 0;	
	MC.Fault.FlagOverIPMTH = 0;
	MC.Fault.FlagOverPCBTH = 0;
	MC.Fault.FlagRestartOV = 0;
	MC.Fault.FlagRestartUV = 0;
	MC.Fault.FlagRestartOC = 0;
	MC.Fault.FlagRestartOT = 0;
	MC.Fault.FlagRestartLR = 0;
	MC.Fault.FlagRestartHE = 0;	
	MC.Fault.OverVoltageBUSCnt = 0;
	MC.Fault.UnderVoltageBUSCnt = 0;
	MC.Fault.OverCurrentBUSCnt = 0;		
	MC.Fault.LockRotorCnt = 0;
	MC.Fault.OverIPMTHCnt = 0;	
	MC.Fault.OverPCBTHCnt = 0;	
	
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void FaultDetect(void)
	Description   :	功能函数，检测故障，包括过压欠压，过流，堵转，Hall错误
                  过压检测--检测母线电压大于设定值进入过压保护，小于设定恢复值自动重启
                  欠压检测--检测母线电压小于设定值进入欠压保护，大于设定恢复值自动重启
                  过流检测--检测母线电流大于设定值进入过流保护，小于设定恢复值选择设定是否重启
                  堵转检测--检测到堵转标志进入堵转保护，选择设定是否重启
                  Hall错误--检测Hall状态错误（0和7）进入Hall故障保护，Hall恢复正常自动重启
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void FaultDetect(void)
{
	/*过压检测 , 欠压检测*/                                  // run time 6us
	
	
	/*过流检测*/                                             // run time 4us
	if(ADCData.CurrentBUS > ADC_OverCurrentBUS)		
	{
		MC.Fault.OverCurrentBUSCnt ++;
		if(MC.Fault.OverCurrentBUSCnt > OverCurrentBUSCntTime)
		{
			MC.Fault.OverCurrentBUSCnt = OverCurrentBUSCntTime;
			MC.Fault.FlagOverCurrentBUS = 1;	                   // 母线过流
			MC.Fault.FaultStatus = OverCurrent;
		}
	}
	else if(ADCData.CurrentBUS < ADC_ReOverCurrentBUS)
 	{
		if(MC.Fault.FlagOverCurrentBUS)
		{
			MC.Fault.OverCurrentBUSCnt --;
			if(MC.Fault.OverCurrentBUSCnt < 0)
			{
				MC.Fault.OverCurrentBUSCnt = 0;
				MC.Fault.FlagOverCurrentBUS = 0;	
				if(MC.Fault.RestartOCCnt < RestartOCCntTime)
				{
					MC.StauteCtl.FaultCnt = 0;                       // 32bit数据赋值run time 8us
					MC.Fault.FlagRestartOC = 1;                      // 过流重启
				}
			}
		}
		else
		{
			MC.Fault.OverCurrentBUSCnt --;     		               // 16bit数据自减run time 1us
			if(MC.Fault.OverCurrentBUSCnt < 0)   	               // 16bit数据不带等号判run time 0.5us，带等号run time 0.8us
			{
				MC.Fault.OverCurrentBUSCnt = 0;
			}
		}			
	}
	
  /*堵转检测*/	                                           // run time 1us
	if(MC.Fault.FlagLockRotor == 1)
	{
		MC.Fault.FaultStatus = LockRotor;
	}

	/*Hall错误检测*/
	MC.HallStatus = CheckHallStatus();
	if((MC.HallStatus == 0)||(MC.HallStatus == 7))
	{
		MC.Fault.FlagHallError = 1;
		MC.Fault.FaultStatus = HallError;
	}
	else
	{
		if(MC.Fault.FlagHallError == 1)
		{
			MC.Fault.FlagHallError = 0;
			MC.Fault.FlagRestartHE = 1;                          // Hall错误重启
		}
	}
	
	if(MC.Fault.FlagOverCurrentBUS 	||                       // run time 2.6us
		 MC.Fault.FlagOverVoltageBUS 	||
	   MC.Fault.FlagUnderVoltageBUS	||
		 MC.Fault.FlagLockRotor				||
	   MC.Fault.FlagHallError     	||
	   MC.Fault.FlagFO)
	{
		MC.MotorStatus = MOTOR_FAULT;
		MC.FlagSystemFault = MC.Fault.FaultStatus;
	}
	else
	{
		MC.FlagSystemFault = None;
	}

}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void FaultMessageRecord(void)
	Description   :	功能函数，记录故障信息
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void FaultMessageRecord(void)
{
	static uint8 MC_FlagSystemFault = 0;
	
	if(MC_FlagSystemFault != MC.FlagSystemFault)
	{
		MC_FlagSystemFault = MC.FlagSystemFault;
    MC.FaultMessage.FaultStatus = MC.Fault.FaultStatus;

		MC.FaultMessage.FlagFO = MC.Fault.FlagFO;
		MC.FaultMessage.FlagOverVoltageBUS = MC.Fault.FlagOverVoltageBUS;
		MC.FaultMessage.FlagUnderVoltageBUS = MC.Fault.FlagUnderVoltageBUS;
		MC.FaultMessage.FlagOverCurrentBUS = MC.Fault.FlagOverCurrentBUS;		
		MC.FaultMessage.FlagLockRotor = MC.Fault.FlagLockRotor;
		MC.FaultMessage.FlagHallError = MC.Fault.FlagHallError;		
		MC.FaultMessage.FlagOverIPMTH = MC.Fault.FlagOverIPMTH;
		MC.FaultMessage.FlagOverPCBTH = MC.Fault.FlagOverPCBTH;

		MC.FaultMessage.Sector = MC.Sector;
		MC.FaultMessage.HallStatus = MC.HallStatus;
		MC.FaultMessage.Angle = MC.Angle;
		MC.FaultMessage.Theta = MC.Theta;
		MC.FaultMessage.StepTime = MC.StepTime;	
		MC.FaultMessage.PeriodTime = MC.PeriodTime;
		
		MC.FaultMessage.Speed = MC.SpeedCtl.Speed;
		MC.FaultMessage.PISpeed = MC.SpeedCtl.PISpeed;
		MC.FaultMessage.TargetSpeed = MC.SpeedCtl.TargetSpeed;
		MC.FaultMessage.PITargetSpeed = MC.SpeedCtl.PITargetSpeed;
		
		MC.FaultMessage.FVoltageU = ADCData.VoltageU;
		MC.FaultMessage.FVoltageV = ADCData.VoltageV;
		MC.FaultMessage.FVoltageW = ADCData.VoltageW;
		MC.FaultMessage.FVoltageBUS = ADCData.VoltageBUS;
		MC.FaultMessage.FPCBTH = ADCData.PCBTH;
		MC.FaultMessage.FIPMTH = ADCData.IPMTH;
		MC.FaultMessage.FVoltageSREF = ADCData.VoltageSREF;
		MC.FaultMessage.FCurrentBUS = ADCData.CurrentBUS;			
		MC.FaultMessage.FCurrentU = ADCData.CurrentU;
		MC.FaultMessage.FCurrentV = ADCData.CurrentV;	
		
	}	

}


/*-------------------------------------------------------------------------------------------------
	Function Name :	void CurrentControl(void)
	Description   :	电流控制，速度闭环的输出作为电流闭环的参考输出，电流闭环输出调节PWMDuty
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void CurrentControl(void)
{
 static uint8 CurrentCtlCnt = 0;
	
	CurrentCtlCnt ++;
	if(CurrentCtlCnt >= CurrentCtlCntTime)
	{		
		CurrentCtlCnt = 0;
		#if (CurrentCloseLoopEnable)
		{	
			if(MC.CurrentCtl.TargetCurrentBUS > TempCurrentRef)
			{
				MC.CurrentCtl.TargetCurrentBUS = TempCurrentRef;
			}
			
			MDUControl.FlagCurrentBUS = 1;
			MDUControl.FlagPICurrentBUS = 1;
			MDUControl.FlagPITargetCurrentBUS = 1;
			
 			MC.PWMDutyCtl.VS = PI_Control(&CurrentPID, MC.CurrentCtl.PITargetCurrentBUS, MC.CurrentCtl.PICurrentBUS);			
		}
    #endif
	}		

}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void ONOFFControl(void)
	Description   :	功能函数，启停控制，小于设定停机占空比系统关机，大于设定开机占空比系统开机
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void ONOFFControl(void)
{
	uint16 MC_MotorONOFFDuty = 0;
	
	#if (SpeedPWMEnable)
	{
		MC_MotorONOFFDuty = PWMINCtl.PWMINHighDuty;
	}	
	#else
	{
		MC_MotorONOFFDuty = ADCData.VoltageSREF;
	}
	#endif
	
	if(MC_MotorONOFFDuty < TempMotorOFFDuty)
	{
		if(MC.MotorStatus != MOTOR_STOP)
		{			
			MC.StopMode = FreeStop;
			MC.MotorStatus = MOTOR_STOP;
		}
	}	
	else if(MC_MotorONOFFDuty > TempMotorONDuty)
	{
		if(MC.MotorStatus == MOTOR_STOP)
		{
			MC.MotorStatus = MOTOR_INIT;
		}
	}	
}


/*-------------------------------------------------------------------------------------------------
	Function Name :	void SystemTick(void)
	Description   :	功能函数，软件系统时钟计数，用于产生1ms的软件系统时间，为系统状态计时
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void SystemTick(void)
{
	if(MC.MotorStatus == MOTOR_STOP)
	{
		MC.StauteCtl.StopCnt ++;
	}

	if(MC.MotorStatus == MOTOR_READY)
	{
		MC.StauteCtl.ReadyCnt ++;
	}	
	
	if(MC.MotorStatus == MOTOR_PRECHARGE)
	{
		MC.StauteCtl.PrechargeCnt ++;
	}	
	
	if(MC.MotorStatus == MOTOR_START)
	{
 		MC.StauteCtl.StartCnt ++;
	}	
	
	if(MC.MotorStatus == MOTOR_NORMAL)
	{
	  MC.StauteCtl.NormalCnt ++;
	}
	
	if(MC.MotorStatus == MOTOR_FAULT)
	{
		MC.StauteCtl.FaultCnt ++;
	}
	
}



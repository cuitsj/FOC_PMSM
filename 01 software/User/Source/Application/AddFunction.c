/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : AddFunction.c
* Author             : Fortiortech  Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the add function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ---------------------------------------------------------*/
FaultStateType		mcFaultSource;
PWMINPUTCAL   xdata mcPwmInput;
FOCCTRL       xdata mcFocCtrl;
FaultVarible  xdata mcFaultDect;
ADCSample           AdcSampleValue;
MCLedDisplay        mcLedDisplay;
MCRAMP       xdata  mcSpeedRamp;
int16               segmentstate;
uint16              led_t;
MotorFRTypeDef		mcFRState;
OLEDShowData		ShowData;

int16 				OMEGA_READ;
int16 				VSP;
uint16              StartCurrentRef;
uint8               MotorSleepFlag;
uint16              MotorSleepDelayCout;
uint8               Sleep_En;
uint16              SMO_EK2;


uint16              ON_Count;
uint16              OFF_Count;
uint32              ONOFF_Times;
uint8               ONOFF_Flag;

uint8				CCWstate;
uint16				RecEsVlue; 


uint16				SKI_RUN;
uint16				SKP_RUN;

uint8				NoLoad_Flag;

uint8				motor_FR;
uint8				key_trg;   //按键触发记录
uint8				key_cont;  //按键长按触发记录
uint8				keydelay_tim;   //按键长按时间记录
bit					key_flag;    //按键按下标志位
uint16				beep_cnt;
uint8				key_data;
uint16				key_static;
uint16				keypres_static;
uint16				keyscanfiltertime;

MotorTailWindTypeDef xdata  TailWindDetect;
const int16  PowerGiven[10] ={0,500,1000,2200,2900,4300,5900,7300,7900,8550};
/*---------------------------------------------------------------------------*/
/* Name		:	int16 KLPF_VALUE(int16 INVlaue, int16 OutLastValue)
/* Input	:	INVlaue，OutLastValue
/* Output	:	int16的变量
/* Description:	滤波函数,用乘法器做的
/*---------------------------------------------------------------------------*/
int16 KLPF_VALUE(int16 INVlaue, int16 OutLastValue)
{	
	int16 Result = 0;
	MD_MA = (INVlaue-OutLastValue);
	MD_MB = (int16)480;		           			/*写被乘数和乘数*/

	Result = MD_MB;
	Result += OutLastValue;
	return(Result);
}

/*---------------------------------------------------------------------------*/
/* Name		:	void FaultProcess(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	保护处理函数，关闭FOC输出，同时将状态变为mcFault
/*---------------------------------------------------------------------------*/
void FaultProcess(void)
{
	FOC_SET0 &= 0x7F;																				// disable FOC and clear all register
	FOC_CR1 = 0x00;																					// disable FOC output and initial register
	mcState = mcFault;
	MOE     = 0;	
}
/*---------------------------------------------------------------------------*/
/* Name		:	void TailWindParInit(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	逆风启动的变量初始化
/*---------------------------------------------------------------------------*/
void TailWindParInit(void)
{
	TailWindDetect.TailWindSpeed =0;
	TailWindDetect.MotorDir	= 0;
	TailWindDetect.AngleState =0;
	TailWindDetect.MotorTailWindState = NormalTailWind;
	TailWindDetect.SpeedCountTimer=0;
	
	TailWindDetect.SpeedStoreNum =0;	
	TailWindDetect.SpeedOverFlowStatus =0;
	
	TailWindDetect.SpeedTimerClearStatus = 0;
	TailWindDetect.TempThailWindSpeedBase = ThailWindSpeedBase;
	TailWindDetect.TimerClearTimes = 0;
}
/*---------------------------------------------------------------------------*/
/* Name		:	void TailWindSpeedDetect(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	逆风判断
/*---------------------------------------------------------------------------*/
void TailWindSpeedDetect(void)
{
	static int16 	LatestTheta;
	
	if(mcState == mcTailWind)
	{	
			//顺风检测过程由于FOC内部根据电流估算角度，故直接对FOC_ETHETA进行处理
			if(TailWindDetect.MotorTailWindState == NormalTailWind)
			{
				LatestTheta = FOC_ETHETA;
				
				//旋转方向判断判断在 <-170度 到 <10度 ， -10度 到 10度， 10度  到  170度   三个状态切换的时间

				// -170度时
				if(LatestTheta<-30946)
				{
					//计数器未清零或从状态3跳到状态1时清零
					if((TailWindDetect.SpeedTimerClearStatus==0)||(TailWindDetect.AngleState==3))
					{
						TailWindDetect.SpeedCountTimer =0;	
						TailWindDetect.SpeedTimerClearStatus = 1;
						if(TailWindDetect.AngleState==3)
						{
							TailWindDetect.TimerClearTimes ++;//来回1和3之间抖动
						}
					}							
					//<-170度  时设置状态为1，并清零SpeedCountTimer在TIM5中计时
					TailWindDetect.AngleState = 1;			
				}
				//在 -10度 和 10度 之间
				else if((LatestTheta>-1820)&&(LatestTheta<1820))
				{
					//状态1或状态3切换到状态2时保存当前转速时间至TailWindDetect.SpeedCount[SpeedStoreNum]
					if((TailWindDetect.AngleState==1)||(TailWindDetect.AngleState==3))
					{
						//计算当前转速，RPM
						TailWindDetect.SpeedCountTimer += 1;//防止为0
						TailWindDetect.TailWindSpeed = MDU_DIV_U16(&TailWindDetect.TempThailWindSpeedBase, &TailWindDetect.SpeedCountTimer);//测出速度
						TailWindDetect.SpeedStoreNum ++;
						
						if(TailWindDetect.SpeedTimerClearStatus == 1)//用于SpeedCountTimer清零
						{
							TailWindDetect.SpeedTimerClearStatus = 0;
						}
							
						//有1状态切换到2状态说明角度递增旋转方向为CW，3->2则为CCW
						if(TailWindDetect.AngleState==1)
						{
							TailWindDetect.MotorDir = CW;			
						}
						else if(TailWindDetect.AngleState==3)
						{
							TailWindDetect.MotorDir = CCW;
						}
						TailWindDetect.TimerClearTimes =0;//清除来回抖动的计数
					}
					TailWindDetect.AngleState = 2;	
										
				}
				//>170度时
				else if(LatestTheta>30946)
				{
					//计数器未清零或从状态1跳到状态3时清零
					if((TailWindDetect.SpeedTimerClearStatus==0)||(TailWindDetect.AngleState==1))
					{
						TailWindDetect.SpeedCountTimer =0;	
						TailWindDetect.SpeedTimerClearStatus = 1;
						if(TailWindDetect.AngleState==1)
						{
							TailWindDetect.TimerClearTimes ++;//来回1和3之间抖动
						}
					}				
					TailWindDetect.AngleState = 3;
				}
			}		
	}
}
/*---------------------------------------------------------------------------*/
/* Name		:	void FOCCloseLoopStart(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	闭环启动
/*---------------------------------------------------------------------------*/
void FOCCloseLoopStart(void)
{
	#if (BEMF_DETECT_ENABLE)
	{
		//关闭运放
		SetBit(CMP_CR2, CMP0EN, 0);
		SetBit(CMP_CR2, CMP1EN, 0);
		SetBit(CMP_CR2, CMP2EN, 0);						

		FOCEN		= Disable;																	// enable FOC module
		FOC_Init();
		
		//关键语句
		FOC_CMR |= 0x3F;                         				// U、V、W相输出  
		MOE = 1;	
		
		/*sample shunt resister mode set*/
		#if (Shunt_Resistor_Mode == Double_Resistor)			// 双电阻
		{
			FOC_CR1 	= 0x0D;																			// 逆风判断已经使能FOC工作																		                            
		}
		#elif (Shunt_Resistor_Mode == Single_Resistor)			// 单电阻
		{
			FOC_CR1 	= 0x05;																		
		}
		#endif	
		/*Estimate Algorithm set*/
		#if (EstimateAlgorithm == SMO)					      // SMO
		{
			SetBit(FOC_CR1,ESEL,0);																		                              
		}
		#elif (EstimateAlgorithm == PLL)		          // PLL		
		{
			SetBit(FOC_CR1,ESEL,1);																				
		}
		#endif		    

		SetBit(FOC_CR1,FCE,1);
		SetBit(FOC_CR1,PWMSS,0);	
	}
	#endif
	
		FOC_EFREQACC              = Motor_Omega_Ramp_ACC;
		FOC_EFREQMIN 	            = Motor_Omega_Ramp_Min;
		FOC_EFREQHOLD             = Motor_Omega_Ramp_End;	

		SetBit(FOC_CR1,EFAE,1);															// 估算器强制输出
		SetBit(FOC_CR1,RFAE,0);															// 禁止强拉
		SetBit(FOC_CR1,ANGM,1);															// 估算模式  								

		//估算参数重新初始化
		/*current loop parameter set*/
		FOC_DKP 			= DKP;	
		FOC_DKI 			= DKI;
		FOC_DMAX 			= DOUTMAX;
		FOC_DMIN 			= DOUTMIN;

		FOC_QKP 			= QKP;
		FOC_QKI 			= QKI;
		FOC_QMAX 			= QOUTMAX;
		FOC_QMIN 			= QOUTMIN;
			
		/*estimate parameter set*/
		FOC_EK1 			= OBS_K1T;
		FOC_EK2 			= OBS_K2T;
		FOC_EK3 			= OBS_K3T;
		FOC_EK4 			= OBS_K4T;

		FOC_FBASE 		= OBS_FBASE;						
		FOC_OMEKLPF 	= SPEED_KLPF;					
		FOC_EBMFK 		= OBS_KLPF;						

		FOC_IDREF                       = ID_RUN_CURRENT;     // D轴启动电流
		mcFocCtrl.mcIqref				= IQ_RUN_CURRENT;
		FOC_IQREF                       = mcFocCtrl.mcIqref	;		  // Q轴启动电流	
		PI_UK 							=	mcFocCtrl.mcIqref	;
		

		TailWindDetect.AntiFlag             = 3;

		mcState                         = mcRun;
    mcFocCtrl.CtrlMode              = 0;
		
		#if (BEMF_DETECT_ENABLE)  //  反电势作为顺风检测依据
		{
				if(BEMFDetect.BEMFSpeed >_Q15(360.0/MOTOR_SPEED_BASE))
				{
					FOC_EKP 			= OBSW_KP_GAIN_RUN6;					
					FOC_EKI 			= OBSW_KI_GAIN_RUN6;	
					
					Time.Count = 10;	
				}
				else
				{
					FOC_EKP 			= OBSW_KP_GAIN_RUN3;					
					FOC_EKI 			= OBSW_KI_GAIN_RUN3;	
					
//					mcFocCtrl.mcIqref								= IQ_RUN_CURRENT;
//					FOC_IQREF                       = mcFocCtrl.mcIqref	;		  // Q轴启动电流	
//					PI_UK 													=	mcFocCtrl.mcIqref	;
					Time.Count = 1800;			
				}
		}
		
    #else	//  FOC状态观测 作为顺风检测依据
    {
			//根据不同转速确启动的ATO_BW值
			if(TailWindDetect.TailWindSpeed > 300)//300rpm/min
			{
				FOC_EKP 			= OBSW_KP_GAIN_RUN6;					
				FOC_EKI 			= OBSW_KI_GAIN_RUN6;	
				
				Time.Count = 10;		
			}
			else
			{
				FOC_EKP 			= OBSW_KP_GAIN_RUN3;					
				FOC_EKI 			= OBSW_KI_GAIN_RUN3;	
				
				mcFocCtrl.mcIqref								= (IQ_RUN_CURRENT<<1);
				FOC_IQREF                       = mcFocCtrl.mcIqref	;		  // Q轴启动电流	
				PI_UK 													=	mcFocCtrl.mcIqref	;
				Time.Count = 1800;						
			}	
    }		
		#endif
}

/*---------------------------------------------------------------------------*/
/* Name		:	int16 Abs_F16(int16 value)
/* Input	:	value
/* Output	:	int16
/* Description:	对变量取16位的绝对值
/*---------------------------------------------------------------------------*/
uint16 Abs_F16(int16 value)
{
	if(value < 0)
	{
		return (- value);
	}
	else
	{
		return (value);
	}
}
/*---------------------------------------------------------------------------*/
/* Name		:	int32 Abs_F32(int32 value)
/* Input	:	value
/* Output	:	int16
/* Description:	对变量取16位的绝对值
/*---------------------------------------------------------------------------*/
uint32 Abs_F32(int32 value)
{
	if(value < 0)
	{
		return (- value);
	}
	else
	{
		return (value);
	}
}

/*****************************************************************************
 * Function:		 void	Fault_OverVoltage(mcFaultVarible *h_Fault)
 * Description:	 过压欠压保护函数：程序每5ms判断一次，母线电压大于过压保护值时，计数器加一，计数器值超过20次，判断为过压保护，关闭输出;反之，计数器慢慢减
                 同理，欠压保护。
	               电机过欠压保护状态下，母线电压恢复到欠压恢复值以上，过压恢复值以下时，计数器加一，超过200次后，恢复。根据档位信息来决定恢复到哪个状态。
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
void Fault_OverUnderVoltage(FaultVarible *h_Fault)
{
	//过压保护
		if(mcFaultSource == FaultNoSource)//程序无其他保护下
		{
				if(mcFocCtrl.mcDcbusFlt > OVER_PROTECT_VALUE)	 //母线电压大于过压保护值时，计数，超过20次，判断为过压保护，关闭输出;反之，计数器慢慢减
				{
					h_Fault->mcOverVoltDetecFaultCount++;
					if(h_Fault->mcOverVoltDetecFaultCount > 20)//检测100ms
					{
						h_Fault->mcOverVoltDetecFaultCount = 0;
						mcFaultSource=FaultOverVoltage;						
						FaultProcess();					
					}
				}
				else
				{
					if(h_Fault->mcOverVoltDetecFaultCount>0)
					{			
						h_Fault->mcOverVoltDetecFaultCount--;
					}
				}
				
			//欠压保护
		
				if(mcFocCtrl.mcDcbusFlt< UNDER_PROTECT_VALUE)
				{
					h_Fault->mcUnderVoltDetecFaultCount++;	
					
					if(h_Fault->mcUnderVoltDetecFaultCount > 20)//检测100ms
					{									
						h_Fault->mcUnderVoltDetecFaultCount = 0;
						mcFaultSource=FaultUnderVoltage;
						FaultProcess();
					}
				}		
				else
				{
					if(h_Fault->mcUnderVoltDetecFaultCount>0)
					{			
						h_Fault->mcUnderVoltDetecFaultCount--;
					}			
				}
		}
		
		/*******过压欠压保护恢复*********/
		if((mcState == mcFault) &&((mcFaultSource==FaultUnderVoltage)||(mcFaultSource==FaultOverVoltage)))
		{
			if((mcFocCtrl.mcDcbusFlt< OVER_RECOVER_VALUE)&&(mcFocCtrl.mcDcbusFlt> UNDER_RECOVER_VALUE))									
			{
				h_Fault->mcVoltRecoverCount++;
				if(h_Fault->mcVoltRecoverCount>200)//连续检测1s，若正常则恢复
				{
          mcState = mcwait;
					mcFaultSource=FaultNoSource;
					h_Fault->mcVoltRecoverCount = 0;	
				}
			}
			else
			{
				h_Fault->mcVoltRecoverCount = 0;
			}	
	 }
}
/*****************************************************************************
 * Function:		 void	Fault_Overcurrent(mcFaultVarible *h_Fault)
 * Description:	 软件过流保护，当三相电流中任何一相的绝对值超过过流保护值，OverCurCnt计数，当1s内超过3次时，
	               程序判断为软件过流保护，电机停止，重新上电才能恢复。当1s内OverCurCnt小于3次时，OverCurCnt清零。
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
void Fault_Overcurrent(FaultVarible *h_Fault)
{
	if((mcState == mcRun)||(mcState == mcStart))						// check over current in rum and open mode
	{
		h_Fault->Abs_ia = Abs_F16(FOC_IA);
		h_Fault->Abs_ib = Abs_F16(FOC_IB);
		h_Fault->Abs_ic = Abs_F16(-FOC_IA-FOC_IB);
		if(h_Fault->Abs_ia> h_Fault->Max_ia)
		{
			 h_Fault->Max_ia = h_Fault->Abs_ia;
		}
	
		if(h_Fault->Abs_ib > h_Fault->Max_ib)
		{
			 h_Fault->Max_ib = h_Fault->Abs_ib;
		}
	
		if(h_Fault->Abs_ic > h_Fault->Max_ic)
		{
			 h_Fault->Max_ic = h_Fault->Abs_ic;
		}	
		
		h_Fault->currenttime++;
		if(h_Fault->currenttime>16000)//1s计数
		{
			h_Fault->currenttime = 0;
			h_Fault->OverCurCnt= 0;
		}
		if((h_Fault->Abs_ia>=h_Fault->OverCurrentValue)||(h_Fault->Abs_ib>=h_Fault->OverCurrentValue)||(h_Fault->Abs_ic>=h_Fault->OverCurrentValue))
		{
			h_Fault->OverCurCnt++;
			if(h_Fault->OverCurCnt>=3)
			{
				h_Fault->Abs_ia=0;
				h_Fault->Abs_ib=0;
				h_Fault->Abs_ic=0;
				h_Fault->OverCurCnt=0;
			  mcFaultSource=FaultSoftOVCurrent;
			  FaultProcess();	
			}
		}		
//		if(mcFocCtrl.Powerlpf>11045)//限功率
//		{		
//		 if(mcFocCtrl.mcIqref>=LIMIT_MIN_CURRENT)
//		 {
//			 mcFocCtrl.mcIqref--;
//			 PI_UK = mcFocCtrl.mcIqref;
//			 FOC_IQREF = mcFocCtrl.mcIqref;
//		 }
//		}	
//		mcFocCtrl.mcIqref=FOC_POW;
	}	
}
/*****************************************************************************
 * Function:		 void	Fault_OverCurrentRecover(mcFaultVarible *h_Fault)
 * Description:	 
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
void Fault_OverCurrentRecover(FaultVarible *h_Fault)
{
	if((mcState == mcFault)&&((mcFaultSource==FaultSoftOVCurrent)||(mcFaultSource==FaultHardOVCurrent)))
	{
		h_Fault->CurrentRecoverCnt++;
		if(h_Fault->CurrentRecoverCnt>=OverCurrentRecoverTime)//2000*5s=10s
		{
			h_Fault->CurrentRecoverCnt=0;
				
			mcState = mcwait;		
			mcFaultSource=FaultNoSource;
    }	
  }
}

/*****************************************************************************
 * Function:		 void	Fault_Start(mcFaultVarible *h_Fault)
 * Description:	 启动保护函数，
								方法一：电机运行状态下，电机在前5s估算转速达到堵转保护值或者5s后反电动势值太低(此方法未验证)
								方法二：
                方法三：4s内还在CtrlMode状态，即速度低于MOTOR_LOOP_RPM，程序判断为启动失败，电机停机。

                  当程序判断为启动失败后，若重启次数少于或等于5次，程序立即进入校准状态，等待重启。
 * Parameter:		 mcFaultVarible *h_Fault               
 * Return:			 no
 *****************************************************************************/
  void Fault_Start(FaultVarible *h_Fault)
  {
		/*******启动保护恢复*********/
		h_Fault->mcEsValue = FOC_ESQU;
		
		 if((mcFaultSource==FaultStart)&&(mcState == mcFault)&&(h_Fault->SecondStart<=5))		//重启恢复次数
		 {						 
			 mcFaultSource=FaultNoSource;
			 mcState = mcwait;			   
		 }
		 
		if(mcState == mcRun)	
		{	
//			//方法一			5秒内电机速度超过最大转速，且反电动势值比较低， 判断为 启动失败
//			if(h_Fault->mcStartSpeedCnt<=1000)		//5s
//			{
//			  h_Fault->mcStartSpeedCnt++;
//				if((mcSpeedFlt > Motor_Max_Speed)&&(h_Fault->mcEsValue<10))
//				{
//					h_Fault->mcStartSpeedCnt = 0;
//					mcFaultSource=FaultStart;
//					mcState = mcFault;					
//					FaultProcess();		
//					h_Fault->SecondStart++;	
//					
//					h_Fault->StartFlag = 1;		
//				}	
//				else
//				{
//					//	h_Fault->mcStartSpeedCnt = 0;
//				}
//			}
			
			
     //方法二		启动后的 第2秒 到 第6秒 内，反电动势值过低，判断为 启动失败
			if(h_Fault->mcStartEsCnt<=1200) //1200 * 5 /1000 = 6S  
			{
				h_Fault->mcStartEsCnt++;
				h_Fault->mcStartDelay++;
				if(h_Fault->mcStartDelay>=400)	//400*5/1000=2s 后	 判断时间
				{
					 h_Fault->mcStartDelay=400;
					
					 if(h_Fault->mcEsValue <3)	//反电动势值调节范围      （这个值应该 设置比堵转的值低 ？）
						{	
							
							h_Fault->mcStartCnt++;
							if(h_Fault->mcStartCnt>=50) //检测次数
							{							
								mcFaultSource=FaultStart;
								mcState = mcFault;
								
								FaultProcess();		
								h_Fault->SecondStart++;
								h_Fault->mcStartDelay=0;	
								h_Fault->mcStartCnt=0;	

								h_Fault->StartFlag = 2;								
							}						 
						}			
						else
						{
							h_Fault->mcStartCnt=0;
						}						
				 }
		 }
		 		  //方法三			2S后仍在模式0（强拖），判断为 启动失败
			if(mcFocCtrl.CtrlMode==0)
			{
				h_Fault->mcStartFocmode++;
				if(h_Fault->mcStartFocmode>=400)//400*5/1000=2s
				{
					h_Fault->mcStartFocmode=0;
					mcFaultSource=FaultStart;
					mcState = mcFault;	
					
					FaultProcess();		
					h_Fault->SecondStart++;
					
					
					h_Fault->StartFlag = 3;					
				}
			}
	  }


	}
 /*****************************************************************************
 * Function:		 void	Fault_Stall(mcFaultVarible *h_Fault)
 * Description:	 堵转保护函数，有三种保护方式，
	               第一种，
	               第二种，电机运行状态下，延迟4s判断，估算速度绝对值超过堵转速度连续5次；
	               第三种，电机运行状态下，当U,V两相电流绝对值大于堵转电流保护值连续6次；
	               当以上三种的任何一种保护触发时，电机停机，程序判断为堵转保护；
	               当堵转保护状态下，U相采集值低于堵转恢复值时，若堵转次数小于或等于堵转重启次数8次，
	               程序延迟mcStallRecover重新启动，进行校准状态。
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
//堵转保护
void Fault_Stall(FaultVarible *h_Fault)
{
		//method 1
	h_Fault->mcEsValue = FOC_ESQU;
	if(mcState == mcRun)
	{	
//method 2				
		if(h_Fault->mcStallDelaDectSpd <=1000)//5s
		{
			h_Fault->mcStallDelaDectSpd ++;
				
		}
		else
		{		
			
			
	    //method 1		
			if((h_Fault->mcEsValue < 3)||(FOC_EOME > Motor_Stall_Max_Speed)) // EsValue 应当比 启动的值高
			{		
				h_Fault->mcStallDelaDectEs++;
				if(h_Fault->mcStallDelaDectEs >= 30)
				{
					h_Fault->mcStallDelaDectEs=0;
					mcFaultSource=FaultStall;
					h_Fault->mcStallTimes++;
					FaultProcess();
					h_Fault->StallFlag  =  1;
				}
			}
			else
			{
				if(h_Fault->mcStallDelaDectEs>0)
				{
				 h_Fault->mcStallDelaDectEs--;
				}
			}
			
//			if((mcSpeedFlt<Motor_Stall_Min_Speed)||((FOC_EOME > _Q15(600.0/MOTOR_SPEED_BASE))&&(h_Fault->mcEsValue < 180)))
			if(mcFocCtrl.mcSpeedFlt<Motor_Stall_Min_Speed)
			{				 
				h_Fault->mcStallDeSpeed++;
				if(h_Fault->mcStallDeSpeed>=8)
				{	
					h_Fault->mcStallDeSpeed=0;
					mcFaultSource=FaultStall;
			    h_Fault->mcStallTimes++;					
					FaultProcess();			
					
					
 					h_Fault->StallFlag =2;
				}						 
			}			
			else
			{
				h_Fault->mcStallDeSpeed=0;	
			}	
			
		}		
//method 3		
		if((h_Fault->Abs_ia>=h_Fault->StallCurrentVale)||
			 (h_Fault->Abs_ib>=h_Fault->StallCurrentVale)||
		   (h_Fault->Abs_ic>=h_Fault->StallCurrentVale))
		{			 
			h_Fault->mcStallDeCurrent++;
			if(h_Fault->mcStallDeCurrent>=6)
			{
				h_Fault->mcStallDeCurrent=0;
				mcFaultSource=FaultStall;	
				h_Fault->mcStallTimes++;
				FaultProcess();									
 				h_Fault->StallFlag =3;
				
				
			}						 
		}	
		else if((h_Fault->Abs_ia<=(h_Fault->StallCurrentVale-I_Value(0.05)))&&
			 (h_Fault->Abs_ia<=(h_Fault->StallCurrentVale-I_Value(0.05))))
		{
			h_Fault->mcStallDeCurrent=0;
		}		
	}		 
		 /*******堵转保护恢复*********/
	if((mcFaultSource==FaultStall)&&(mcState == mcFault)&&(h_Fault->mcStallTimes<=5))	//恢复次数  	2017-10-31 去掉(ADC4_DR<h_Fault->StallRecoverCurrent)&&
	{
		h_Fault->mcStallCount++;			 
		if(h_Fault->mcStallCount>=h_Fault->mcStallRecover)
		{
			h_Fault->mcStallRecover=h_Fault->mcStallRecover;
			h_Fault->mcStallCount=16000;
			mcFaultSource=FaultNoSource;
 			mcState = mcwait;	
		}					 				 
	}
	else
	{
		h_Fault->mcStallCount=0;	
	}
}
 /*****************************************************************************
 * Function:		 void	Fault_phaseloss(mcFaultVarible *h_Fault)
 * Description:	 缺相保护函数，当电机运行状态下，10ms取三相电流的最大值，
	               1.5s判断各相电流最大值，若存在两相电流值大于一定值，而第三相电流值却非常小，则判断为缺相保护，电机停机；	               
 * Parameter:		 mcFaultVarible *h_Fault
 * Return:			 no
 *****************************************************************************/
  void Fault_phaseloss(FaultVarible *h_Fault)
  {
     if(mcState == mcRun)	
		   {
			   h_Fault->LphaseDelayCount++;
			   if(h_Fault->LphaseDelayCount>=2)//wait 1ms*5*2=10ms
			   {		
            h_Fault->LphaseDelayCount=0;					 
				
						h_Fault->Lphasecnt++;
						if(h_Fault->Lphasecnt>50)
							 {
								 h_Fault->Lphasecnt=0;
								 if(((h_Fault->Max_ia>(h_Fault->Max_ib*3))||(h_Fault->Max_ia>(h_Fault->Max_ic*3)))&&(h_Fault->Max_ia>PhaseLossCurrentValue))
//								 if(h_Fault->Max_ia < h_Fault->LowCurrent && h_Fault->Max_ib > h_Fault->NomCurrent && h_Fault->Max_ic > h_Fault->NomCurrent)
								 {
										h_Fault->AOpencnt++;
								 }
								 else
								 {
										h_Fault->AOpencnt = 0;
								 }
                 if(((h_Fault->Max_ib>(h_Fault->Max_ia*3))||(h_Fault->Max_ib>(h_Fault->Max_ic*3)))&&(h_Fault->Max_ib>PhaseLossCurrentValue))								 
//								 if(h_Fault->Max_ib < h_Fault->LowCurrent && h_Fault->Max_ia > h_Fault->NomCurrent && h_Fault->Max_ic > h_Fault->NomCurrent)
								 {
									 h_Fault->BOpencnt++;
								 }
								else
								 {
									 h_Fault->BOpencnt = 0;
								 }
								 if(((h_Fault->Max_ic>(h_Fault->Max_ia*3))||(h_Fault->Max_ic>(h_Fault->Max_ib*3)))&&(h_Fault->Max_ic>PhaseLossCurrentValue))
//								if(h_Fault->Max_ic < h_Fault->LowCurrent && h_Fault->Max_ib > h_Fault->NomCurrent && h_Fault->Max_ia > h_Fault->NomCurrent)
								 {
									 h_Fault->COpencnt++;
								 }
								else
								 {
										h_Fault->COpencnt = 0;
								 }
							 
								h_Fault->Max_ia = 0;
								h_Fault->Max_ib = 0;
								h_Fault->Max_ic = 0;
							 if(h_Fault->AOpencnt > 1|| h_Fault->BOpencnt > 1 || h_Fault->COpencnt > 1)
								 {
										h_Fault->mcLossPHTimes++;
										mcFaultSource=FaultLossPhase;
										mcState = mcFault;	
										FaultProcess();									 
								 }
							}
				 }
	   }	
		 /*******缺相保护恢复*********/
      if((mcFaultSource==FaultLossPhase)&&(mcState == mcFault)&&
			(h_Fault->mcLossPHTimes<0))//可重启5次
			 {
					 h_Fault->mcLossPHCount++;			 
					 if(h_Fault->mcLossPHCount>=PhaseLossRecoverTime)
					 {
						 h_Fault->AOpencnt=0;
						 h_Fault->BOpencnt=0;
						 h_Fault->COpencnt=0;					
						 // restart	
             mcState = mcwait;		
						 mcFaultSource=FaultNoSource;					
           }				 
       }
    else
			 {
			 h_Fault->mcLossPHCount=0;	
			 }			 

  }

/*---------------------------------------------------------------------------*/
/* Name		:	void Fault_Detection(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	保护函数，因保护的时间响应不会很高，采用分段处理，每5个定时器中断执行一次对应的保护
	              常见保护有过欠压、过温、堵转、启动、缺相等保护，调试时，可根据需求，一个个的调试加入。
/*---------------------------------------------------------------------------*/
void Fault_Detection(void)
{
	segmentstate++;
	if(segmentstate>=5)
	{
		segmentstate=0;
	}
	switch(segmentstate)
	{
		case 0:
		if(CurrentRecoverEnable) //过流保护恢复
		{	
			Fault_OverCurrentRecover(&mcFaultDect);	
		}
		break;
		case 1:
		if(VoltageProtectEnable==1)//电压保护
		{					
			Fault_OverUnderVoltage(&mcFaultDect);
		}
		break;
		case 2:
		if(StartProtectEnable==1)//启动保护
		{	
		 
			Fault_Start(&mcFaultDect);
		}
		break;
		case 3:
		if(StallProtectEnable==1)//堵转保护
		{
			Fault_Stall(&mcFaultDect);
		}
		break;
		case 4:
		if(PhaseLossProtectEnable==1)//缺相保护
		{
			Fault_phaseloss(&mcFaultDect);
		}
		break;
		default:
		break;
	}
}
/*---------------------------------------------------------------------------*/
/* Name		:	void Speed_response(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	速度响应函数，可根据需求加入控制环，如恒转矩控制、恒转速控制、恒功率控制
/*---------------------------------------------------------------------------*/
void Speed_response(void)
{
	if((mcState ==mcRun)||(mcState ==mcStop))       
	{
		switch(mcFocCtrl.CtrlMode)
		{
			case 0:
			 if(mcFocCtrl.mcSpeedFlt >Motor_Loop_Speed)        //启动，纯电流环控制
			 {
					mcFocCtrl.CtrlMode= 1;
					mcFocCtrl.mcIqref=IQ_Start_CURRENT;  //不决定速度，只影响切入速度环或者其他外环的那一瞬间。
					FOC_IQREF = IQ_Start_CURRENT;
					//mcSpeedRamp.ActualValue = FOC_EOME;  					 
					mcFocCtrl.TorqueLoopTime=0;	
					PI_UK = mcFocCtrl.mcIqref;
			 }					
			break;
			case 1:
			mcFocCtrl.TorqueLoopTime++;
			if(mcFocCtrl.TorqueLoopTime>5)
			{
				mcFocCtrl.TorqueLoopTime=0;
				#if (OUTLoop_Mode==OUTLoop_Disable)//无外环，在这可调节电流，用来验证电机输出能力和电流环的稳定性
				{	
					FOC_IQREF = IQ_RUN_CURRENT;								
				}
				#elif (OUTLoop_Mode==OUTLoop_Enable)//加外环，根据实际情况加速度环，功率环或其他环路。
				{
					//mc_ramp(&mcSpeedRamp);
					mcSpeedRamp.ActualValue = 19114 - mcFocCtrl.Vspinput*4.3;
					if (mcSpeedRamp.ActualValue > Motor_Max_Speed)
					{
						mcSpeedRamp.ActualValue = Motor_Max_Speed;
					}
					else if (mcSpeedRamp.ActualValue < Motor_Min_Speed)					
					{
						mcSpeedRamp.ActualValue = Motor_Min_Speed;
					}
					if (mcFocCtrl.Vspinput == 4095)
					{
						mcSpeedRamp.ActualValue = Motor_Min_Speed;
					}
					else if (mcFocCtrl.Vspinput == 0)
					{
						mcSpeedRamp.ActualValue = Motor_Max_Speed;
					}
					
					HW_Speed_PI();					
				}
				#endif
			}	
			break;
		}
	}
}

/*---------------------------------------------------------------------------*/
/* Name		:	 uint16 SoftLPF(uint16 Xn1, uint16 Xn0, uint16 K)
/* Input	:	uint16 Xn1, uint16 Xn0, uint16 K
/* Output	:	uint16
/* Description:	软件低通滤波
/*---------------------------------------------------------------------------*/
 int16 SoftLPF(int16 Xn1, int16 Xn0, int16 K)
 {
 	int16 Temp16 = 0;
 	int32 Temp32 = 0;
 	
 	Temp32 = (((int32)Xn1 - (int32)Xn0) * (int32)K) >> 15;
 	Temp16 = Xn0 + (int16)Temp32;
 	return Temp16;
 }
 
 void Sleepmode(void)
 {
	 		MotorSleepDelayCout++;
		if(MotorSleepDelayCout>=20000)//最大65530，若要再大，需改数据类型
		{
			mcSpeedRamp.TargetValue=0;
			mcFocCtrl.McuRef =0;
			SetBit(P0_OE, P02, 0);
			Uart.R_DATA[0]                     = 0;
			Uart.R_DATA[1]                     = 0;
			Uart.R_DATA[2]                     = 0;
			Uart.R_DATA[3]                     = 0;
			Uart.R_DATA[4]                     = 0;	
			Uart.R_DATA[5]                     = 0;
			Uart.R_DATA[6]                     = 0;
			Uart.R_DATA[7]                     = 0;
			Uart.R_DATA[8]                     = 0;	
			Uart.R_DATA[9]                     = 0;
		  SetBit(PH_SEL,UT1EN,0);	
			REN = 0;						//关闭串口接收
	    ES0 = 0;						//先关中断串口	
			FOC_SET0 &= 0x7F;																				// disable FOC and clear all register
			FOC_CR1 = 0x00;																					// disable FOC output and initial register
			MOE     = 0;	
			MotorSleepDelayCout=0;
			MotorSleepFlag=1;
			SetBit(P1IE, P11, 1);   // config P11 as the source of EXTI1
			SetBit(PCON, STOP, 1);
		}			
 }
 
/*---------------------------------------------------------------------------*/
/* Name		:	void ONOFF_Starttest(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	启动测试
/*---------------------------------------------------------------------------*/
void ONOFF_Starttest(void)
{
	if(ONOFF_Flag==1)
	{
		mcSpeedRamp.FlagONOFF = 1;
		mcSpeedRamp.TargetValue =  Motor_Min_Speed;
		 ON_Count++;
		if(ON_Count>StartON_Time)
		{
			ON_Count=0;
			ONOFF_Times++;
			ONOFF_Flag=0;

			mcSpeedRamp.TargetValue = 0;
		}
	}
	else
	{
		 OFF_Count++;
		mcSpeedRamp.FlagONOFF = 0;
		mcSpeedRamp.TargetValue = 0;
		if(OFF_Count>StartOFF_Time)
		{
			OFF_Count=0;
			ONOFF_Flag=1;
			mcSpeedRamp.TargetValue = Motor_Min_Speed;
			
		}
		
  }
} 
 
/*---------------------------------------------------------------------------*/
/* Name		:	void LED_Display(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	LED灯显示
/*---------------------------------------------------------------------------*/
void LED_Display(void)
{
	switch(mcFaultSource)
	{
    		case FaultNoSource:					
    		    ResetLEDPin;	    	//低电平点亮，高电平灭			
    		  break;
    	  case FaultStall:
				    Led_OnOff(&mcLedDisplay,1);								
				  break;
				case FaultLossPhase:
					  Led_OnOff(&mcLedDisplay,2);					
					break;	
    		case FaultSoftOVCurrent:
            Led_OnOff(&mcLedDisplay,3);	
    			break;
    		case FaultHardOVCurrent:
            Led_OnOff(&mcLedDisplay,4);				
    			break;
    		case FaultOverVoltage:
					  Led_OnOff(&mcLedDisplay,5);		
					break;
				case FaultUnderVoltage:
					  Led_OnOff(&mcLedDisplay,6);	
				  break;
    		case  FaultStart:
            Led_OnOff(&mcLedDisplay,7);	
    			break;
				case  FaultOverwind:
					  Led_OnOff(&mcLedDisplay,8);					
					break;					
    		default:
    			break;
  }

}

  //LED灯的闪烁
  void Led_OnOff(MCLedDisplay *hLedDisplay,uint8 htime)
  {	
		hLedDisplay->LedCount++;
	  if(hLedDisplay->LedCount<hLedDisplay->Counttime)
	   {  
		    if(hLedDisplay->Count<200)
	      {
				 hLedDisplay->Count++;
		 
	      }
				else if((hLedDisplay->Count>=200)&&(hLedDisplay->Count<201))
				{
					hLedDisplay->Count=0;
				  LEDPinONOFF;
					hLedDisplay->LedTimCot++;
				}
				else
					;
				if(hLedDisplay->LedTimCot>=2*htime)
		  	{
					hLedDisplay->Count=202;
				  SetLEDPin;
				}
      }
	    else if(hLedDisplay->LedCount>=hLedDisplay->Counttime)
	    {	
	     hLedDisplay->LedCount=0;
	     hLedDisplay->LedTimCot=0;
	     hLedDisplay->Count=0;
	    }
 }
/*---------------------------------------------------------------------------*/
/* Name		:	void mc_ramp(void)
/* Input	:	hTarget,MC_RAMP *hSpeedramp
/* Output	:	NO
/* Description:	
/*---------------------------------------------------------------------------*/		

void mc_ramp(MCRAMP *hSpeedramp)
{
	  if( --hSpeedramp->DelayCount < 0)
		{
				hSpeedramp->DelayCount = hSpeedramp->DelayPeriod;
			
				if (hSpeedramp->ActualValue < hSpeedramp->TargetValue)
				{
						if(hSpeedramp->ActualValue + hSpeedramp->IncValue < hSpeedramp->TargetValue)
						{
								hSpeedramp->ActualValue += hSpeedramp->IncValue;
						}
						else
						{
								hSpeedramp->ActualValue = hSpeedramp->TargetValue;
						}
				}
				else
				{
						if(hSpeedramp->ActualValue - hSpeedramp->DecValue > hSpeedramp->TargetValue)
						{

								hSpeedramp->ActualValue -= hSpeedramp->DecValue;
						}
						else
						{
								hSpeedramp->ActualValue = hSpeedramp->TargetValue;
						}	
				}
		}
}
void HW_Speed_PI(void)
{
	PI_EK =  mcSpeedRamp.ActualValue - mcFocCtrl.mcSpeedFlt;	//给定转速与实际转速之差
	PL_CR |= 0x02;									// Start PI
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_();
	_nop_(); 
	_nop_();
	mcFocCtrl.mcIqref= PI_UK;//可区分限功率与不限功率
	FOC_IQREF= PI_UK;
}
/*-------------------------------------------------------------------------------------------------
	Function Name :	uint16 LPF(uint16 Xn1, uint16 Xn0, uint16 K)
	Description   :	LFP控制
	Input         :	Xn1--当前输入值
	                Xn0--上一次滤波输出值
									K--LPF滤波系数
  Output				:	LPF_Y--当前滤波输出值
-------------------------------------------------------------------------------------------------*/
uint16 LPF(uint16 Xn1, uint16 Xn0, uint16 K)
{	
	LPF_K = K;
	LPF_X = Xn1;
	LPF_Y = Xn0;	
	SetBit(PL_CR, LPFSTA, 1);
	_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();
	return LPF_Y;
}

/*---------------------------------------------------------------------------*/
/* Name		:	void PWMInputCapture(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	PWM输入信号捕获处理,包括速度给定
/*---------------------------------------------------------------------------*/
void PWMInputCapture(void)
{
	  uint32 Temp32;
	  uint16 MotorControlVSP;
	
		if(mcPwmInput.PWMFlag==0)//周期值和比较值
		{
			mcPwmInput.PWMCompare = TIM3_DR;
			mcPwmInput.PWMARR     = TIM3_ARR;
		}	
		else 
		{
			mcPwmInput.PWMCount++;
			if(mcPwmInput.PWMCount>=1600)//100ms，2Hz清一次PWMFlag
			{
				mcPwmInput.PWMCount=0;
				mcPwmInput.PWMFlag=0;
			}		
		}
		
		if(Abs_F32(mcPwmInput.PWMCompare-mcPwmInput.PWMCompareOld)<1000)//两次比较值相近时，减少读错率
		{	
				//启动乘法器
				MDU_MUL_U32(mcPwmInput.PWMCompare, 32767, &Temp32);
				//启动除法器
				mcPwmInput.PWMDuty = MDU_DIV_U16(&Temp32, &mcPwmInput.PWMARR);
			
			  MotorControlVSP=mcPwmInput.PWMDuty;
			
			 if((MotorControlVSP > ONPWMDuty)&&(MotorControlVSP <=OFFPWMDutyHigh))
				{
					mcSpeedRamp.FlagONOFF = 1;
				}
			 else if(MotorControlVSP < OFFPWMDuty)//||(MotorControlVSP >= OFFPWMDutyHigh))
				{
					mcSpeedRamp.FlagONOFF = 0;
				}	
			
				//转速曲线计算
				if(mcSpeedRamp.FlagONOFF==1)
				{
					
					if(NoLoad_Flag == 1)
					{
							if(MotorControlVSP <= MINPWMDuty)
							{
								mcSpeedRamp.TargetValue = Motor_Min_Speed;
							}
							else if(MotorControlVSP < MAXPWMDuty)
							{							
								mcSpeedRamp.TargetValue = Motor_Min_Speed + SPEED_K*(MotorControlVSP-MINPWMDuty);						
							}	
							else		
							{
								mcSpeedRamp.TargetValue	=	Motor_Max_Speed;
							}
					}
					else
					{
						
							if(MotorControlVSP <= MINPWMDuty)
							{
								mcSpeedRamp.TargetValue = Motor_NoLoad_Min_Speed;
							}
							else if(MotorControlVSP < MAXPWMDuty)
							{							
								mcSpeedRamp.TargetValue = Motor_NoLoad_Min_Speed + SPEED_K*(MotorControlVSP-MINPWMDuty);						
							}	
							else		
							{
								mcSpeedRamp.TargetValue	=	Motor_Max_Speed;
							}												
					}
			  }	
        else
				{
					mcSpeedRamp.TargetValue =0;
        }					
    }
		mcPwmInput.PWMCompareOld=mcPwmInput.PWMCompare;//将此次比较值赋值给上次比较值
		mcPwmInput.PWMARROld=mcPwmInput.PWMARR;//将此次周期值赋值给上次周期值
	}
	
	/*---------------------------------------------------------------------------*/
/* Name		:	void FGOutput(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	FG信号输出
/*---------------------------------------------------------------------------*/
void FGOutput(void)
{
	if(mcState==mcRun)
	{
		if((FOC_THETA>=0)&&(FOC_THETA<32768))//0-180
		{
			ResetFGPin; 
		}
		else //180-360
		{
			SetFGPin;
		}
	
//		if((FOC_THETA>=0)&&(FOC_THETA<10922))//0-60
//		{
//			ResetFGPin; 
//		}
//		else if((FOC_THETA>=10922)&&(FOC_THETA<21845))//60-120
//		{
//			SetFGPin;
//		}
//		else if((FOC_THETA>=21845)&&(FOC_THETA<32768))//120-180
//		{
//			ResetFGPin; 
//		}
//		else if((FOC_THETA>=32768)&&(FOC_THETA<43690))//180-240
//		{
//			SetFGPin;
//		}
//		else if((FOC_THETA>=43690)&&(FOC_THETA<54613))//240-300
//		{
//			ResetFGPin; 
//		}
//		else if((FOC_THETA>=54613)&&(FOC_THETA<65536))//300-360
//		{
//		  SetFGPin;
//		}
  }
	else if(mcState == mcFault)
	{
			SetFGPin;
	}
	else
	{
			ResetFGPin; 
	}
}

/*---------------------------------------------------------------------------*/
/* Name		:	void VSPSample(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	VSP采样
/*---------------------------------------------------------------------------*/
void VSPSample(void)
{
	/*****VREF的采样获取值并滤波******/
	AdcSampleValue.ADCVref = (ADC3_DR<<3);//采集对应端口
	VSP=LPF(AdcSampleValue.ADCVref,VSP,1480);//低通滤波
			
	 if((VSP > ONPWMDuty)&&(VSP <= OFFPWMDutyHigh))//在ONPWMDuty-OFFPWMDutyHigh之间，电机有转速运行
		{
			mcSpeedRamp.FlagONOFF = 1;
		}
	 else if((VSP < OFFPWMDuty)||(VSP > OFFPWMDutyHigh))//电机停机
		{
			mcSpeedRamp.FlagONOFF = 0;
		}		
		//转速曲线计算
		if(mcSpeedRamp.FlagONOFF==1)//  
		{
			if(VSP <= MINPWMDuty)    //最小转速运行
			{
				mcSpeedRamp.TargetValue = Motor_Min_Speed;
			}
			else if(VSP < MAXPWMDuty)//调速
			{							
				mcSpeedRamp.TargetValue = Motor_Min_Speed + SPEED_K*(VSP-MINPWMDuty);						
			}	
			else		     //最大转速运行
			{
				mcSpeedRamp.TargetValue	=	Motor_Max_Speed;
			}	
		}	
		else
		{
			mcSpeedRamp.TargetValue =0;
		}						
}
/*---------------------------------------------------------------------------*/
/* Name		:led_flashing(void
/* Input	:	NO
/* Output	:	NO
/* Description:	led闪烁处理函数
/*---------------------------------------------------------------------------*/

void led_flashing(void)//LED
{

	led_t++;
	if (led_t >= 20000)
	{
		GP37=1;
		led_t = 0;
  }
}
/*---------------------------------------------------------------------------*/
/* Name		:led_flashing(void
/* Input	:	NO
/* Output	:	NO
/* Description:	按键扫描函数
/*---------------------------------------------------------------------------*/
void key_scan(void)
{
////	uint8 key_data = 1;
//	key_data = GP06;//读取GP06
////	key_data &= 0x01;
////	key_trg = key_data&(key_data^key_cont);
////	key_cont = key_data;//key_cont始终跟随key_data
//	
//	if(key_data == 0)
//	{
//		keydelay_tim++;
//		if(keydelay_tim > 100)
//		{
//			keydelay_tim = 0;
//			key_data = GP06;
//		  if(key_data == 0)
//			{
//		   key_flag = 1;
//      }				
//			
//    }
////		key_flag = 1;
//  }
//	else
//	{
//		key_flag = 0;
//  }
if (stop_pin)
  {
    if (key_static == 1)
		{ 
       keypres_static = 1;

    }
		key_static = 0;
			key_pro();
  }
else 
  {
	if(key_static!=1)
	  {
     keyscanfiltertime++;
			if(keyscanfiltertime > 100)
			{
				keyscanfiltertime = 0;
				key_static = 1;
      }
    }
  }
	
}
/*---------------------------------------------------------------------------*/
/* Name		:led_flashing(void
/* Input	:	NO
/* Output	:	NO
/* Description:	按键处理函数
/*---------------------------------------------------------------------------*/
void key_pro(void)
{
	if (keypres_static == 1)
	{
//		keypres_static = 0;
		beep_cnt++;
		GP10 = 1;
		GP37 = 0;
		if(beep_cnt > 500)
		{

			GP10 = 0;
			GP37 = 1;
			beep_cnt = 0;
			keypres_static = 0;
    }
	}
		
//else if (keypres_static ==0)
//		{
//		GP10 = 0;
//    GP37 = 1;			
//    }

}
/*---------------------------------------------------------------------------*/
/* Name		:led_flashing(void
/* Input	:	NO
/* Output	:	NO
/* Description:	延时函数
/*---------------------------------------------------------------------------*/
void beep_pro(void)
{
	;
}



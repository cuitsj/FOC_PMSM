/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FocControl.c
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

/* Private variables ----------------------------------------------------------------------------*/
MotStaType mcState;
MotStaTim  MotorStateTime;
CurrentOffset mcCurOffset;	
TimeCnt Time;

/*---------------------------------------------------------------------------*/
/* Name		:	void MC_Control(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	电机状态机函数，包括初始化、预充电、顺风逆风判断、预定位、启动、运行、故障等
/*---------------------------------------------------------------------------*/
void MC_Control(void)
{
	switch(mcState)
	{
		case mcInit:                          // 初始化状态，进入mcCharge状态
		{

			VariablesPreInit();                 // 电机相关变量初始化
			FOC_Init();							 // FOC的相关初始化
			PI_Init();			                  // PI初始化
			mcState =  mcCharge;                // 跳入mcCharge状态
			Time.Charge_Count = Charge_Time;    // 给定预充电时间
			break;
		}
		case mcCharge:                        // 预充电状态，MCU输出固定频率占空比，预充电结束后，
		{	                                    // 进入mcAlign或mcTailWind状态，若不进入下一个状态，可以用于IPM或MOS测试

			Motor_Charge();                     // 预充电配置
			#if (IPMState == IPMtest)	          // IPM测试或者MOS测试，MCU输出固定占空比			
			{	   
				
			}
			#elif (IPMState == NormalRun)		          // 正常按电机状态机运行
			{
				if(Time.Charge_Count == 0)			     
				{	
				#if (TailWind_Mode == NoTailWind)	  // 无逆风顺风判断程序，直接预定位				
						{	
							#if (PosCheckEnable)
							{	
									mcState 					 = mcPosiCheck;
									MotorStateTime.PosiCheckOneTime	= 0;	
									mcFocCtrl.mcPosCheckAngle		    = 0xffff;						
							}
							#else
							{
							    mcState                          = mcAlign;
							    Time.Count                       = Align_Time;	
							}
							#endif							
						}
				#elif (TailWind_Mode == TailWind)		// 加入逆风顺风判断程序			
						{														
//									/*刹车程序-------------------------------*/
//									//避免 高速顺风启动时  出现母线电压过冲
//									FOC_SWDUTY                      = 0.99*FOC_ARR;   
//									FOC_CR1                         = 0x06;	 					
//									FOC_CMR |= 0x3F;  
//									MOE = 1;   													
//									Time.TailWind_Count											= 200;																				
							    mcState                         = mcTailWind;
							    Time.TailWind_Count             = 0;									
						}
							#endif	
				}	
			}
			#endif		
			break;
		}
		case mcTailWind:                            // 顺风逆风程序判断，根据客户需求可调试加入，判断结束后，根据当前转速进入刹车预定位状态或直接运行                        
		if(Time.TailWind_Count == 0)		 // Time.TailWind_Count仅用作刹车计时
		{				
			Motor_TailWind();                // 初始化顺风启动检测
			TailWindDealwith();               // 顺逆风处理
		}
		break;
		 case mcPosiCheck:	
		{

			#if (PosCheckEnable)
			{				
				// 调用函数获取当前角度，若返回值为0xffff则表示位置检测未完成，否则返回值为当前角度
				mcFocCtrl.mcPosCheckAngle = PosCheckGetAngle();
				if(mcFocCtrl.mcPosCheckAngle != 0xffff)
				{		
					mcState    = mcAlign;
					Time.Count = Align_Time;
					TailWindDetect.AntiFlag	=1;				
				}
			}
			#else
			{				
				// 初始位置检测不使能时初始角度为预定位角度
				mcFocCtrl.mcPosCheckAngle = Align_Angle;
				mcState     = mcAlign;
				Time.Count  = Align_Time;
				TailWindDetect.AntiFlag	=1;
			}	
			#endif

			break;
		}			
		case mcAlign:                           
		{

			Motor_Align();                       // 预定位配置函数

			#if (!AlignTestMode)                 // 预定位时间结束后，直接启动
			{
				if(Time.Count == 0)
				{
					mcState = mcStart;	
					Time.Stop_Count  = 10000;
				}
				
				mcSpeedRamp.IncValue         = 15;  // 速度环增量
				mcSpeedRamp.DecValue         = 60;  // 速度环减量
				
			}
			#else
			{
				while(1);                           // 适用于初始位置检测调试用
			}
			#endif 						
			break;
		}
		case mcStart:                           // 启动程序，配置电机启动参数，启动结束后，进入运行状态。
		{

			Motor_Open();                         // 启动配置函数
			break;
		}
		case mcRun:                             // 运行状态，若运行状态的给定变为0，进入mcStop状态。
		{		

				if(mcSpeedRamp.TargetValue==0)
				{
					mcState = mcStop; 
					Time.Stop_Count  = 10000;
				}	
				
				mcSpeedRamp.IncValue              = 40;//
				mcSpeedRamp.DecValue              = 40;
				
			break;
		}
		case mcStop:                             
		{	

			if((mcFocCtrl.mcSpeedFlt<Motor_Min_Speed)||(Time.Stop_Count==0))// 电机慢慢停机，当低于Motor_Min_Speed时，关闭输出
			{
				FOC_CR1 = 0x00;
				FOCEN	= Disable;																	
				mcState = mcwait;					
			}
			else if(mcSpeedRamp.TargetValue > 0)  // 若此时重给信号，则跳入运行状态
			{
				mcState = mcRun;	
				mcFocCtrl.CtrlMode= 0;
				FOC_IQREF = IQ_RUN_CURRENT;								
			}
			break;
		}
		case mcwait:                               // 关闭输出，当启动信号产生时，则跳入初始化程序中。
		{

			
			FOC_CR1 = 0x00;
			FOCEN		= Disable;																				
			MOE			= Disable;
			if(mcSpeedRamp.FlagONOFF == 1)
			{
				mcState = mcInit;
			}
		}
		break;
		
		case mcFault:
			break;

		default:
			break;
	}
}

void GetCurrentOffset(void)
{
	SetBit(ADC_STA, ADCBSY, 1);		                           // 使能ADC		
	while(GetBit(ADC_STA, ADCBSY)	);
	#if (Shunt_Resistor_Mode == Double_Resistor)					   // 双电阻模式，上电验证硬件电路时，需观察mcCurOffset.IaOffset、mcCurOffset.IbOffset是否为4096
	{
		mcCurOffset.IaOffsetSum+=((ADC0_DR& 0x0fff) << 3);			//换算为Q15格式
		mcCurOffset.IaOffset = mcCurOffset.IaOffsetSum >> 4;		//理论上Iaoffset = 32767 / 8 = 4096;
		mcCurOffset.IaOffsetSum -= mcCurOffset.IaOffset;				//理论上IaOffsetSum = 32767 - 4096 = 28671
		
		mcCurOffset.IbOffsetSum+=((ADC1_DR& 0x0fff) << 3);
		mcCurOffset.IbOffset = mcCurOffset.IbOffsetSum >> 4;
		mcCurOffset.IbOffsetSum -= mcCurOffset.IbOffset;
	}
	#elif (Shunt_Resistor_Mode == Single_Resistor)			     // 单电阻模式，上电验证硬件电路时，需观察mcCurOffset.IbusOffset是否为4096	
	{
		mcCurOffset.IbusOffsetSum+=((ADC4_DR& 0x0fff) << 3);
		mcCurOffset.IbusOffset = mcCurOffset.IbusOffsetSum >> 4;
		mcCurOffset.IbusOffsetSum -= mcCurOffset.IbusOffset;		//母线电流采样上的偏置电压
	}
	#endif	
	mcCurOffset.OffsetCount++;
	if(mcCurOffset.OffsetCount>mcCurOffset.OffsetTimes)
	{
		mcCurOffset.OffsetFlag=1;
	}
}
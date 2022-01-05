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

/*---------------------------------------------------------------------------*/
/* Name		:	void FOC_Init(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	mcInit状态下，对FOC的相关寄存器进行配置
/*---------------------------------------------------------------------------*/
void FOC_Init(void)
{
	/*deadtime set*/	
	/* 死区时间设置 */
	TIM0_DTR 			= PWM_LOAD_DEADTIME;				// Deadtime = 0x18/24MHZ = 1us
	
	/*enable FOC register set*/
	/* FOC使能 */
	FOCEN					= Enable;						// enable FOC module
	
	CLR(FOC_SR,FUIF);                 // 清除FOC中断标志位
	SET(FOC_IER,FUIE);                // 使能FOC中断
		
	PFOC1 = 1;                        // 中断优先级设置为2，优先级低于FO硬件过流
	PFOC0 = 0;
		
	/*set FOC register*/
	/* FOC寄存器 初始化设置 */
	FOC_CR1 		= 0;			// clear FOC_CR1
	FOC_CR2 		= 0;			// clear FOC_CR2
	FOC_IDREF 		= 0;			// clear Id						IDREF参考值清0 
	FOC_IQREF 		= 0;			// clear Iq						IQREF参考值清0
	
	FOC_THETA 		= 0;			// clear theta					THETA参考值清0
	FOC_RTHEACC 	= 0;			// 爬坡函数的初始加速度
	FOC_RTHESTEP 	= 0;			// 0 degree acce speed			爬坡初始速度清0
	FOC_RTHECNT 	= 0;			// acce time					爬坡次数清0
	FOC_THECOMP 	= 0x0000;		// smo estimater angle comp		角度补偿值清0
	FOC_THECOR 		= 0x0003;		// Theta error compensate		角度切换修正值 为1，该寄存器的MSB和LSB恒为1，即 1xxx xxxx xxxx xxx1

	FOC_ARR 		= PWM_VALUE_LOAD;   // set 16Khz carry frequency			计数器重载值，决定载波周期（频率)
	FOC_POWKLPF 	= POWER_KLQFP;		// set FOC power cacl filter value		低通滤波系数
	
	/*current loop parameter set*/
	/* 电流环 PI控制器 参数设置 */
	FOC_DKP 			= DKP;			// D轴 比例系数
	FOC_DKI 			= DKI;			// D轴 积分系数
	FOC_DMAX 			= DOUTMAX;		// D轴 输出 最大限幅
	FOC_DMIN 			= DOUTMIN;		// D轴 输出 最小限幅
	
	FOC_QKP 			= QKP;			// Q轴 比例系数
	FOC_QKI 			= QKI;			// Q轴 积分系数
	FOC_QMAX 			= QOUTMAX;		// Q轴 输出 最大限幅
	FOC_QMIN 			= QOUTMIN;		// Q轴 输出 最小限幅
		
	/*estimate parameter set*/
	/* 估算器 参数设置 */
	FOC_EK1 			= OBS_K1T;		// 估算器估算电流的 第一个系数
	FOC_EK2 			= OBS_K2T;		// 估算器估算电流的 第二个系数
	FOC_EK3 			= OBS_K3T;		// 估算器估算电流的 第三个系数
	FOC_EK4 			= OBS_K4T;		// 估算器估算电流的 第四个系数
	
	FOC_FBASE 		= OBS_FBASE;		// 估算器 由OMEGA算出角度增量DELTA THETA的系数			
	FOC_OMEKLPF 	= SPEED_KLPF;		// 估算器 速度计算的低通滤波系数			
	FOC_EBMFK 		= OBS_KLPF;			// 估算器 反电动势低通滤波器系数KLPF的系数 			
	FOC_EKP 			= OBSW_KP_GAIN;	// 估算器 PI控制器 KP系数				
	FOC_EKI 			= OBSW_KI_GAIN;	// 估算器 PI控制器 KI系数

	SetBit(FOC_SET1,FOCFR,motor_FR);		// 设置F/R

//	FocSpi_Init();                    //Foc的SPI初始化,用于观察FOC里的变量	
	
	/*driver mode and active level set*/
	/* 驱动模式 和 有效电平 设置 */
	#if (DriverMode == Driver_3P3N)				// FOC 3P3N mode，一般使用于低压
	{		
		SetBit(FOC_CMR,CCPH,0);					// 上管：high level 有效
		SetBit(FOC_CMR,CCPL,0);					// 下管：high level 有效
	}
	#elif (DriverMode == Driver_6N)				// FOC 6N mode，一般适用于高压
	{
		#if (PWM_Level_Mode == High_Level)										
		{
			SetBit(FOC_CMR,CCPH,0);				// 上管：high level 有效
			SetBit(FOC_CMR,CCPL,0);				// 下管：high level 有效		                               
		}
		#elif (PWM_Level_Mode == Low_Level)										
		{
			SetBit(FOC_CMR,CCPH,1);				// 上管：low level 有效
			SetBit(FOC_CMR,CCPL,1);				// 下管：low level 有效
		}
		#endif
	}
	#endif
	
	/*Estimate Algorithm set*/
	/* 估算器 模式设置*/
	#if (EstimateAlgorithm == SMO)			// SMO  滑模模式
	{
		FOC_KSLIDE    = OBS_KSLIDE;			// 估算器 KSLIDE系数
		FOC_EKLPFMIN	= OBS_EA_KS;		// 估算器 反电动势低筒滤波系数的最小值																		                              
	}
	#elif (EstimateAlgorithm == PLL)		// PLL  PLL模式		
	{		
		FOC_KSLIDE   	= 	OBSE_PLLKP_GAIN;	// PLL的 PI控制器 的KP系数
		FOC_EKLPFMIN	= 	OBSE_PLLKI_GAIN;	// PLL的 PI控制器 的KI系数																	
	}
	#endif

	/*estimate omega mode set*/
	#if (Estimate_Omega_Mode == Sin_Theta)			// sin和PI方式计算角度
	{
			SetBit(FOC_CR2,ESCMS,0);															
	}
	#elif (Estimate_Omega_Mode == Arctan_Theta)		// arctan计算角度
	{
			SetBit(FOC_CR2,ESCMS,1);		
	}
	#endif
	
	/*valpha mode set*/
	#if (Valpha_Mode == DC_Bus_Calc)												//  Valpha、Vbeta用DC_bus计算
	{
			SetBit(FOC_CR2,EIVMS,0);		
	}
	#elif (Valpha_Mode == Ipark_Calc)												//  Valpha、Vbeta用Ipark计算
	{
			SetBit(FOC_CR2,EIVMS,1);
	}
	#endif	
	
	/*sample shunt resister mode set*/
	/*双电阻采样，不需要最小采样窗口，即为0，在下降沿结束前开始采样Ia，配置8009*/
	#if (Shunt_Resistor_Mode == Double_Resistor)		// double resistor sample
	{
		SetBit(FOC_CR1,CSM,1);							// 双电阻
		FOC_TSMIN = 0;									// clear min windows time   清除最小窗口时间
		FOC_TRGDLY = 0x8003;							// ADC采样的时刻，FOC_TRGDLY[15]=0,上升区间；=1，下降区间。 8003为下降沿结束前41.67ns*3=0.125us 开始采样Ia， 
		                                                // 0001为上升沿开始时开始采样，建议下降沿结束前采样	
		/*double resister SVPWM 5/7 segment*/
		#if (SVPMW_Mode == SVPWM_7_Segment)
		{
			SetBit(FOC_CR2,F5SEG,0);					// 7段式SVPWM
		}
		#elif (SVPMW_Mode == SVPWM_5_Segment)
		{
			SetBit(FOC_CR2,F5SEG,1);					// 5段式SVPWM
		}
		#endif
		
		/*double resister sample period*/
		#if (DouRes_Sample_Mode == DouRes_1_Cycle)
		{
			SetBit(FOC_CR2,DSS,0);						// 双电阻模式下1周期采样
		}
		#elif (DouRes_Sample_Mode == DouRes_2_Cycle)
		{
			SetBit(FOC_CR2,DSS,1);						// 双电阻模式下2周期采样
		}
		#endif
	}
		/*单电阻采样；需要最小采样窗,FOC_TRGDLY为0，七段式SVPWM方式*/
	#elif (Shunt_Resistor_Mode == Single_Resistor)					// signel resistor sample 
	{
		SetBit(FOC_CR1,CSM,0);										// 单电阻
		FOC_TSMIN = PWM_TS_LOAD;									// 最小采样窗口
		FOC_TRGDLY = 7;	//-7											// 采集触发延迟  +为滞后采样，-为提前采样
		SetBit(FOC_CR2,F5SEG,0);									// 7段式
	}
	#endif	
	#if (CalibENDIS == Enable)	                            // 使能电流基准校正				
	{
		if(mcCurOffset.OffsetFlag==1)
		{
			#if (Shunt_Resistor_Mode == Double_Resistor)			// 双电阻校正		
			{
						/*set ia, ib current sample offset*/
						SET(FOC_CHC,CSOC0);
						CLR(FOC_CHC,CSOC1);
				
						if (motor_FR	== CW)
						{
							FOC_CSO = mcCurOffset.IaOffset;
						}
						else			
						{
							FOC_CSO = mcCurOffset.IbOffset;  
						}

																														
						SET(FOC_CHC,CSOC1);
						CLR(FOC_CHC,CSOC0);
						if (motor_FR	== CW)
						{
							FOC_CSO = mcCurOffset.IbOffset;
						}
						else			
						{
							FOC_CSO = mcCurOffset.IaOffset;  
						}

				
						
				
			}
			#elif (Shunt_Resistor_Mode == Single_Resistor)	  // 单电阻校正						
			{
						/*set ibus current sample offset*/
						SET(FOC_CHC,CSOC1);
						SET(FOC_CHC,CSOC0);
						FOC_CSO = mcCurOffset.IbusOffset;	              // 写入IC的偏置        
			}
			#endif	
		}
	}
	#elif (Shunt_Resistor_Mode == Single_Resistor)					
	{
	}
	#endif	

	
	#if (PWM_Level_Mode == High_Level)										
	{                               
	}
	#elif (PWM_Level_Mode == Low_Level)										
	{
	  /*enable FD6536 output*/
		SetBit(P1_OE, P10, 1);	// config P10 as output
		SetBit(P1_PU, P10, 1);  // enable P10 pull up
		GP10 = 1;
	}
	#endif	

}
/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Charge(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	预充电，当一直处于预充电状态下，不接电机，可用于验证IPM或者Mos
/*---------------------------------------------------------------------------*/
void Motor_Charge(void)
{
     if(MotorStateTime.ChargeOneTime==0)				// 首次预充电 自锁开关
		 {																					
				MotorStateTime.ChargeOneTime = 1;		// 闭锁，保证 预充电只进入一次		 
				FOC_SWDUTY = 0.7 * FOC_ARR;				// IPM 30% duty
				FOC_CR1 = 0x06;							// FOC计数器使能，软件写PWM占空比
				Time.Charge_Count = Charge_Time;
		 }
		 if(Time.Charge_Count == Charge_Time) 	 // 3/3 Charge_Time
		 {
			 #if (IPMState == IPMtest)	 
			 {
			   FOC_CMR |= 0x03;                         // U相输出		/* 通道1上下桥开启 */
			 }			 
			 #elif (IPMState == NormalRun)		          // 正常按电机状态机运行
			 {			 
			   FOC_CMR |= 0x01;                         // U相下桥臂通   /* 下桥通道1开启 */
			 }
			 #endif	
			 MOE = 1;																								
		 }
		 if(Time.Charge_Count == (Charge_Time<<1)/3) // 2/3 Charge_Time
		 {
			 #if (IPMState == IPMtest)	 
			 {
         FOC_CMR |= 0x0F;                         // U、V相输出   /* 通道1、2上下桥开启 */
			 }			 
			 #elif (IPMState == NormalRun)		  // 正常按电机状态机运行
			 {
         FOC_CMR |= 0x05;                         // U、V相下桥臂导通 		/* 通道1、2上下桥开启 */
			 }  
       #endif				 
		 }
		 if(Time.Charge_Count == Charge_Time/3) // 1/3 Charge_Time
		 {
			 #if (IPMState == IPMtest)	 
			 {
         FOC_CMR |= 0x3F;                         // U、V、W相输出 		/* 通道1、2、3上下桥开启 */
	     }			 
			 #elif (IPMState == NormalRun)		   // 正常按电机状态机运行
			 {				 
			  FOC_CMR |= 0x15;                     // U、V、W相下桥臂导通  	/* 通道1、2、3上下桥开启 */
			 }  
       #endif	
		 }
}
/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Align(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	预定位函数，当无逆风判断时，采用预定位固定初始位置;当有逆风判断时，采用预定位刹车
/*---------------------------------------------------------------------------*/
void Motor_Align(void)
{
	if(MotorStateTime.AlginOneTime==0)		// 首次预定位 自锁开关
	{
		 MotorStateTime.AlginOneTime=1;		// 闭锁，保证 预定位只进入一次
		
			/*enable FOC register set*/	
			FOCEN		= Disable;			// disable FOC Module	关闭 FOC模块
			FOC_Init();
			FOC_CMR |= 0x3F;                // U、V、W相输出使能 ，且上下桥 低电平有效 
			MOE = 1;						// 主输出UVW 来源于TIMER0/TIMER1/FOC模块的输出，TIM0输出来源于 TIMER0 通道4输出T0_OC4
		
			FOC_IDREF = ID_Align_CURRENT;
			FOC_IQREF = IQ_Align_CURRENT ;			
			
			FOC_EKP 	= OBSW_KP_GAIN;					
			FOC_EKI 	= OBSW_KI_GAIN;	
			
			FOC_DKP = DKP_Alignment;	
			FOC_DKI = DKI_Alignment;
			FOC_QKP = QKP_Alignment;	
			FOC_QKI = QKI_Alignment;		
			
			FOC_RTHEACC 	= 0x0000;																	// 爬坡函数的初始加速度
			FOC_RTHESTEP 	= 0x0000;																	// 0.62 degree acce speed

			#if (AlignTestMode)
			{
				FOC_THETA		= _Q15((float)Align_Angle/180.0); 
			}
			#else
			{
				FOC_THETA 	= _Q15((float)mcFocCtrl.mcPosCheckAngle/180.0);// alginment angle
			}
			#endif
			
//			SetBit(FOC_CR1,EFAE,0);															    // 禁止估算器强制输出
//			SetBit(FOC_CR1,RFAE,1);															    // 使能强拉
			
			/*sample shunt resister mode set*/
			#if (Shunt_Resistor_Mode == Double_Resistor)						// 双电阻
			{
				FOC_CR1 	= 0x0D;																			// 逆风判断已经使能FOC工作																		                            
			}
			#elif (Shunt_Resistor_Mode == Single_Resistor)					// 单电阻
			{
				FOC_CR1 	= 0x05;																		
			}
			#endif	
			/*Estimate Algorithm set*/
			#if (EstimateAlgorithm == SMO)					             	  // SMO
			{
				SetBit(FOC_CR1,ESEL,0);																		                              
			}
			#elif (EstimateAlgorithm == PLL)		                    // PLL		
			{
				SetBit(FOC_CR1,ESEL,1);																				
			}
			#endif		    
			
  }	
}

/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Open(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	开环启动的参数配置
/*---------------------------------------------------------------------------*/
void Motor_Open(void)
{
		static uint8 OpenRampCycles;	
    if(MotorStateTime.OpenOneTime==0)
		{
			MotorStateTime.OpenOneTime=1;
			
			SetBit(FOC_CR1,FCE,1);
			SetBit(FOC_CR1,PWMSS,0);
			
			#if (PosCheckEnable)
			{

				FOC_THETA 	= _Q15((float)mcFocCtrl.mcPosCheckAngle/180.0);//有初始位置检测，则用初始位置角
			}
			#else
			{
				FOC_THETA		= _Q15((float)Align_Angle/180.0); //无初始位置检测，则用预定位角
			}
			#endif			

			if(TailWindDetect.TailWindBrakeCount>=4)	
			{
				FOC_IDREF = ID_Start_CURRENT;                         // D轴启动电流
				mcFocCtrl.mcIqref= IQ_Start_CURRENT+I_Value(0.4);	  // Q轴启动电流	
				FOC_IQREF = mcFocCtrl.mcIqref;		                    // Q轴启动电流
			}
			else if(TailWindDetect.TailWindBrakeCount>=1)	
			{
				FOC_IDREF = ID_Start_CURRENT;                         // D轴启动电流
				mcFocCtrl.mcIqref= IQ_Start_CURRENT+I_Value(0.3);		  // Q轴启动电流	
				FOC_IQREF = mcFocCtrl.mcIqref;		                    // Q轴启动电流
			}
			else
			{
				FOC_IDREF = ID_Start_CURRENT;                         // D轴启动电流
				mcFocCtrl.mcIqref= IQ_Start_CURRENT;		          // Q轴启动电流	
				FOC_IQREF = mcFocCtrl.mcIqref;		                  // Q轴启动电流
      }
			
			FOC_DKP = DKP;		//D轴 比例系数
			FOC_DKI = DKI;		//D轴 积分系数	
			FOC_QKP = QKP;		//Q轴 比例系数	
			FOC_QKI = QKI;		//Q轴 积分系数	
			
			FOC_EKP 	= OBSW_KP_GAIN;		//	与 ATO_BW 成正比，只需改动 ATO_BW			
			FOC_EKI 	= OBSW_KI_GAIN;		//	与 ATO_BW 成正比，只需改动 ATO_BW
					         									
			/*Omega mode set*/
			#if (Open_Start_Mode == Omega_Start)            //Omega 启动
			{
				FOC_EFREQACC 	= Motor_Omega_Ramp_ACC;		// omega 启动的增量	  1~20
				FOC_EFREQMIN 	= Motor_Omega_Ramp_Min;		// omega 启动的最小切换转速(RPM)
				FOC_EFREQHOLD   = Motor_Omega_Ramp_End;		// omega 启动的限制转速(RPM)
				
				SetBit(FOC_CR1,EFAE,1);															// 估算器强制输出
				SetBit(FOC_CR1,RFAE,0);															// 禁止强拉
				SetBit(FOC_CR1,ANGM,1);															// 估算模式                             
			}
			
			/*open mode set*/
			#elif (Open_Start_Mode == Open_Start)
			{
				FOC_RTHEACC 	= MOTOR_OPEN_ACC;								// 爬坡函数的初始加速度
				FOC_RTHESTEP 	= Motor_Open_Ramp_Min;								// 0.62 degree acce speed
				FOC_RTHECNT 	= MOTOR_OPEN_ACC_CNT;									// acce time			
				
				SetBit(FOC_CR1,EFAE,0);															// 禁止估算器强制输出
				SetBit(FOC_CR1,RFAE,1);															// 使能强拉
				SetBit(FOC_CR1,ANGM,1);															// 估算模式
			}
			#elif (Open_Start_Mode == Open_Omega_Start)
			{
				FOC_RTHEACC 	= MOTOR_OPEN_ACC;								// 爬坡函数的初始加速度
				FOC_RTHESTEP 	= Motor_Open_Ramp_Min;								// 0.62 degree acce speed
				FOC_RTHECNT 	= MOTOR_OPEN_ACC_CNT;									// acce time			
				
				FOC_EFREQACC 	= Motor_Omega_Ramp_ACC;
				FOC_EFREQMIN 	= Motor_Omega_Ramp_Min;
				FOC_EFREQHOLD = Motor_Omega_Ramp_End;	
				
				SetBit(FOC_CR1,EFAE,1);															// 禁止估算器强制输出
				SetBit(FOC_CR1,RFAE,1);															// 使能强拉
				SetBit(FOC_CR1,ANGM,1);															// 估算模式
			}
			#endif
	  }	
			//OPEN状态启动时拖动多次
		  #if (Open_Start_Mode == Open_Start)
			{
				if(OpenRampCycles<(MOTOR_OPEN_ACC_CYCLE-1))
				{
					if(!GetBit(FOC_CR1,RFAE))
					{				
						SetBit(FOC_CR1,RFAE,1);			
						OpenRampCycles++;	
					}
				}		
				else
				{			
					Time.Count = 2;	
			    FOC_EKP = OBSW_KP_GAIN_RUN7;	                        // 估算器里的PI的KP			
				  FOC_EKI	= OBSW_KI_GAIN_RUN7;				                  // 估算器里的PI的KI						
					mcState = mcRun;
				}	
			}
			#elif (Open_Start_Mode == Open_Omega_Start)
			{		
					Time.Count = 2600;					
					mcState = mcRun;
			}
			#elif (Open_Start_Mode == Omega_Start) 
			{			
				Time.Count = 2500;				 
				mcState = mcRun;			
			}			
			#endif
}
/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Align(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	顺风逆风参数配置函数
/*---------------------------------------------------------------------------*/
void TailWindDealwith(void)
{
	
		   // 正转且转速计数溢出时开环启动，或当计数溢出或者1-3，3-1次数超过2，说明当前电机静止
				if(((TailWindDetect.MotorDir == CW)&&(TailWindDetect.SpeedOverFlowStatus))||(TailWindDetect.TimerClearTimes>=3))
				{
					mcState 			 = mcPosiCheck;
					MotorStateTime.PosiCheckOneTime	= 0;
				
				}
				else if(TailWindDetect.MotorDir == CCW)
				{
														
					//连续刹车MinBrakeTimes次且转速低于BrakeStartSpeed时强制开环启动
					
						FOC_SWDUTY                      = 0.99*FOC_ARR;   
						FOC_CR1                         = 0x06;	 				// enable break function			
						//根据不同速度设置刹车时间
						if(TailWindDetect.TailWindSpeed<50)//rpm/min
						{
							Time.TailWind_Count											= 300;
							 CCWstate = 1;
								
						}
						else if(TailWindDetect.TailWindSpeed<300)
						{
							Time.TailWind_Count											= 500;	
							 CCWstate = 2;
							
						}
						else if(TailWindDetect.TailWindSpeed<600)
						{
							Time.TailWind_Count											= 900;
							CCWstate = 3;							
						}
						else
						{
							Time.TailWind_Count											= 4000;	
							CCWstate = 4;
						}
						
						TailWindDetect.TailWindBrakeCount ++;							//刹车次数累计	
            if((TailWindDetect.TailWindBrakeCount>=4)&&(TailWindDetect.TailWindSpeed<100))	
						{	
							mcState 												= mcPosiCheck;
							MotorStateTime.PosiCheckOneTime	= 0;	
					 						
						}
						else if(TailWindDetect.TailWindBrakeCount>6)
						{
							 mcFaultSource=FaultOverwind;
					     mcState = mcFault;					
					     FaultProcess();		
							
						}
					MotorStateTime.TailWindOneTime 	= 0;
				}
				else if(TailWindDetect.MotorDir == CW)//正转，且速度大于一定，且连续三次判断都超过
				{	
					//超过检测次数TailWindDetect.SpeedStoreNum切转速TailWindStartMinSpeed以上时直接顺风启动
					if((TailWindDetect.TailWindSpeed >TailWindStartMinSpeed)&&(TailWindDetect.SpeedStoreNum > 3))
					{	
						FOCCloseLoopStart();		
				  							
					}
				}
	
}
/*---------------------------------------------------------------------------*/
/* Name		:	void Motor_Align(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	顺风逆风参数配置函数
/*---------------------------------------------------------------------------*/
void Motor_TailWind(void)
{
	if(MotorStateTime.TailWindOneTime ==0)
	{	
		MotorStateTime.TailWindOneTime=1;
		
		TailWindParInit();																		//顺逆风变量初始化
		
		FOCEN		= Disable;																	  // FOC模块先关闭后重新初始化
		FOC_Init();
		
		FOC_CMR |= 0x3F;                                      // U、V、W相输出  
		MOE = 1;			
		
		FOC_IDREF = 0;                                        // D轴给定电流
		FOC_IQREF = 0;                                        // Q轴给定电流
	
		FOC_DKP = DKP_TailWind;	                              // D轴KP
		FOC_DKI = DKI_TailWind;                               // D轴KI
		FOC_QKP = QKP_TailWind;	                              // Q轴KP
		FOC_QKI = QKI_TailWind;	                              // Q轴KI
		
    FOC_OMEKLPF = SPEED_KLPF_WIND;	                      // 估算器里的速度计算的低通滤波值
		
		FOC_EKP = OBSW_KP_GAIN_WIND;	                        // 估算器里的PI的KP			
		FOC_EKI	= OBSW_KI_GAIN_WIND;				                  // 估算器里的PI的KI	
	
		FOC_CR2 |= 0x80;							                        //  使能逆风检测
	
		/*sample shunt resister mode set*/
		#if (Shunt_Resistor_Mode == Double_Resistor)					
		{
			FOC_CR1 	= 0x0D;				                            // 使能双电阻，FOC计数器，启动FOC															                             
		}
		#elif (Shunt_Resistor_Mode == Single_Resistor)					
		{
			FOC_CR1 	= 0x05;						                        // 使能单电阻，FOC计数器，启动FOC															
		}
		#endif	
		
		/*Estimate Algorithm set*/
		#if (EstimateAlgorithm == SMO)					             	  // SMO
		{
			SetBit(FOC_CR1,ESEL,0);																		                              
		}
		#elif (EstimateAlgorithm == PLL)		                    // PLL		
		{
			SetBit(FOC_CR1,ESEL,1);	
			FOC_KSLIDE    = OBSE_PLLKP_GAIN;
			FOC_EKLPFMIN	= OBSE_PLLKI_GAIN;						

		}
		#endif	
	}	

}
/*---------------------------------------------------------------------------*/
/* Name		:	void MC_Stop(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	inital motor control parameter
/*---------------------------------------------------------------------------*/
void MC_Stop(void)
{
  FOC_SET0 &= 0x7F;																							// disable FOC and clear all register
	FOC_CR1 = 0x00;																					// disable FOC output and initial register
	mcState = mcInit;
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void MotorControlInit(void)
	Description   :	控制变量初始化清零,包括保护参数的初始化、电机状态初始化
	Input         :	输入说明（详细）
  Output				:	输出说明（详细）
-------------------------------------------------------------------------------------------------*/
void MotorcontrolInit(void)
{
	led_t    =0;     //LED闪烁
  key_trg  = 0;    //按键触发记录
  key_cont = 0;    //按键长按触发记录
  keydelay_tim  = 0;   //按键长按时间记录
  key_flag = 0;    //按键按下标志位
	beep_cnt = 0;
	key_data = 0;
  key_static = 0;
  keypres_static = 0;
  keyscanfiltertime = 0;
	
/***********保护******************/
/*****电压保护*****/
	mcFaultDect.mcOverVoltDetecFaultCount  = 0;
	mcFaultDect.mcUnderVoltDetecFaultCount = 0;
	mcFaultDect.mcVoltRecoverCount      = 0;
	
  NoLoad_Flag = 1;	
	
	
/*******过流保护*****************/	
	mcFaultDect.Abs_ia				          = 0;
	mcFaultDect.Abs_ib				          = 0;
	mcFaultDect.Abs_ic			            = 0;
	mcFaultDect.OverCurrentValue        = OverSoftCurrentValue;
	mcFaultDect.OverCurCnt              = 0;  
	mcFaultDect.currenttime             = 0;
	mcFaultDect.CurrentRecoverCnt       = 0;

/*******启动保护*****************/		
	mcFaultDect.SecondStart             = 0;
	mcFaultDect.mcStartDelay            = 0;	
	mcFaultDect.mcStartCnt              = 0;	
	mcFaultDect.mcStartFocmode          = 0;
	mcFaultDect.mcStartSpeedCnt         = 0;
	mcFaultDect.mcStartEsCnt            = 0;
	mcFaultDect.StartFlag								= 0;
		
/******堵转保护*********/	
	mcFaultDect.mcStallCount            = 0;	
	mcFaultDect.mcStallRecover          = 1000;//5s
	mcFaultDect.mcStallTimes            = 0;	
	mcFaultDect.mcStallDelaDectEs       = 0;
	mcFaultDect.mcStallDeSpeed          = 0;
	mcFaultDect.mcStallDelaDectSpd      = 0;
	mcFaultDect.mcStallDeCurrent        = 0;
	mcFaultDect.StallCurrentVale        = StallCurrentValue1;
	mcFaultDect.StallFlag               = 0;
	mcFaultDect.StallRecoverCurrent     = 2090;//堵转保护恢复值
	mcFaultDect.mcEsValue               = 0;	
	
/*******缺相保护*****************/		
	mcFaultDect.LphaseDelayCount        = 0;
	mcFaultDect.Lphasecnt               = 0;
	mcFaultDect.LowCurrent              = I_Value(0.03);//缺相电流判断条件(低电流条件)
	mcFaultDect.NomCurrent              = I_Value(0.08);;//缺相电流判断条件(高电流条件)
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.AOpencnt                = 0;
	mcFaultDect.BOpencnt                = 0;
	mcFaultDect.COpencnt                = 0;
	mcFaultDect.mcLossPHCount           = 0;	
	mcFaultDect.mcLossPHTimes           = 0;
	
/*****各状态参数初始化******/	
	MotorStateTime.ChargeOneTime       = 0;
	MotorStateTime.AlginOneTime        = 0;
	MotorStateTime.TailWindOneTime     = 0;
	MotorStateTime.OpenOneTime         = 0;	

/*****外部控制环*******/
  mcFocCtrl.CtrlMode                 = 0;
	mcFocCtrl.SpeedLoopTime            = 0;
	mcFocCtrl.PowerLoopTime            = 0;
	mcFocCtrl.RunCurrent               = 0;
	mcFocCtrl.mcIqref                  = 0;
	mcFocCtrl.TorqueLoopTime           = 0;
	mcFocCtrl.McuRef                   = 0;
	mcFocCtrl.Powerlpf                 = 0;


/******ADC采样滤波值*********/
	AdcSampleValue.ADCSpeed            = 0;  
	AdcSampleValue.ADCVref             = 0;	
	
/******母线电压滤波值*******/
  mcFocCtrl.mcDcbusFlt	                       = 0;
	
/******分段处理的变量*******/	
	segmentstate                       = 0;

/******状态机计数变量******/	
	Time.Charge_Count                  = 0;
  Time.TailWind_Count                = 0;
  Time.Break_Count                   = 0;
	Time.Stop_Count                    = 0;
	Time.Count                         = 0;
	Time.MotorRun_Count								 = 0;	
	Time.MODE_Flag  = 0;
	Time.START_Flag = 0;
/******电流偏置校准变量*****/		
	mcCurOffset.IaOffsetSum            = 16383;
	mcCurOffset.IaOffset               = 0;
	mcCurOffset.IbOffsetSum            = 16383;
	mcCurOffset.IbOffset               = 0;
	mcCurOffset.IbusOffsetSum          = 16383;
	mcCurOffset.IbusOffset             = 0;
	mcCurOffset.OffsetCount            = 0;
	mcCurOffset.OffsetFlag             = 0;
	mcCurOffset.OffsetTimes            = Calib_Time;
	
	/*****LED灯响应***/	
	mcLedDisplay.LedCount             = 0;
	mcLedDisplay.LedTimCot            = 0;
	mcLedDisplay.Count                = 0;
	mcLedDisplay.Counttime            = 4999;	
	
	/*****速度环的响应***/	
	mcSpeedRamp.TargetValue           = 0;
	mcSpeedRamp.ActualValue           = 0;
	mcSpeedRamp.IncValue              = 60;
	mcSpeedRamp.DecValue              = 60;
	mcSpeedRamp.DelayCount            = 0;
	mcSpeedRamp.DelayPeriod           = 2;

  /*****通信***/
	/**接收的数组**/	
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
	
	/**发送的数组******/
  Uart.T_DATA[0]                     = 0;
	Uart.T_DATA[1]                     = 0;
  Uart.T_DATA[2]                     = 0;
  Uart.T_DATA[3]                     = 0;
  Uart.T_DATA[4]                     = 0;	
  Uart.T_DATA[5]                     = 0;
	Uart.T_DATA[6]                     = 0;
  Uart.T_DATA[7]                     = 0;
  Uart.T_DATA[8]                     = 0;	
  Uart.T_DATA[9]                     = 0;
	
	Uart.Uredata                       = 0;
  Uart.UARxCnt                       = 0;
  Uart.RxFSM                         = 0;
  Uart.UsaRxLen                      = 0;
  Uart.flagUsaRxComm                 = 0;
  Uart.CheckDate                     = 0;
	Uart.ResponceCount                 = 0;
	Uart.ResponceFlag                  = 0;
	Uart.UsaTxlen                      = 0;
	Uart.SendCnt                       = 0;

  /**睡眠模式***/
	MotorSleepFlag                     = 0; 
	MotorSleepDelayCout                = 0;
	Sleep_En                           = 1;
	
	/***PWM调速****/
	mcPwmInput.PWMFlag                 = 0;
	mcPwmInput.PWMCount                = 0;
			
  /*****顺逆风判断*******/		
	TailWindDetect.TailWindBrakeCount  = 0;	
	TailWindDetect.AntiFlag            = 0;			
	mcFRState.FlagFR = 0;
	mcFRState.FRStatus = 0;	
	mcFRState.FR=0;
	
	
	/*****自定义变量初始化*****/
	ONOFF_Times=0;
	ON_Count=0;
	ONOFF_Flag=1;
	OFF_Count=0;	
	
	CCWstate = 0;
		
	RecEsVlue = 0; 
}

/*---------------------------------------------------------------------------*/
/* Name		:	void VariablesPreInit(void)
/* Input	:	NO
/* Output	:	NO
/* Description:	初始化电机参数
/*---------------------------------------------------------------------------*/
void VariablesPreInit(void)
{
	/***********保护******************/
	mcFaultSource = 0;
	
		Time.MotorRun_Count								 = 0;	
	
	  NoLoad_Flag = 1;
	
  /*****电压保护*****/
	mcFaultDect.mcOverVoltDetecFaultCount  = 0;
	mcFaultDect.mcUnderVoltDetecFaultCount = 0;
	mcFaultDect.mcVoltRecoverCount      = 0;

/*******过流保护*****************/	
	mcFaultDect.Abs_ia				          = 0;
	mcFaultDect.Abs_ib				          = 0;
	mcFaultDect.Abs_ic			            = 0;
	mcFaultDect.OverCurrentValue        = OverSoftCurrentValue;
	mcFaultDect.OverCurCnt              = 0;  
	mcFaultDect.currenttime             = 0;
  mcFaultDect.CurrentRecoverCnt       = 0;
	mcFaultDect.StartFlag								= 0;
	
	
/******堵转保护*********/	
	mcFaultDect.mcStallCount            = 0;	
	mcFaultDect.mcStallRecover          = 1000;//5s
	mcFaultDect.mcStallDelaDectEs       = 0;
	mcFaultDect.mcStallDeSpeed          = 0;
	mcFaultDect.mcStallDelaDectSpd      = 0;
	mcFaultDect.mcStallDeCurrent        = 0;
	mcFaultDect.StallCurrentVale        = StallCurrentValue1;
	mcFaultDect.StallFlag               = 0;
	mcFaultDect.StallRecoverCurrent     = 2090;
	mcFaultDect.mcEsValue               = 0;
	
/*******启动保护*****************/		
	mcFaultDect.mcStartDelay            = 0;	
	mcFaultDect.mcStartCnt              = 0;	
	mcFaultDect.mcStartFocmode          = 0;
	mcFaultDect.mcStartSpeedCnt         = 0;
	mcFaultDect.mcStartEsCnt            = 0;

/******缺相保护*********/	
	mcFaultDect.LphaseDelayCount        = 0;
	mcFaultDect.Lphasecnt               = 0;
	mcFaultDect.LowCurrent              = I_Value(0.03);//缺相电流判断条件(低电流条件)
	mcFaultDect.NomCurrent              = I_Value(0.08);;//缺相电流判断条件(高电流条件)
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.Max_ia                  = 0;
	mcFaultDect.AOpencnt                = 0;
	mcFaultDect.BOpencnt                = 0;
	mcFaultDect.COpencnt                = 0;
	mcFaultDect.mcLossPHCount           = 0;	
//	mcFaultDect.mcLossPHTimes           = 0;
	

/*****外部控制环*******/
    mcFocCtrl.CtrlMode                 = 0;
	mcFocCtrl.SpeedLoopTime            = 0;
	mcFocCtrl.PowerLoopTime            = 0;
	mcFocCtrl.RunCurrent               = 0;
	mcFocCtrl.mcIqref                  = 0;	
	mcFocCtrl.TorqueLoopTime           = 0;
	
/*****顺逆风判断*******/
	TailWindDetect.AntiFlag            = 0;		
	TailWindDetect.TailWindBrakeCount  = 0;
	
/*****电机状态机时序变量***********/	
	MotorStateTime.ChargeOneTime       = 0;
	MotorStateTime.AlginOneTime        = 0;
	MotorStateTime.TailWindOneTime     = 0;
	MotorStateTime.OpenOneTime         = 0;

	mcFocCtrl.mcSpeedFlt						   = 0;
	//memset(&ShowData,0,sizeof(OLEDShowData));
	ShowData.Ibus=0;
	ShowData.Vbus=0;
	ShowData.VSP=0;
	
	#if (PosCheckEnable)//初始位置检测使能
	{	
			//初始位置检测脉冲参数初始化
		PosCheckPWMVar.PWMONTimeUp 			 = PWM_ON_Time_Up;
		PosCheckPWMVar.PWMONTimeDown 		 = PWM_ON_Time_Down;	
		PosCheckPWMVar.PWMOFFTimeInput 		 = PWM_OFF_Time_Input;
		PosCheckPWMVar.PWMBrakeTimeInput 	 = PWM_Brake_Time_Input;
		PosCheckPWMVar.PWMLOADInput       	 = PWM_VALUE_LOAD;
		PosCheckPWMVar.PWMONDutyInput 		 = (PWM_VALUE_LOAD*(1-(PWM_Duty)));
		PosCheckPWMVar.PWMOFFDutyInput 		 = (PWM_VALUE_LOAD*(PWM_Duty));		
	
		//根据不同电压调整初始位置检测脉冲占空比
//		if(mcDcbusFlt < _Q15(183.0/HW_BOARD_VOLT_MAX_Actual))		//130*1.414
//		{	
//			PosCheckPWMVar.PWMONDutyInput 			 = (PWM_VALUE_LOAD*(1-(PWM_Duty+0.16)));
//			PosCheckPWMVar.PWMOFFDutyInput 			 = (PWM_VALUE_LOAD*(PWM_Duty+0.16));					
//		}
//		else if(mcDcbusFlt < _Q15(254.0/HW_BOARD_VOLT_MAX_Actual))	//180*1.414
//		{
//			PosCheckPWMVar.PWMONDutyInput 			 = (PWM_VALUE_LOAD*(1-(PWM_Duty+0.08)));
//			PosCheckPWMVar.PWMOFFDutyInput 			 = (PWM_VALUE_LOAD*(PWM_Duty+0.08));		
//		}
//		else
//		{
//			PosCheckPWMVar.PWMONDutyInput 			 = (PWM_VALUE_LOAD*(1-(PWM_Duty)));
//			PosCheckPWMVar.PWMOFFDutyInput 			 = (PWM_VALUE_LOAD*(PWM_Duty));					
//		}		
	}
	#endif
}

#endif
/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : Interrupt.c
* Author             : Fortiortech  Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the interrupt function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Name		:	void EXTERN_INT(void) interrupt 2
/* Input	:	NO
/* Output	:	NO
/* Description:	睡眠唤醒
/*---------------------------------------------------------------------------*/
void EXTERN_INT(void) interrupt 2   
{	
	if(MotorSleepFlag==1)
	{
		MotorSleepFlag=0;
		Sleep_En = 0;
	}
	CLR(P1IF, P11);                    // 清零P10标志位	
}
/*-------------------------------------------------------------------------------------------------
	Function Name :	void FO_INT(void)
	Description   :	FO_INT interrupt，硬件FO过流保护，关断输出，中断优先级最高
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void FO_INT(void) interrupt 1   	 	 // 硬件FO过流中断，关闭输出
{
	FaultProcess();	                   // 关闭输出
	mcFaultSource=FaultHardOVCurrent;  // 硬件过流保护
	mcState = mcFault;                 // 状态为mcFault
	IF0	= 0;						   // clear P00 interrupt flag
}
/*-------------------------------------------------------------------------------------------------
	Function Name :	void CMP_ISR(void)
	Description   :	CMP interrupt，硬件比较器过流保护，关断输出，中断优先级最高
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void CMP_ISR(void) interrupt 7
{

	
	if(GetBit(CMP_SR, CMP3INTR))
	{
		
		FaultProcess();	                   // 关闭输出
		mcFaultSource=FaultHardOVCurrent;  // 硬件过流保护
		mcState = mcFault;                 // 状态为mcFault
		CLR(CMP_SR, CMP3INTR);
	}
}
/*---------------------------------------------------------------------------*/
/* Name		:	void TIM0_INT(void) interrupt 4
/* Input	:	NO
/* Output	:	NO
/* Description:	用于初始位置检测
/*---------------------------------------------------------------------------*/
void TIM0_INT(void) interrupt 4
{
		if(GetBit(TIM0_SR1, CC4IF))
	  {
      PosCheckCurrentGet();
		  CLR(TIM0_SR1, CC4IF);	
	  }
}

/*---------------------------------------------------------------------------*/
/* Name		:	void TIM23_INT(void) interrupt 9
/* Input	:	NO
/* Output	:	NO
/* Description:	Capture PWM ，中断优先级第二，高于FOC中断，用于PWM调速
/*---------------------------------------------------------------------------*/
void TIM23_INT(void) interrupt 9
{
	CLR(TIM3_CR1,T3IR);
	if(GetBit(TIM3_CR1,T3IF))
	{
		if(GetBit(TIM3_CR1,T3IF))//溢出中断	,366Hz
		{
			if(1==GP11)//PWM 100%输出
			{
			  mcPwmInput.PWMCompare = 3300;
				mcPwmInput.PWMARR     = 3300;
			}
			else//PWM 为0%
			{
				mcPwmInput.PWMCompare = 0;
				mcPwmInput.PWMARR     = 3300;
		}
		
			mcPwmInput.PWMFlag    = 1;
			mcPwmInput.PWMCount   = 0;
			CLR(TIM3_CR1,T3IF);					
		}				

	}	
}
/*---------------------------------------------------------------------------*/
/* Name		:	void FOC_INT(void) interrupt 3
/* Input	:	NO
/* Output	:	NO
/* Description:	FOC中断，每个载波周期执行一次，用于处理响应较高的程序，中断优先级第二。
/*---------------------------------------------------------------------------*/
void FOC_INT(void) interrupt 3
{
	if(GetBit(FOC_SR,FUIF))                                 // 上溢中断
	{
		Fault_Overcurrent(&mcFaultDect);					// 软件过流保护	
		
		FGOutput();		// FG信号输出
		
	#if (BEMF_DETECT_ENABLE!=1)
	{
		TailWindSpeedDetect();
	}
	#endif
		CLR(FOC_SR,FUIF);                                   // 清零标志位
	}
	if(GetBit(FOC_SR,FCIF))                                 // 比较中断
	{
		CLR(FOC_SR, FCIF);	
	}
}

/*---------------------------------------------------------------------------*/
/* Name		:	void TIM45_INT(void) interrupt 10
/* Input	:	NO
/* Output	:	NO
/* Description:	1ms 定时器中断，用于处理附加功能，如控制环路响应、各种保护等。中断优先级低于FO中断和FOC中断。
/*---------------------------------------------------------------------------*/
void TIM45_INT(void) interrupt 10
{
	static uint8 i = 0;
	static uint16 key_count = 3000;
	
	if (Time.START_Flag == 0)
	{
		key_count--;
		if (key_count == 0)
		{
			Time.START_Flag = 1;//等待结束
		}
	}
	
	if(GetBit(TIM4_CR1,T4IF))								// TIM4 overflow interrupt	
	{
		SET(ADC_STA,ADCBSY);                                //使能ADC的DCBUS采样
		/****功率滤波*****/
		if(mcState==mcRun)	
		{	
//		 mcFocCtrl.Powerlpf=LPF(FOC_POW,mcFocCtrl.Powerlpf,400);
			mcFocCtrl.Powerlpf=FOC_POW;
		}	                                

		/****速度滤波*****/
		if((mcState!= mcInit)&&(mcState!= mcwait))
		{
			mcFocCtrl.mcSpeedFlt=LPF(FOC_EOME,mcFocCtrl.mcSpeedFlt,400); //硬件滤波
		}
		else
		{
			mcFocCtrl.mcSpeedFlt=0;
    }
	mcFocCtrl.mcDcbusFlt=LPF(ADC2_DR,mcFocCtrl.mcDcbusFlt,400);
	mcFocCtrl.mcIbusFlt=LPF(ADC4_DR,mcFocCtrl.mcIbusFlt,400);
	mcFocCtrl.Vspinput=ADC3_DR;
	i++;	
	if (i == 5)
	{
		i=0;
		OLED_P6x8Str(0,0,"mcSpeedFlt:");
		OLED_Print_Num1(66,0,mcFocCtrl.mcSpeedFlt/5.4);
		OLED_P6x8Str(0,1,"Powerlpf:");
		OLED_Print_Num1(66,1,mcFocCtrl.Powerlpf>>4);
		OLED_P6x8Str(0,2,"mcDcbusFlt:");
		OLED_Print_Num1(66,2,mcFocCtrl.mcDcbusFlt/9);
		OLED_P6x8Str(0,3,"mcIbusFlt:");
		OLED_Print_Num1(66,3,(mcFocCtrl.mcIbusFlt/9-225));
		OLED_P6x8Str(0,4,"Vspinput:");
		OLED_Print_Num1(66,4,4095-mcFocCtrl.Vspinput);
		OLED_P6x8Str(0,5,"motor_FR:");
		if (motor_FR == CW)
		{
			OLED_P6x8Str(66,5,"CCW");
		}
		else if (motor_FR == CCW)
		{
			OLED_P6x8Str(66,5,"CW ");
		}
		OLED_P6x8Str(0,6,"Count down:");
		OLED_Print_Num1(66,6,key_count);
	}
	/****环路响应，如速度环、功率环、转矩环****/
		Speed_response();    

		/*****故障保护函数功能，如过欠压保护、启动保护、缺相、堵转等********/
		Fault_Detection();
		
		/***LED灯显示****/
		LED_Display();
		/***led闪烁**/
		led_flashing();
		key_scan();
	

	/**** 启动-停止测试 ****/
	
	#if (ONOFFtestMode)
	{
		if(mcState != mcFault)
			ONOFF_Starttest();
		else
			;
	}
	 #endif
		
		
	 /****模拟调速****/
	 #if (SpeedMode==SREF_Speed)	                    //选择串口作为调试方式
	 {
		 
//		 		/******若上电就运行，则以下两行打开******/

//		 
		#if (!ONOFFtestMode)
		 {
//				 if(ONOFFPin == 1)
//				 {
//					 PWMInputCapture();
			//				VSPSample();				 
					 mcSpeedRamp.FlagONOFF = 1;		
//						mcSpeedRamp.TargetValue = _Q15(3000/MOTOR_SPEED_BASE)	;				 
						mcSpeedRamp.TargetValue = Motor_Max_Speed;//恒速度 
//				 }
		 }
		 #endif
//		mcFocCtrl.McuRef=9550;//恒功率
//		mcSpeedRamp.TargetValue=9550;//恒功率
	 }
	 #endif

	 /*****电机状态机的时序处理*****/	
	 
	 Time.MotorRun_Count++;
	 
		if(Time.Charge_Count > 0)      Time.Charge_Count--;
		if(Time.TailWind_Count  > 0)   Time.TailWind_Count--;
		if(Time.Break_Count  > 0)      Time.Break_Count--;
		if(Time.Count > 0)						 Time.Count--;
		if(Time.Stop_Count > 0)				 Time.Stop_Count--;	
		 
					
		//顺风启动速度下限限定
		if(mcState == mcTailWind)
		{
			//1Hz Min
			if(TailWindDetect.SpeedCountTimer<TailWind_Time)//SpeedCountTimer计算时间间隔
			{
				TailWindDetect.SpeedCountTimer++;
			}
			else
			{
				//溢出时速度
				TailWindDetect.TailWindSpeed = ThailWindSpeedBase_zhouqi/TailWind_Time;	
				TailWindDetect.SpeedCountTimer = 0;	
				TailWindDetect.SpeedOverFlowStatus = 1; 				//转速计数溢出标志				
			}
		 }
		/*****电机启动爬坡函数处理*****/	
		if((mcState == mcRun))
		{			
			if(Time.Count == 2500)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN;	                        // 估算器里的PI的KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN;				                  // 估算器里的PI的KI	
			}
			else if(Time.Count == 2000)
			{				
				FOC_EKP = OBSW_KP_GAIN_RUN1;	                        // 估算器里的PI的KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN1;				                  // 估算器里的PI的KI	
			}
			else if(Time.Count == 1600)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN2;	                        // 估算器里的PI的KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN2;				                  // 估算器里的PI的KI	
			}
			else if(Time.Count ==1400)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN3;	                        // 估算器里的PI的KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN3;				                  // 估算器里的PI的KI	
			}
			else if(Time.Count ==1300)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN4;	                        // 估算器里的PI的KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN4;				                  // 估算器里的PI的KI	
			}
			else if(Time.Count == 1200)
			{
				FOC_EKP = OBSW_KP_GAIN_RUN5;	                        // 估算器里的PI的KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN5;				                  // 估算器里的PI的KI	
			}
			else if(Time.Count == 600)
			{	
				FOC_EKP = OBSW_KP_GAIN_RUN6;	                        // 估算器里的PI的KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN6;				                  // 估算器里的PI的KI	
			}
			else if(Time.Count == 1)
			{	
				FOC_EKP = OBSW_KP_GAIN_RUN7;	                        // 估算器里的PI的KP			
				FOC_EKI	= OBSW_KI_GAIN_RUN7;				                  // 估算器里的PI的KI	
			}
			else;
		}
		CLR(TIM4_CR1,T4IF);                                    // 清零标志位
	}
}


/*---------------------------------------------------------------------------*/
/* Name		:	void USART_INT(void) interrupt 12
/* Input	:	NO
/* Output	:	NO
/* Description:	串口中断，中断优先级最低，用于接收调速信号,无中断插入时8us
/*---------------------------------------------------------------------------*/
void USART_INT(void)  interrupt 12
{
	if(TI==1)//发送完成中断标志置1
	{
		TI=0;	//发送完成中断标志位清零
   if(Uart.SendCnt<Uart.UsaTxlen)
		{
			Uart.SendCnt++;
			UART_SendData(Uart.T_DATA[Uart.SendCnt]);
    }
	}
	if(RI == 1)
	{
		RI = 0;
		Uart.Uredata= UT_DR;			//读接收数据
		
    Uart.R_DATA[Uart.UARxCnt]=Uart.Uredata;
		if(Uart.R_DATA[0] ==0xAA)
		{
			MotorSleepDelayCout=0;
			Uart.UARxCnt ++ ;
      if(Uart.R_DATA[1] ==0x55)	
			{
				Uart.R_DATA[1] =0;
				Uart.UARxCnt = 0;
      }				
		}
		else
		{
			 Uart.UARxCnt = 0;
		}
		if(Uart.UARxCnt>=10)
		{
			Uart.UARxCnt=0;
		  Uart.ResponceFlag=1;
    }
  }
}







/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : TimerInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains Timer Initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ----------------------------------------------------------------------------*/
PWMINCtl_TypeDef xdata PWMINCtl;


/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM0_Init(void)
	Description   :	TIM0初始化配置
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void TIM0_Init(void)
{
	SetBit(TIM0_CR, CEN, 0);					                       // 停止计数器	
	
/*-------------------------------------------------------------------------------------------------              
	时基单元
	CMS = 00：边沿对齐计数模式
	CMS = 01：中央对齐计数模式1，比较输出中断标志位在向下计数时置位
	CMS = 10：中央对齐计数模式2，比较输出中断标志位在向上计数时置位
	CMS = 11：中央对齐计数模式3，比较输出中断标志位在向上和向下计数时置位
-------------------------------------------------------------------------------------------------*/	
  /* 计数模式 */
	SetBit(TIM0_CR, DIR, 0);					                       // 0:向上计数，1:向下计数
	SetBit(TIM0_CR, CMS1, 1);
	SetBit(TIM0_CR, CMS0, 1);				                         // 00:边沿对齐计数模式，other:中央对齐计数模式
		
	/* ARR与转载使能 */
	SetBit(TIM0_CR, ARPE, 1);    		                         // 0:禁止ARR预装载，1:使能ARR预装载

	/* OPM模式 */
	SetBit(TIM0_CR, OPM, 0);        	                       // 0:禁止OPM模式，1:使能ARR预装载
	
	/* RCR、PSCR、ARR、CNTR赋值初始化 */
	TIM0_RCR = 1;	
	TIM0_PSCR = 0;								                           // Fck_cnt=Fck_psc/(TIM0_PSCR+1)
	TIM0_ARR = PWM_VALUE_LOAD;					
	TIM0_CNTR = 1;					
	
/*-------------------------------------------------------------------------------------------------
	输出单元
	OCiM = 000：冻结OCiREF，OCiREF保持原来状态不变
	OCiM = 001：匹配时OCiREF输出有效电平
	OCiM = 010：匹配时OCiREF输出无效电平
	OCiM = 011：匹配时OCiREF输出翻转电平
	OCiM = 100：OCiREF强制输出低电平,OCi强制输出无效电平
	OCiM = 101：OCiREF强制输出高电平,OCi强制输出有效电平
	OCiM = 110：PWM模式1，CNTR<CCRi输出有效电平，CNTR>CCRi输出无效电平
	OCiM = 111：PWM模式2，CNTR<CCRi输出无效电平，CNTR>CCRi输出有效电平
-------------------------------------------------------------------------------------------------*/	
	/* 比较控制位CCiE/CCiNE/CCiP/CCiNP/OCiM预装载使能 */
	SetBit(TIM0_CCER2, CCPC, 0);     	                       // 0:比较控制位预装载禁止，1:比较控制位预装载使能

  /* OC1通道配置 */
	SetBit(TIM0_CCMR1, OC1M2, 1);
	SetBit(TIM0_CCMR1, OC1M1, 1);		
	SetBit(TIM0_CCMR1, OC1M0, 1);		                         // OC1比较输出模式	
	SetBit(TIM0_CCMR1, OC1PE, 0);                            // OC1比较输出寄存器预装载使能
	#if (DriverMode == Driver_3P3N)
	{
		SetBit(TIM0_CCER1, CC1P, 1);                           // OC1输出极性，0：高有效，1：低有效
	}
	#elif (DriverMode == Driver_6N)
	{
		SetBit(TIM0_CCER1, CC1P, 0);                           // OC1输出极性，0：高有效，1：低有效
	}
	#endif
	SetBit(TIM0_CCER1, CC1E, 1);                             // OC1输出使能
	SetBit(TIM0_CCER1, CC1NP, 0);                            // OC1N输出极性，0：高有效，1：低有效
	SetBit(TIM0_CCER1, CC1NE, 1);                            // OC1N输出使能

	/* OC2通道配置 */
	SetBit(TIM0_CCMR1, OC2M2, 1);
	SetBit(TIM0_CCMR1, OC2M1, 1);
	SetBit(TIM0_CCMR1, OC2M0, 1);		                         // OC2比较输出模式	
	SetBit(TIM0_CCMR1, OC2PE, 0);		                         // OC2比较输出寄存器预装载使能
	#if (DriverMode == Driver_3P3N)
	{
		SetBit(TIM0_CCER1, CC2P, 1);                           // OC2输出极性，0：高有效，1：低有效
	}
	#elif (DriverMode == Driver_6N)
	{
		SetBit(TIM0_CCER1, CC2P, 0);                           // OC2输出极性，0：高有效，1：低有效
	}	
	#endif
	SetBit(TIM0_CCER1, CC2E, 1);                             // OC2输出使能
	SetBit(TIM0_CCER1, CC2NP, 0);                            // OC2N输出极性，0：高有效，1：低有效
	SetBit(TIM0_CCER1, CC2NE, 1);                            // OC2N输出使能

	/* OC3通道配置 */
	SetBit(TIM0_CCMR2, OC3M2, 1);
	SetBit(TIM0_CCMR2, OC3M1, 1);
	SetBit(TIM0_CCMR2, OC3M0, 1);		                         // OC3比较输出模式	
	SetBit(TIM0_CCMR2, OC3PE, 0);		                         // OC3比较输出寄存器预装载使能
	#if (DriverMode == Driver_3P3N)
	{
		SetBit(TIM0_CCER2, CC3P, 1);                           // OC3输出极性，0：高有效，1：低有效
	}
	#elif (DriverMode == Driver_6N)
	{
		SetBit(TIM0_CCER2, CC3P, 0);                           // OC3输出极性，0：高有效，1：低有效
	}
	#endif	
	SetBit(TIM0_CCER2, CC3E, 1);                             // OC3输出使能
	SetBit(TIM0_CCER2, CC3NP, 0);                            // OC3N输出极性，0：高有效，1：低有效
	SetBit(TIM0_CCER2, CC3NE, 1);                            // OC3N输出使能
		
	/* OC4通道配置 */
	SetBit(TIM0_CCMR2, OC4M2, 1);
	SetBit(TIM0_CCMR2, OC4M1, 1);
	SetBit(TIM0_CCMR2, OC4M0, 1);		                         // OC4比较输出模式	
	SetBit(TIM0_CCMR2, OC4PE, 0);		                         // OC4比较输出寄存器预装载使能
	SetBit(TIM0_CCER2, CC4P, 0);                             // OC4输出极性，0：高有效，1：低有效
	SetBit(TIM0_CCER2, CC4E, 1);                             // OC4输出使能
	

	/* 死区时间配置 */
	TIM0_DTR = 48;							                             // count 24, time = 24/24MHZ = 1us

	/* CCRi赋值初始化 */				
	TIM0_CCR4 = PWM_VALUE_LOAD/2;								
	TIM0_CCR3 = PWM_VALUE_LOAD;			
	TIM0_CCR2 = PWM_VALUE_LOAD;		
	TIM0_CCR1 = PWM_VALUE_LOAD;		
	
	/* 输出IO使能 */
	SetBit(DRV_CTL, OCS1, 1);					
	SetBit(DRV_CTL, OCS0, 0);						                       // OCi和OCiN选择TIM0输出		
	MOE = 0;
	OIS4 = 0;										                             // OC4空闲电平设置
	#if (DriverMode == Driver_3P3N)
	{
		OISWL = 0;											                       // OC3N空闲电平设置
		OISWH = 1;											                       // OC3空闲电平设置
		OISVL = 0;											                       // OC2N空闲电平设置
		OISVH = 1;											                       // OC2空闲电平设置
		OISUL = 0;											                       // OC1N空闲电平设置
		OISUH = 1;											                       // OC1空闲电平设置		
	}
	#elif (DriverMode == Driver_6N)
	{
		OISWL = 0;											                       // OC3N空闲电平设置
		OISWH = 0;											                       // OC3空闲电平设置
		OISVL = 0;											                       // OC2N空闲电平设置
		OISVH = 0;											                       // OC2空闲电平设置
		OISUL = 0;											                       // OC1N空闲电平设置
		OISUH = 0;											                       // OC1空闲电平设置		
	}		
	#endif
	
/*-------------------------------------------------------------------------------------------------
	事件/中断响应单元
	UEV更新事件：更新PSCR、ARR、CCRi
	COM更新事件：更新CCiE/CCiNE、CCiP/CCiNP、OCiM
-------------------------------------------------------------------------------------------------*/	
	/* UEV更新使能UEV更新请求源配置 */
	SetBit(TIM0_CR, URS, 0);                                 // 0：计数器溢出和置位UG，1：计数器溢出
	SetBit(TIM0_CR, UDIS, 0);                                // 0：允许UEV更新事件，1：禁止UEV更新事件
	
	/*中断状态标志清零*/
	CLR(TIM0_SR1, COMIF);    			                           // COM中断标志清零
	CLR(TIM0_SR1, CC4IF); 	 			                           // 比较通道4中断标志清零
	CLR(TIM0_SR1, CC3IF);    			                           // 比较通道3中断标志清零
	CLR(TIM0_SR1, CC2IF); 	 			                           // 比较通道2中断标志清零
	CLR(TIM0_SR1, CC1IF); 				                           // 比较通道1中断标志清零
	CLR(TIM0_SR1, UIF);      			                           // 更新事件中断标志清零
	
	/*中断使能*/
	SetBit(TIM0_IER, COMIE, 0);                              // COM中断使能
	SetBit(TIM0_IER, CC4IE, 1); 	                           // 比较通道4中断使能
	SetBit(TIM0_IER, CC3IE, 0);                              // 比较通道3中断使能
	SetBit(TIM0_IER, CC2IE, 0); 	                           // 比较通道2中断使能
	SetBit(TIM0_IER, CC1IE, 0); 	                           // 比较通道1中断使能
	SetBit(TIM0_IER, UIE, 1);                                // 更新事件中断使能
	PTIM01 = 1;
	PTIM00 = 1;
	EA = 1;
	
	/*事件发生*/
	SetBit(TIM0_EGR, COMG, 0);                               // COM事件
	SetBit(TIM0_EGR, CC4G, 0); 	 	                           // 比较通道4比较事件
	SetBit(TIM0_EGR, CC3G, 0);                               // 比较通道3比较事件
	SetBit(TIM0_EGR, CC2G, 0); 	                             // 比较通道2比较事件
	SetBit(TIM0_EGR, CC1G, 0); 	                             // 比较通道1比较事件
	SetBit(TIM0_EGR, UG, 0);                                 // 更新事件
	
	SetBit(TIM0_CR, CEN, 1);					                       // 启动计数器	

}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM1_Init(void)
	Description   :	TIM1初始化配置
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void TIM1_Init(void)
{
/*-------------------------------------------------------------------------------------------------
	噪声滤波使能及模式选择	
	TIM1_CR2 = 0x00;		4-machine cycle noise,3MHz
	TIM1_CR2 = 0x15;		8-machine cycle noise,1.5MHz
	TIM1_CR2 = 0x2a;		16-machine cycle noise,0.75MHz	
	TIM1_CR2 = 0x3f;		32-machine cycle noise,0.375MHz		
-------------------------------------------------------------------------------------------------*/
	TIM1_CR2 = 0x3f;
	SetBit(TIM1_CR0, INC2, 1);		                           // 0:禁止TI2噪声滤波,1:使能TI2噪声滤波
	SetBit(TIM1_CR0, INC1, 1);		                           // 0:禁止TI1噪声滤波,1:使能TI1噪声滤波
	SetBit(TIM1_CR0, INC0, 1);		                           // 0:禁止TI0噪声滤波,1:使能TI0噪声滤波
	
/*-------------------------------------------------------------------------------------------------
	输入信号选择
	0: TI0--COMP0_OUT    TI1--COMP1_OUT    TI2--COMP2_OUT
	1: TI0--P1.4_IN      TI1--P1.6_IN      TI2--P2.1_IN
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM1_CR2, TIS, 1);		
	
/*-------------------------------------------------------------------------------------------------
	输入极性检测
	00:不检测边沿	  01:上升沿检测	  10:下升沿检测   11:上/下升沿检测	
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM1_CR0, CPE1, 1);	
	SetBit(TIM1_CR0, CPE0, 1);			
	
/*边沿检测使能-----------------------------------------------------------------------------------*/
	SetBit(TIM1_CR1, CMPE, 0);			
	SetBit(TIM1_CR0, IEE2, 0);			                         // 0:禁止TI2边沿检测,1:使能TI2边沿检测
	SetBit(TIM1_CR0, IEE1, 0);			                         // 0:禁止TI1边沿检测,1:使能TI1边沿检测
	SetBit(TIM1_CR0, IEE0, 1);			                         // 0:禁止TI0边沿检测,1:使能TI0边沿检测
	SetBit(TIM1_CR1, CMPE, 0);		                           // 0:边沿检测比较,   1:位置检测比较

/*CPD值设置--------------------------------------------------------------------------------------*/
	SetBit(TIM1_CR1, CPD2, 0);
	SetBit(TIM1_CR1, CPD2, 0);
	SetBit(TIM1_CR1, CPD2, 0);				
	
/*RDA值设置--------------------------------------------------------------------------------------*/
	SetBit(TIM1_DRH, RDA2, 0);
	SetBit(TIM1_DRH, RDA1, 0);
	SetBit(TIM1_DRH, RDA0, 0);				
	
/*-------------------------------------------------------------------------------------------------
	数据更新模式设置
	000:软件写TIM1_BDBR更新TIM1_BDR
	001:PTimer下溢触发TIM1_BDBR更新TIM1_BDR
	010:位置检测输入触发TIM1_BDBR更新TIM1_BDR
	011:位置检测输入启动PTimer，PTimer下溢更新TIM1_BDR
	100:位置检测输入或者PTimer下溢更新TIM1_BDR
	101:位置检测比较单次有效输入或PTimer单次下溢更新TIM1_BDR
	110:位置检测比较单次有效输入触发TIM1_BDBR更新TIM1_BDR
	111:位置检测比较单次有效输入启动PTimer，PTimer下溢更新TIM1_DBR										
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM1_CR3, OPS2, 0);
	SetBit(TIM1_CR3, OPS1, 0);
	SetBit(TIM1_CR3, OPS0, 0);
	
/*Basic Timer & Preload Timer设置----------------------------------------------------------------*/
	SetBit(TIM1_CR3, BCEN, 0);			                         // 禁止定时器
	SetBit(TIM1_CR3, T1PSC3, 0);
	SetBit(TIM1_CR3, T1PSC2, 1);
	SetBit(TIM1_CR3, T1PSC1, 0);
	SetBit(TIM1_CR3, T1PSC0, 1);		                         // 定时器预分频: 32
	SetBit(TIM1_CR2, BRS, 1);	                               // 0:写入时序信号复位定时器,1:位置检测信号复位定时器
	TIM1_BARR = 60000;                                       // 80000us
	TIM1_BCNTR = 1;	
	SetBit(TIM1_CR3, BCEN, 1);
	
	SetBit(TIM1_CR1, RCEN, 0);
	SetBit(TIM1_CR1, ROPM, 1);			                         // 0:禁止OPM模式,1:使能OPM模式
	TIM1_RARR = 65535;
	TIM1_RCNTR = 65534;	
// 	SetBit(TIM1_CR1, RCEN, 1);
	
/*中断使能---------------------------------------------------------------------------------------*/
	TIM1_SR &= 0x0f;
	SetBit(TIM1_IER, BOIE, 1);			                         // TIM1基本定时器溢出中断	
	SetBit(TIM1_IER, RUIE, 1);			                         // TIM1重载定时器下溢中断
	SetBit(TIM1_IER, WTIE, 0);			                         // 写入时序中断
	SetBit(TIM1_IER, PDIE, 1);			                         // 位置检测中断
	SetBit(TIM1_IER, CPIE, 0);			                         // 比较中断
	EA = 1;												                           // 使能全局中断	
	PTIM11 = 1;
	PTIM10 = 0;										                           // TIM1中断优先级别为0


/*死区设置---------------------------------------------------------------------------------------*/
	TIM1_DTR = 48;                                           // DeadTime = (TIM1_DTR+1)/cpuclk, 

/*-------------------------------------------------------------------------------------------------
	PPG同步方式设置
	WTS = 00: 无同步，立即更新输出   
	WTS = 01: 上升沿同步输出   
	WTS = 10: 下降沿同步输出  
	WTS = 11: 上升/下降沿同步输出
-------------------------------------------------------------------------------------------------*/
    #if (DriverMode == Driver_3P3N)
  	{
  		SetBit(TIM1_CR1, WTS1, 1);
  		SetBit(TIM1_CR1, WTS0, 0);
  	}
  	#elif (DriverMode == Driver_6N)
  	{
  		SetBit(TIM1_CR1, WTS1, 0);
  		SetBit(TIM1_CR1, WTS0, 1);		
  	}	
  	#endif
	
/*-------------------------------------------------------------------------------------------------
	输出模式选择及使能
	OCM = 00：强制输出低电平
	OCM = 01：输出同相PPG（TIM2输出）
	OCM = 10：输出反相PPG（TIM2输出）
	OCM = 11：强制输出高电平
-------------------------------------------------------------------------------------------------*/
	#if (DriverMode == Driver_3P3N)
	{  
    TIM1_DBR = 0x0333;                                     // UH/VH/WH = 1; UL/VL/WL = 0
	} 
	#elif (DriverMode == Driver_6N)
	{
    TIM1_DBR = 0x0000;                                     // UH/VH/WH = 0; UL/VL/WL = 0
	}		
	#endif
	
	SetBit(DRV_CTL, OCS1, 0);					
	SetBit(DRV_CTL, OCS0, 0);						                       // OCi和OCiN选择BLDC输出	
//	MOE = 1;	
	OIS4 = 0;										                             // OC4空闲电平设置
	#if (DriverMode == Driver_3P3N)
	{
		OISWL = 0;											                       // OC5空闲电平设置
		OISWH = 1;											                       // OC4空闲电平设置
		OISVL = 0;											                       // OC3空闲电平设置
		OISVH = 1;											                       // OC2空闲电平设置
		OISUL = 0;											                       // OC1空闲电平设置
		OISUH = 1;											                       // OC0空闲电平设置		
	}
	#elif (DriverMode == Driver_6N)
	{
		OISWL = 0;											                       // OC5空闲电平设置
		OISWH = 0;											                       // OC4空闲电平设置
		OISVL = 0;											                       // OC3空闲电平设置
		OISVH = 0;											                       // OC2空闲电平设置
		OISUL = 0;											                       // OC1空闲电平设置
		OISUH = 0;											                       // OC0空闲电平设置		
	}	
	#endif
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM2_Init(void)
	Description   :	TIM2初始化配置
	Input         :	无
	Output				:	无
-------------------------------------------------------------------------------------------------*/
//void TIM2_Init(void)
//{
////	SetBit(PH_SEL,T2SEL,1);					                       // GP10选择Capture Timer输入/输出(for test)
//	SetBit(TIM2_CR1, T2CEN, 0);			                         // 停止计数*/
//	
///*-------------------------------------------------------------------------------------------------
//	时钟分频设置(T4PSC)
//  000:cpuclk			001:cpuclk/2^1	010:cpuclk/2^2	011:cpuclk/2^3
//  100:cpuclk/2^4	101:cpuclk/2^5	110:cpuclk/2^6	111:cpuclk/2^7	
//-------------------------------------------------------------------------------------------------*/
//	SetBit(TIM2_CR0, T2PSC2, 0);
//	SetBit(TIM2_CR0, T2PSC1, 0);
//	SetBit(TIM2_CR0, T2PSC0, 0);

///*-------------------------------------------------------------------------------------------------
//	比较输出模式
//	OCM = 00：强制输出低电平
//	OCM = 01：强制输出高电平
//	OCM = 10：PWM模式1，CNTR<DR输出低电平，CNTR>DR输出高电平
//	OCM = 11：PWM模式2，CNTR<DR输出高电平，CNTR>DR输出低电平
//-------------------------------------------------------------------------------------------------*/
//  SetBit(TIM2_CR0, T2MODE, 1);			                       // 选择输出模式				
//	SetBit(TIM2_CR0, T2OCM1, 1);
//	SetBit(TIM2_CR0, T2OCM0, 0);			                       // 输出比较模式2,1-->0
// 	
//	CLRFlag_T2IR;
//	CLRFlag_T2IP;
//	CLRFlag_T2IF;
//	SetBit(TIM2_CR1, T2IDE, 0);				                       // 禁止输出比较/下降沿检测中断和上升沿检测中断									
//	SetBit(TIM2_CR1, T2IFE, 0);				                       // 使能溢出中断	
//	PTIM231 = 0;
//	PTIM230 = 1;										                         // TIM2/3中断优先级别为3
//	EA = 1;

//	TIM2_ARR = PWM_VALUE_LOAD;								                       // PPG PWMfreq = 24MHz/1500 = 16KHz				
//	TIM2_CNTR = 1;	
//	TIM2_DR = PWM_VALUE_LOAD >> 1;	
//	TIM2_CMTR = 0;
//	TIM2_ADTR = PWM_VALUE_LOAD >> 1;
//	
//	SetBit(TIM2_CR1,T2CEN,1);				                         // 启动计数		
//}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM3_Init(void)
	Description   :	TIM3初始化配置
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void TIM3_Init(void)
{
	SetBit(PH_SEL, T3SEL, 1);					                       // GP11选择Capture Timer输入/输出(for test)
	SetBit(TIM3_CR1, T3CEN, 0);			                         // 停止计数*/

/*-------------------------------------------------------------------------------------------------
	时钟分频设置(T5PSC)
	000:cpuclk			001:cpuclk/2^1	010:cpuclk/2^2	011:cpuclk/2^3
	100:cpuclk/2^4	101:cpuclk/2^5	110:cpuclk/2^6	111:cpuclk/2^7	
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM3_CR0, T3PSC2, 0);
	SetBit(TIM3_CR0, T3PSC1, 0);
	SetBit(TIM3_CR0, T3PSC0, 0);

/*-------------------------------------------------------------------------------------------------
	比较输出模式
	OCM = 00：强制输出低电平
	OCM = 01：强制输出高电平
	OCM = 10：PWM模式1，CNTR<DR输出低电平，CNTR>DR输出高电平
	OCM = 11：PWM模式2，CNTR<DR输出高电平，CNTR>DR输出低电平
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM3_CR0, T3MODE, 0);			                       // 选择输入模式				
//	SetBit(TIM3_CR0, T3OCM1, 1);
//	SetBit(TIM3_CR0, T3OCM0, 0);			                       // 输出比较模式2,0-->1
	SetBit(TIM3_CR0, T3CTM, 0);	// 输入Timer模式
	
	
	CLRFlag_T3IR;
	CLRFlag_T3IP;
	CLRFlag_T3IF;
//	SetBit(TIM3_CR1, T3FE, 0);
	SetBit(TIM3_CR1, T3IDE, 0);				                       // 禁止输出比较/下降沿检测中断和上升沿检测中断									
	SetBit(TIM3_CR1, T3IFE, 1);				                       // 使能溢出中断	
	PTIM231 = 1;
	PTIM230 = 0;										                         // TIM2/3中断优先级别为1
	EA = 1;
	
	TIM3_ARR = 0;							                               // TIM3 Period = 100us				
	TIM3_DR = 0;	
	TIM3_CNTR = 1;
	
	SetBit(TIM3_CR1, T3CEN, 1);				                       // 启动计数	
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM4_Init(void)
	Description   :	TIM4初始化配置
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void TIM4_Init(void)
{
//	SetBit(PH_SEL, T4SEL, 1);					                     	 // GP01选择Capture Timer输入/输出(for test)
	SetBit(TIM4_CR1, T4CEN, 0);			                           // 0，停止计数；1,使能计数

/*-------------------------------------------------------------------------------------------------
	时钟分频设置(T5PSC)
	000:cpuclk			001:cpuclk/2^1	010:cpuclk/2^2	011:cpuclk/2^3
	100:cpuclk/2^4	101:cpuclk/2^5	110:cpuclk/2^6	111:cpuclk/2^7	
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM4_CR0, T4PSC2, 0);
	SetBit(TIM4_CR0, T4PSC1, 0);
	SetBit(TIM4_CR0, T4PSC0, 0);

/*-------------------------------------------------------------------------------------------------
	比较输出模式
	OCM = 00：强制输出低电平
	OCM = 01：强制输出高电平
	OCM = 10：PWM模式1，CNTR<DR输出低电平，CNTR>DR输出高电平
	OCM = 11：PWM模式2，CNTR<DR输出高电平，CNTR>DR输出低电平
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM4_CR0, T4MODE, 1);			                       // 1，选择输出模式	;0，选择输入模式			
	SetBit(TIM4_CR0, T4OCM1, 1);
	SetBit(TIM4_CR0, T4OCM0, 0);			                       // 输出比较模式2,0-->1

	CLRFlag_T4IR;
	CLRFlag_T4IP;
	CLRFlag_T4IF;
	SetBit(TIM4_CR1, T4IDE, 0);				                       // 禁止输出比较/下降沿检测中断和上升沿检测中断									
	SetBit(TIM4_CR1, T4IFE, 1);				                       // 使能溢出中断，更新中断
	PTIM451 = 0;
	PTIM450 = 0;										                         // TIM4/5中断优先级别为1
	EA = 1;
	
	TIM4_ARR = 24000;							                           // TIM4 Period = 1ms				
	TIM4_DR = TIM4_ARR >> 1;	
	TIM4_CNTR = 1;
	
	SetBit(TIM4_CR1, T4CEN, 1);				                       //启动计数	
	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM5_Init(void)
	Description   :	TIM5初始化配置
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/

void TIM5_Init(void)
{
//	SetBit(PH_SEL, T5SEL, 1);				                       // 选择Capture Timer输入/输出(for test)
	SetBit(TIM5_CR1, T5CEN, 0);			                         // 停止计数
	
/*-------------------------------------------------------------------------------------------------
	时钟分频设置(T5PSC)
  000:cpuclk			001:cpuclk/2^1	010:cpuclk/2^2	011:cpuclk/2^3
  100:cpuclk/2^4	101:cpuclk/2^5	110:cpuclk/2^6	111:cpuclk/2^7	
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM5_CR0, T5PSC2, 1);
	SetBit(TIM5_CR0, T5PSC1, 1);
	SetBit(TIM5_CR0, T5PSC0, 1);				// 187.5K

/*-------------------------------------------------------------------------------------------------
	比较输出模式
	OCM = 00：强制输出低电平
	OCM = 01：强制输出高电平
	OCM = 10：PWM模式1，CNTR<DR输出低电平，CNTR>DR输出高电平
	OCM = 11：PWM模式2，CNTR<DR输出高电平，CNTR>DR输出低电平
-------------------------------------------------------------------------------------------------*/
	SetBit(TIM5_CR0, T5MODE, 1);			                       // 选择输出模式				
	SetBit(TIM5_CR0, T5OCM1, 1);
	SetBit(TIM5_CR0, T5OCM0, 0);			                       // PWM模式1
  
	CLRFlag_T5IR;
	CLRFlag_T5IP;
	CLRFlag_T5IF;
	SetBit(TIM5_CR1, T5IDE, 0);				                       // 禁止输出比较/下降沿检测中断和上升沿检测中断									
	SetBit(TIM5_CR1, T5IFE, 0);				                       // 使能溢出中断	
	PTIM451 = 0;
	PTIM450 = 0;										                         // TIM4/5中断优先级别为1
	EA = 1;
	
	TIM5_ARR = 60000;								                         // TIM5 Period = 80000us				
	TIM5_DR = TIM5_ARR >> 1;	
	TIM5_CNTR = 1;
	
	SetBit(TIM5_CR1, T5CEN, 1);				                       // 启动计数	

}




/*-------------------------------------------------------------------------------------------------
	Function Name :	void TIM5_Timing_us(uint16 DR, uint16 ARR)
	Description   :	定时函数，计数时钟750KHz，两个定时通道
	Input         :	DR--定时通道1，比较匹配中断产生定时
                  ARR--定时通道2，溢出中断产生定时
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void TIM5_Timing_us(uint16 DR, uint16 ARR)
{
	SetBit(TIM5_CR1, T5CEN, 0);				                       // 启动计数
	CLRFlag_T5IR;
	CLRFlag_T5IF;

	if(DR > 0)
	{
		TIM5_DR = DR;		
		SetBit(TIM5_CR1, T5IDE, 1);				                     // 输出比较/下降沿检测中断和上升沿检测中断					
	}
	else
	{
		SetBit(TIM5_CR1, T5IDE, 0);				                     // 禁止输出比较/下降沿检测中断和上升沿检测中断			
	}
	if(ARR > 0)
	{
		TIM5_ARR = ARR;
		SetBit(TIM5_CR1, T5IFE, 1);				                     // 使能溢出中断				
	}
	else
	{
		TIM5_ARR = DR;
		SetBit(TIM5_CR1, T5IFE, 0);				                     // 使能溢出中断	
	}

	TIM5_CNTR = 0;
	SetBit(TIM5_CR1, T5CEN, 1);				                       // 启动计数
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void PWMIN_Init(void)
	Description   :	PWM检测参数初始化
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void PWMIN_Init(void)
{
	PWMINCtl.PWMINCnt = 0;
	PWMINCtl.PWMINHigh = 0;
	PWMINCtl.PWMINPeriod = 0;
	PWMINCtl.PWMINHighDuty = 0;
}

/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : Customer.h
* Author             : Fortiortech  Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the user interface parameter used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef		__CUSTOMER_H_
#define		__CUSTOMER_H_

/*芯片参数值------------------------------------------------------------------*/
/*CPU and PWM Parameter*/
/* CPU和载波 频率 */
#define 	MCU_CLOCK							(24.0)							// MCU主频 24M
#define 	PWM_FREQUENCY						(20.0)							// 载波频率(KHZ)

/*deadtime Parameter*/
/* 死区时间参数 */
#define 	PWM_DEADTIME						(0.8)							// 死区时间(us)

/*single resistor sample Parameter*/
#define 	MIN_WIND_TIME						(1.6)							// 单电阻最小采样窗口(us)

/*Power filter Parameter*/
#define 	POWER_KLQFP							_Q15(0.25)						// 功率计算低通滤波系数

///*电机参数值------------------------------------------------------------------*/
///*motor Parameter*/
//#define	Pole_Pairs 			    				(2.0)				// 极对数
//#define 	RS               						(0.0945)			// 相电阻 , ohm 	
//#define 	LD               						(0.000067)			// D轴相电感,H
//#define 	LQ               						(0.000067)			// Q轴相电感,H
//#define	Ke										(0.22)				// 反电动势常数

/*电机参数值------------------------------------------------------------------*/
/*motor Parameter*/
#define		Pole_Pairs 			    			(2.0)							// 极对数
#define 	RS               					(0.01)							// 相电阻,ohm
#define 	LD               					(0.0003)						// D轴相电感,H
#define 	LQ               					(0.0003)						// Q轴相电感,H
#define		Ke									(1.16)							// 反电动势常数

/*电机旋转方向---------------------------------------------------------------------*/
#define 	CW					 				(0)
#define 	CCW									(1)
#define		MOTOR_FR							(CCW)

/*硬件板子参数设置值------------------------------------------------------------------*/
/*hardware current sample Parameter*/
/*电流基准的电路参数*/
#define 	HW_RSHUNT							(0.01)			 				// 采样电阻， ohm
#define 	HW_AMPGAIN							(10.00)							// 运放放大倍数
#define 	HW_ADC_REF							(4.5)							// ADC参考电压

/*hardware voltage sample Parameter*/
/*母线电压采样分压电路参数*/
#define 	RV1		  							(15)  		 					// 单位K欧姆
#define 	RV2 		  						(0) 	   						// 单位K欧姆
#define 	RV3		  							(2) 	        				// 单位K欧姆，实际PCB上的值
#define 	RV									((RV1+RV2+RV3)/RV3)				// Max Voltage 468V
#define 	HW_BOARD_VOLT_MAX					(HW_ADC_REF*RV)					// 5*(R1+R2+R3)/R3 = 5*(330+300+6.8)/6.9 = 468

/*时间设置值------------------------------------------------------------*/
#define		Calib_Time							1000							// 单位:次，校正次数，固定1000次
#define		Charge_Time							15								// 单位:ms，预充电时间
#define		Align_Time							1								// 单位:ms，预定位时间  10

/*正常运行时估算算法的参数设置值------------------------------------------------------------*/
/* obsever parameter value */
#define 	BASE_FREQ							((MOTOR_SPEED_BASE/60)*Pole_Pairs)// speed base value UINT HZ，基准频率
#define 	OBS_KSLIDE							_Q15(0.85)						// SMO算法里的滑膜增益值
#define     E_BW                                (300.0)                         // PLL算法里的反电动势滤波值


/*逆风判断时的估算算法设置值------------------------------------------------------------*/
#define		TailWind_Time						250								// 顺逆风检测时间，单位:ms 
#define 	ATO_BW_Wind							15.0							// observer bandwidth filter value，逆风判断观测器带宽的滤波值，经典值为8.0-100.0   30
#define 	SPD_BW_Wind							40.0							// speed bandwidth filter value，逆风判断速度带宽的滤波值，经典值为5.0-40.0		15

/**逆风顺风状态下的KP、KI****/
#define 	DKP_TailWind						_Q12(2.0)						// 顺逆风 D轴 比例系数 KP	1.8
#define 	DKI_TailWind						_Q12(0.4)						// 顺逆风 D轴 积分系数 KI 	0.38
#define 	QKP_TailWind						_Q12(2.0)						// 顺逆风 Q轴 比例系数 KP	1.8
#define 	QKI_TailWind						_Q12(0.4)						// 顺逆风 Q轴 积分系数 KI	0.38

/*启动参数参数值------------------------------------------------------------*/
/* current set value */
#define		I_Value(Curr_Value)					_Q15(((Curr_Value*HW_RSHUNT*HW_AMPGAIN)/(HW_ADC_REF)))   //A


/*********** IPD parameter ***********/
/******* 初始位置检查参数 **********/
#define		PosCheckEnable						(1)								// 初始位置使能
#define		AlignTestMode						(0)								// 预定位测试模式

#define		PWM_ON_Time							(200)							// us，必须大于200us
#define		PWM_OFF_Time						(560)							// us ，必须大于200us
#define		PWM_Brake_Time						(200)							// us ，必须大于200us
#define		PWM_Duty							(0.7)							// 必须大于0.6 


/***预定位的Kp、Ki****/
#define		DKP_Alignment						_Q12(0.5)						// 预定位的KP
#define		DKI_Alignment						_Q12(0.05)						// 预定位的KI
#define		QKP_Alignment						_Q12(0.5)						// 预定位的KP
#define		QKI_Alignment						_Q12(0.05)						// 预定位的KI
#define		ID_Align_CURRENT					I_Value(0.0)					// D 轴定位电流
#define		IQ_Align_CURRENT					I_Value(0.15)					// Q 轴定位电流 
#define		Align_Angle							(0.0)							// 预定位角度

#define		ID_Start_CURRENT					I_Value(0.0)					// D 轴启动电流
#define		IQ_Start_CURRENT					I_Value(1.5)					// Q 轴启动电流 0.7 

#define		ID_RUN_CURRENT						I_Value(0.0)					// D 轴运行电流
#define		IQ_RUN_CURRENT						I_Value(0.3)					// Q 轴运行电流

/********Omega启动的参数**********/
#define		ATO_BW								2.0								// observer bandwidth filter value，观测器带宽的滤波值，经典值为8.0-100.0
#define		ATO_BW_RUN							3.0
#define		ATO_BW_RUN1							5.0
#define		ATO_BW_RUN2							9.0
#define		ATO_BW_RUN3							15.0
#define		ATO_BW_RUN4							24.0
#define		ATO_BW_RUN5							30.0
#define		ATO_BW_RUN6							50.0
#define		ATO_BW_RUN7							80.0

#define 	SPD_BW								15.0							// speed bandwidth filter value，速度带宽的滤波值，经典值为5.0-40.0
#define 	ATT_COEF							0.85							// obeserver filter value，无需改动

/*转速参数值------------------------------------------------------------*/
/* motor run speed value */
#define 	MOTOR_SPEED_BASE					(6000.0)						// 速度基准 RPM

/* motor start speed value */
//open 算法启动参数
#define 	MOTOR_OPEN_ACC						(50.0)							// 强拖启动的增量(每载波周期加一次)
#define 	MOTOR_OPEN_ACC_MIN					(1.0)							// 强拖启动的初始速度
#define		MOTOR_OPEN_ACC_CNT					(40.0)							// 强拖启动的执行次数(MOTOR_OPEN_ACC_CNT*256)
#define     MOTOR_OPEN_ACC_CYCLE				(1)								// OPEN启动循环拖动的次数

//OMEGA启动参数
#define		Motor_Omega_Ramp_ACC				(1.0)							// omega 启动的增量	  1~20		 4
#define 	MOTOR_OMEGA_ACC_MIN					(100.0)							// omega 启动的最小切换转速(RPM)  36
#define 	MOTOR_OMEGA_ACC_END					(500.0)							// omega 启动的限制转速(RPM)		 128

/* motor loop control speed value */
#define 	MOTOR_LOOP_RPM						(400.0)							// 由mode 0到mode 1切换转速  不决定速度，只影响切入速度环或者其他外环的那一瞬间。 300

/* motor run speed value */
//电机运行时最大最小转速、堵转保护转速
#define 	MOTOR_SPEED_MIN_SMO					(1000.0)						// 估算器 反电动势低通滤波系数的最小值	 成正比  ACC_END < MIN_SMO < MIN_RPM  400

#define 	MOTOR_SPEED_MIN_RPM					(250.0)							// 
#define 	MOTOR_SPEED_MAX_RPM					(3500.0)						// 运行最大转速(RPM)

#define 	MOTOR_SPEED_MIN_RPM					(500.0)							// 
#define 	MOTOR_SPEED_MAX_RPM					(25000.0)						// 运行最大转速(RPM)

#define   	MOTOR_NOLOAD_MIN_RPM				(MOTOR_SPEED_MIN_RPM + 50)

#define 	MOTOR_SPEED_LIMIT_RPM				(1500.0)	


#define 	MOTOR_SPEED_STAL_MAX_RPM			(6500.0)						// 堵转保护最大转速(RPM)
#define 	MOTOR_SPEED_STAL_MIN_RPM			(30.0)							// 堵转保护最小转速(RPM)

#define 	Motor_Max_Speed						_Q15(MOTOR_SPEED_MAX_RPM/MOTOR_SPEED_BASE)
#define 	Motor_Min_Speed						_Q15(MOTOR_SPEED_MIN_RPM/MOTOR_SPEED_BASE)

#define		Motor_NoLoad_Min_Speed				_Q15(MOTOR_NOLOAD_MIN_RPM/MOTOR_SPEED_BASE)

#define		NoLoad_Judge_Speed					_Q15(NOLOAD_JUDGE_SPEED/MOTOR_SPEED_BASE)

/******电机开机、关机的设置------------------------------------------------------*/
/* motor ON/0FF value */
#define 	OFFPWMDuty							_Q15(0.02)						// 关机PWM占空比，小于该占空比关机//关机PWM占空比，小于该占空比时关机
#define 	OFFPWMDutyHigh						_Q15(1.0)						// 关机PWM占空比，大于该占空比关机
#define 	ONPWMDuty							_Q15(0.05)						// 开机PWM占空比，大于该占空比时开机

#define		MINPWMDuty							_Q15(0.05)						// 速度曲线上最小PWM占空比
#define		MAXPWMDuty							_Q15(0.95)						// 速度曲线上最大PWM占空比
#define		SPEED_K								((float)(Motor_Max_Speed-Motor_Min_Speed)/(float)(MAXPWMDuty-MINPWMDuty)) //2498

/*******运行时的参数*****************/

/**电流环参数设置值------------------------------------------------------------*/
/* D axis parameter set value */
/* D轴 参数设置 */
#define 	DKP									_Q12(1.0)						// D axis KP					D轴 比例系数
#define 	DKI									_Q12(0.01)						// D axis KI					D轴 积分系数
#define 	DOUTMAX								_Q15(0.3)//0.4						// D axis output max value		D轴 最大限幅  0.05
#define 	DOUTMIN								_Q15(-0.3)						// D axis output min value		D轴 最小限幅

/* Q axis parameter set value */
/* Q轴 参数设置 */
#define 	QKP									_Q12(1.0)						//Q轴 比例系数  0.2   0.08 
#define 	QKI									_Q12(0.01)						//Q轴 积分系数

#define 	QOUTMAX								_Q15(0.96)//0.91				//Q轴 最大限幅	 0.95
#define 	QOUTMIN								_Q15(-0.96)						//Q轴 最小限幅

/*速度环参数设置值------------------------------------------------------------*/
#define		SpeedloopDeley_Time					500								// 速度环生效延迟时间 单位:ms
#define		SPEED_LOOP_TIME						50								// 速度环调节周期，单位：ms		


#define		DetectionDelay_Time					(1500)							// 空载识别延时时间，单位：ms
#define		NOLOAD_JUDGE_SPEED					(500)							// 空载判定速度


#define 	SKP								_Q12(1.5)						//速度环 比例系数KP	 (0.2  0.001  无超调) (0.9,0.002 高速稳定 超调很大) (0.9,0.001  低速稳定)
#define 	SKI								_Q12(0.01)						//速度环 积分系数KI   (0.5  0.0015  低速稳定，无超调)



#define 	SOUTMAX								I_Value(6.0)					// speed loop output max value   速度环 输出  最大限幅
#define 	SOUTMIN								I_Value(0.001)					// speed loop output min value   速度环 输出  最小限幅



/*模式选择设置值------------------------------------------------------------*/
/*TailWind mode*/
#define 	IPMtest								(0)								// IPM测试或者MOS测试，MCU输出固定占空比
#define 	NormalRun							(1)								// 正常按电机状态机运行
#define		IPMState							(NormalRun)

/*Driver Mode:3P3N or 6N*/
#define 	Driver_6N							(0)								// FOC 6N mode，一般适用于高压
#define 	Driver_3P3N							(1)								// FOC 3P3N mode，一般使用于低压
#define 	DriverMode							(Driver_3P3N)

/*PWM high or low level Mode*/
#define 	High_Level							(0)								// 驱动高电平有效
#define 	Low_Level							(1)								// 驱动低电平有效
#define 	PWM_Level_Mode						(High_Level)

/*speed loop mode*/
#define 	OUTLoop_Disable						(0)								// 使能外环
#define 	OUTLoop_Enable						(1)								// 关闭外环
#define 	OUTLoop_Mode						(OUTLoop_Enable)

/*TailWind mode*/
#define 	NoTailWind							(0)								// 无逆风顺风判断
#define 	TailWind							(1)								// 逆风顺风判断
#define		TailWind_Mode						(NoTailWind)

/*open loop mode*/
#define 	Open_Start							(0)								// 开环强拖启动
#define 	Omega_Start							(1)								// Omega启动
#define 	Open_Omega_Start					(2)								// 先开环启，后Omega启动
#define		Open_Start_Mode						(Omega_Start)

/*sample resister mode*/
#define 	Single_Resistor						(0)								// 单电阻
#define 	Double_Resistor						(1)								// 双电阻
#define 	Shunt_Resistor_Mode					(Single_Resistor)

/*保护参数值------------------------------------------------------------------*/
/* protect value */
#define		Hardware_FO_Protect					(1)								// 硬件FO过流保护使能，适用于IPM有FO保护的场合
#define		Hardware_CMP_Protect				(2)								// 硬件CMP比较过流保护使能，适用于MOS管应用场合
#define		Hardware_FO_CMP_Protect				(3)								// 硬件CMP比较和FO过流保护都使能
#define		HardwareCurrent_Protect				(Hardware_CMP_Protect)			// 硬件过流保护实现方式，必选其一
#define		OverSoftCurrentValue				I_Value(20.0)					// 软件过流值
#define		CurrentRecoverEnable				(1)								// 过流保护使能位, 0，不使能；1，使能
#define		OverCurrentRecoverTime				(2000)							// 过流保护恢复时间，单位5ms ， 2000*5ms = 10S

#define 	VoltageProtectEnable				(0)								// 电压保护，0,不使能；1，使能
#define 	StartProtectEnable					(0)								// 启动保护，0,不使能；1，使能
#define 	StallProtectEnable					(0)								// 堵转保护，0,不使能；1，使能
#define 	PhaseLossProtectEnable				(0)								// 缺相保护，0,不使能；1，使能


#define		Over_Protect_Voltage 				(29.0)							// 直流电压过压保护值，V
#define		Over_Recover_Vlotage				(27.0)							// 直流电压过压保护恢复值，V
#define		Under_Protect_Voltage				(12.0)							// 直流电压欠压保护值，V
#define		Under_Recover_Vlotage				(15.0)							// 直流电压欠压保护恢复值，V

#define		PhaseLossCurrentValue				I_Value(0.2)					// PhaseLossCurrentValue，缺相电流值
#define		PhaseLossRecoverTime				(600)							// 缺相保护时间，单位 5ms

#define		StallCurrentValue1					I_Value(3.5)					// stall over current value，堵转过流值

#define		Usart_Speed							(0)
#define		SREF_Speed							(1)
#define		SpeedMode							(SREF_Speed)


#define		ONOFFtestMode						(0)	


#define		StartON_Time						5000
#define		StartOFF_Time						10000


#endif

/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : Main.c
* Author             : Fortiortech Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains main function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx_Sys.h>
#include <FU68xx.h>
#include <Myproject.h>


/* Private typedef ------------------------------------------------------------------------------*/
/* Private define -------------------------------------------------------------------------------*/
/* Private macro --------------------------------------------------------------------------------*/
/* Private variables ----------------------------------------------------------------------------*/
unsigned char code SysTable[] = {0x1a,0x40,0x00,0x00,0x00};

/* Private function prototypes ------------------------------------------------------------------*/
/* Private functions ----------------------------------------------------------------------------*/
void SoftwareInit(void);
void HardwareInit(void);
void MotorControlInit(void);
int16 foc_ud;
int16 foc_uq;
/*-------------------------------------------------------------------------------------------------
	Function Name :	void main(void)
	Description   :	主函数主要功能是初始化，包括系统初始化，软件初始化，硬件初始化，主循环
                  扫描。
                  系统初始化--系统时钟配置，系统电源配置，看门狗配置
                  软件初始化--初始化所有定义的变量
                  硬件初始化--初始化硬件设备配置
                  主循环扫描--电机控制状态扫描、其他附加功能
	Input         :	无
	Output				:	无
-------------------------------------------------------------------------------------------------*/
void main(void)
{
 	uint32 PowerUpCnt = 0;	//上电等待延时计数
	
	/*System Init,系统初始化*/
	SystemInit();

	/*Wait For Power Up*/
	for(PowerUpCnt=0;PowerUpCnt<SystemPowerUpTime;PowerUpCnt++){};

	/*Software Init，软件初始化*/	
	SoftwareInit();
		
	/*Hardware Init，硬件初始化*/	
	HardwareInit();
		
	motor_FR = CCW;			
	SPI_Init();
	OLED_Init();
		
	do
	{
		if (GP00 == 0)
		{
			motor_FR = !motor_FR;
		}
	} while(Time.START_Flag == 0);
		
	while(1)
	{
		if(mcCurOffset.OffsetFlag==0)
		{
			/****上电先进行硬件电流基准采集，再进行主控函数扫描***/
			 GetCurrentOffset();
		}
		else
		{
			/****主控函数，状态扫描****/
			MC_Control();
			foc_ud=FOC_UD;
			foc_uq=FOC_UQ;
			#if (SpeedMode==Usart_Speed)	                          //选择串口作为调试方式
			{
				UartDealComm();	
			}
			#endif	
		}
	}
}


/*-------------------------------------------------------------------------------------------------
	Function Name :	void SoftwareInit(void)
	Description   :	软件初始化，初始化所有定义变量
	Input         :	无
	Output				:	无
-------------------------------------------------------------------------------------------------*/
void SoftwareInit(void)
{
 /****初始化所有定义的参数变量****/
	MotorcontrolInit();
 /****电机初始状态为mcwait******/
	mcState = mcwait;
	mcFaultSource = 0;  
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void HardwareInit(void)
	Description   :	硬件初始化，初始化需要使用的硬件设备配置，FOC必须配置的是运放电压、运放初始化、ADC初始化、Driver初始化
                  TIM4初始化，其他的可根据实际需求加。
	Input         :	无
	Output				:	无
-------------------------------------------------------------------------------------------------*/
void HardwareInit(void)
{
	/****功能IO初始化***********/
	GPIO_Init();	
	/******硬件FO过流，比较器初始化，用于硬件过流比较保护，根据硬件决定选择哪种模式******/
	#if (HardwareCurrent_Protect==Hardware_FO_Protect)	//外部中断初始化，用于外部中断硬件过流FO的保护
	{
		EXTI_Init();		
	}
	#elif (HardwareCurrent_Protect==Hardware_CMP_Protect)//选择比较过流，比较器初始化
	{
		CMP_Iint();	
	}
	#elif (HardwareCurrent_Protect==Hardware_FO_CMP_Protect)	//两者都选择
	{
		EXTI_Init();	
		CMP_Iint();			
	}
	#endif
	
	/*****运算放大器初始化*********/
	AMP_VHALF_Init();																				// 运放电压偏置选择
	
	AMP_Iint();                                             // 运放管脚初始化
	
	/*****ADC初始化*********/
 	ADC_Init();	
	
	/*****MDU初始化*********/
	MDU_16MUL16_INT(15,1);		                             //右移15位 ，有符号乘法                 
	
	/*****Driver初始化******/
	Driver_Init();
	
	/*****UART初始化*********/	
	#if (SpeedMode==Usart_Speed)	                          //选择串口作为调试方式
	{
		UART_Init();	
	}
	#endif

/*****Timer初始化*******/
	 TIM3_Init();            //采样定时器3中断作为PWM捕获中断
	 TIM4_Init();              //采用定时器4中断作为常见中断,时间为1ms，处理故障保护等附加功能*
//	 TIM5_Init();            //少用
}

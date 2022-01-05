/**
  ******************************************************************************
  * @file    PosiCheck.h 
  * @author  Fortior Application Team
  * @version V1.0.0
  * @date    05/06/2014
  * @brief   define motor contorl parameter
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __PosiCheck_H_
#define __PosiCheck_H_


/*********** IPD parameter ***********/
#define   PWM_ON_Time_Up			   (PWM_ON_Time/TPWM_VALUE/1000000)
#define   PWM_ON_Time_Down			   (PWM_ON_Time_Up/2)
#define   PWM_OFF_Time_Input           (PWM_OFF_Time/TPWM_VALUE/1000000)                        // us  														
#define   PWM_Brake_Time_Input         (PWM_Brake_Time/TPWM_VALUE/1000000)  					// us  
#define   PWM_ONDuty_Input             (PWM_VALUE_LOAD*(1-PWM_Duty)) 	
#define   PWM_OFFDuty_Input            (PWM_VALUE_LOAD*PWM_Duty)  

#define 	Degree_0			0
#define 	Degree_60		10923
#define 	Degree_120		21845
#define 	Degree_180		32767
#define 	Degree_240		43691
#define 	Degree_300		54613

/* Exported types -------------------------------------------------------------------------------*/
typedef struct
{  
	uint16   PWMONTimeUp;	
	uint16   PWMONTimeDown;	
	uint16   PWMOFFTimeInput;	
	uint16   PWMBrakeTimeInput;  
	uint16   PWMONDutyInput;
	uint16   PWMOFFDutyInput;	
	uint16   PWMLOADInput;	
}MCPosCheckPWMVar;

/* Exported variables ---------------------------------------------------------------------------*/
extern MCPosCheckPWMVar 			PosCheckPWMVar;	

/* Exported functions ---------------------------------------------------------------------------*/
extern void PosCheckVarInit(void);
extern void PosCheckCurrentGet(void);
extern int16	PosCheckGetAngle(void);
#endif
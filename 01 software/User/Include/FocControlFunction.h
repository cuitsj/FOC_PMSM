/**
  ******************************************************************************
  * @file    FocControl.h 
  * @author  Fortior Application Team
  * @version V1.0.0
  * @date    05/06/2014
  * @brief   define motor contorl parameter
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __FocControlFunction_H_
#define __FocControlFunction_H_
extern void FOC_Init(void);
extern void Motor_Charge(void);
extern void MC_Stop(void);
extern void Motor_TailWind(void);
extern void TailWindDealwith(void);
extern void Motor_Open(void);
extern void Motor_Align(void);
extern void McTailWindDealwith(void);
extern void MotorcontrolInit(void);
#endif
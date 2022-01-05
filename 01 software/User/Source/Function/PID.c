/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : PID.c
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains PI control function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>

/* Private variables ----------------------------------------------------------------------------*/
bit FlagStartPI = 0;

Ramp_TypeDef idata SpeedRamp;
PID_TypeDef idata SpeedPID;
PID_TypeDef idata CurrentPID;


/*-------------------------------------------------------------------------------------------------
	Function Name :	void SpeedPIDInit(void)
	Description   :	ËÙ¶ÈPID²ÎÊý³õÊ¼»¯
	Input         :	ÎÞ
  Output				:	ÎÞ
-------------------------------------------------------------------------------------------------*/
void SpeedPIDInit(void)
{
	SpeedPID.Kp = TempSpeedKP;						
	SpeedPID.Ki = TempSpeedKI;						
	SpeedPID.Err = 0;
	SpeedPID.Err_Last1 = 0;
	SpeedPID.Err_Err = 0;
	#if (SpeedCloseLoopEnable)
	{
		SpeedPID.Out = TempMotorStartDuty;
		SpeedPID.OutMax = MotorVSMax;
		SpeedPID.OutMin = TempMotorStartDuty;	
	}
	#elif ((SpeedCloseLoopEnable)&&(CurrentCloseLoopEnable))
	{	
		SpeedPID.Out = 0;
		SpeedPID.OutMax = TempCurrentMax;
		SpeedPID.OutMin = TempCurrentMin;					
	}
	#endif		
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void SpeedRampInit(void)
	Description   :	ËÙ¶ÈÅÀÆÂ²ÎÊý³õÊ¼»¯
	Input         :	ÎÞ
  Output				:	ÎÞ
-------------------------------------------------------------------------------------------------*/
void SpeedRampInit(void)
{
	SpeedRamp.RampInc = SpeedRampInc;
	SpeedRamp.RampDec = SpeedRampDec;
	SpeedRamp.RampStep = MC.SpeedCtl.PISpeed;
}	

/*-------------------------------------------------------------------------------------------------
	Function Name :	void CurrentPIDInit(void)
	Description   :	µçÁ÷PI²ÎÊý³õÊ¼»¯
	Input         :	ÎÞ
  Output				:	ÎÞ
-------------------------------------------------------------------------------------------------*/
void CurrentPIDInit(void)
{
	CurrentPID.Kp = TempCurrentKP;						
	CurrentPID.Ki = TempCurrentKI;						
	CurrentPID.Err = 0;
	CurrentPID.Err_Last1 = 0;
	CurrentPID.Err_Err = 0;
	CurrentPID.Out = TempMotorStartDuty;
	CurrentPID.OutMax = MotorVSMax;
	CurrentPID.OutMin = TempMotorStartDuty;
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 RampControl(Ramp_TypeDef *Ramp, int16 Ref)
	Description   :	ÅÀÆÂ¿ØÖÆº¯Êý£¬¸ù¾ÝÉè¶¨ÅÀÆÂÔöÁ¿°Ñµ±Ç°ÖµÏòÄ¿±êÖµ¿¿Â£
	Input         :	Ramp--ÅÀÆÂ¶ÔÏóÊý×é
                  Ref--ÅÀÆÂ²Î¿¼Ä¿±ê
  Output				:	RampStep--ÅÀÆÂµ±Ç°Êä³ö
-------------------------------------------------------------------------------------------------*/
int16 RampControl(Ramp_TypeDef *Ramp, int16 Ref)
{
	Ramp->RampTarget = Ref;
	if(Ramp->RampTarget > Ramp->RampStep)
	{
		if((Ramp->RampStep + Ramp->RampInc) <= Ramp->RampTarget)
		{
			Ramp->RampStep += Ramp->RampInc;
		}
		else
		{
			Ramp->RampStep = Ramp->RampTarget;
		}
	}
	else if(Ramp->RampTarget < Ramp->RampStep)
	{
		if((Ramp->RampStep - Ramp->RampDec) >= Ramp->RampTarget)
		{
			Ramp->RampStep -= Ramp->RampDec;
		}
		else
		{
			Ramp->RampStep = Ramp->RampTarget;
		}
	}
	
	return 	Ramp->RampStep;
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur)
	Description   :	PIµ÷½Úº¯Êý
	Input         :	PID--PIµ÷½Ú¶ÔÏóÊý×é
                  Ref--PIµ÷½Ú²Î¿¼Ä¿±ê
                  Cur--PIµ±Ç°ÊäÈë
  Output				:	Out--PIµ÷½ÚÊä³ö
-------------------------------------------------------------------------------------------------*/
int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur)
{
	int32 Kp_Out, Ki_Out, PID_Out;

	/*PIÔËËãÊ±¼ä67us*/
	if(!PID->Err)
	{
		PID->Err_Last1 = Ref - Cur;														 // ³õÊ¼»¯PIDÉÏ´ÎÆ«²î
		PID->Err = Ref - Cur;																	 // ³õÊ¼»¯PIDµ±Ç°Æ«²î
		PID->Err_Err=PID->Err - PID->Err_Last1;								 // ³õÊ¼»¯PIDÉÏ´ÎÆ«²îºÍÉÏÉÏ´ÎÆ«²îÖ®²î
	}
	else
	{
		PID->Err_Last1 = PID->Err;														 // ±£´æPIDÉÏ´ÎÆ«²î
		PID->Err = Ref - Cur;																	 // ¼ÆËãPIDµ±Ç°Æ«²î
		PID->Err_Err=PID->Err - PID->Err_Last1;								 // ¼ÆËãPIDÉÏ´ÎÆ«²îºÍÉÏÉÏ´ÎÆ«²îÖ®²î		
	}

	Kp_Out = ((int32)PID->Kp * (int32)PID->Err_Err) >> 12;
	Ki_Out = ((int32)PID->Ki * (int32)PID->Err) >> 12;

	PID_Out = PID->Out;
	PID_Out += Kp_Out + Ki_Out;

	if(PID_Out > PID->OutMax)
	{
		PID_Out = PID->OutMax;												         // PID×î¸ßÊä³ö
	}
	if(PID_Out < PID->OutMin)
	{
		PID_Out = PID->OutMin;												         // PID×îµÍÊä³ö
	}
	PID->Out = PID_Out;
 	
	return PID->Out;	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur)
	Description   :	PIµ÷½Úº¯Êý
	Input         :	PID--PIµ÷½Ú¶ÔÏóÊý×é
                  Ref--PIµ÷½Ú²Î¿¼Ä¿±ê
                  Cur--PIµ±Ç°ÊäÈë
  Output				:	Out--PIµ÷½ÚÊä³ö
-------------------------------------------------------------------------------------------------*/
int16 PID_Control(PID_TypeDef *PID, int16 Ref, int16 Cur)
{
	int16 Kp_Out;
	int16 Ki_Out;
	int32 PID_Out;

	/*PIÔËËãÊ±¼ä42us*/
	if(!PID->Err)
	{
		PID->Err_Last1 = Ref - Cur;														 // ³õÊ¼»¯PIDÉÏ´ÎÆ«²î
		PID->Err = Ref - Cur;																	 // ³õÊ¼»¯PIDµ±Ç°Æ«²î
		PID->Err_Err=PID->Err - PID->Err_Last1;								 // ³õÊ¼»¯PIDÉÏ´ÎÆ«²îºÍÉÏÉÏ´ÎÆ«²îÖ®²î
	}
	else
	{
		PID->Err_Last1 = PID->Err;														 // ±£´æPIDÉÏ´ÎÆ«²î
		PID->Err = Ref - Cur;																	 // ¼ÆËãPIDµ±Ç°Æ«²î
		PID->Err_Err=PID->Err - PID->Err_Last1;								 // ¼ÆËãPIDÉÏ´ÎÆ«²îºÍÉÏÉÏ´ÎÆ«²îÖ®²î		
	}
	
	Kp_Out = MDU_MULA_S16(PID->Kp, PID->Err_Err, 12);
	Ki_Out = MDU_MULA_S16(PID->Ki, PID->Err, 12);
	PID_Out = PID->Out;
	PID_Out += Kp_Out + Ki_Out;
		
	if(PID_Out > PID->OutMax)
	{
		PID_Out = PID->OutMax;												         // PID×î¸ßÊä³ö
	}
	if(PID_Out < PID->OutMin)
	{
		PID_Out = PID->OutMin;												         // PID×îµÍÊä³ö
	}
	PID->Out = PID_Out;
	
	return PID->Out;	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void PI_Init(void)
	Description   :	PIÄ£¿é³õÊ¼»¯
	Input         :	ÎÞ
  Output				:	ÎÞ
-------------------------------------------------------------------------------------------------*/
void PI_Init(void)
{	
/*-------------------------------------------------------------------------------------------------
	PIÊý¾Ý¸ñÊ½
	Q = PIRANGE + 8(Q8~Q23Êý¾Ý¸ñÊ½)
  ¸ø¼Ä´æÆ÷¸³ÖµÊ±ÒªÍ³Ò»Êý¾Ý¸ñÊ½
-------------------------------------------------------------------------------------------------*/		
	SetBit(SV_CR, PIRANGE3, 0);															// 0100: 12bit PI
	SetBit(SV_CR, PIRANGE2, 1);
	SetBit(SV_CR, PIRANGE1, 0);
	SetBit(SV_CR, PIRANGE0, 0);				                       // PI½ØÎ»Q=PIRANGE+8£¬¼´PIÊý¾Ý¸ñÊ½							

  PI_KP = 0;												                       // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16 Q12¸ñÊ½£¬PI_KP=Kp*2^12£¬
	PI_KI = 0;												                       // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16 Q12¸ñÊ½£¬PI_KI=Kp*2^12
	PI_EK = 0;												
  PI_UK = 0;												                       // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16
	PI_UKMAX = 0;											                       // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16
	PI_UKMIN = 0;											                       // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	int16 PIDControl(PID_TypeDef *PID, int16 Ref, int16 Cur)
	Description   :	PIµ÷½Úº¯Êý
	Input         :	PID--PIµ÷½Ú¶ÔÏóÊý×é
                  Ref--PIµ÷½Ú²Î¿¼Ä¿±ê
                  Cur--PIµ±Ç°ÊäÈë
  Output				:	Out--PIµ÷½ÚÊä³ö
-------------------------------------------------------------------------------------------------*/
int16 PI_Control(PID_TypeDef *PID, int16 Ref, int16 Cur)
{
	static uint8 *PIDType = 0;
	
	/*PIÔËËãÊ±¼ä10us*/	
	FlagStartPI = 1;
	while(FlagStartPI)                                       // MDU·ÖÊ±¸´ÓÃ±êÖ¾
	{
		FlagStartPI = 0;	

		/*Ö¸ÕëÀàÐÍÊý¾ÝÅÐ¶ÏÖ´ÐÐÊ±¼ä2.5us*/
		if(PIDType != PID)                                     // Í¨¹ýPIDÊý¾ÝµØÖ·»ñÈ¡PIDÀàÐÍ£¬
		{
			PIDType = PID;
			
			PI_KP = PID->Kp;									                   // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16 Q12¸ñÊ½£¬PI_KP=Kp*2^12£¬
			PI_KI = PID->Ki;									                   // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16 Q12¸ñÊ½£¬PI_KI=Kp*2^12
			PI_EK = PID->Err;												
			PI_UK = PID->Out;									                   // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16
			PI_UKMAX = PID->OutMax;						                   // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16
			PI_UKMIN = PID->OutMin;						                   // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16		
			
			SetBit(SV_CR,PISTA,1);						                   // Æô¶¯PI
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();													                   // µÈ´ýPIÔËËãÍê³É,PI_LastErr³õÊ¼»¯³É¹

		}
		/*¸³ÖµÔËËãÊ±¼ä3.2us*/	
		PID->Err = Ref - Cur;								                   // ¼ÆËãPIDµ±Ç°Æ«²î  

		/*PIÓ²ºËÔËËãÊ±¼ä1.8us*/
		PI_EK = PID->Err;										                   // PIÊäÈë
		PI_UK = PID->Out;									                     // ¼Ä´æÆ÷Êý¾ÝÀàÐÍ£ºint16
		SetBit(SV_CR,PISTA,1);
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		PID->Out = PI_UK;										                    // PIÊä³ö
	}
  FlagStartPI = 1;
	
  return PID->Out;
}


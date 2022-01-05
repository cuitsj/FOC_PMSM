
#include <FTC68xx.h>
#include <Myproject.h>

/******************************************************************************
 tan(Angle)= 2^0, 2^(-1), 2^(-2), 2^(-3), 2^(-4), 2^(-5), 2^(-6), 2^(-7), 
						 2^(-8), 2^(-9), 2^(-10), 2^(-11), 2^(-12), 2^(-13)
 对应角度的值,Q15格式, 
*******************************************************************************/
int16 xdata TanAngle[13] = {4096, 2418, 1277, 648, 326, 163, 81, 41, 20, 10, 5, 3, 1};


/*******************************************************************************
Name 		: Sin()
Input		: Angle,Angle Value=0°~360°(Q15 Value=0~32767)
Output	: Sin(Angle)
Function:	caclulate Sin(Angle)
*******************************************************************************/
int16 Sin(int16 Angle)
{
	uint8 i;
	uint8 Quadrant;
	int16 z;
	int32 x[2], y[2];
	x[0] = 19898;															//起始旋转矢量模，Q15格式,0.607252941*2^15=19898
	y[0] = 0;
	z = 0;
	if((0<=Angle) && (Angle<8192))							//Angle=0°~90°,1th quadrant
	{
		Quadrant = 1;
	}
	else if((8192<=Angle) && (Angle<16384))		//Angle=90°~180°,2th quadrant
	{
		Quadrant = 2;
		Angle = 16384 - Angle;
	}
	else if((16384<=Angle) && (Angle<24576))		//Angle=180°~270°,3th quadrant
	{
		Quadrant = 3;
		Angle = Angle - 16384;
	}
	else if((24576<=Angle) && (Angle<32768))		//Angle=270°~360°,4th quadrant
	{
		Quadrant = 4;
		Angle = 32767 - Angle;
	}

	for(i=0;i<13;i++)
	{
		if(z < Angle)
		{
			x[1] = x[0] - (y[0] >> i);
			y[1] = y[0] + (x[0] >> i);
			z += TanAngle[i];
		}
		else
		{
			x[1] = x[0] + (y[0] >> i);
			y[1] = y[0] - (x[0] >> i);
			z -= TanAngle[i];
		}
		x[0] = x[1];
		y[0] = y[1];
	}
	
	if((Quadrant==3) || (Quadrant==4))
	{
		y[0]= -y[0];
	}
	
	if(y[0] > 32767)
	{
		y[0] = 32767;
	}
	if(y[0] < -32768)
	{
		y[0] = -32768;
	}
	return (int16)y[0];	
	
}


/*******************************************************************************
Name 		: Cos()
Input		: Angle,Angle Value=0°~360°(Q15 Value=0~32767)
Output	: Cos(Angle)
Function:	caclulate Cos(Angle)
*******************************************************************************/
int16 Cos(int16 Angle)
{
	uint8 i;
	uint8 Quadrant;
	int16 z;
	int32 x[2], y[2];
	x[0] = 19898;															//起始旋转矢量模，Q15格式,0.607252941*2^15=19898	
	y[0] = 0;
	z = 0;
	if((0<=Angle) && (Angle<8192))							//Angle=0°~90°,1th quadrant
	{
		Quadrant = 1;
	}
	else if((8192<=Angle) && (Angle<16384))		//Angle=90°~180°,2th quadrant
	{
		Quadrant = 2;
		Angle = 16384 - Angle;
	}
	else if((16384<=Angle) && (Angle<24576))		//Angle=180°~270°,3th quadrant
	{
		Quadrant = 3;
		Angle = Angle - 16384;
	}
	else if((24576<=Angle) && (Angle<32768))		//Angle=270°~360°,4th quadrant
	{
		Quadrant = 4;
		Angle = 32767 - Angle;
	}
	
	for(i=0;i<13;i++)
	{	
		if(z < Angle)
		{
			x[1] = x[0] - (y[0] >> i);
			y[1] = y[0] + (x[0] >> i);
			z += TanAngle[i];
		}
		else
		{
			x[1] = x[0] + (y[0] >> i);
			y[1] = y[0] - (x[0] >> i);
			z -= TanAngle[i];
		}
		x[0] = x[1];
		y[0] = y[1];
	}
	
	if((Quadrant==2) || (Quadrant==3))
	{
		x[0] =  -x[0];
	}	
	
	if(x[0] > 32767)
	{
		x[0] = 32767;
	}
	if(x[0] < -32768)
	{
		x[0] = -32768;
	}
	return (int16)x[0];
}

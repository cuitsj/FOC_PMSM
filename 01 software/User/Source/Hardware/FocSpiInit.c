/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : DriverInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains driver initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>


/*-------------------------------------------------------------------------------------------------
	Function Name :	void FocSpi_Init(void)
	Description   :	FOC的spi初始化配置
	Input         :	无
  Output				:	无
-------------------------------------------------------------------------------------------------*/
void FocSpi_Init(void)
{
	SPI_CFG = 0X50;
	SPI_SCR = 0X08;
	SPI_CTRL = 0X01;
	
	FOC_FDS= 0X01;
	SetBit(FOC_CR2,DBEN,1);	
}



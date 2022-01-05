/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : SPIInit.c
* Author             : Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains SPI initial function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>


/*-------------------------------------------------------------------------------------------------
	Function Name :	void SPI_Init(void)
	Description   :	SPI初始化配置
	Input         :	输入说明（详细）
	Output				:	输出说明（详细）
-------------------------------------------------------------------------------------------------*/
void SPI_Init(void)
{
	SetBit(SPI_CTRL, SPIEN, 0);								               // 0,disable SPI;1 enable
	
/*-------------------------------------------------------------------------------------------------
  SPI管脚配置
	1、禁止UART复用，P06配置为MISO，P05配置为MOSI
	2、禁止比较器输出复用，P07配置为SCLK
-------------------------------------------------------------------------------------------------*/
	SetBit(PH_SEL, UT1EN, 0);						 // 0,P06 as GPIO or SPI_MISO,P05 as GPIO or SPI_MOSI;1,P06 and p07 as USART
	SetBit(CMP_CR2, CMPOE, 0);						// P07 as GPIO or SPI_SCLK
		
/*-------------------------------------------------------------------------------------------------
	SPI时钟相位/极性配置
	CPHA = 0, CPOL = 0:上升沿接收，下降沿发送，空闲电平为低
	CPHA = 0, CPOL = 1:上升沿发送，下降沿接收，空闲电平为高
	CPHA = 1, CPOL = 0:上升沿发送，下降沿接收，空闲电平为低
	CPHA = 1, CPOL = 1:上升沿接收，下降沿发送，空闲电平为高
-------------------------------------------------------------------------------------------------*/
	SetBit(SPI_CFG, CKPHA, 1);								
	SetBit(SPI_CFG, CKPOL, 1);									

/*-------------------------------------------------------------------------------------------------
	SPI从方式选择配置
	00：3线从方式或3线主方式，NSS信号不连到端口管脚
	01：4线从方式或4线多主方式，NSS配置为输入
	1x：4线单主方式，NSS配置为输出，NSS信号输出x电平
-------------------------------------------------------------------------------------------------*/
	SetBit(SPI_CTRL, NSSMID1, 1);								
	SetBit(SPI_CTRL, NSSMID0, 1);		//OLED复位 1：不复位 0：复位						

	SetBit(SPI_CFG, MSTEN, 1);                               // 0:Slave, 1:Master
	
/*-------------------------------------------------------------------------------------------------
	SPI中断配置
	SPIF：SPI字节传输完成中断标志，硬件置位，软件清零
	WCOL：SPI写冲突中断（发送缓冲器非空时写操作），硬件置位，软件清零
	MODF：SPI方式错误中断（多主方式NSS被拉低，MSTEN和SPIEN被清零）硬件置位，软件清零
	RXOVRN：SPI接收溢出中断（SPI接收缓冲器接收溢出），硬件置位，软件清零
-------------------------------------------------------------------------------------------------*/
	SetBit(SPI_CTRL, SPIF, 0);					                     // SPI传输完成中断标志
	SetBit(SPI_CTRL, WCOL, 0);					                     // SPI写冲突中断标志
	SetBit(SPI_CTRL, MODF, 0);					                     // SPI方式错误中断标志
	SetBit(SPI_CTRL, RXOVRN, 0);				                     // SPI接受溢出中断
	SPIIE = 0;                                               // SPI中断使能	
	EA = 1;
	
	SPI_SCR = 0;												                     // Fspi = Fcpu / (2*(SPI_SCR + 1)) = 6MHz	
	
	SetBit(SPI_CTRL, SPIEN, 1);						                   // enable SPI	
}

/*-------------------------------------------------------------------------------------------------
	Function Name :	void SPI_SendData(uint16 Data, uint8 Channel)
	Description   :	SPI传输数据
	Input         :	Data--传输数据，uint16格式
	Output				:	Channel--传输通道，取值0~3
-------------------------------------------------------------------------------------------------*/
void SPI_SendData(uint16 Data, uint8 Channel)
{		
	if(!GetBit(SPI_CFG, SPIBSY))
	{	
		SetBit(SPI_CTRL, NSSMID0, 1);					
		Data = Data >> 4 + 0x1000;
		switch(Channel)
		{
			case 0:
				Data &= 0x3fff;
				break;
			case 1:	
				Data |= 0x4000;
				break;
			case 2:
				Data |= 0x8000;
				break;
			case 3:
				Data |= 0xc000;
				break;
			default:
				break;
		}	
	  SetBit(SPI_CTRL, NSSMID0, 0);	
		SPI_DAT = Data>>8;
		SPI_DAT = Data;	
	}
}

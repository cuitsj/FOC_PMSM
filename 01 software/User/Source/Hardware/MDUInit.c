/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : AddFunction.c
* Author             : Fortiortech  Appliction Team
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the add function used for Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/


/* Includes -------------------------------------------------------------------------------------*/
#include <FU68xx.h>
#include <Myproject.h>
/*-------------------------------------------------------------------------------------------------
	Function Name : uint16 MDU_DIV_U16(uint16 xdata *TData0, uint16 xdata *TData1)
	Description   :	16位无符号除法，返回16位无符号值，运行时间6.4us，函数在调用过程中如果
                  被中断打断则会重复调用
	Input         :	TData0-被除数，TData1-除数，32位数据类型  24/8=3  24是被除数
	Output				:	MD_DAL-商低16位
-------------------------------------------------------------------------------------------------*/
uint16 MDU_DIV_U16(uint16 xdata *TData0, uint16 xdata *TData1)
{
	MD_DAH = *TData0;
	MD_DAL = *(TData0+1);

	MD_DBH = *TData1;
	MD_DBL = *(TData1+1);
							

	SetBit(MD_MODE,DIVSTART,1);
	while(!GetBit(MD_MODE,DIVDONE)); // 等待MDU计算完成			

	return MD_DAL;			
}
/*---------------------------------------------------------------------------*/
/* Name		:	void MDU_16MUL16_INT(uint8 ALIGN, uint8 SIGN)
/* Input	:	NO
/* Output	:	NO
/* Description:	Inital MDU parameter
/*---------------------------------------------------------------------------*/
void MDU_16MUL16_INT(uint8 ALIGN, uint8 SIGN)
{
	if(ALIGN == 0)
	{
		MD_MODE &= ~(1<<ALIGN1);
		MD_MODE &= ~(1<<ALIGN0);				/*00:乘法结果右移0位*/
	}
	else if(ALIGN == 8)
	{
		MD_MODE &= ~(1<<ALIGN1);
		MD_MODE |= (1<<ALIGN0);					/*01:乘法结果右移8位*/
	}	
	else if(ALIGN == 12)
	{
		MD_MODE |= (1<<ALIGN1);
		MD_MODE &= ~(1<<ALIGN0);					/*10:乘法结果右移12位*/
	}		
	else if(ALIGN == 15)
	{
		MD_MODE |= (1<<ALIGN1);
		MD_MODE |= (1<<ALIGN0);					/*11:乘法结果右移15位*/
	}		
	else ;
	
	if(SIGN == 0)
	{
		MD_MODE &= ~(1<<MDSN);					/*选择无符号乘法*/
	}
	else if(SIGN == 1)
	{
		MD_MODE |= (1<<MDSN);						/*选择有符号乘法*/
	}
	else ;
}
/*-------------------------------------------------------------------------------------------------
	Function Name :	void MDU_MUL_U32(uint16 TData0, uint16 TData1, uint16 xdata *Result32)
	Description   :	16位无符号乘法，返回32位无符号值，运行时间3.2us，函数在调用过程中如果
                  被中断打断则会重复调用
	Input         :	TData0-乘数0; TData1-乘数1; Result32-32bi乘积变量指针
-------------------------------------------------------------------------------------------------*/
void MDU_MUL_U32(uint16 TData0, uint16 TData1, uint16 xdata *Result32)
{

	SetBit(MD_MODE, ALIGN1, 0);
	SetBit(MD_MODE, ALIGN0, 0);				                     // 00:乘法结果右移0位				
	SetBit(MD_MODE, MDSN, 0);							                 // 选择无符号乘法

	MD_MA = TData0;
	MD_MB = TData1;								                         // 写被乘数和乘数
	
	*Result32 = MD_MA;
	*(Result32+1) = MD_MB;
}
#ifndef __UARTINIT_H_
#define __UARTINIT_H_

typedef struct
{
	uint16     R_DATA[10];//={0,0,0,0,0,0,0,0,0,0};
  uint16     T_DATA[10];//={0,0,0,0,0,0,0,0,0,0};
	uint16   Uredata;
  uint16   UARxCnt;
  uint16   RxFSM;
  uint16   UsaRxLen;
  uint16   flagUsaRxComm;
  uint32   CheckDate;
	uint16   ResponceCount;
	uint8    ResponceFlag;
	uint8    UsaTxlen;
	uint8    SendCnt;
}MCUART;

extern MCUART Uart;
extern void UART_Init(void);
extern void UART_SendData(unsigned char T_Data);
extern void UartDealResponse(void);
extern void UartDealComm(void);

#endif
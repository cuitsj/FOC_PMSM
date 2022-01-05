/**************************** (C) COPYRIGHT 2015 Fortiortech shenzhen *****************************
* File Name          : FU68xx_FOC.c
* Author             : Billy Long Fortiortech  Market Dept
* Version            : V1.0
* Date               : 01/07/2015
* Description        : This file contains all the common data types used for
*                      Motor Control.
***************************************************************************************************
* All Rights Reserved
**************************************************************************************************/ 

/* Define to prevent recursive inclusion --------------------------------------------------------*/
#ifndef __FU68xx_FOC_H_
#define __FU68xx_FOC_H_

/* Includes -------------------------------------------------------------------------------------*/
#include <absacc.h>

/* Base Config Define ---------------------------------------------------------------------------*/
#define SET(reg,bit)					(reg |= (1<<bit))
#define CLR(reg,bit)					(reg &= ~(1<<bit))
#define SetBit(reg,bit,val)		val?(reg |= (1<<bit)):(reg &= ~(1<<bit))
#define GetBit(reg,bit)				(reg & (1<<bit))
#define SetReg(reg,val)	 			(reg = val)

/* Register Define ------------------------------------------------------------------------------*/
sfr FOC_SET0 = 0xe8;															
sbit FOCEN = FOC_SET0^7;

sfr FOC_SET1 = 0xe9;														
#define FOCFR        0

#define FOC_DTR				    XBYTE[0X4064]

#define FOC_CR1				    XBYTE[0X4090]
#define ESEL        7
#define EFAE        6
#define RFAE        5
#define ANGM        4
#define CSM         3
#define FCE         2
#define PWMSS       1
#define FOCST       0

#define FOC_CR2				    XBYTE[0X4091]
#define ESCMS       7
#define EIVMS       6
#define F5SEG       5
#define DSS         4
#define DBEN        0


#define FOC_CR3				    XBYTE[0X4092]
#define FCIM1       3
#define FCIM0       2
#define FUIM1       1
#define FUIM0       0


#define FOC_IER				    XBYTE[0X4093]
#define FCIE        1
#define FUIE        0

#define FOC_SR				    XBYTE[0X4094]
#define EUOF        7
#define FCIF        1
#define FUIF        0

#define FOC_CHC				    XBYTE[0X4095]
#define CSOC1       7
#define CSOC0       6
#define ITCH2       5
#define ITCH1       4
#define ITCH0       3
#define UCH2        2
#define UCH1        1
#define UCH0        0

#define PI_RAN				    XBYTE[0X4096]
#define EPIRAN3     7
#define EPIRAN2     6
#define EPIRAN1     5
#define EPIRAN0     4
#define DQPIRAN3    3
#define DQPIRAN2    2
#define DQPIRAN1    1
#define DQPIRAN0    0

#define FOC_CMR				    XBYTE[0X4097]
#define CCPH        7
#define CCPL        6 
#define CC3EH       5
#define CC3EL       4
#define CC2EH       3
#define CC2EL       2
#define CC1EH       1
#define CC1EL       0

#define FOC_RTHECNT				XBYTE[0X408E]
#define FOC_FDS				    XBYTE[0X408F]


#define FOC_EFREQACCH			XBYTE[0X4088]
#define FOC_EFREQACCL			XBYTE[0X4089]
#define FOC_EFREQMINH			XBYTE[0X408a]
#define FOC_EFREQMINL			XBYTE[0X408b]
#define FOC_EFREQHOLDH			XBYTE[0X408c]
#define FOC_EFREQHOLDL			XBYTE[0X408d]

#define FOC_DKPH				XBYTE[0X4098]
#define FOC_DKPL				XBYTE[0X4099]
#define FOC_DKIH				XBYTE[0X409A]
#define FOC_DKIL				XBYTE[0X409B]
#define FOC_DMAXH				XBYTE[0X409C]
#define FOC_DMAXL				XBYTE[0X409D]
#define FOC_DMINH				XBYTE[0X409E]
#define FOC_DMINL				XBYTE[0X409F]
#define FOC_QKPH				XBYTE[0X40A0]
#define FOC_QKPL				XBYTE[0X40A1]
#define FOC_QKIH				XBYTE[0X40A2]
#define FOC_QKIL				XBYTE[0X40A3]
#define FOC_QMAXH				XBYTE[0X40A4]
#define FOC_QMAXL				XBYTE[0X40A5]
#define FOC_QMINH				XBYTE[0X40A6]
#define FOC_QMINL				XBYTE[0X40A7]
#define FOC_UDH				    XBYTE[0X40A8]
#define FOC_UDL				    XBYTE[0X40A9]
#define FOC_UQH				    XBYTE[0X40AA]
#define FOC_UQL				    XBYTE[0X40AB]
#define FOC_IDREFH				XBYTE[0X40AC]
#define FOC_IDREFL				XBYTE[0X40AD]
#define FOC_IQREFH				XBYTE[0X40AE]
#define FOC_IQREFL				XBYTE[0X40AF]
#define FOC_ARRH				XBYTE[0X40B0]
#define FOC_ARRL				XBYTE[0X40B1]
#define FOC_SWDUTYH				XBYTE[0X40B2]
#define FOC_SWDUTYL				XBYTE[0X40B3]
#define FOC_TSMINH				XBYTE[0X40B4]
#define FOC_TSMINL				XBYTE[0X40B5]
#define FOC_TRGDLYH				XBYTE[0X40B6]
#define FOC_TRGDLYL				XBYTE[0X40B7]
#define FOC_THETAH				XBYTE[0X40B8]
#define FOC_THETAL				XBYTE[0X40B9]
#define FOC_THECOMPH			XBYTE[0X40BA]
#define FOC_THECOMPL			XBYTE[0X40BB]
#define FOC_RTHESTEPH			XBYTE[0X40BC]
#define FOC_RTHESTEPL			XBYTE[0X40BD]
#define FOC_RTHEACCH			XBYTE[0X40BE]
#define FOC_RTHEACCL			XBYTE[0X40BF]
#define FOC_THECORH				XBYTE[0X40C0]
#define FOC_THECORL				XBYTE[0X40C1]
#define FOC_ETHETAH				XBYTE[0X40C2]
#define FOC_ETHETAL				XBYTE[0X40C3]
#define FOC_KSLIDEH				XBYTE[0X40C4]
#define FOC_KSLIDEL				XBYTE[0X40C5]
#define FOC_EKLPFMINH			XBYTE[0X40C6]
#define FOC_EKLPFMINL			XBYTE[0X40C7]
#define FOC_EBMFKH				XBYTE[0X40C8]
#define FOC_EBMFKL				XBYTE[0X40C9]
#define FOC_OMEKLPFH			XBYTE[0X40CA]
#define FOC_OMEKLPFL			XBYTE[0X40CB]
#define FOC_FBASEH				XBYTE[0X40CC]
#define FOC_FBASEL				XBYTE[0X40CD]
#define FOC_EOMEH				XBYTE[0X40CE]
#define FOC_EOMEL				XBYTE[0X40CF]
#define FOC_EKPH				XBYTE[0X40D0]
#define FOC_EKPL				XBYTE[0X40D1]
#define FOC_EKIH				XBYTE[0X40D2]
#define FOC_EKIL				XBYTE[0X40D3]
#define FOC_POWKLPFH			XBYTE[0X40D4]
#define FOC_POWKLPFL			XBYTE[0X40D5]
#define FOC_POWH				XBYTE[0X40D6]
#define FOC_POWL				XBYTE[0X40D7]
#define FOC_EK1H				XBYTE[0X40D8]
#define FOC_EK1L				XBYTE[0X40D9]
#define FOC_EK2H				XBYTE[0X40DA]
#define FOC_EK2L				XBYTE[0X40DB]
#define FOC_EK3H				XBYTE[0X40DC]
#define FOC_EK3L				XBYTE[0X40DD]
#define FOC_EK4H				XBYTE[0X40DE]
#define FOC_EK4L				XBYTE[0X40DF]
#define FOC_IAH				    XBYTE[0X40E0]
#define FOC_IAL				    XBYTE[0X40E1]
#define FOC_IBH				    XBYTE[0X40E2]
#define FOC_IBL				    XBYTE[0X40E3]
#define FOC_IBETH				XBYTE[0X40E4]
#define FOC_IBETL				XBYTE[0X40E5]
#define FOC_COMRH				XBYTE[0X40E6]
#define FOC_COMRL				XBYTE[0X40E7]
#define FOC_IDH				    XBYTE[0X40E8]
#define FOC_IDL				    XBYTE[0X40E9]
#define FOC_IQH				    XBYTE[0X40EA]
#define FOC_IQL				    XBYTE[0X40EB]
#define FOC_VALPH				XBYTE[0X40EC]
#define FOC_VALPL				XBYTE[0X40ED]
#define FOC_VBETH				XBYTE[0X40EE]
#define FOC_VBETL				XBYTE[0X40EF]
#define FOC_UALPH				XBYTE[0X40F0]
#define FOC_UALPL				XBYTE[0X40F1]
#define FOC_UBETH				XBYTE[0X40F2]
#define FOC_UBETL				XBYTE[0X40F3]
#define FOC_EALPH				XBYTE[0X40F4]
#define FOC_EALPL				XBYTE[0X40F5]
#define FOC_EBETH				XBYTE[0X40F6]
#define FOC_EBETL				XBYTE[0X40F7]
#define FOC_ESQUH				XBYTE[0X40F8] 
#define FOC_ESQUL				XBYTE[0X40F9]
#define FOC_UDCFLTH				XBYTE[0X40FA] 
#define FOC_UDCFLTL				XBYTE[0X40FB]
#define FOC_CSOH				XBYTE[0X40FC]
#define FOC_CSOL				XBYTE[0X40FD]


#define FOC_EFREQACC			*(volatile unsigned int xdata *)0X4088
#define FOC_EFREQHOLD			*(volatile unsigned int xdata *)0X408C
#define FOC_EFREQMIN			*(volatile unsigned int xdata *)0X408A
#define FOC_DKP				    *(volatile unsigned int xdata *)0X4098
#define FOC_DKI				    *(volatile unsigned int xdata *)0X409A
#define FOC_DMAX				*(volatile unsigned int xdata *)0X409C
#define FOC_DMIN				*(volatile unsigned int xdata *)0X409E
#define FOC_QKP				    *(volatile unsigned int xdata *)0X40A0
#define FOC_QKI				    *(volatile unsigned int xdata *)0X40A2
#define FOC_QMAX				*(volatile unsigned int xdata *)0X40A4
#define FOC_QMIN				*(volatile unsigned int xdata *)0X40A6
#define FOC_UD				    *(volatile unsigned int xdata *)0X40A8
#define FOC_UQ				    *(volatile unsigned int xdata *)0X40AA
#define FOC_IDREF				*(volatile unsigned int xdata *)0X40AC
#define FOC_IQREF				*(volatile unsigned int xdata *)0X40AE
#define FOC_ARR				    *(volatile unsigned int xdata *)0X40B0
#define FOC_SWDUTY				*(volatile unsigned int xdata *)0X40B2
#define FOC_TSMIN				*(volatile unsigned int xdata *)0X40B4
#define FOC_TRGDLY				*(volatile unsigned int xdata *)0X40B6
#define FOC_THETA				*(volatile unsigned int xdata *)0X40B8
#define FOC_THECOMP			    *(volatile unsigned int xdata *)0X40BA
#define FOC_RTHESTEP			*(volatile unsigned int xdata *)0X40BC
#define FOC_RTHEACC			    *(volatile unsigned int xdata *)0X40BE
#define FOC_THECOR				*(volatile unsigned int xdata *)0X40C0
#define FOC_ETHETA				*(volatile unsigned int xdata *)0X40C2
#define FOC_KSLIDE				*(volatile unsigned int xdata *)0X40C4
#define FOC_EKLPFMIN			*(volatile unsigned int xdata *)0X40C6
#define FOC_EBMFK				*(volatile unsigned int xdata *)0X40C8
#define FOC_OMEKLPF			    *(volatile unsigned int xdata *)0X40CA
#define FOC_FBASE				*(volatile unsigned int xdata *)0X40CC
#define FOC_EOME				*(volatile unsigned int xdata *)0X40CE
#define FOC_EKP				    *(volatile unsigned int xdata *)0X40D0
#define FOC_EKI				    *(volatile unsigned int xdata *)0X40D2
#define FOC_POWKLPF			    *(volatile unsigned int xdata *)0X40D4
#define FOC_POW				    *(volatile unsigned int xdata *)0X40D6
#define FOC_EK1				    *(volatile unsigned int xdata *)0X40D8
#define FOC_EK2				    *(volatile unsigned int xdata *)0X40DA
#define FOC_EK3				    *(volatile unsigned int xdata *)0X40DC
#define FOC_EK4				    *(volatile unsigned int xdata *)0X40DE
#define FOC_IA				    *(volatile unsigned int xdata *)0X40E0
#define FOC_IB				    *(volatile unsigned int xdata *)0X40E2
#define FOC_IBET				*(volatile unsigned int xdata *)0X40E4
#define FOC_COMR				*(volatile unsigned int xdata *)0X40E6
#define FOC_ID				    *(volatile unsigned int xdata *)0X40E8
#define FOC_IQ				    *(volatile unsigned int xdata *)0X40EA
#define FOC_VALP				*(volatile unsigned int xdata *)0X40EC
#define FOC_VBET				*(volatile unsigned int xdata *)0X40EE
#define FOC_UALP				*(volatile unsigned int xdata *)0X40F0
#define FOC_UBET				*(volatile unsigned int xdata *)0X40F2
#define FOC_EALP				*(volatile unsigned int xdata *)0X40F4
#define FOC_EBET				*(volatile unsigned int xdata *)0X40F6
#define FOC_ESQU				*(volatile unsigned int xdata *)0X40F8 
#define FOC_UDCFLT				*(volatile unsigned int xdata *)0X40FA 
#define FOC_CSO				    *(volatile unsigned int xdata *)0X40FC


#endif


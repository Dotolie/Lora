//------------------------------------------------------------------------------
//
//  Copyright (C) 2008 MVTech Co., Ltd. All Rights Reserved
//  MVTech Co.,Ltd. Proprietary & Confidential
//
//  Reproduction in whole or in part is prohibited without the written consent 
//  of the copyright owner. MVTech reserves the right to make changes 
//  without notice at any time. MVTech makes no warranty, expressed, 
//  implied or statutory, including but not limited to any implied warranty of 
//  merchantability or fitness for any particular purpose, or that the use will
//  not infringe any third party patent,  copyright or trademark. 
//  MVTech must not be liable for any loss or damage arising from its use.
//
//  Module      :
//  File        : lora.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __LORA_H__
#define __LORA_H__

#include <queue>
#include "base.h"
#include "object.h"
#include "rs232.h"
#include "httpclient.h"
#include "thread.h"

typedef union _stPacket {
	struct _Urup{
		unsigned int nRSd;
		unsigned int nIA;
		unsigned int nRSu;
		unsigned int nSeq;
		unsigned int nCO2;
		unsigned int nRH;
		unsigned int nTemp;
		unsigned int nPm1P0;
		unsigned int nPm2P5;
		unsigned int nPm4P0;
		unsigned int nPm10P0;
		unsigned short sBat;
		unsigned int nTime;
		char szDateTime[16];
		} Urup;
		
} stPacket;

class CLora: public CObject, CBase
{
public:
	CRs232 *m_pRs232;
	Thread *m_pComm;
	stPacket m_stPkt;
	std::queue<stPacket> m_qPkt;

	CHttpClient *m_pHttpClient;
	Thread		*m_pClientThread;
	
public:
	CLora();
	~CLora();
	
	virtual int On_MSG_TIMER( int, int );
	virtual int On_MSG_EVENT( int, int );
	
	stPacket parsePacket(char*);
	
};

#endif  // __LORA_H__

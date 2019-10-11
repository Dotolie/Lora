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
//  File           : Timer.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include "object.h"
#include "packet.h"
#include "message.h"
#include "timer.h"
#include "task.h"

#define DEBUG
#include "debug.h"

extern CTask app;

CTimer::CTimer() 
{
}

CTimer::~CTimer() 
{
	DBG("[%s:%s]  destroy \r\n", __FILE__,__func__ );
	
}

void CTimer::Stop()
{
	m_bRun = false;
//	DBG("[%s:%s]  m_brun= false\r\n", __FILE__,__func__);
}

void CTimer::Run()
{
	static int nVal = 1;
	DBG("[%s:%s]  end of loop \r\n", __FILE__,__func__ );

	usleep(6000000);
	SendMessage(0, PKT_REQ_GETPARAMS, 0, 0);	

	m_bRun = true;
	while( m_bRun ) {
		usleep(500000);
		app.m_pDevice->LedSet(nVal);
		usleep(500000);
		app.m_pDevice->LedSet(!nVal);
		}
	
}

bool CTimer::IsRun()
{
	return m_bRun;
}



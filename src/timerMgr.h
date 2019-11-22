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
//  File        : timerMgr.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __TIMERMGR_H__
#define __TIMERMGR_H__

#include <time.h>

#include "stdafx.h"
#include "base.h"

typedef void (*TimerHandler)(int key, void *userPtr);

typedef struct {
	bool			m_IsUsed;
	timer_t			m_TmId;
	int				m_wParam;
	int				m_lParam;
	void*			m_pObj;
} TimerInfo;


class CTimerMgr: public CBase
{
public:
	TimerInfo m_TmInfo[TASK_ID_MAX];

public:
	CTimerMgr();
	~CTimerMgr();
	
	bool setSignal();
	bool addTimer(int key, long resMSec, int wParam, int lParam, void *pObj);
	bool setTimer(int key, long resMSec, int wParam, int lParam, void *pObj);
	bool delTimer(int key);

};

#endif  // __TIMERMGR_H__

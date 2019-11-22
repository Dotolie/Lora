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
//  File        : TimerMgr.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "msg.h"
#include "timerMgr.h"
#include "taskMgr.h"


#define DEBUG
#include "debug.h"

#define MY_TIMER_SIGNAL		SIGRTMIN
#define ONE_MSEC_TO_NSEC	1000000
#define ONE_SEC_TO_NSEC		1000000000

extern CTaskMgr g_TaskMgr;
CTimerMgr g_TimerMgr;

static void timer_handler(int sig, siginfo_t *si, void *context)
{
	int key = si->si_value.sival_int;
	int wParam = 0;
	int lParam = 0;
	CObject *pObj = NULL;
	
	pObj = (CObject *)g_TimerMgr.m_TmInfo[key].m_pObj;
	wParam = g_TimerMgr.m_TmInfo[key].m_wParam;
	lParam = g_TimerMgr.m_TmInfo[key].m_lParam;
	
//	DBG("[%s:%s]  timer_handler : send MSG_TIMER key=%d pObj=0x%x\r\n", __FILE__,__func__, key, pObj );

	g_TaskMgr.SendMessage(pObj, MSG_TIMER, wParam, lParam);
}

CTimerMgr::CTimerMgr() 
{
	DBG("[%s:%s]  create \r\n", __FILE__,__func__ );

	memset( m_TmInfo, 0, sizeof(TimerInfo)*TASK_ID_MAX);
	
	setSignal();
}

CTimerMgr::~CTimerMgr() 
{
	for(auto i=0;i<TASK_ID_MAX;i++) {
		if( m_TmInfo[i].m_TmId != 0) {
			delTimer(i);
			}
		}
	
	DBG("[%s:%s]  destroy \r\n", __FILE__,__func__ );
}

bool CTimerMgr::setSignal()
{
	int rtn;
	struct sigaction sa;

	sa.sa_flags 	= SA_SIGINFO;
	sa.sa_sigaction = timer_handler;

	sigemptyset(&sa.sa_mask);

	rtn = sigaction(MY_TIMER_SIGNAL, &sa, NULL);

	if (rtn == -1)
	{
		return false;
	}

	DBG("[%s:%s] registered \r\n", __FILE__,__func__);

	return true;	
}

bool CTimerMgr::addTimer(int key, long intv, int wParam, int lParam, void *pObj)
{
	int rtn;
	timer_t timerId;
	struct sigevent sigEvt;

	/* Create Timer */

	memset(&sigEvt, 0x00, sizeof(sigEvt));

	sigEvt.sigev_notify			 = SIGEV_SIGNAL;
	sigEvt.sigev_signo			 = MY_TIMER_SIGNAL;
	sigEvt.sigev_value.sival_int = key;

	rtn = timer_create(CLOCK_REALTIME, &sigEvt, &timerId);
	if (rtn != 0) {
		DBG("[%s:%s] fail timer_create \r\n", __FILE__,__func__);
		return false;
		}

	/* Set Timer Interval */

	long nano_intv;
	struct itimerspec its;

	nano_intv = intv;// * ONE_MSEC_TO_NSEC;

	// initial expiration
	its.it_value.tv_sec 	= nano_intv / 1000;
	its.it_value.tv_nsec 	= (nano_intv % 1000 ) * 1000000;
	// timer interval
	its.it_interval.tv_sec	= its.it_value.tv_sec;
	its.it_interval.tv_nsec	= its.it_value.tv_nsec;
	
	rtn = timer_settime(timerId, 0, &its, NULL);

	if (rtn != 0) {
		DBG("[%s:%s] fail timer_settime \r\n", __FILE__,__func__);
		return false;
		}

	/* Save Timer Inforamtion */
	m_TmInfo[key].m_TmId 		= timerId;
	m_TmInfo[key].m_wParam		= wParam;
	m_TmInfo[key].m_lParam		= lParam;
	m_TmInfo[key].m_pObj	 	= pObj;


	DBG("[%s:%s]  addTimer: key=%d, timerId=%x, tv=%ld, tv_n=%ld\r\n", __FILE__,__func__, key, timerId, its.it_value.tv_sec,  its.it_value.tv_nsec);
		

	return true;
}

bool CTimerMgr::setTimer(int key, long intv, int wParam, int lParam, void *pObj)
{
	if( m_TmInfo[key].m_TmId != 0 ) {
		int rtn;		
		long nano_intv;
		struct itimerspec its;
		
		nano_intv = intv;// * ONE_MSEC_TO_NSEC;
		
		// initial expiration
		its.it_value.tv_sec 	= nano_intv / 1000;
		its.it_value.tv_nsec	= (nano_intv % 1000 ) * 1000000;
		// timer interval
		its.it_interval.tv_sec	= its.it_value.tv_sec;
		its.it_interval.tv_nsec = its.it_value.tv_nsec;
		
		rtn = timer_settime(m_TmInfo[key].m_TmId, 0, &its, NULL);
		
		if (rtn != 0) {
			DBG("[%s:%s] fail timer_settime \r\n", __FILE__,__func__);
			return false;
			}
		
		/* Save Timer Inforamtion */
		m_TmInfo[key].m_wParam		= wParam;
		m_TmInfo[key].m_lParam		= lParam;
		m_TmInfo[key].m_pObj		= pObj;
		
//		DBG("[%s:%s]  setTimer: t=%ld key=%d, timerId=%x\r\n", __FILE__,__func__, intv, key, m_TmInfo[key].m_TmId );

		return true;
		}

	return false;
}

bool CTimerMgr::delTimer(int key)
{
	int rtn;
	timer_t tTimerId;


	tTimerId = m_TmInfo[key].m_TmId;
	rtn = timer_delete(tTimerId);

	if (rtn != 0)
	{
		DBG("[%s:%s] err delTimer \r\n", __FILE__, __func__);
		return false;
	}


	m_TmInfo[key].m_TmId = 0;
	m_TmInfo[key].m_pObj = 0;

	DBG("[%s:%s] delTimer: key=%d\r\n", __FILE__, __func__, key);

	return true;
}

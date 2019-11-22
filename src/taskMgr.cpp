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
//  File           : TaskMgr.cpp
//  Description :
//  Author      : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "thread.h"
#include "rs232.h"
#include "taskMgr.h"
#include "timerMgr.h"

#define DEBUG
#include "debug.h"



using namespace std;

extern CTimerMgr g_TimerMgr;
extern CLora *g_pLora;


CTaskMgr g_TaskMgr;


CTaskMgr::CTaskMgr() :	CObject(), CBase()
{
	SetName("CTaskMgr");
	
	DBG("[%s:%s] create id=0x%x\r\n", __FILE__,__func__, this);
	
	SendMessage( this, MSG_CREATE, 0, 0 );

	AddTask(TASK_ID_MGR, this);
}
CTaskMgr::~CTaskMgr()
{
	DelTask(TASK_ID_MGR);

	DBG("[%s:%s] destroy\r\n", __FILE__,__func__);
}

void CTaskMgr::Run()
{
	int i = 0;
	int ret = 0;
	CObject *hObj;
	MSG msg;
	
	while( ret==0 && PeekMessage( msg, 0, 0, 0) ){
		hObj = msg.hdl;
//		DBG("[%s:%s] hObj=0x%x\r\n", __FILE__,__func__, hObj);
		
		switch( msg.message) {
		case MSG_CREATE:
			ret = hObj->On_MSG_CREATE(msg.wparam, msg.lparam);
			break;
		case MSG_CLOSE:
			ret = hObj->On_MSG_CLOSE(msg.wparam, msg.lparam);
			break;
		case MSG_TIMER:
			ret = hObj->On_MSG_TIMER(msg.wparam, msg.lparam);
			break;
		case MSG_EVENT:
			ret = hObj->On_MSG_EVENT(msg.wparam, msg.lparam);
			break;
		case MSG_QUIT:
			ret = hObj->On_MSG_QUIT(msg.wparam, msg.lparam);		
			break;
		default:
			DBG("[%s:%s] no function \r\n", __FILE__,__func__);
			break;
			}
	}

}

int CTaskMgr::PeekMessage(MSG& msg, int a, int b, int c)
{
	while(1) {
		if( !m_MsgQ.IsEmpty() ) {
			m_MsgQ.pop( msg);

//			DBG("[%s:%s] gMsgQ msg=0x%x \r\n", __FILE__,__func__, msg.message );
			
//			if( msg.message == MSG_END )
//				return 0;

			break;
			}
		else {
			usleep(1000);
//			DBG("[%s:%s] gMsgQ empty \r\n", __FILE__,__func__);
			}
		}

	return 1;
}

int CTaskMgr::SendMessage(CObject* hdl, int message, int wparam, int lparam)
{
	MSG msg;

	msg.message = message;
	msg.wparam = wparam;
	msg.lparam = lparam;
	msg.hdl = hdl;
	
//	DBG("[%s:%s] msg=0x%x, w=0x%x, l=0x%x \r\n", __FILE__,__func__, message, wparam, lparam);		
	
	if( !m_MsgQ.IsFull() ) {
		m_MsgQ.push(msg);
		return 1;
		}
	
	return 0;
}

int CTaskMgr::AddTask(const int key, CObject *pObj)
{
	int nRet = 0;

	m_mObjs.insert(pair<int,CObject*>(key, pObj));
	
	DBG("[%s:%s] add Task: key=%d, pobj=0x%x\r\n", __FILE__, __func__, key, pObj);

	return nRet;
}

int CTaskMgr::DelTask(const int key)
{
	int nRet = 0;

	nRet = m_mObjs.erase(key);

	DBG("[%s:%s] erase Task: key=%d, nRet=%d\r\n", __FILE__, __func__, key, nRet);

	return nRet;
}

CObject* CTaskMgr::FindTask(const int key)
{
	map<int, CObject*>::iterator it;
	CObject *pObj = NULL;

	it = m_mObjs.find(key);
	if( it != m_mObjs.end()) {
		pObj = it->second;
//		DBG("[%s:%s] find Tasks, key=%d, pObj=0x%x\r\n", __FILE__, __func__, key, pObj);
		}

	return pObj;
}

int CTaskMgr::ClearTasks(void)
{
	int nRet = 0;
	
	m_mObjs.clear();
	
	DBG("[%s:%s] clear Tasks\r\n", __FILE__, __func__);

	return nRet;
}



int CTaskMgr::On_MSG_CREATE( int wparam, int lparam )
{
	DBG("\033[7;33m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);

	g_pLora = new CLora;
	AddTask(TASK_ID_LORA, g_pLora);
	
	return 0;

}
int CTaskMgr::On_MSG_CLOSE( int wparam, int lparam )
{
	DBG("\033[7;33m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);

	
	return 0;
}
int CTaskMgr::On_MSG_TIMER( int wparam, int lparam )
{
	tm *tm;
	timeval tv;

	gettimeofday( &tv, NULL);
	tm = localtime(&tv.tv_sec);
	
	DBG("\033[7;33m[%s:%s]\033[0m %d:%d:%d:%d w=%d, l=%d\r\n", __FILE__,__func__, 
		tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec, 
		wparam, lparam);

	
	return 0;
}
int CTaskMgr::On_MSG_EVENT( int wparam, int lparam )
{
	DBG("\033[7;32m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);

	
	return 0;
}

int CTaskMgr::On_MSG_QUIT( int wparam, int lparam )
{
	DBG("\033[7;33m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);

	if( g_pLora != NULL) {	
		delete g_pLora;
		g_pLora = NULL;
	
		}

	return -1;
}


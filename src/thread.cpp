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
//  File           : thread.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "thread.h"

#define DEBUG
#include "debug.h"

Thread::Thread(): _thread(0), _runnable(0)
{
	DBG("[%s:%s] mutext_init fail !!! \r\n", __FILE__, __func__);
}
Thread::Thread( Runnable* pRunnable ): _thread(0), _runnable(pRunnable)
{
	DBG("[%s:%s] _runnable=%s \r\n", __FILE__, __func__,_runnable->m_szName);
}
Thread::~Thread()
{
	DBG("[%s:%s] _runnable=%s \r\n", __FILE__, __func__, _runnable->m_szName);
//	delete _runnable;
//	_runnable = NULL;


	DBG("[%s:%s] destroy\r\n",__FILE__, __func__);
}
void Thread::Start() 
{
	int nr = pthread_create(&_thread, NULL, &Thread::Main, this );

	DBG("[%s:%s] _runnable=%s, nr=%d\r\n",__FILE__, __func__, _runnable->m_szName, nr);

}

void Thread::Wait()
{
	void* pData;
	int nr = 0;
	
	if (_runnable != 0) {
		if( _thread ) {
			nr = pthread_join( _thread, &pData);
			}
		}
	
	DBG("[%s:%s] Thread:Wait, _thread=%d, ret=%d \r\n",__FILE__,__func__, _thread, nr);
}

void Thread::Run() {
	if (_runnable != 0) {
		DBG("[%s:%s] %s:Run \r\n",__FILE__,__func__, _runnable->m_szName);
		_runnable->Run();
	}
}

void Thread::Stop() {
	if (_runnable != 0) {
		DBG("[%s:%s] %s:stop \r\n",__FILE__,__func__, _runnable->m_szName);	
		_runnable->Stop();
	}
}

void* Thread::Main(void* pInst)
{
	DBG("[%s:%s] Thread:Main \r\n",__FILE__,__func__);
	
	Thread* pt = static_cast<Thread*>(pInst);
	pt->Run();

	return NULL;
}



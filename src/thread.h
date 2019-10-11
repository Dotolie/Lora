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
//  File           : thread.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __THREAD_H__
#define __THREAD_H__


#include <pthread.h>
#include <string.h>
#include "runnable.h"

class Thread : public Runnable
{
public:
	Thread();

	Thread(Runnable* pRunnable);
	~Thread();
	
	void Start();
	void Wait();
	virtual void Run();
	virtual void Stop();

	
private:
	pthread_t _thread;
	static void* Main(void* pInst);

public:
	Runnable* _runnable;

};

#endif  // __THREAD_H__


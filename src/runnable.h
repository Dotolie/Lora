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


#ifndef __RUNNABLE_H__
#define __RUNNABLE_H__

#include <pthread.h>
#include "base.h"

class Runnable : public CBase
{
private:
	bool m_bRun;
	pthread_mutex_t	m_Mutex;
	
public:
	Runnable():m_bRun(false) { SetName(""); pthread_mutex_init( &m_Mutex, NULL );	};
	virtual ~Runnable() {	pthread_mutex_unlock( &m_Mutex );	pthread_mutex_destroy( &m_Mutex );};
	

	virtual void Run() = 0;
	virtual void Stop() = 0;

	void SetRunBit(bool bit) { m_bRun = bit; };
	bool IsRun() { return m_bRun; };
	
	void LOCK() { pthread_mutex_lock( &m_Mutex );};
	void UNLOCK()	{ pthread_mutex_unlock( &m_Mutex );};
};


#endif  // __RUNNABLE_H__


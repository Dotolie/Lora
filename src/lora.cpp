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
//  File           : rs232.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "object.h"
#include "lora.h"
#include "task.h"
#include <typeinfo>

#define DEBUG
#include "debug.h"



extern CTask app;

CLora::CLora()
{
	SetName("CLora");
}

CLora::~CLora() 
{
	DBG("[%s:%s]  destroy \r\n", __FILE__,__func__ );
}

void CLora::Stop()
{
	SetRunBit(false);
//	DBG("[%s:%s]  m_brun= false\r\n", __FILE__,__func__);
}

void CLora::Run()
{
	int ret = 0;
	SetRunBit(true);

	DBG("[%s:%s] run : decode \r\n", __FILE__,__func__ );

//	usleep(2000000);

	while( IsRun() ) {
			if( app.m_pRs232->m_count > 0  ) {
				printf("%s", app.m_pRs232->m_szBuff[ app.m_pRs232->m_op]);
				LOCK();
				app.m_pRs232->m_count--;
				UNLOCK();
				app.m_pRs232->m_op++;
				app.m_pRs232->m_op%=24;
				}
			else {
				usleep(10000);
//				printf("+");
				}
			}


	DBG("[%s:%s]  end of loop \r\n", __FILE__,__func__ );

}




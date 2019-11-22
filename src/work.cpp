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
//  File           : work.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>

#include "message.h"
#include "work.h"


#define DEBUG
#include "debug.h"


BEGIN_MESSAGE_MAP(CWork)
	{ MSG_CREATE,			EXEC(CWork, MSG_CREATE)			},
	{ MSG_CLOSE,			EXEC(CWork, MSG_CLOSE)			},	
	{ MSG_TIMER,			EXEC(CWork, MSG_TIMER)			},	
	{ MSG_DISCONNECT,		EXEC(CWork, MSG_DISCONNECT)		},	
	{ MSG_QUIT,				EXEC(CWork, MSG_QUIT)			},
END_MESSAGE_MAP()

CWork::CWork() 
{
	DBG("[%s:%s]  create \r\n", __FILE__,__func__ );
}

CWork::~CWork() 
{
	DBG("[%s:%s]  destroy \r\n", __FILE__,__func__ );
}

int CWork::On_MSG_CREATE( int wparam, int lparam )
{

	DBG("\033[7;33m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);

	return 0;
}
int CWork::On_MSG_CLOSE( int wparam, int lparam )
{
	DBG("\033[7;33m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);

	
	return 0;
}
int CWork::On_MSG_TIMER( int wparam, int lparam )
{
	DBG("\033[7;33m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);

	
	return 0;
}
int CWork::On_MSG_DISCONNECT( int wparam, int lparam )
{
	
	DBG("\033[7;33m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);


	return 0;
}

int CWork::On_MSG_QUIT( int wparam, int lparam )
{
	DBG("\033[7;33m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);


	return 0;
}


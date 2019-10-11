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
//  File           : object.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "message.h"
#include "msg.h"
#include "object.h"

#define DEBUG
#include "debug.h"







CObject::CObject()
{
	m_callback = NULL;
	DBG("[%s:%s] create \r\n", __FILE__,__func__);
}

CObject::~CObject()
{
	DBG("[%s:%s] destroy \r\n", __FILE__,__func__);
}

int CObject::InitInstance( funcPtr callback)
{
	
	MSG msg;

	m_callback = callback;

	DBG("[%s:%s]\r\n", __FILE__,__func__);

	SendMessage( 0, MSG_CREATE, 0, 0 );
	
	return 0;
}
void CObject::Run()
{
	int ret = 0;

	while( PeekMessage( m_msg, 0, 0, 0) )
	{
		if( m_callback ) {
//			DBG("[%s:%s] m_callback msg=0x%x, w=0x%x, l=0x%x\r\n", __FILE__,__func__,
//				m_msg.message, m_msg.wparam, m_msg.lparam);
			ret = m_callback( m_hdl, m_msg.message, m_msg.wparam, m_msg.lparam );
			}
		else {
			DBG("[%s:%s]  m_callback is null !!! \r\n", __FILE__,__func__);
			break;
			}
		
	}
	
}
int CObject::ExitInstance()
{
	DBG("[%s:%s]\r\n", __FILE__,__func__);
	
	return m_msg.wparam;
}




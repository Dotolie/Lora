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
//  File           : task.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __TASK_H__
#define __TASK_H__



#include "stdafx.h"
#include "object.h"
#include "thread.h"
#include "base.h"
#include "rs232.h"
#include "lora.h"
#include "msg.h"


typedef int (*funcPtr)(int, int, int, int);


class CTask : public CObject, CBase
{     
protected:
	MSG		m_msg;
	funcPtr 	m_callback;
	
public:
//	DEFINE_MSG_MAP(CTask)
//	DECLARE_MESSAGE_MAP()

	static unsigned short CRC16TBL[];
	CRs232 *m_pRs232;
	Thread *m_pComm;
	CLora *m_pLora;
	Thread *m_pDecode;
	
public:	
	CTask();
	~CTask();
	unsigned short CRC16( const unsigned char* pdata, int size );

	int InitInstance( funcPtr callback);
	void Run();

	int On_PKT_ACK( int, int );
	int On_PKT_NAK( int, int );
	int On_PKT_REQ_GETPARAMS( int, int );
	int On_PKT_REQ_SETPARAMS( int, int );
	int On_PKT_RESET( int, int );		

	virtual int On_MSG_CLOSE( int, int );
	virtual int On_MSG_CREATE( int, int );
	virtual int On_MSG_DISCONNECT( int, int );
	virtual int On_MSG_QUIT( int, int );
	


}; 


#endif /* __TASK_H__ */


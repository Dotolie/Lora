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
//  File           : rs232.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __RS232_H__
#define __RS232_H__

#include <termios.h>
#include "runnable.h"
#include "thread.h"



#define DEVICE_PORT		"/dev/ttyUSB0"


class CRs232: public Runnable
{
private:
	int m_fd;
	struct termios m_oldtio, m_newtio;

public:	
	char m_szBuff[24][1024];
	int m_ip;
	int m_op;
	int m_count;
	
public:
	CRs232();
	virtual ~CRs232();
	

	virtual void Run();
	virtual void Stop();

	void Setup(  );
	int Open();
	int Close();
	
};

#endif  // __WORK_H__

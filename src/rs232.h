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
//  File        : rs232.h
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
#define NUM_BUFF		100


enum {
	CMD_B_RHELP = 0,
	CMD_A_RPAR,
	CMD_A_WGID,
	CMD_A_WNWID,
	CMD_A_RRFWR,
	CMD_A_RFLH,
	CMD_A_WFLH,
	CMD_B_RRSD,
	CMD_B_CRSD,
	CMD_R_RRSU,
	CMD_A_WRST,
	CMD_A_RRSSI,
	CMD_A_RVSN,
	

	CMD_B_TTXS,
	CMD_B_TTXE,
	CMD_A_WARUP,
	CMD_B_WARUP2,
	CMD_B_TBD3,
	CMD_B_WBD1P,
	CMD_B_TBD1,
	CMD_B_TBD2,
	CMD_B_TBD2C,
	CMD_B_TRDP,
	CMD_B_TRDPC,
	CMD_R_TRUP,
	CMD_R_TBUP,

	CMD_X_MAX,
};

class CRs232: public Runnable
{
private:
	int m_fd;
	struct termios m_oldtio, m_newtio;
	char *m_szCmd[CMD_X_MAX];
	
public:	
	char m_szBuff[NUM_BUFF][512];
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
	int SendPkt( char*);
	int SendPkt( int );
	int SendPkt( int, int );
	int SendPkt( int, int, int, int, int );
	char *ReadPkt();
	
};

#endif  // __RS232_H__

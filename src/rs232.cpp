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
#include "stdafx.h"
#include "msg.h"
#include "object.h"
#include "rs232.h"
#include "event.h"
#include "taskMgr.h"


#define DEBUG
#include "debug.h"


extern CTaskMgr g_TaskMgr;



CRs232::CRs232() : m_ip(0), m_op(0), m_count(0)
{
	SetName("CRs232");

	m_szCmd[CMD_B_RHELP] = 	"$?=";
	m_szCmd[CMD_A_RPAR] = 	"$Rpar=";
	m_szCmd[CMD_A_WGID] = 	"$Wgid=";	
	m_szCmd[CMD_A_WNWID] =	"$Wntwid=";
	m_szCmd[CMD_A_RRFWR] =	"$Wrfpower=";
	m_szCmd[CMD_A_RFLH] =	"$Rflash=";
	m_szCmd[CMD_A_WFLH] =	"$Wflash=";
	m_szCmd[CMD_B_RRSD] =	"$Rrsd=";
	m_szCmd[CMD_B_CRSD] =	"$Crsd=";
	m_szCmd[CMD_R_RRSU] =	"$Rrsu=";
	m_szCmd[CMD_A_WRST] =	"$Wreset=";
	m_szCmd[CMD_A_RRSSI] =	"$Rrssi=";
	m_szCmd[CMD_A_RVSN] =	"$Rvsn=";
	

	m_szCmd[CMD_B_TTXS] =	"$Ttx=$";
	m_szCmd[CMD_B_TTXE] =	"$Ttx=";
	m_szCmd[CMD_A_WARUP] =	"$Warup=";
	m_szCmd[CMD_B_WARUP2] =	"$Warup2=";
	m_szCmd[CMD_B_TBD3] =	"$Tbd3=";
	m_szCmd[CMD_B_WBD1P] =	"$Wbd1p=";
	m_szCmd[CMD_B_TBD1] =	"$Tbd1=";
	m_szCmd[CMD_B_TBD2] =	"$Tbd2=";
	m_szCmd[CMD_B_TBD2C] =	"$Tbd2c=";
	m_szCmd[CMD_B_TRDP] =	"$Trdp=";
	m_szCmd[CMD_B_TRDPC] =	"$Trdpc=";
	m_szCmd[CMD_R_TRUP] =	"$Trup=";
	m_szCmd[CMD_R_TBUP] =	"$Tbup=";
	
	DBG("[%s:%s]  create \r\n", __FILE__,__func__ );
}

CRs232::~CRs232() 
{
	SetRunBit(false);

	DBG("[%s:%s]  destroy \r\n", __FILE__,__func__ );
}

void CRs232::Stop()
{
	SetRunBit(false);
//	DBG("[%s:%s]  m_brun= false\r\n", __FILE__,__func__);
}

void CRs232::Run()
{
	int ret = 0;
	SetRunBit(true);

	DBG("[%s:%s] run : read \r\n", __FILE__,__func__ );

	while( IsRun() ) {
			if( (ret = read( m_fd, m_szBuff[m_ip],1024)) > 0 ) {
//				printf("[%d] %s", ret, m_szBuff[m_ip] );			
				m_szBuff[m_ip][ret] = 0;

				LOCK();
				m_count++;
				UNLOCK();
			
				m_ip++;
				m_ip%=NUM_BUFF;
				}
//			usleep(1);
//			printf(".");
			}


	DBG("[%s:%s]  end of loop \r\n", __FILE__,__func__ );

}

int CRs232::Open()
{
	m_fd = open(DEVICE_PORT, O_RDWR | O_NOCTTY);
	
	if( m_fd < 0 ) {
		DBG("[%s:%s] open err \r\n", __FILE__,__func__);
		return m_fd;
		}

	DBG("[%s:%s] open m_fd=%d \r\n", __FILE__,__func__, m_fd);

	return m_fd;
}

int CRs232::Close()
{
	DBG("[%s:%s] close m_fd=%d \r\n", __FILE__,__func__, m_fd);

	if( m_fd > -1 ) {
		tcsetattr(m_fd, TCSANOW, &m_oldtio);
		close(m_fd);
	
		m_fd = -1;
		}
	return 0;
}

void CRs232::Setup()
{
	if( m_fd > -1 ) {
		tcgetattr(m_fd, &m_oldtio);
		bzero(&m_newtio, sizeof(m_newtio));

		m_newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD ;
		m_newtio.c_iflag = IGNPAR;
		m_newtio.c_oflag = 0;
		m_newtio.c_lflag = ICANON;


		m_newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */
		m_newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
		m_newtio.c_cc[VERASE]   = 0;     /* del */
		m_newtio.c_cc[VKILL]    = 0;     /* @ */
		m_newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
		m_newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
		m_newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
		m_newtio.c_cc[VSWTC]    = 0;     /* '\0' */
		m_newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
		m_newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
		m_newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
		m_newtio.c_cc[VEOL]     = 0;     /* '\0' */
		m_newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
		m_newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
		m_newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
		m_newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
		m_newtio.c_cc[VEOL2]    = 0;     /* '\0' */

		
		tcflush(m_fd, TCIFLUSH);
		tcsetattr(m_fd, TCSANOW, &m_newtio);
		
		}

}

int CRs232::SendPkt(char *pBuf)
{
	int nRet = 0;
	char szBuf[128] = {0,};

	if( m_fd > -1 ) {	
		nRet = write(m_fd, szBuf, strlen(szBuf));
		}


	return nRet;
}

int CRs232::SendPkt( int nIdx)
{
	int nRet = 0;
	char szBuf[128] = {0,};

	if( m_fd > -1 ) {
		sprintf(szBuf, "%s\r\n", m_szCmd[nIdx]);		
		nRet = write(m_fd, szBuf, strlen(szBuf));
		}

	return nRet;
}

int CRs232::SendPkt( int nIdx, int nPara1)
{
	int nRet = 0;
	char szBuf[128] = {0,};

	if( m_fd > -1 ) {
		sprintf(szBuf, "%s%d\r\n", m_szCmd[nIdx], nPara1);		
		nRet = write(m_fd, szBuf, strlen(szBuf));
		}

	return nRet;
}

int CRs232::SendPkt( int nIdx, int nPara1, int nPara2, int nPara3, int nPara4)
{
	int nRet = 0;
	char szBuf[128] = {0,};

	if( m_fd > -1 ) {
		sprintf(szBuf, "%s%d:%d:%d:%d\r\n", m_szCmd[nIdx], nPara1, nPara2, nPara3, nPara4);		
		nRet = write(m_fd, szBuf, strlen(szBuf));
		}

	return nRet;
}

char *CRs232::ReadPkt()
{
	char *pBuf = NULL;

	if ( m_count > 0 ) {
		LOCK();
		pBuf = m_szBuff[m_op];
		m_count--;
		UNLOCK();
		m_op++;
		m_op %= NUM_BUFF;
		}
	
	return pBuf;
}




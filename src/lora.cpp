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
#include <sys/time.h>
#include <string.h>

#include "object.h"
#include "taskMgr.h"
#include "lora.h"
#include "timerMgr.h"
#include "event.h"

#define DEBUG
#include "debug.h"



#define TIME_500_MS		500
#define TIME_1000_MS	1000
#define TIME_5000_MS	5000
#define TIME_10000_MS	10000
#define TIME_60_S		60000


#define LEN_RS			10
#define MOD_RS			10
#define FA_BASE			2300

enum {
	LORA_ST_INIT = 0,
	LORA_ST_SYNC,

	LORA_ST_END,
};


using namespace std;

extern CTimerMgr g_TimerMgr;
extern CTaskMgr g_TaskMgr;

CLora *g_pLora = NULL;


CLora::CLora()
{
	SetName("CLora");

	DBG("[%s:%s] create id=0x%x\r\n", __FILE__,__func__, this);
	
	g_TaskMgr.AddTask(TASK_ID_LORA, this);

	m_pHttpClient = new CHttpClient;
	m_pClientThread = new Thread( m_pHttpClient);
	m_pClientThread->Start();
	
	
	m_pRs232 = new CRs232;
	m_pRs232->Open();
	m_pRs232->Setup();
	m_pComm = new Thread( m_pRs232);
	m_pComm->Start();

	m_pRs232->SendPkt(CMD_A_RPAR);
	m_pRs232->SendPkt(CMD_B_TBD1);


	g_TimerMgr.addTimer(TASK_ID_LORA, TIME_1000_MS, LORA_ST_INIT, 0, (void *) this);
}

CLora::~CLora() 
{

	if( m_pComm != NULL) {
		m_pComm->Stop();
		delete m_pComm;
		m_pComm = NULL;
		}
	
	if( m_pRs232 != NULL ) {
		delete m_pRs232;
		m_pRs232 = NULL;
		}

	if( m_pClientThread != NULL) {
		m_pClientThread->Stop();
		delete m_pClientThread;
		m_pClientThread = NULL;
		}
	
	if( m_pHttpClient != NULL ) {
		delete m_pHttpClient;
		m_pHttpClient = NULL;
		}


	g_TaskMgr.DelTask(TASK_ID_LORA);

	DBG("[%s:%s]  destroy \r\n", __FILE__,__func__ );
}

int CLora::On_MSG_TIMER( int wparam, int lparam )
{
	g_TaskMgr.SendMessage(this, MSG_EVENT, EVENT_READ_RS232, 0);

	
	DBG("\033[7;33m[%s:%s]\033[0m w=%d, l=%d\r\n", __FILE__,__func__, wparam, lparam);

	if ( wparam == LORA_ST_INIT ) {
		g_TimerMgr.setTimer(TASK_ID_LORA, TIME_60_S, LORA_ST_SYNC, 0, (void *) this);
		
		}
	else if ( wparam == LORA_ST_SYNC ) {
		m_pRs232->SendPkt(CMD_B_TBD3, 43, LEN_RS, FA_BASE, MOD_RS);		
		}


	
	return 0;
}
int CLora::On_MSG_EVENT( int wparam, int lparam )
{
	char *pBuf = NULL;
	stPacket stPkt;
	
	DBG("\033[7;32m[%s:%s]\033[0m w=%d, l=%d\n", __FILE__,__func__,	wparam, lparam);

	switch(wparam) {
		case EVENT_READ_RS232:
			while(1) {
				pBuf = m_pRs232->ReadPkt();
				if( pBuf == NULL ) 
					break;

//				DBG("(%d) %s", m_pRs232->m_count, pBuf);
				stPkt = parsePacket(pBuf);
				if( stPkt.Urup.nRSd != 0)
					m_qPkt.push(stPkt);
				}
			break;

		default:
			break;
		}
	
	return 0;
}

stPacket CLora::parsePacket(char *pBuf)
{
	char *token = NULL;
	char del[] = " ,:";
	int column = 0;
	char *pMsg[20];
	time_t currunt;
	tm *tm;
	timeval tv;
	stPacket stPkt = {0,};
	
	gettimeofday( &tv, NULL);
	tm = localtime(&tv.tv_sec);

	currunt = time(NULL);
	

	token = strtok( pBuf, del );
	while( token != NULL ) {
		pMsg[column] = token;
		token = strtok( NULL, del);
		column++;
		}

	if ( strcmp( pMsg[0], "Urup") == 0 ) {
		stPkt.Urup.nRSd = atoi( pMsg[1]);
		stPkt.Urup.nIA = atoi( pMsg[2]);
		stPkt.Urup.nRSu = atoi( pMsg[3]);
		stPkt.Urup.nSeq = atoi( pMsg[4]);
		stPkt.Urup.nCO2 = atoi( pMsg[5]);
		stPkt.Urup.nRH = atoi( pMsg[6]);
		stPkt.Urup.nTemp = atoi( pMsg[7]);
		stPkt.Urup.nPm1P0 = atoi( pMsg[8]);
		stPkt.Urup.nPm2P5 = atoi( pMsg[9]);
		stPkt.Urup.nPm4P0 = atoi( pMsg[10]);		
		stPkt.Urup.nPm10P0 = atoi( pMsg[11]);
		stPkt.Urup.sBat = atoi( pMsg[12]);
		stPkt.Urup.nTime = currunt;		
		sprintf(stPkt.Urup.szDateTime, "%04d%02d%02d%02d%02d%02d", 
								tm->tm_year+1900,
								tm->tm_mon+1,
                                tm->tm_mday,
                                tm->tm_hour,
                                tm->tm_min,
                                tm->tm_sec);
	printf("====%d\r\n", currunt);
#if 0

		DBG("[%d:%d:%d:%d] ", tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec/10000 );

		DBG("Urup:RSd=%d IA=%d RSu=%d S=%d CO2=%.1f RH=%.1f T=%.1f pm1=%.2f pm2.5=%.2f pm4=%.2f pm10=%.2f ,B=%d\r\n",
			stPkt.Urup.nRSd,
			stPkt.Urup.nIA,
			stPkt.Urup.nRSu,
			stPkt.Urup.nSeq,
			stPkt.Urup.nCO2/10.0f,
			stPkt.Urup.nRH/10.0f,
			stPkt.Urup.nTemp/10.0f,
			stPkt.Urup.nPm1P0/100.0f,
			stPkt.Urup.nPm2P5/100.0f,
			stPkt.Urup.nPm4P0/100.0f,
			stPkt.Urup.nPm10P0/100.0f,
			stPkt.Urup.sBat
			)
		
#endif
		}
	else {
		printf("[%d]", column);
		for(auto i=0;i<column;i++) {
			printf(" : %s", pMsg[i]);
			}

		}

		
	return stPkt;
}



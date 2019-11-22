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
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "stdafx.h"
#include "msg.h"
#include "object.h"
#include "httpclient.h"
#include "event.h"
#include "taskMgr.h"


#define DEBUG
#include "debug.h"


extern CTaskMgr g_TaskMgr;

#define STR_HOST_URI		"http://192.168.0.4:3000/sensing/new"
#define STR_CONTENT_TYPE	"Content-Type: application/json"
#define STR_USER_AGENT		"User-Agent: MVTech LORA-10 Device/1.0.0"
#define STR_OTHER_MSG		"SERIAL: 2019M010002"

#define HTTP_TIMEOUT_MS		1000L


using namespace std;

CHttpClient::CHttpClient()
{
	SetName("CHttpClient");

	m_pList = NULL;
	m_pCurl = NULL;

	InitCurl();
	MakeHeader();
	
	DBG("[%s:%s]  create \r\n", __FILE__,__func__ );
}

CHttpClient::~CHttpClient() 
{
	SetRunBit(false);
	
	EndCurl();

	DBG("[%s:%s]  destroy \r\n", __FILE__,__func__ );
}

void CHttpClient::Stop()
{
	SetRunBit(false);
	
	DBG("[%s:%s]  m_brun= false\r\n", __FILE__,__func__);
}

void CHttpClient::Run()
{
	int ret = 0;
	stPacket stPkt;
	char szJson[256];
	
	SetRunBit(true);
	CLora *pTaskLora = (CLora*)g_TaskMgr.FindTask(TASK_ID_LORA);
	
	DBG("[%s:%s] run : read \r\n", __FILE__,__func__ );

	while( IsRun() ) {
		if( !pTaskLora->m_qPkt.empty()) {
			stPkt = pTaskLora->m_qPkt.front();
	        sprintf(szJson, "{\"RSD\":%d, \"IA\":%d, \"RSU\":%d, \"SEQ\":%d, \"REGDATE\":\"%d\", \"CO2\":%.1f, \"RH\":%.1f, \"TEMP\":%.1f, \"PM1P0\":%.2f, \"PM2P5\":%.2f, \"PM4P0\":%.2f, \"PM10P0\":%.2f, \"BATT\":%d }",
				stPkt.Urup.nRSd, 
				stPkt.Urup.nIA,
				stPkt.Urup.nRSu,
				stPkt.Urup.nSeq,
				stPkt.Urup.nTime,
				stPkt.Urup.nCO2/10.0f,
				stPkt.Urup.nRH/10.0f,
				stPkt.Urup.nTemp/10.0f,
				stPkt.Urup.nPm1P0/100.0f,
				stPkt.Urup.nPm2P5/100.0f,
				stPkt.Urup.nPm4P0/100.0f,
				stPkt.Urup.nPm10P0/100.0f,
				stPkt.Urup.sBat				
				);
			ret = PostSend(szJson);
			
			DBG("[%d] Urup:IA=%d RSu=%d S=%d CO2=%.1f RH=%.1f T=%.1f PM1=%.2f PM2.5=%.2f PM4=%.2f PM10=%.2f B=%d\r\n",
				stPkt.Urup.nTime,
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
				);
		
			pTaskLora->m_qPkt.pop();
			}
		else {
			usleep(100000);
			}
		}


	DBG("[%s:%s]  end of loop \r\n", __FILE__,__func__ );

}

void CHttpClient::InitCurl()
{
	curl_global_init(CURL_GLOBAL_ALL);

	DBG("[%s:%s] \r\n", __FILE__,__func__ );
}

void CHttpClient::MakeHeader()
{
	m_pList = curl_slist_append(m_pList, STR_CONTENT_TYPE); // content-type >정의 내용 list에 저장
    m_pList = curl_slist_append(m_pList, STR_USER_AGENT); // content-type 정의 내용 list에 저장
    m_pList = curl_slist_append(m_pList, STR_OTHER_MSG); // content-type 정의 내용 list에 저장

	DBG("[%s:%s] \r\n", __FILE__,__func__ );
}

int CHttpClient::PostSend(char *pJson)
{
	m_pCurl = curl_easy_init();
	string html;
	
	if(m_pCurl) {
		curl_easy_setopt(m_pCurl, CURLOPT_URL, STR_HOST_URI); 	// webserver ip 주소와 포트번호, flask 대상 router
		curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_pList); // content-type 설정
		curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT_MS, HTTP_TIMEOUT_MS); 
//		curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, writeCallback); 
//		curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, &html); 
//		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 1L);	// 값을 false 하면 에러가 떠서 공식 문서 참고함
//		curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 1L);	// 값을 false 하면 에러가 떠서 공식 문서 참고함 	
		curl_easy_setopt(m_pCurl, CURLOPT_POST, 1L);			// POST option
		curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, pJson );

		/* Perform the request, res will get the return code */
		m_Res = curl_easy_perform(m_pCurl);   // curl 실행 res는 curl 실행후 응답내용이

		

		/* Check for errors */
		if(m_Res == CURLE_OPERATION_TIMEDOUT) {
			DBG("[%s:%s] %s \r\n", __FILE__,__func__, curl_easy_strerror(m_Res) );
			}
		else if(m_Res != CURLE_OK) {
			DBG("[%s:%s] %s\r\n", __FILE__,__func__, curl_easy_strerror(m_Res) );
			}
		else {
			long statlong;
			
			curl_easy_getinfo(m_pCurl, CURLINFO_HTTP_CODE, &statlong);
			
			DBG("[%s:%s] response=%ld\r\n", __FILE__,__func__, statlong );
			}

		/* always cleanup */
		curl_easy_cleanup(m_pCurl);
		m_pCurl = NULL;

		}


	return m_Res;
}

void CHttpClient::EndCurl()
{
	curl_slist_free_all(m_pList);
	m_pList = NULL;
	
	curl_global_cleanup(); // curl_global_init 과 세트

	DBG("[%s:%s]\r\n", __FILE__,__func__ );
}

size_t CHttpClient::writeCallback(void *ptr, size_t size, size_t count, void *stream)
{
	((string*)stream)->append((char*)ptr, 0, size*count); // stream에 문자열을 추가한다.
	
	DBG("[%s:%s] %s\r\n", __FILE__,__func__, (char*)ptr );

	
	return size*count;
}


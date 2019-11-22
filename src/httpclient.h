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
//  File        : httpclient.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#include <curl/curl.h>
#include "runnable.h"
#include "thread.h"






class CHttpClient: public Runnable
{
public:
	CURL *m_pCurl;
	CURLcode m_Res;
	struct curl_slist *m_pList;
	
public:
	CHttpClient();
	virtual ~CHttpClient();
	

	virtual void Run();
	virtual void Stop();

	void InitCurl();
	void EndCurl();
	void MakeHeader();
	int PostSend(char *);
	static size_t writeCallback(void *, size_t, size_t, void *);
	
};

#endif  // __HTTPCLIENT_H__

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
//  File           : work.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __WORK_H__
#define __WORK_H__


#include "object.h"
#include "stdafx.h"


class CWork: public CObject
{
public:
	DEFINE_MSG_MAP(CWork)
	DECLARE_MESSAGE_MAP()


public:
	CWork();
	virtual ~CWork();
	
	virtual int On_MSG_CREATE( int, int );
	virtual int On_MSG_CLOSE( int, int );
	virtual int On_MSG_DISCONNECT( int, int );
	virtual int On_MSG_QUIT( int, int );

};

#endif  // __WORK_H__

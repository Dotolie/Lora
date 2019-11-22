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
//  File           : object.h
//  Description :
//  Author      : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#ifndef __OBJECT_H__
#define __OBJECT_H__


class CObject
{     
public:
	CObject *m_pId;
	
public:
	CObject() { m_pId = this; };
	virtual ~CObject() {};


	virtual int On_MSG_CLOSE( int, int ) {};
	virtual int On_MSG_CREATE( int, int ) {};
	virtual int On_MSG_TIMER( int, int ) {};
	virtual int On_MSG_EVENT( int, int ) {};
	virtual int On_MSG_QUIT( int, int ) {};

}; 


#endif /* __OBJECT_H__ */


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
//  File           : stdafx.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __STDAFX_H__
#define __STDAFX_H__

#define DEFINE_MSG_MAP(class)	struct MessageMap { \
									int iMsg;	\
									int (class::*mfp)( int wparam, int lparam );	\
								};


//#define DECLARE_MESSAGE_MAP()			static MessageMap messageMap[];
#define BEGIN_MESSAGE_MAP(class_name)	class_name::MessageMap class_name::messageMap[] = {
#define END_MESSAGE_MAP()				{0, NULL}};	



#define 	EXEC_R(c,x)		&c::On_##x
#define 	EXEC_S(c,x)		&c::On_##x
#define 	EXEC(c,x)		&c::On_##x


//int MsgProc(int hdl, int msg, int wparam, int lparam);


#endif /* __STDAFX_H__ */


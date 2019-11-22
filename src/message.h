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
//  File           : message.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

enum {

	MSG_CREATE 		= 0x1000,
	MSG_CLOSE		= 0x1100,
	
	MSG_TIMER		= 0x2000,
	
	MSG_DISCONNECT 	= 0x4000,

	MSG_QUIT		= 0x9900,
	MSG_END			= 0xf000,
};

#endif /* __MESSAGE_H__ */



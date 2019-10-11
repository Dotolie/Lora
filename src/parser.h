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
//  File           : parser.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __PARSER_H__
#define __PARSER_H__

#define BUFFER_SIZE		4096
#define BUFFER_MAX		12

class CParser
{     
private:         
	int m_state;
	int m_idx;
	int m_size;
	short m_cmd;
	int m_nIndex;
	
public:
	char m_buffer[BUFFER_MAX][BUFFER_SIZE];
	char *m_pBuffer;

public:
	CParser();
	int decode(const char);
	short GetCmd();
	int GetSize();
	char* GetData();


}; 


#endif /* __PARSER_H__ */


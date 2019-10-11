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
//  File           : parser.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include "parser.h"
#include <stdio.h>

#define DEBUG
#include "debug.h"



CParser::CParser()
: m_idx(0), m_state(0), m_size(0)
{
	m_pBuffer = NULL;
	m_nIndex = 0;
	DBG("[%s:%s] create\r\n", __FILE__,__func__, m_size);	
}
int CParser::decode(const char ch)
{
	int ret = 0;
	
	switch(m_state) {
	case 0:
		if( ch == 'M' ) m_state++;
		break;
	case 1:
		if( ch == 'V' ) m_state++;
		else m_state = 0;
		m_size = 0;
		m_cmd = 0;
		break;
	//------------------------------
	case 2:
		m_size = (ch&0xff);
		m_state++;
		break;
	case 3:
		m_size |= (ch<<8)&0xff;
		m_size-=5;
		m_state++;
		break;
	case 4:
		m_cmd = ch;
		if( m_size > 0 )  {
			m_idx = 0;			
			m_state++;

			m_nIndex++;
			m_nIndex %=BUFFER_MAX;
			m_pBuffer = m_buffer[m_nIndex];
			}
		else {
			m_state = 0;	
			ret = 1;
			}
		break;
	//----------------------------------
	case 5:
		*(m_pBuffer+m_idx) = ch;
		m_idx++;
		if(m_idx >= (m_size)) {
//			DBG("[%s] data[0]=%x,%x,%x,%x\r\n", __func__, m_buffer[0], m_buffer[1],m_buffer[2],m_buffer[3]);
			m_state = 0;
			ret = 1;
			}
		break;
	default:
		m_state = 0;
		break;
		}

	return ret;
}

short CParser::GetCmd()
{
	return m_cmd;
}
int CParser::GetSize()
{
	return m_size;
}
char* CParser::GetData()
{
	return m_pBuffer;
}



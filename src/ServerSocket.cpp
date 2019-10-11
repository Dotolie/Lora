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
//  File           : serverSocket.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#include "ServerSocket.h"
#include "SocketException.h"
#include <stdio.h>


CServerSocket::CServerSocket ( int port )
{
	if ( ! CSocket::create() )
	{
		throw SocketException ( "Could not create server socket." );
	}

	if ( ! CSocket::bind ( port ) )
	{
		throw SocketException ( "Could not bind to port." );
	}
#if 0
	if ( ! CSocket::listen() )
	{
		throw SocketException ( "Could not listen to socket." );
	}
#endif
}

CServerSocket::CServerSocket ()
{
}

CServerSocket::~CServerSocket()
{
}

void CServerSocket::accept ( CServerSocket& sock )
{
	if ( ! CSocket::accept ( sock ) )
	{
		throw SocketException ( "Could not accept socket." );
	}
}

void CServerSocket::accept ( CServerSocket* psock )
{
	if ( ! CSocket::accept ( psock ) )
	{
		throw SocketException ( "Could not accept socket." );
	}
}


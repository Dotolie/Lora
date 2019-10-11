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
//  File           : Socket.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

CSocket::CSocket()
: m_nSock ( -1 )
{
	if( 0 != pthread_mutex_init( &m_Mutex, NULL ) ){
		;
	}
	memset ( &m_addr, 0, sizeof ( m_addr ) );
}

CSocket::~CSocket()
{
	pthread_mutex_unlock( &m_Mutex );
	pthread_mutex_destroy( &m_Mutex );

//	fprintf(stderr, "[%s:%s] status == -1   errno == %d\r\n", __FILE__,__func__,  errno);

	if ( is_valid() )
		::close ( m_nSock );
}

bool CSocket::create()
{
	m_nSock = socket( AF_INET,  SOCK_DGRAM, 0 );

	if ( ! is_valid() )
		return false;

	// TIME_WAIT - argh
	int on = 1;
	if ( setsockopt ( m_nSock, SOL_SOCKET, SO_BROADCAST, ( const char* ) &on, sizeof ( on ) ) == -1 )
		return false;

	return true;
}


bool CSocket::bind ( const int port )
{
	if ( ! is_valid() )
	{
		return false;
	}
	//Prepare the sockaddr_in structure
	memset(&m_addr, 0x00, sizeof(struct sockaddr_in));

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_addr.sin_port = htons( port );

	int bind_return = ::bind ( m_nSock,  (struct sockaddr*)&m_addr, sizeof( m_addr ) );

	if ( bind_return == -1 )
	{
		return false;
	}

	return true;
}


bool CSocket::listen() const
{
	if ( ! is_valid() )
	{
		return false;
	}

	int listen_return = ::listen ( m_nSock, MAXCONNECTIONS );


	if ( listen_return == -1 )
	{
		return false;
	}

 	return true;
}


bool CSocket::accept ( CSocket& new_socket ) const
{
	int addr_length = sizeof( m_addr );
	new_socket.m_nSock = ::accept ( m_nSock, (sockaddr*) &m_addr, (socklen_t*) &addr_length );

	if ( new_socket.m_nSock <= 0 )
		return false;
	else
		return true;
}

bool CSocket::accept ( CSocket* psocket ) const
{
	int addr_length = sizeof( m_addr );
	psocket->m_nSock = ::accept ( m_nSock, (sockaddr*) &m_addr, (socklen_t*) &addr_length );

	if ( psocket->m_nSock <= 0 )
		return false;
	else
		return true;
}


int CSocket::send ( const char* s, int size )
{
	int status = -1;

	if( m_nSock != -1 ) {
		pthread_mutex_lock( &m_Mutex );
		status  = ::send( m_nSock, s, size, MSG_NOSIGNAL );
		pthread_mutex_unlock( &m_Mutex );
		}


	return status;
}

int CSocket::sendTo( const char* s, int size, struct sockaddr_in* pClient_addr )
{
	int status = -1;

	if( m_nSock != -1 ) {
		pthread_mutex_lock( &m_Mutex );
		status  = ::sendto( m_nSock, s, size, 0, (struct sockaddr*)pClient_addr, sizeof(struct sockaddr_in)  );
		pthread_mutex_unlock( &m_Mutex );
		}
	else {
		fprintf(stderr, "[%s:%s] m_nSock=%d\r\n", __FILE__,__func__,m_nSock);
		}
}

int CSocket::recv ( char* s ) const
{
	char buf[4];
	int status = ::recv ( m_nSock, buf, 1, MSG_NOSIGNAL );

	if ( status == -1 )
	{
//		fprintf(stderr, "[%s:%s] status == -1   errno == %d\r\n", __FILE__,__func__,  errno);
		return -1;
	}
	else if ( status == 0 )
	{
//		fprintf(stderr, "[%s:%s] status == 0   errno == %d\r\n", __FILE__,__func__,  errno);
		return 0;
	}

	*s = buf[0];
	
	return status;
}

int CSocket::recvFrom ( char* s,  struct sockaddr_in* pClient_addr) const
{
	
	int client_addr_size = sizeof( struct sockaddr_in );

	int status = ::recvfrom ( m_nSock, s, 2048,  0,( struct sockaddr*)pClient_addr, (socklen_t*)&client_addr_size );

	if ( status == -1 )
	{
		fprintf(stderr, "[%s:%s] status == -1  m_nSock=%d errno == %d\r\n", __FILE__,__func__, m_nSock,  errno);
		return -1;
	}
	else if ( status == 0 )
	{
		fprintf(stderr, "[%s:%s] status == 0   errno == %d\r\n", __FILE__,__func__,  errno);
		return 0;
	}
	
	return status;
}


bool CSocket::connect ( const std::string host, const int port )
{
	if ( ! is_valid() ) 
		return false;

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons ( port );

	int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

	if ( errno == EAFNOSUPPORT )
		return false;

	status = ::connect ( m_nSock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

	if ( status == 0 )
		return true;
	else
		return false;
}

void CSocket::set_non_blocking ( const bool b )
{
	int opts;

	opts = fcntl ( m_nSock, F_GETFL );

	if ( opts < 0 )
	{
		return;
	}

	if ( b )
		opts = ( opts | O_NONBLOCK );
	else
		opts = ( opts & ~O_NONBLOCK );

	fcntl ( m_nSock, F_SETFL,opts );
}


int CSocket::close ()
{
	fprintf(stderr, "[%s:%s] m_nSock=%d\r\n", __FILE__,__func__,m_nSock);
	
	if( is_valid() ) {
		::close(m_nSock);
		}
	
	m_nSock = -1;

	return 0;
}


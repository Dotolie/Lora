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
//  File           : Socket.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __SOCKET_H__
#define __SOCKET_H__


#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <pthread.h>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 1;

class CSocket
{
 private:
//	int m_nSock;
	sockaddr_in m_addr;
	pthread_mutex_t	m_Mutex;

 public:
	CSocket();
	virtual ~CSocket();

	// Server initialization
	bool create();
	bool bind ( const int port );
	bool listen() const;
	bool accept ( CSocket& ) const;
	bool accept ( CSocket* ) const;
	
	// Client initialization
	bool connect ( const std::string host, const int port );

	// Data Transimission
	int send ( const char*, int );
	int sendTo( const char*, int,  struct sockaddr_in* );
	int recv ( char* ) const;
	int recvFrom( char*,  struct sockaddr_in* ) const;
	int close();
	
	void set_non_blocking ( const bool );
	bool is_valid() const { return m_nSock != -1; }

	int m_nSock;
};


#endif  // __SOCKET_H__

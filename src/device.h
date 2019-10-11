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
//  File           : device.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <unistd.h>
#include <arpa/inet.h>
#include "thread.h"
#include "INIReader.h"
#include "packet.h"


#define UGPIO_NAME    		"/dev/ugpio"

#define UGPIO_MINOR   		253
#define IOCTL_MAGIC			UGPIO_MINOR
#define IOCTL_GHIGH_SET		_IO(IOCTL_MAGIC,2)
#define IOCTL_GLOW_SET		_IO(IOCTL_MAGIC,3)

#define gWDT			1
#define gLED_PWR1		2
#define gLED_PWR2		3
#define gLED_WIFI1		4
#define gLED_WIFI2		5
#define gLED_CPU		6	
#define gETH_PWR		7	
#define gWIFI_PWR		8	

class CDevice: public Runnable
{
protected:
	bool m_bRun;
	
public:
	CDevice();
	virtual ~CDevice();
	virtual void Run();
	virtual void Stop();	

private:
	INIReader *m_pReader;
	int getMac( char*);
	int getLink();
	int m_nFd;
	int m_nLink;
	
public:
	;
	int SaveIniFile(DEVICE_t*);
	int SaveWpaFile(DEVICE_t*);
	int SetDefault();
	int StopProcess();
	void LedSet(int);
	DEVICE_t m_Device;
	
};

#endif  // __DEVICE_H__

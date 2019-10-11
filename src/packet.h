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
//  File           :
//  Description :
//  Author      :
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#ifndef __PACKET_H__
#define __PACKET_H__

#include <arpa/inet.h>

#define PACKET_ID			0x434f5802


#define MAKE_NETHDR( data, size, cmd ) 	\
	do {									\
		data[0] = 'M'; 					\
		data[1] = 'V'; 					\
		data[2] = size & 0xff;				\
		data[3] = (size >> 8) & 0xff;		\
		data[4] = cmd;					\
	} while(0)



enum {
	PKT_ACK                 		= 0x01,
	PKT_NAK                 		= 0x02,
	PKT_REQ_GETPARAMS		= 0x03,
	PKT_ANS_GETPARAMS		= 0x04,
	PKT_REQ_SETPARAMS		= 0x05,
	PKT_ANS_SETPARAMS		= 0x06,	
	PKT_RESET				= 0x07,

	
	PKT_END				= 0xff,
};

typedef struct  
{
	uint8_t		type;
	uint16_t		address;
	uint16_t		value;
} __attribute__((packed)) REGISTER_t, *REGISTER_PTR_t;

typedef struct  
{
	char d_szMac[6];
	int d_nVersion;
	char d_szSerial[5];

	bool d_bEth0Dhcp;
	uint32_t d_nEth0Ip;
	uint32_t d_nEth0Gw;
	uint32_t d_nEth0Mask;

	bool d_bWlan0Enable;
	char d_szSsid[16];
	char d_szKey[16];
} __attribute__((packed)) DEVICE_t, *DEVICE_PTR_t;



#endif /* __PACKET_H__ */


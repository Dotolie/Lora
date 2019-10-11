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

#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>   //ifreq
#include <unistd.h>   //close
#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>

#include "object.h"
#include "device.h"
	   
#define DEBUG
#include "debug.h"


#define INI_FILE_NAME		"/root/system.ini"
#define WPA_FILE_NAME		"/root/wpa_supplicant.conf"

CDevice::CDevice()
{
	char strTemp[32] = {0,};
	struct sockaddr_in addr;
	
	DBG("[%s:%s] create\r\n", __FILE__,__func__);

	m_nFd = open(UGPIO_NAME, 0);
	if( m_nFd >= 0 ) {
		ioctl(m_nFd,IOCTL_GHIGH_SET,gLED_PWR1 );
//		ioctl(m_nFd,IOCTL_GHIGH_SET,gLED_PWR2 );
//		ioctl(m_nFd,IOCTL_GHIGH_SET,gLED_WIFI1);
//		ioctl(m_nFd,IOCTL_GHIGH_SET,gLED_WIFI2);
//		ioctl(m_nFd,IOCTL_GHIGH_SET,gLED_CPU  );	
		}
	
	SetDefault();
	
	m_pReader = new INIReader(INI_FILE_NAME);
	if( m_pReader->ParseError() < 0 ) {
		fprintf(stderr, "[%s:%s] system.ini does't exist error, INIReader\r\n", __FILE__,__func__);
		SaveIniFile( &m_Device );
		}
	
	getMac(strTemp);
	m_nLink = getLink();
	strcpy(m_Device.d_szMac, strTemp);
	m_Device.d_nVersion = m_pReader->GetInteger("system", "version", 1);
	strcpy(m_Device.d_szSerial, m_pReader->Get("system", "serial", "0001").c_str());

	m_Device.d_bEth0Dhcp =  m_pReader->GetBoolean("ethernet", "dhcp", true);
	m_Device.d_nEth0Ip = inet_addr( m_pReader->Get("ethernet", "ip", "192.168.0.10").c_str());
	m_Device.d_nEth0Gw = inet_addr( m_pReader->Get("ethernet", "gw", "192.168.0.1").c_str());
	m_Device.d_nEth0Mask = inet_addr( m_pReader->Get("ethernet", "mask", "255.255.0.0").c_str());


	m_Device.d_bWlan0Enable = m_pReader->GetBoolean("wifi", "enable", false);
	strcpy(m_Device.d_szSsid, m_pReader->Get("wifi", "ssid", "send0").c_str());
	strcpy(m_Device.d_szKey, m_pReader->Get("wifi", "key", "1234abcd").c_str());	

#if 1
	std::cout << " ver." << m_Device.d_nVersion << std::endl;

	std::cout << "[system]" << std::endl;
	std::cout << "version = " << m_Device.d_nVersion << std::endl;
	std::cout << "Serial = " << m_Device.d_szSerial << std::endl  << std::endl;

	std::cout << "[ethernet]" << std::endl;
	std::cout << "enable dhcp = " << m_Device.d_bEth0Dhcp << std::endl;
	addr.sin_addr.s_addr = m_Device.d_nEth0Ip;
	std::cout << "ip Eth = " << inet_ntoa(addr.sin_addr) << std::endl;
	addr.sin_addr.s_addr = m_Device.d_nEth0Gw;
	std::cout << "gw Eth = " << inet_ntoa(addr.sin_addr) << std::endl;
	addr.sin_addr.s_addr = m_Device.d_nEth0Mask;
	std::cout << "Mask Eth = " << inet_ntoa(addr.sin_addr) << std::endl << std::endl;

	std::cout << "[wifi]" << std::endl;
	std::cout << "enable wifi = " << m_Device.d_bWlan0Enable << std::endl;
	std::cout << "ssid = " << m_Device.d_szSsid << std::endl;
	std::cout << "key = " << m_Device.d_szKey << std::endl;
#endif

}



CDevice::~CDevice()
{
	DBG("[%s:%s] destroy\r\n", __FILE__,__func__);

	if( m_nFd >= 0 ) {
		ioctl(m_nFd,IOCTL_GLOW_SET,gLED_PWR1 );
		ioctl(m_nFd,IOCTL_GLOW_SET,gLED_PWR2 );
		ioctl(m_nFd,IOCTL_GLOW_SET,gLED_WIFI1);
		ioctl(m_nFd,IOCTL_GLOW_SET,gLED_WIFI2);
		ioctl(m_nFd,IOCTL_GLOW_SET,gLED_CPU  );	

		::close(m_nFd);
		m_nFd = -1;
		}

	if( m_pReader )
		delete m_pReader;
	
	m_pReader = NULL;
}

void CDevice::Stop()
{

	m_bRun = false;
	DBG("[%s:%s]  m_brun= false\r\n", __FILE__,__func__);
}


void CDevice::Run()
{
	
	int nRet = 0;
	int nPid = 0;
	char szBuf[128];
	char szAddr0[16];
	char szAddr1[16];
	std::ifstream inFile;
	struct sockaddr_in addr0, addr1;
	
#if 0
	nRet = system("cat /run/dhclient.eth0.pid | xargs kill >> /dev/null 2>&1");
	printf("kill dhclient.eth0 =============> nRet = %d\r\n", nRet);
	nRet = system("cat /run/dhclient.wlan0.pid | xargs kill >> /dev/null 2>&1");
	printf("kill dhclient.wlan0 =============> nRet = %d\r\n", nRet);
	nRet = system("cat /run/wpa_supplicant.pid | xargs kill >> /dev/null 2>&1");
	printf("kill wpa_supplicant =============> nRet = %d\r\n", nRet);	
	//usleep(1000000);
#else
	StopProcess();
#endif 


//	nRet = system("ifconfig eth0 down >> /dev/null 2>&1");
//	DBG("ifconfig eth0 down =============> nRet = %d\r\n", nRet);
	nRet = system("ifconfig wlan0 down >> /dev/null 2>&1");
	DBG("ifconfig wlan0 down =============> nRet = %d\r\n", nRet);
		

	if( m_Device.d_bEth0Dhcp ) {
		nRet = system("ifconfig eth0 up >> /dev/null 2>&1");
		DBG("ifconfig eth0 up =============> nRet = %d\r\n", nRet);
		usleep(5000000);
		if (getLink()) {	
			nRet = system("dhclient -pf /run/dhclient.eth0.pid eth0 >> /dev/null 2>&1");
			DBG("dhclient eth0=============> nRet = %d\r\n", nRet);
			}
		}
	else {
		addr0.sin_addr.s_addr = m_Device.d_nEth0Ip;
		strcpy(szAddr0, inet_ntoa(addr0.sin_addr) );
		addr1.sin_addr.s_addr = m_Device.d_nEth0Mask;
		strcpy(szAddr1, inet_ntoa(addr1.sin_addr));
		sprintf(szBuf, "ifconfig eth0 %s netmask %s up >> /dev/null 2>&1", szAddr0 , szAddr1);
		nRet = system(szBuf);
		DBG("%s =============> nRet = %d\r\n", szBuf, nRet);

		addr0.sin_addr.s_addr = m_Device.d_nEth0Gw;
		sprintf(szBuf, "route add default gw %s eth0 >> /dev/null 2>&1", inet_ntoa(addr0.sin_addr));
		nRet = system(szBuf);

		DBG("%s =============> nRet = %d\r\n", szBuf, nRet);
		}
		
	
	if( m_Device.d_bWlan0Enable ) {
		nRet = system("ifconfig wlan0 up >> /dev/null 2>&1");
		DBG("ifconfig wlan0 up =============> nRet = %d\r\n", nRet);

		nRet = system("wpa_supplicant -P /run/wpa_supplicant.pid -Dnl80211 -iwlan0 -c/root/wpa_supplicant.conf -B >> /dev/null 2>&1");
		DBG("wpa_supplicant  =============> nRet = %d\r\n", nRet);

		nRet = system("dhclient -pf /run/dhclient.wlan0.pid wlan0 >> /dev/null 2>&1");
		DBG("dhclient wlan0=============> nRet = %d\r\n", nRet);
		}

	DBG("[%s:%s] \r\n", __FILE__,__func__);
}

int CDevice::SetDefault()
{
	m_Device.d_nVersion = 1;
	strcpy( m_Device.d_szSerial, "0001");

	m_Device.d_bEth0Dhcp = true;
	m_Device.d_nEth0Ip =  inet_addr("192.168.0.10");
	m_Device.d_nEth0Gw = inet_addr("192.168.0.1");
	m_Device.d_nEth0Mask = inet_addr("255.255.0.0");
 
	m_Device.d_bWlan0Enable = false;
	strcpy( m_Device.d_szSsid, "send0" );
	strcpy( m_Device.d_szKey, "1234abcd" );
}

int CDevice::SaveIniFile(DEVICE_t *pDev)
{
	struct sockaddr_in addr0, addr1;
	std::ofstream outFile(INI_FILE_NAME);


	outFile << "; Generated ini file for default" << std::endl << std::endl;

	outFile << "[system]" << std::endl;
	outFile << "version = " << pDev->d_nVersion << std::endl;
	outFile << "serial = " << pDev->d_szSerial << std::endl << std::endl;

	outFile << "[ethernet]" << std::endl;
	outFile << "dhcp = " << pDev->d_bEth0Dhcp << std::endl;
	addr0.sin_addr.s_addr = pDev->d_nEth0Ip;
	outFile << "ip = " << inet_ntoa(addr0.sin_addr) << std::endl;
	addr1.sin_addr.s_addr = pDev->d_nEth0Gw;
	outFile << "gw = " << inet_ntoa(addr1.sin_addr) << std::endl;
	addr0.sin_addr.s_addr = pDev->d_nEth0Mask;
	outFile << "mask = " << inet_ntoa(addr0.sin_addr) << std::endl << std::endl;

	outFile << "[wifi]" << std::endl;
	outFile << "enable = " << pDev->d_bWlan0Enable << std::endl;
	outFile << "ssid = " << pDev->d_szSsid << std::endl;
	outFile << "key = " << pDev->d_szKey << std::endl;

	outFile.close();

	DBG("\033[7;31m[%s:%s]\033[0m \r\n", __FILE__,__func__);
	
	return 0;
}

int CDevice::SaveWpaFile(DEVICE_t *pDev)
{
	std::ofstream outFile(WPA_FILE_NAME);

	outFile << "ctrl_interface=/var/run/wpa_supplicant" << std::endl;
	outFile << "update_config=1" << std::endl << std::endl;
	outFile << "network={" << std::endl;
	outFile << "	ssid=\"" << pDev->d_szSsid << "\"" << std::endl;
	outFile << "	psk=\"" << pDev->d_szKey << "\"" << std::endl;
	outFile << "}" << std::endl;

	outFile.close();

	DBG("\033[7;31m[%s:%s]\033[0m \r\n", __FILE__,__func__);

	return 0;
}

int CDevice::StopProcess()
{

	int nRet = 0;
	int nPid = 0;
	char szBuf[128];
	std::ifstream inFile;

//	nRet = system("dd if=up.bin of=/dev/mmcblk0 skip=0 seek=17440256 bs=1 count=4 > /dev/null 2>&1");
//	nRet = system("dd if=up.bin of=/dev/mmcblk0 skip=0 seek=17423872 bs=1 count=4 > /dev/null 2>&1");


	inFile.open("/run/dhclient.eth0.pid", std::ios::in);
	inFile.getline(szBuf, 128);
	inFile.close();
	nPid = atoi(szBuf);
//	DBG("cat dhclient.eth0.pid=%d =============> nRet = %d\r\n", nPid, nRet);
	if( nPid > 0 ) {
		sprintf(szBuf, "kill %d >> /dev/null 2>&1", nPid);
		nRet = system(szBuf);
		DBG("%s =============> nRet = %d\r\n", szBuf, nRet);
		}


	inFile.open("/run/dhclient.wlan0.pid", std::ios::in);
	inFile.getline(szBuf, 128);
	inFile.close();
	nPid = atoi(szBuf);
//	DBG("cat dhclient.wlan0.pid=%d =============> nRet = %d\r\n", nPid, nRet);
	if( nPid > 0 ) {
		sprintf(szBuf, "kill %d >> /dev/null 2>&1", nPid);
		nRet = system(szBuf);
		DBG("%s =============> nRet = %d\r\n", szBuf, nRet);
		}

	inFile.open("/run/wpa_supplicant.pid", std::ios::in);
	inFile.getline(szBuf, 128);
	inFile.close();
	nPid = atoi(szBuf);
//	DBG("cat wpa_supplicant.pid=%d =============>nRet = %d\r\n",nPid, nRet);
	if( nPid > 0 ) {
		sprintf(szBuf, "kill %d >> /dev/null 2>&1", nPid);
		nRet = system(szBuf);
		}

}

int CDevice::getMac ( char* pMac )
{
	int nRet = -1;
	int fd = -1;
	char *iface = "eth0";
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(ifr));

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);

	if (0 == (nRet = ioctl(fd, SIOCGIFHWADDR, &ifr))) {
		pMac = (char *)ifr.ifr_hwaddr.sa_data;

		//display mac address
		DBG("[%s:%s] Mac : %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n" , __FILE__, __func__, pMac[0], pMac[1], pMac[2], pMac[3], pMac[4], pMac[5]);
	}

	::close(fd);

	return nRet;
}

int CDevice::getLink ()
{
	int nRet = -1;
	int fd = -1;
	char *iface = "eth0";
	struct ifreq ifr;
	struct ethtool_value eth;

	memset(&ifr, 0, sizeof(ifr));

	fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);
	ifr.ifr_data = (caddr_t)&eth;
	eth.cmd = ETHTOOL_GLINK;

	if (0 == (nRet = ioctl(fd, SIOCETHTOOL, &ifr)) ) {
		nRet = eth.data;
		DBG("\033[7;31m[%s:%s]\033[0m nLink=%d\r\n", __FILE__,__func__, nRet);
	}

	::close(fd);

	return nRet;
}

void CDevice::LedSet(int nVal)
{

	if( nVal ) {
		if( m_Device.d_bWlan0Enable )
			ioctl(m_nFd,IOCTL_GHIGH_SET,gLED_CPU );
		ioctl(m_nFd,IOCTL_GHIGH_SET,gLED_WIFI1);
		}
	else {
		if( m_Device.d_bWlan0Enable )
			ioctl(m_nFd,IOCTL_GLOW_SET,gLED_CPU );
		ioctl(m_nFd,IOCTL_GLOW_SET,gLED_WIFI1);
		}
}


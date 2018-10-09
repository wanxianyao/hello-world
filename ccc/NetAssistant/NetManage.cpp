#include "stdafx.h"
#include "NetManage.h"
#include "TcpClient.h"
#include "TcpServer.h"

CNetManage::CNetManage()
{
}

CNetManage::~CNetManage()
{
}

void CNetManage::InitNet()
{
	WSADATA wsaData;  
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void CNetManage::Cleanup()
{
	WSACleanup();
}

INetClass* CNetManage::CreateNetInstance(ENetType eNetType, NetConfig config)
{
	if (!ValidateConfig(&config, eNetType))
	{
		return NULL;
	}
	
	if (eNetType == TCP_CLIENT)
	{
		return new CTcpClient(config);
	}
	else if (eNetType == TCP_SERVER)
	{
		return new CTcpServer(config);
	}
	else if (eNetType == UDP_CLIENT)
	{
		return NULL;
	}
	else if (eNetType == UDP_SERVER)
	{
		return NULL;
	}
	else
	{
		return NULL;
	}
}

bool CNetManage::ValidateConfig(NetConfig *pConf, ENetType eType)
{
	if (eType == TCP_CLIENT)
	{
		bool b = IsIPAddressValid(pConf->PeerIP.c_str());
		if (!b)
		{
			return b;
		}
		b = IsPortValid(pConf->PeerPort);
		if (!b)
		{
			return b;
		}
	}
	else if (eType == TCP_SERVER)
	{
		bool b = IsIPAddressValid(pConf->LocalIP.c_str());
		if (!b)
		{
			return b;
		}
		b = IsPortValid(pConf->LocalPort);
		if (!b)
		{
			return b;
		}
	}
	else if (eType == UDP_CLIENT)
	{
		return false;
	}
	else if (eType == UDP_SERVER)
	{
		return false;
	}
	else
	{
		return false;
	}
	return true;
}

bool CNetManage::IsIPAddressValid(const char* pszIPAddr)
{
	if (!pszIPAddr) return false; //若pszIPAddr为空  
	char IP1[100], cIP[4];
	int len = strlen(pszIPAddr);
	int i = 0, j = len - 1;
	int k, m = 0, n = 0, num = 0;
	//去除首尾空格(取出从i-1到j+1之间的字符):  
	while (pszIPAddr[i++] == ' ');
	while (pszIPAddr[j--] == ' ');

	for (k = i - 1; k <= j + 1; k++)
	{
		IP1[m++] = *(pszIPAddr + k);
	}
	IP1[m] = '\0';

	char *p = IP1;

	while (*p != '\0')
	{
		if (*p == ' ' || *p<'0' || *p>'9') return false;
		cIP[n++] = *p; //保存每个子段的第一个字符，用于之后判断该子段是否为0开头  

		int sum = 0;  //sum为每一子段的数值，应在0到255之间  
		while (*p != '.'&&*p != '\0')
		{
			if (*p == ' ' || *p<'0' || *p>'9') return false;
			sum = sum * 10 + *p - 48;  //每一子段字符串转化为整数  
			p++;
		}
		if (*p == '.') {
			if ((*(p - 1) >= '0'&&*(p - 1) <= '9') && (*(p + 1) >= '0'&&*(p + 1) <= '9'))//判断"."前后是否有数字，若无，则为无效IP，如“1.1.127.”  
				num++;  //记录“.”出现的次数，不能大于3  
			else
				return false;
		};
		if ((sum > 255) || (sum > 0 && cIP[0] == '0') || num > 3) return false;//若子段的值>255或为0开头的非0子段或“.”的数目>3，则为无效IP  

		if (*p != '\0') p++;
		n = 0;
	}
	if (num != 3) return false;
	return true;
}

bool CNetManage::IsPortValid(int port)
{
	if (port > 0 && port < 65535)
	{
		return true;
	}
	return false;
}

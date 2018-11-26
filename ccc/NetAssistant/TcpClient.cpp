#include "stdafx.h"
#include "TcpClient.h"
#include <thread>


CTcpClient::CTcpClient(NetConfig config)
{
	m_fd = INVALID_SOCKET;
	m_config.PeerIP = config.PeerIP;
	m_config.PeerPort = config.PeerPort;
	m_config.LocalIP = config.LocalIP;
	m_config.LocalPort = config.LocalPort;
	m_config.cbAcp = config.cbAcp;
	m_config.pAcpcbParam = config.pAcpcbParam;
	m_config.cb = config.cb;
	m_config.pcbParam = config.pcbParam;

	m_bIsExit = false;
	m_RecvBuf = new char[BUFLEN];
	m_handle = CreateEvent(NULL, FALSE, FALSE, NULL);
}


CTcpClient::~CTcpClient()
{
	WaitForSingleObject(m_handle, 1000);
	if (m_RecvBuf)
	{
		delete m_RecvBuf;
		m_RecvBuf = NULL;
	}

	if (m_fd != INVALID_SOCKET)
	{
		closesocket(m_fd);
		m_fd = INVALID_SOCKET;
	}

	CloseHandle(m_handle);
}

int CTcpClient::Recv(char *recvBuf, int len)
{
	return NET_OK;
}

int CTcpClient::Send(char *sendBuf, int len)
{
	int n = send(m_fd, sendBuf, len, 0);
	if (SOCKET_ERROR == n)
	{
		closesocket(m_fd);
		return NET_ERR;
	}
	return n;
}

int CTcpClient::Run()
{
	//2 创建套接字(socket)  
	m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_fd)
	{
		closesocket(m_fd);
		return NET_ERR;
	}
	//3 准备通信地址  
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(m_config.PeerPort);
	addrServer.sin_addr.s_addr = inet_addr(m_config.PeerIP.c_str());
	//4 连接服务器（connect)  
	if (SOCKET_ERROR == connect(m_fd, (const sockaddr*)&addrServer, sizeof(addrServer)))
	{
		closesocket(m_fd);
		return NET_ERR;
	}

	std::thread t1(CTcpClient::SelectFunc, this);
	t1.detach();

	return NET_OK;
}

int CTcpClient::Stop()
{
	m_bIsExit = true;
	if (m_fd != INVALID_SOCKET)
	{
		closesocket(m_fd);
	}
	return NET_OK;
}

void CTcpClient::SelectFunc(CTcpClient* p)
{
	while (!p->m_bIsExit)
	{
		timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		fd_set fdsets;//创建集合
		FD_ZERO(&fdsets); //初始化集合
		FD_SET(p->m_fd, &fdsets);
		
		select(NULL, &fdsets, NULL, NULL, &tv);//只检查可读性，即fd_set中的fd_read进行操作

		if (FD_ISSET(p->m_fd, &fdsets))
		{
			memset(p->m_RecvBuf, 0, BUFLEN);
			int nRet = recv(p->m_fd, p->m_RecvBuf, BUFLEN, 0);
			if (nRet == 0 || nRet == SOCKET_ERROR)
			{
				if (p->m_bIsExit)
				{
					break;
				}
				//断开连接直接退出
				if (p->m_fd != INVALID_SOCKET)
				{
					closesocket(p->m_fd);
				}
				SetEvent(p->m_handle);
				p->Stop();
				//客户端时候作为退出回调函数
				p->m_config.cbAcp(p->m_fd, p->m_config.pAcpcbParam);
				delete p;
				return;
			}
			else
			{
				p->m_config.cb(p->m_fd, p->m_RecvBuf, nRet, p->m_config.pcbParam);
			}
		}
		
	}
	SetEvent(p->m_handle);
}
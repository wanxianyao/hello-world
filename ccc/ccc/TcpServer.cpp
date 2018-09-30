#include "TcpServer.h"
#include <thread>
#include "ProgramLog.h"


CTcpServer::CTcpServer(NetConfig config)
{
	m_fd = INVALID_SOCKET;
	m_config.PeerIP = config.PeerIP;
	m_config.PeerPort = config.PeerPort;
	m_config.LocalIP = config.LocalIP;
	m_config.LocalPort = config.LocalPort;

	m_bIsExit = false;
	m_RecvBuf = new char[BUFLEN];
}


CTcpServer::~CTcpServer()
{
	Stop();
	if (m_RecvBuf)
	{
		delete m_RecvBuf;
		m_RecvBuf = NULL;
	}
}

int CTcpServer::Recv(char *recvBuf, int len)
{
	return NET_OK;
}

int CTcpServer::Send(char *sendBuf, int len)
{
	return NET_OK;
}

void CTcpServer::AcceptFunc(CTcpServer* p)
{
	while (!p->m_bIsExit)
	{
		sockaddr_in addrClient;
		int addrClientLen = sizeof(addrClient);
		SOCKET sClient = accept(p->m_fd, (sockaddr *)&addrClient, &addrClientLen);
		if (INVALID_SOCKET == sClient)
		{
			LOG_DEBUG("accept close");
			if (p->m_fd != INVALID_SOCKET)
			{
				closesocket(p->m_fd);
			}
			closesocket(sClient);
			return;
		}

		p->m_AcceptFd.push_back(sClient);
	}
}

void CTcpServer::SelectFunc(CTcpServer* p)
{
	while (!p->m_bIsExit)
	{
		timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		fd_set fdsets;//创建集合
		FD_ZERO(&fdsets); //初始化集合

						  //将socket加入到集合中（此例子是一个socket）,将多个socket加入时，可以用数组加for循环
		for (int i = 0; i < p->m_AcceptFd.size(); i++)
		{
			FD_SET(p->m_AcceptFd[i], &fdsets);
		}

		select(NULL, &fdsets, NULL, NULL, &tv);//只检查可读性，即fd_set中的fd_read进行操作

		for (int i = 0; i < p->m_AcceptFd.size(); i++)
		{
			if (FD_ISSET(p->m_AcceptFd[i], &fdsets))
			{
				memset(p->m_RecvBuf, 0, BUFLEN);
				int nRet = recv(p->m_AcceptFd[i], p->m_RecvBuf, BUFLEN, 0);//看是否能正常接收到数据
				if (nRet == 0 || (nRet == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
				{
					LOG_DEBUG("recv close");
					if (p->m_AcceptFd[i] != INVALID_SOCKET)
					{
						closesocket(p->m_AcceptFd[i]);
					}

					//将失效的sockClient剔除
					p->RemoveFd(p->m_AcceptFd[i]);
				}
				else
				{
					// to do操作接收到的数据
					nRet = send(p->m_AcceptFd[i], p->m_RecvBuf, nRet + 1, 0);
				}
			}
		}
	}
}

int CTcpServer::Run()
{
	//1 创建套接字(socket)  
	m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_fd)
	{
		WSACleanup();
		return FALSE;
	}

	//2 设置socket reuse
	BOOL bReuseaddr = TRUE;
	if (SOCKET_ERROR == setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL)))
	{
		closesocket(m_fd);
		return FALSE;
	}
	
	//3 准备通信地址  
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(m_config.LocalPort);
	addrServer.sin_addr.s_addr = inet_addr(m_config.LocalIP.c_str());;
	//addrServer.sin_addr.s_addr = INADDR_ANY;//任意可用地址

	int ret = ::bind(m_fd, (SOCKADDR*)&addrServer, sizeof(addrServer));
	if (SOCKET_ERROR == ret)
	{
		closesocket(m_fd);
		return FALSE;
	}

	//5 监听 （listen)  
	if (SOCKET_ERROR == listen(m_fd, SOMAXCONN))
	{
		closesocket(m_fd);
	}

	std::thread t1(CTcpServer::AcceptFunc, this);
	t1.detach();

	std::thread t2(CTcpServer::SelectFunc, this);
	t2.detach();

	return NET_OK;
}

int CTcpServer::Stop()
{
	m_bIsExit = true;
	m_lock.lock();
	vector<SOCKET>::iterator it = m_AcceptFd.begin();
	for (; it != m_AcceptFd.end();)
	{
		if (*it != INVALID_SOCKET)
		{
			closesocket(*it);
		}
		it++;
	}
	m_lock.unlock();

	if (m_fd != INVALID_SOCKET)
	{
		closesocket(m_fd);
	}

	return NET_OK;
}

void CTcpServer::RemoveFd(SOCKET fd)
{
	m_lock.lock();
	vector<SOCKET>::iterator it = m_AcceptFd.begin();
	for (; it != m_AcceptFd.end();)
	{
		if (*it == fd)
		{
			LOG_DEBUG("remove fd %d", fd);
			it = m_AcceptFd.erase(it);
			break;
		}
		it++;
	}
	m_lock.unlock();
}

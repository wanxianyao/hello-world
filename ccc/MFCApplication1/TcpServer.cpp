#include "stdafx.h"
#include "TcpServer.h"
#include <thread>
#include "ProgramLog.h"


CTcpServer::CTcpServer(NetConfig config)
<<<<<<< HEAD
{
=======
{	
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	m_fd = INVALID_SOCKET;
	m_config.PeerIP = config.PeerIP;
	m_config.PeerPort = config.PeerPort;
	m_config.LocalIP = config.LocalIP;
	m_config.LocalPort = config.LocalPort;
<<<<<<< HEAD

	m_bIsExit = false;
	m_RecvBuf = new char[BUFLEN];
=======
	m_config.cbAcp = config.cbAcp;
	m_config.pAcpcbParam = config.pAcpcbParam;
	m_config.cb = config.cb;
	m_config.pcbParam = config.pcbParam;

	m_bIsExit = false;
	m_RecvBuf = new char[BUFLEN];
	m_handle = CreateEvent(NULL, FALSE, FALSE, NULL);
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
}


CTcpServer::~CTcpServer()
{
<<<<<<< HEAD
	//Stop();
=======
	//防止CTcpServer被delete掉的时候该线程还在运行，造成资源被销毁后还在使用
	WaitForSingleObject(m_handle, 1000);
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	if (m_RecvBuf)
	{
		delete m_RecvBuf;
		m_RecvBuf = NULL;
	}
<<<<<<< HEAD
=======

	CloseHandle(m_handle);
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
}

int CTcpServer::Recv(char *recvBuf, int len)
{
	return NET_OK;
}

<<<<<<< HEAD
int CTcpServer::Send(char *sendBuf, int len)
{
	return NET_OK;
=======
int CTcpServer::Send(SOCKET fd, char *sendBuf, int len)
{
	int n = send(fd, sendBuf, len, 0);
	if (SOCKET_ERROR == n)
	{
		closesocket(fd);
		return NET_ERR;
	}
	return n;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
}

void CTcpServer::AcceptFunc(CTcpServer* p)
{
	while (!p->m_bIsExit)
	{
		sockaddr_in addrClient;
<<<<<<< HEAD
		int addrClientLen = sizeof(addrClient);
=======
		int addrClientLen = sizeof(addrClient);		
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
		SOCKET sClient = accept(p->m_fd, (sockaddr *)&addrClient, &addrClientLen);
		if (INVALID_SOCKET == sClient)
		{
			LOG_DEBUG("accept close");
			if (p->m_fd != INVALID_SOCKET)
			{
				closesocket(p->m_fd);
<<<<<<< HEAD
			}
=======
			}		
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
			closesocket(sClient);
			return;
		}
		p->m_lock.lock();
		p->m_AcceptFd.push_back(sClient); 
		p->m_lock.unlock();
<<<<<<< HEAD
		TRACE("push back %d\n", sClient);
=======

		p->m_config.cbAcp(sClient, p->m_config.pAcpcbParam);
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	}
}

void CTcpServer::SelectFunc(CTcpServer* p)
{
	while (!p->m_bIsExit)
	{
		timeval tv;
<<<<<<< HEAD
		tv.tv_sec = 1;
		tv.tv_usec = 0;
=======
		tv.tv_sec = 0;
		tv.tv_usec = 1000;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
		fd_set fdsets;//创建集合
		FD_ZERO(&fdsets); //初始化集合

		p->m_lock.lock();
		int vec_size = p->m_AcceptFd.size();
		p->m_lock.unlock();
<<<<<<< HEAD
		//将socket加入到集合中（此例子是一个socket）,将多个socket加入时，可以用数组加for循环
		for (int i = 0; i < vec_size; i++)
		{
			TRACE("FD_SET %d\n", p->m_AcceptFd[i]);
			FD_SET(p->m_AcceptFd[i], &fdsets);
		}
=======
		
		//将socket加入到集合中（此例子是一个socket）,将多个socket加入时，可以用数组加for循环
		for (int i = 0; i < vec_size; i++)
		{
			FD_SET(p->m_AcceptFd[i], &fdsets);
		}		
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe

		select(NULL, &fdsets, NULL, NULL, &tv);//只检查可读性，即fd_set中的fd_read进行操作

		for (int i = 0; i < vec_size; i++)
		{
			if (FD_ISSET(p->m_AcceptFd[i], &fdsets))
			{
				memset(p->m_RecvBuf, 0, BUFLEN);
				int nRet = recv(p->m_AcceptFd[i], p->m_RecvBuf, BUFLEN, 0);//看是否能正常接收到数据
<<<<<<< HEAD
				if (nRet == 0 || (nRet == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
				{
					LOG_DEBUG("recv close");
=======
				if (nRet == 0 || nRet == SOCKET_ERROR)
				{
					p->m_config.cb(p->m_AcceptFd[i], NULL, -1, p->m_config.pcbParam);
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
					if (p->m_AcceptFd[i] != INVALID_SOCKET)
					{
						closesocket(p->m_AcceptFd[i]);
					}
<<<<<<< HEAD

					//将失效的sockClient剔除
					p->RemoveFd(p->m_AcceptFd[i]);
=======
					
					//将失效的sockClient剔除
					p->RemoveFd(p->m_AcceptFd[i]);
					break;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
				}
				else
				{
					// to do操作接收到的数据
<<<<<<< HEAD
					nRet = send(p->m_AcceptFd[i], p->m_RecvBuf, nRet + 1, 0);
				}
			}
		}
	}
=======
					//nRet = send(p->m_AcceptFd[i], p->m_RecvBuf, nRet + 1, 0);
					p->m_config.cb(p->m_AcceptFd[i], p->m_RecvBuf, nRet, p->m_config.pcbParam);
				}
			}		
		}		
	}

	SetEvent(p->m_handle);
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
}

int CTcpServer::Run()
{
	//1 创建套接字(socket)  
	m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_fd)
	{
		WSACleanup();
<<<<<<< HEAD
		return FALSE;
=======
		return NET_ERR;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	}

	//2 设置socket reuse
	BOOL bReuseaddr = TRUE;
	if (SOCKET_ERROR == setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL)))
	{
		closesocket(m_fd);
<<<<<<< HEAD
		return FALSE;
=======
		return NET_ERR;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
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
<<<<<<< HEAD
		return FALSE;
=======
		return NET_ERR;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	}

	//5 监听 （listen)  
	if (SOCKET_ERROR == listen(m_fd, SOMAXCONN))
	{
		closesocket(m_fd);
<<<<<<< HEAD
=======
		return NET_ERR;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
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
<<<<<<< HEAD
	vector<SOCKET>::iterator it = m_AcceptFd.begin();
=======
	std::vector<SOCKET>::iterator it = m_AcceptFd.begin();
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
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
<<<<<<< HEAD

=======
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	return NET_OK;
}

void CTcpServer::RemoveFd(SOCKET fd)
{
	m_lock.lock();
<<<<<<< HEAD
	vector<SOCKET>::iterator it = m_AcceptFd.begin();
=======
	std::vector<SOCKET>::iterator it = m_AcceptFd.begin();
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
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

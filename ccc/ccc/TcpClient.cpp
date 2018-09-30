#include "TcpClient.h"



CTcpClient::CTcpClient(NetConfig config)
{
	m_fd = INVALID_SOCKET;
	m_config.PeerIP = config.PeerIP;
	m_config.PeerPort = config.PeerPort;
	m_config.LocalIP = config.LocalIP;
	m_config.LocalPort = config.LocalPort;
}


CTcpClient::~CTcpClient()
{
	if (m_fd != INVALID_SOCKET)
	{
		closesocket(m_fd);
		m_fd = INVALID_SOCKET;
	}
}

int CTcpClient::Recv(char *recvBuf, int len)
{
	int n = recv(m_fd, recvBuf, len, 0);
	if (SOCKET_ERROR == n)
	{
		closesocket(m_fd);
		WSACleanup();
		return -1;
	}
	return n;
}

int CTcpClient::Send(char *sendBuf, int len)
{
	//char sendBuf[100] = "你好服务器!";
	int n = send(m_fd, sendBuf, len, 0);
	if (SOCKET_ERROR == n)
	{
		closesocket(m_fd);
		WSACleanup();
		return -1;
	}
	return n;
}

int CTcpClient::Connect()
{
	//2 创建套接字(socket)  
	m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_fd)
	{
		closesocket(m_fd);
		WSACleanup();
		return FALSE;
	}
	//3 准备通信地址  
	//char szHostaddress[200];
	//getIP(szHostaddress);
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(m_config.PeerPort);
	addrServer.sin_addr.s_addr = inet_addr(m_config.PeerIP.c_str());
	//4 连接服务器（connect)  
	if (SOCKET_ERROR == connect(m_fd, (const sockaddr*)&addrServer, sizeof(addrServer)))
	{
		closesocket(m_fd);
		WSACleanup();
		return FALSE;
	}
}

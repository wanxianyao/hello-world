#pragma once
#include "INetClass.h"
#include <vector>
#include <mutex>

#define BUFLEN 1024 * 1024

class CTcpServer : public INetClass
{
public:
	CTcpServer(NetConfig config);
	~CTcpServer();

	int Recv(char *recvBuf, int len);
	int Send(char *sendBuf, int len);
	static void AcceptFunc(CTcpServer* p);
	static void SelectFunc(CTcpServer* p);
	int Run();
	int Stop();
	void RemoveFd(SOCKET fd);

private:
	NetConfig m_config;
	SOCKET m_fd;
	std::vector<SOCKET> m_AcceptFd;
	std::mutex m_lock;
	bool m_bIsExit;
	char *m_RecvBuf;
};


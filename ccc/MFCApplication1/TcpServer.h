#pragma once
#include "INetClass.h"
#include <vector>
#include <mutex>

<<<<<<< HEAD
#define BUFLEN 1024 * 1024
=======
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe

class CTcpServer : public INetClass
{
public:
	CTcpServer(NetConfig config);
	~CTcpServer();

	int Recv(char *recvBuf, int len);
<<<<<<< HEAD
	int Send(char *sendBuf, int len);
=======
	int Send(SOCKET fd, char *sendBuf, int len);
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	static void AcceptFunc(CTcpServer* p);
	static void SelectFunc(CTcpServer* p);
	int Run();
	int Stop();
<<<<<<< HEAD
	void RemoveFd(SOCKET fd);

private:
=======
	void RemoveFd(SOCKET fd);	

private:
	HANDLE m_handle;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
	NetConfig m_config;
	SOCKET m_fd;
	std::vector<SOCKET> m_AcceptFd;
	std::mutex m_lock;
	bool m_bIsExit;
	char *m_RecvBuf;
};


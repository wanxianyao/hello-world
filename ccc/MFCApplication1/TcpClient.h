#pragma once
#include "INetClass.h"

class CTcpClient : public INetClass
{
public:
	CTcpClient(NetConfig config);
	~CTcpClient();

	int Recv(char *recvBuf, int len);
	int Send(char *sendBuf, int len);

<<<<<<< HEAD
	int Connect();

private:
	NetConfig m_config;
	SOCKET m_fd;
=======
	int Run();
	int Stop();

	static void SelectFunc(CTcpClient* p);
private:
	HANDLE m_handle;
	bool m_bIsExit;
	NetConfig m_config;
	SOCKET m_fd;
	char *m_RecvBuf;
>>>>>>> df2573f07fa1fe68bd516138ccadaf4f7da15fbe
};

